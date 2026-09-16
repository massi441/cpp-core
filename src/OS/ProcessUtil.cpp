#include "Core/OS/ProcessUtil.h"

#include "Core/Result/ReturnValue.h"

#ifdef WIN32
#include <TlHelp32.h>
#endif

namespace ml {

#ifndef NN_SWITCH

// TODO: Add process iterator

ProcessHandle openProcess(ProcessId pid, DWORD desiredAccess) {
    return OpenProcess(desiredAccess, FALSE, pid);
}

ProcessHandle findProcess(const std::wstring& processName, DWORD access) {
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapShot == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    DWORD pid = -1;
    PROCESSENTRY32W entry{ sizeof(entry) };

    if (Process32FirstW(snapShot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapShot, &entry));
    }

    CloseHandle(snapShot);

    if (pid == -1) {
        return nullptr;
    }

    return ml::openProcess(pid, access);
}

int findProcessInstanceCount(const std::wstring& processName) {
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapShot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    int count = 0;
    PROCESSENTRY32W entry{ sizeof(entry) };

    if (Process32FirstW(snapShot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                count++;
            }
        } while (Process32NextW(snapShot, &entry));
    }

    CloseHandle(snapShot);

    return count;
}

bool tryTerminateProcess(const std::wstring& processName, int returnCode) {
    ProcessHandle handle = findProcess(processName);

    if (handle == nullptr) {
        return false; // process already closed
    }

    return TerminateProcess(handle, returnCode);
}

bool terminateProcess(ProcessHandle handle, int returnCode) {
    return TerminateProcess(handle, returnCode);
}

ml::ReturnValue<int> tryTerminateAllProcessInstance(const std::wstring& processName, int returnCode) {
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapShot == INVALID_HANDLE_VALUE) {
        return "Failed to create windows 32 snapshot";
    }

    int closedCount = 0;
    PROCESSENTRY32W entry{ sizeof(entry) };
    if (Process32FirstW(snapShot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                ProcessHandle handle = ml::openProcess(entry.th32ProcessID);
                if (handle == nullptr) {
                    return "Failed to open running instance of process";
                }

                if (!ml::terminateProcess(handle, returnCode)) {
                    return "Failed to terminate running instance of process";
                }
            }
        } while (Process32NextW(snapShot, &entry));
    }

    CloseHandle(snapShot);

    return closedCount;
}

#endif

}
