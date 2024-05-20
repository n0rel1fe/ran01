#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <knownfolders.h>
#include <shlobj.h>
#include <string.h>
#include <process.h>

// 함수 선언
void ListFiles(const char* basePath);
void Root(char* rootDir, DWORD size);
unsigned __stdcall ThreadFunc(void* param);

// 스레드 데이터 구조체
typedef struct {
    char directory[MAX_PATH];
} ThreadData;

int main() {
    char rootDir[MAX_PATH];
    Root(rootDir, sizeof(rootDir));

    // 하드코딩된 디렉토리 경로
    char documentsDir[MAX_PATH];
    char downloadsDir[MAX_PATH];
    char desktopDir[MAX_PATH];

    snprintf(documentsDir, sizeof(documentsDir), "%s\\Documents", rootDir);
    snprintf(downloadsDir, sizeof(downloadsDir), "%s\\Downloads", rootDir);
    snprintf(desktopDir, sizeof(desktopDir), "%s\\Desktop", rootDir);

    // 스레드 데이터 초기화
    ThreadData docData = { 0 }, downData = { 0 }, deskData = { 0 };
    strcpy(docData.directory, documentsDir);
    strcpy(downData.directory, downloadsDir);
    strcpy(deskData.directory, desktopDir);

    // 스레드 생성
    HANDLE hThreads[3];
    hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &docData, 0, NULL);
    hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &downData, 0, NULL);
    hThreads[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &deskData, 0, NULL);

    // 스레드 종료 대기
    WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);

    // 스레드 핸들 닫기
    for (int i = 0; i < 3; i++) {
        CloseHandle(hThreads[i]);
    }

    return 0;
}

// 스레드 함수 정의
unsigned __stdcall ThreadFunc(void* param) {
    ThreadData* data = (ThreadData*)param;
    printf("%s 디렉토리 탐색: %s\n", data->directory, data->directory);
    ListFiles(data->directory);
    return 0;
}

// 재귀적으로 파일을 나열
void ListFiles(const char* basePath) {
    char searchPath[MAX_PATH];
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // 검색 경로 구성
    snprintf(searchPath, sizeof(searchPath), "%s\\*", basePath);

    // 디렉토리 하위 파일 검색
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("오류: 경로를 찾을 수 없거나 접근할 수 없습니다: [%s] (오류 코드: %lu)\n", basePath, GetLastError());
        return;
    }

    do {
        // 현재 디렉토리와 부모 디렉토리를 건너뜀
        if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
            char filePath[MAX_PATH];
            snprintf(filePath, sizeof(filePath), "%s\\%s", basePath, findFileData.cFileName);

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // 하위 디렉토리 재귀적 탐색
                ListFiles(filePath);
            }
            else {
                // 파일 경로 출력
                printf("파일: %s\n", filePath);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// 현재 사용자의 홈 디렉토리 경로를 얻는 함수 정의
void Root(char* rootDir, DWORD size) {
    PWSTR path = NULL;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &path))) {
        wcstombs(rootDir, path, size);
        CoTaskMemFree(path);
    }
    else {
        printf("Error.\n");
    }
}
