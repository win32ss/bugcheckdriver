// bugcheckclient.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

typedef struct BugCheckData {
	ULONG BugCheckCode;
	ULONG_PTR BugCheckParameter1;
	ULONG_PTR BugCheckParameter2;
	ULONG_PTR BugCheckParameter3;
	ULONG_PTR BugCheckParameter4;
} BugCheckData, *PBugCheckData;


int _tmain(int argc, _TCHAR* argv[])
{
    BugCheckData bcd;
	DWORD BytesWritten;
	printf("Enter a BugCheckCode:\n");
	scanf("%x", &bcd.BugCheckCode);

	HANDLE hDevice = CreateFile(L"\\\\.\\BugCheckDriver", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		puts("\nFailed to open device. Make sure that the driver is loaded using \"sc load bugcheckdriver\".");
		puts("\nPress the any key to continue.");
		getchar();
		return -1;
	}

	BOOL attempt = WriteFile(hDevice, &bcd, sizeof(bcd), &BytesWritten, NULL);
	// Just in case the system does not bugcheck
	if(!attempt)
	{
		printf("\nIf you see this, you survived the great bug check assault. Please try again later and remember the error code: %d", GetLastError());
		puts("\nPress the any key to continue.");
		getchar();
		return -1;
	}

	CloseHandle(hDevice);

	return 0;
}

