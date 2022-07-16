#ifndef A8C469F6_21D2_44BD_96B5_6AF753AC5D46
#define A8C469F6_21D2_44BD_96B5_6AF753AC5D46

#include <atlbase.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <memory>

class WICImagingFactory {
  public:
    inline static WICImagingFactory& GetInstance() {
        if(nullptr == m_pInstance.get()) {
            m_pInstance.reset(new WICImagingFactory());
        }

        return *m_pInstance;
    }

    virtual IWICImagingFactory* GetFactory() const;

  protected:
    CComPtr<IWICImagingFactory> m_pWICImagingFactory;

  private:
    WICImagingFactory();   // Private because singleton
    static std::shared_ptr<WICImagingFactory> m_pInstance;
};

#endif // A8C469F6_21D2_44BD_96B5_6AF753AC5D46
