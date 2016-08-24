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

void RecursiveFileFind(std::vector<CString>& outFiles, const CString& inPath, const CString& inRelPath, const CString& inFileName);

void RecursiveFileFind(std::set<CString, comparePaths>& outFiles, const CString& inPath, const CString& inRelPath, const CString& inFileName);

void RecycleFileOnWindows(const CString& inFileName);

void RecycleFilesOnWindows(const std::vector<CString> inFiles);

DWORD GetMD5(const TCHAR* InFileName, TCHAR* OutMD5Characters, TCHAR* OutErrorMessage);