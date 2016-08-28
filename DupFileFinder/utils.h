#pragma once

#include <vector>
#include <set>
#include <map>

#define BUFSIZE 1024
#define MD5LEN  16

// 비교 함수 정의 : 파일 이름만 비교
struct comparePaths
{
	bool operator()(const CString& a, const CString& b) const
	{
		int FindPosa = a.ReverseFind(TEXT('\\'));
		CString Suba = (FindPosa != -1) ? a.Mid(FindPosa + 1) : a;

		int FindPosb = b.ReverseFind(TEXT('\\'));
		CString Subb = (FindPosb != -1) ? b.Mid(FindPosb + 1) : b;

		return Suba < Subb;
	}
};

struct sFindFilesParam
{
	HWND hwnd;
	std::vector<CString> Exts;
	CString SrcFilePath;
	CString DestFilePath;
	std::vector<CString> DestFileList;
	std::map<CString, CString> DupFilesMap;	// <Dest, Src>
	INT ProgressBarID;
};

void RecycleFileOnWindows(const CString& inFileName);

void RecycleFilesOnWindows(const std::vector<CString> inFiles);

__int64 FileSize(const CString& inFileName);

DWORD GetMD5(const TCHAR* InFileName, TCHAR* OutMD5Characters, TCHAR* OutErrorMessage);

DWORD WINAPI FindDuplicatedFunc(PVOID pvParam);