#include <iostream>
#include "FileSearch.h"
#include <vector>
#include <thread>
#include <queue>
#include <mutex>

using namespace std;

const int MAX_THREADS = 8;
atomic<bool> stopSearch(false);
queue<wstring> directoriesQueue;
mutex queueMutex;

int main()
{
    wstring FileName;
    wstring rootDirectory = L"C:\\";

    wcout << L"Enter the file name: ";
    wcin >> FileName;

    vector<thread> executionThreads;


    directoriesQueue.push(rootDirectory);

    auto searchDirectories = [&]() {
        while (true) {
            wstring currentDirectory;


            {
                lock_guard<mutex> lock(queueMutex);
                if (directoriesQueue.empty()) {
                    break;
                }

                currentDirectory = directoriesQueue.front();
                directoriesQueue.pop();
            }


            FileSearchLibrary::FindFileByName(currentDirectory, FileName, directoriesQueue, stopSearch);


            if (stopSearch) {
                break;
            }
        }
    };


    for (int i = 0; i < MAX_THREADS; ++i) {
        executionThreads.emplace_back(searchDirectories);
    }


    for (auto& thread : executionThreads) {
        thread.join();
    }

    return 0;
}
