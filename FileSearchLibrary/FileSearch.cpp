#include "FileSearch.h"
#include <Windows.h>
#include <iostream>
#include <mutex>



void FileSearchLibrary::FindFileByName(const std::wstring& rootDirectory, const std::wstring& targetFileName, std::queue<std::wstring>& directoriesQueue, std::atomic<bool>& stopSearch) {
    setlocale(LC_ALL, "Ukr");
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((rootDirectory + L"\\*").c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0) {
            continue;
        }

        std::wstring fullPath = rootDirectory + L"\\" + findData.cFileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && !stopSearch) {

            std::lock_guard<std::mutex> lock(queueMutex);
            directoriesQueue.push(fullPath);
        }

        if (wcscmp(findData.cFileName, targetFileName.c_str()) == 0 && !stopSearch) {

            stopSearch = true;
            std::wcout << L"Full Path to your file: " << fullPath << std::endl;
            FindClose(hFind);
            return;
        }

    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}
