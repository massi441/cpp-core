#pragma once

#include <string>

#include "Core/OS/OSTypes.h"
#include "Core/Result/ReturnValue.h"

namespace ml {

#ifdef WIN32
ProcessHandle openProcess(ProcessId pid, DWORD desiredAccess = PROCESS_ALL_ACCESS);
ProcessHandle findProcess(const std::wstring& processName, DWORD access = PROCESS_ALL_ACCESS);
int findProcessInstanceCount(const std::wstring& processName);
bool tryTerminateProcess(const std::wstring& processName, int returnCode = -1);
bool terminateProcess(ProcessHandle handle, int returnCode = -1);
ml::ReturnValue<int> tryTerminateAllProcessInstance(const std::wstring& processName, int returnCode = -1);
#endif


}
