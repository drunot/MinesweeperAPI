#include "WICImageLoader.hpp"
#include "WICHelpers/WICImagingFactory.hpp"
#include <string>
#include "ConsoleLogging.hpp"
#include <cassert>

#define DIB_WIDTHBYTES(bits) ((((bits) + 31)>>5)<<2)
#define IfFailedThrowHR(expr) {HRESULT hr = (expr); if (FAILED(hr)) throw hr;}

WICImageLoader::WICImageLoader()
    : m_pDecoder(nullptr)
    , m_pFrame(nullptr)
    , m_pConvertedFrame(nullptr)
    , m_nWidth(0)
    , m_nHeight(0) {
    // Initialize COM
    CoInitialize(nullptr);
}

void WICImageLoader::Cleanup() {
    m_nWidth = m_nHeight = 0;

    if(m_pConvertedFrame) {
        m_pConvertedFrame.Release();
        m_pConvertedFrame = nullptr;
    }

    if(m_pFrame) {
        m_pFrame.Release();
        m_pFrame = nullptr;
    }

    if(m_pDecoder) {
        m_pDecoder.Release();
        m_pDecoder = nullptr;
    }
}

void WICImageLoader::Open(const std::string File, UINT nFrame/* = 0*/) {
    try {
        // Cleanup a previously loaded image
        Cleanup();
        // Get the WIC factory from the singleton wrapper class
        IWICImagingFactory* pFactory = WICImagingFactory::GetInstance().GetFactory();
        assert(pFactory);

        if(!pFactory) {
            throw WINCODEC_ERR_NOTINITIALIZED;
        }

        std::wstring pszFile = std::wstring(File.begin(), File.end());
        // Create a decoder for the given image file
        IfFailedThrowHR(pFactory->CreateDecoderFromFilename(
                            pszFile.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &m_pDecoder));
        // Validate the given frame index nFrame
        UINT nCount = 0;

        // Get the number of frames in this image
        if(SUCCEEDED(m_pDecoder->GetFrameCount(&nCount))) {
            if(nFrame >= nCount) {
                nFrame = nCount - 1;    // If the requested frame number is too big, default to the last frame
            }
        }

        // Retrieve the requested frame of the image from the decoder
        IfFailedThrowHR(m_pDecoder->GetFrame(nFrame, &m_pFrame));
        // Retrieve the image dimensions
        IfFailedThrowHR(m_pFrame->GetSize(&m_nWidth, &m_nHeight));
        // Convert the format of the image frame to 32bppBGR
        IfFailedThrowHR(pFactory->CreateFormatConverter(&m_pConvertedFrame));
        IfFailedThrowHR(m_pConvertedFrame->Initialize(
                            m_pFrame,                        // Source frame to convert
                            GUID_WICPixelFormat32bppBGR,     // The desired pixel format
                            WICBitmapDitherTypeNone,         // The desired dither pattern
                            NULL,                            // The desired palette
                            0.f,                             // The desired alpha threshold
                            WICBitmapPaletteTypeCustom       // Palette translation type
                        ));

    } catch(...) {
        // Cleanup after something went wrong
        Cleanup();
        // Rethrow the exception, so the client code can handle it
        throw;
    }
}
bool WICImageLoader::IsLoaded() const {
    return m_pConvertedFrame != nullptr;
}
UINT WICImageLoader::GetWidth() const {
    return m_nWidth;
}
UINT WICImageLoader::GetHeight() const {
    return m_nHeight;
}

void WICImageLoader::Render(HDC hdc, UINT xDist, UINT yDist, UINT width, UINT height) {
    // Make sure an image has been loaded
    if(!IsLoaded()) {
        throw WINCODEC_ERR_WRONGSTATE;
    }

    // Get the WIC factory from the singleton wrapper class
    IWICImagingFactory* pFactory = WICImagingFactory::GetInstance().GetFactory();

    if(!pFactory) {
        throw WINCODEC_ERR_NOTINITIALIZED;
    }

    // Create a WIC image scaler to scale the image to the requested size
    CComPtr<IWICBitmapScaler> pScaler = nullptr;
    IfFailedThrowHR(pFactory->CreateBitmapScaler(&pScaler));
    IfFailedThrowHR(pScaler->Initialize(m_pConvertedFrame, width, height,
                                        WICBitmapInterpolationModeFant));
    // Render the image to a GDI device context
    HBITMAP hDIBBitmap = NULL;

    try {
        // Get a DC for the full screen
        HDC hdcScreen = GetDC(NULL);

        if(!hdcScreen) {
            throw 1;
        }

        BITMAPINFO bminfo;
        ZeroMemory(&bminfo, sizeof(bminfo));
        bminfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
        bminfo.bmiHeader.biWidth        = width;
        bminfo.bmiHeader.biHeight       = -(LONG)height;
        bminfo.bmiHeader.biPlanes       = 1;
        bminfo.bmiHeader.biBitCount     = 32;
        bminfo.bmiHeader.biCompression  = BI_RGB;
        void* pvImageBits = nullptr;    // Freed with DeleteObject(hDIBBitmap)
        hDIBBitmap = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);

        if(!hDIBBitmap) {
            throw 2;
        }

        ReleaseDC(NULL, hdcScreen);
        // Calculate the number of bytes in 1 scanline
        UINT nStride = DIB_WIDTHBYTES(width * 32);
        // Calculate the total size of the image
        UINT nImage = nStride * height;
        // Copy the pixels to the DIB section
        IfFailedThrowHR(pScaler->CopyPixels(nullptr, nStride, nImage,
                                            reinterpret_cast<BYTE*>(pvImageBits)));
        // Copy the bitmap to the target device context
        ::SetDIBitsToDevice(hdc, xDist, yDist, width, height, 0, 0, 0, height, pvImageBits, &bminfo,
                            DIB_RGB_COLORS);
        DeleteObject(hDIBBitmap);

    } catch(...) {
        if(hDIBBitmap) {
            DeleteObject(hDIBBitmap);
        }

        // Rethrow the exception, so the client code can handle it
        throw;
    }
}