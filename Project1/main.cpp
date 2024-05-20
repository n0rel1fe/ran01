#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <knownfolders.h>
#include <shlobj.h>
#include <string.h>
#include <process.h>

// �Լ� ����
void ListFiles(const char* basePath);
void Root(char* rootDir, DWORD size);
unsigned __stdcall ThreadFunc(void* param);

// ������ ������ ����ü
typedef struct {
    char directory[MAX_PATH];
} ThreadData;

int main() {
    char rootDir[MAX_PATH];
    Root(rootDir, sizeof(rootDir));

    // �ϵ��ڵ��� ���丮 ���
    char documentsDir[MAX_PATH];
    char downloadsDir[MAX_PATH];
    char desktopDir[MAX_PATH];

    snprintf(documentsDir, sizeof(documentsDir), "%s\\Documents", rootDir);
    snprintf(downloadsDir, sizeof(downloadsDir), "%s\\Downloads", rootDir);
    snprintf(desktopDir, sizeof(desktopDir), "%s\\Desktop", rootDir);

    // ������ ������ �ʱ�ȭ
    ThreadData docData = { 0 }, downData = { 0 }, deskData = { 0 };
    strcpy(docData.directory, documentsDir);
    strcpy(downData.directory, downloadsDir);
    strcpy(deskData.directory, desktopDir);

    // ������ ����
    HANDLE hThreads[3];
    hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &docData, 0, NULL);
    hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &downData, 0, NULL);
    hThreads[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &deskData, 0, NULL);

    // ������ ���� ���
    WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);

    // ������ �ڵ� �ݱ�
    for (int i = 0; i < 3; i++) {
        CloseHandle(hThreads[i]);
    }

    return 0;
}

// ������ �Լ� ����
unsigned __stdcall ThreadFunc(void* param) {
    ThreadData* data = (ThreadData*)param;
    printf("%s ���丮 Ž��: %s\n", data->directory, data->directory);
    ListFiles(data->directory);
    return 0;
}

// ��������� ������ ����
void ListFiles(const char* basePath) {
    char searchPath[MAX_PATH];
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // �˻� ��� ����
    snprintf(searchPath, sizeof(searchPath), "%s\\*", basePath);

    // ���丮 ���� ���� �˻�
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("����: ��θ� ã�� �� ���ų� ������ �� �����ϴ�: [%s] (���� �ڵ�: %lu)\n", basePath, GetLastError());
        return;
    }

    do {
        // ���� ���丮�� �θ� ���丮�� �ǳʶ�
        if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
            char filePath[MAX_PATH];
            snprintf(filePath, sizeof(filePath), "%s\\%s", basePath, findFileData.cFileName);

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // ���� ���丮 ����� Ž��
                ListFiles(filePath);
            }
            else {
                // ���� ��� ���
                printf("����: %s\n", filePath);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// ���� ������� Ȩ ���丮 ��θ� ��� �Լ� ����
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
