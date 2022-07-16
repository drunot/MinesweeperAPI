#ifndef B2642534_B5B9_4CDD_99D5_B5289DC8C1DF
#define B2642534_B5B9_4CDD_99D5_B5289DC8C1DF
#include <atlbase.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <string>
class WICImageLoader {
  public:
    WICImageLoader();
    virtual ~WICImageLoader() = default;

    // Opens the nFrame-th frame of the given image file.
    // Throws HRESULT in case of failure.
    virtual void Open(const std::string File, UINT nFrame = 0);

    // Returns true if an image is loaded successfully, false otherwise
    virtual bool IsLoaded() const;

    // Renders the loaded image to the given device context hDC,
    // at position x,y and size cx, cy.
    // Throws HRESULT in case of failure.
    virtual void Render(HDC hdc, UINT xDist, UINT yDist, UINT width, UINT height);

    // Returns the width of the loaded image.
    virtual UINT GetWidth() const;

    // Returns the height of the loaded image.
    virtual UINT GetHeight() const;

  protected:
    virtual void Cleanup();

    CComPtr<IWICBitmapDecoder> m_pDecoder;
    CComPtr<IWICBitmapFrameDecode> m_pFrame;
    CComPtr<IWICFormatConverter> m_pConvertedFrame;
    UINT m_nWidth;
    UINT m_nHeight;

};

#endif // B2642534_B5B9_4CDD_99D5_B5289DC8C1DF
