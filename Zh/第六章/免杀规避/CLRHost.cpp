#include <windows.h>
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

int main() {
    HRESULT hr;
    ICLRMetaHost *pMetaHost = NULL;
    ICLRRuntimeInfo *pRuntimeInfo = NULL;
    ICLRRuntimeHost *pRuntimeHost = NULL;

    //初始化CLR Hosting
    hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&pMetaHost);
    if (FAILED(hr)) {
        printf("CLRCreateInstance failed with HRESULT 0x%x\n", hr);
        return -1;
    }

    //获取CLR运行时信息
    hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo);
    if (FAILED(hr)) {
        printf("GetRuntime failed with HRESULT 0x%x\n", hr);
        pMetaHost->Release();
        return -1;
    }

    //获取CLR运行时接口
    hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&pRuntimeHost);
    if (FAILED(hr)) {
        printf("GetInterface failed with HRESULT 0x%x\n", hr);
        pRuntimeInfo->Release();
        pMetaHost->Release();
        return -1;
    }

    //启动CLR
    hr = pRuntimeHost->Start();
    if (FAILED(hr)) {
        printf("Start failed with HRESULT 0x%x\n", hr);
        pRuntimeHost->Release();
        pRuntimeInfo->Release();
        pMetaHost->Release();
        return -1;
    }

    //执行程序集中的方法
    DWORD dwRet;
    hr = pRuntimeHost->ExecuteInDefaultAppDomain(L"MaliciousAssembly.dll", L"MaliciousNamespace.MaliciousClass", L"ShowMessage", L"Hello from malicious code!", &dwRet);
    if (FAILED(hr)) {
        printf("ExecuteInDefaultAppDomain failed with HRESULT 0x%x\n", hr);
    } else {
        printf("Method executed with return value %d\n", dwRet);
    }

    //释放资源
    pRuntimeHost->Release();
    pRuntimeInfo->Release();
    pMetaHost->Release();

    return 0;
}
