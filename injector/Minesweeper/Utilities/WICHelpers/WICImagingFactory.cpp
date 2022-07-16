#include "WICImagingFactory.hpp"
#include <assert.h>
#include <wincodec.h>

std::shared_ptr<WICImagingFactory> WICImagingFactory::m_pInstance;

WICImagingFactory::WICImagingFactory()
    : m_pWICImagingFactory(nullptr) {
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                                  IID_IWICImagingFactory, (LPVOID*)&m_pWICImagingFactory);
    assert(SUCCEEDED(hr));
}

IWICImagingFactory* WICImagingFactory::GetFactory()  const {
    assert(m_pWICImagingFactory);
    return m_pWICImagingFactory;
}