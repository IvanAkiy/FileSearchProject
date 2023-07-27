#ifndef FILESEARCH_H
#define FILESEARCH_H

#include <string>
#include <queue>
#include <atomic>
#include <mutex> 

extern std::mutex queueMutex;

class FileSearchLibrary {
public:
    static void FindFileByName(const std::wstring& rootDirectory, const std::wstring& targetFileName, std::queue<std::wstring>& directoriesQueue, std::atomic<bool>& stopSearch);
};

#endif 

