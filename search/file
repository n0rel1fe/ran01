#include <windows.h>
#include <stdio.h>

// 블랙리스트에 포함할 디렉토리 이름
const wchar_t* blacklist[] = { L"AppData", L"All Users", L"Default", L"Public" };
const int blacklistSize = sizeof(blacklist) / sizeof(blacklist[0]);

// 주어진 디렉토리 이름이 블랙리스트에 있는지 확인
int IsBlacklisted(const wchar_t* directoryName) {
    for (int i = 0; i < blacklistSize; i++) {
        if (wcscmp(directoryName, blacklist[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// 디렉토리 내의 파일들을 리스트화
void ListFilesInDirectory(const wchar_t* directoryPath) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    wchar_t searchPath[512];

    // 경로 설정 및 와일드카드 추가 (모든 파일과 디렉토리 포함)
    swprintf(searchPath, sizeof(searchPath) / sizeof(wchar_t), L"%s\\*", directoryPath);
    hFind = FindFirstFileW(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        wprintf(L"Unable to access directory: %s\n", directoryPath);
        return;
    }

    do {
        if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
            swprintf(searchPath, sizeof(searchPath) / sizeof(wchar_t), L"%s\\%s", directoryPath, findFileData.cFileName);
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // 재귀적으로 하위 디렉토리 탐색
                ListFilesInDirectory(searchPath);
            }
            else {
                // 파일 경로 출력
                wprintf(L"File: %s\n", searchPath);
            }
        }
    } while (FindNextFileW(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// 특정 디렉토리 탐색
void SearchDirectory(const wchar_t* directoryPath) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    wchar_t pathBuffer[512]; // 경로를 저장할 버퍼

    // 경로 설정 및 검색 패턴 추가 (* 모든 파일과 디렉토리)
    swprintf(pathBuffer, sizeof(pathBuffer) / sizeof(wchar_t), L"%s\\*", directoryPath);

    hFind = FindFirstFileW(pathBuffer, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return; // 접근할 수 없는 디렉토리는 건너뜀
    }

    do {
        // 현재 경로와 ".." 경로는 무시
        if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
            if (IsBlacklisted(findFileData.cFileName)) {
                continue; // 블랙리스트에 포함된 디렉토리는 건너뜀
            }

            swprintf(pathBuffer, sizeof(pathBuffer) / sizeof(wchar_t), L"%s\\%s", directoryPath, findFileData.cFileName);

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // 'Documents', 'Desktop', 'Downloads' 디렉토리만 정확하게 확인
                if (wcscmp(findFileData.cFileName, L"Documents") == 0 ||
                    wcscmp(findFileData.cFileName, L"Desktop") == 0 ||
                    wcscmp(findFileData.cFileName, L"Downloads") == 0) {
                    /*wprintf(L"%s\n", pathBuffer);*/
                    ListFilesInDirectory(pathBuffer);
                }
                // 다음 디렉토리를 재귀적으로 탐색
                SearchDirectory(pathBuffer);
            }
        }
    } while (FindNextFileW(hFind, &findFileData) != 0);

    FindClose(hFind);
}

int main() {
    wchar_t userPath[256] = L"C:\\Users"; // 모든 사용자의 디렉토리를 탐색하기 위한 경로 설정

    // 지정된 경로에서 탐색 시작
    SearchDirectory(userPath);

    // 사용자 입력 대기 추가
    wprintf(L"Press any key to exit...\n");
    getchar(); // 사용자 입력을 기다림

    return 0;
}
