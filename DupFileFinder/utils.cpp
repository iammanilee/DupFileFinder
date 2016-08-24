#include "stdafx.h"

#include <Wincrypt.h>

#include "utils.h"


// inFileName을 찾아서, 상대 경로를 포함해서, outFIles에 담음.
void RecursiveFileFind(std::vector<CString>& outFiles, const CString& inPath, const CString& inRelPath, const CString& inFileName)
{
	CFileFind finder;

	BOOL bWorking = (inRelPath != TEXT("")) ? finder.FindFile(inPath + TEXT("\\") + inRelPath + TEXT("\\") + inFileName) : finder.FindFile(inPath + TEXT("\\") + inFileName);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			//파일의 이름
			CString _fileName = finder.GetFileName();
			if (inRelPath != TEXT(""))
				outFiles.push_back(inRelPath + TEXT("\\") + _fileName);
			else
				outFiles.push_back(_fileName);
		}
		else
		{
			CString _fileName = finder.GetFileName();
			if (_fileName != TEXT(".") && _fileName != TEXT(".."))
				RecursiveFileFind(outFiles, inPath, inRelPath + TEXT("\\") + _fileName, inFileName);
		}
	}

	bWorking = (inRelPath != TEXT("")) ? finder.FindFile(inPath + TEXT("\\") + inRelPath + TEXT("\\*")) : finder.FindFile(inPath + TEXT("\\*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString _fileName = finder.GetFileName();
			if (_fileName != TEXT(".") && _fileName != TEXT(".."))
				RecursiveFileFind(outFiles, inPath, inRelPath + TEXT("\\") + _fileName, inFileName);
		}
	}
}

// inFileName을 찾아서, 상대 경로를 포함해서, outFIles에 담음.
void RecursiveFileFind(std::set<CString, comparePaths>& outFiles, const CString& inPath, const CString& inRelPath, const CString& inFileName)
{
	CFileFind finder;

	BOOL bWorking = (inRelPath != TEXT("")) ? finder.FindFile(inPath + TEXT("\\") + inRelPath + TEXT("\\") + inFileName) : finder.FindFile(inPath + TEXT("\\") + inFileName);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			//파일의 이름
			CString _fileName = finder.GetFileName();
			if (inRelPath != TEXT(""))
				outFiles.insert(inRelPath + TEXT("\\") + _fileName);
			else
				outFiles.insert(_fileName);
		}
		else
		{
			CString _fileName = finder.GetFileName();
			if (_fileName != TEXT(".") && _fileName != TEXT(".."))
				RecursiveFileFind(outFiles, inPath, inRelPath + TEXT("\\") + _fileName, inFileName);
		}
	}

	bWorking = (inRelPath != TEXT("")) ? finder.FindFile(inPath + TEXT("\\") + inRelPath + TEXT("\\*")) : finder.FindFile(inPath + TEXT("\\*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString _fileName = finder.GetFileName();
			if (_fileName != TEXT(".") && _fileName != TEXT(".."))
				RecursiveFileFind(outFiles, inPath, inRelPath + TEXT("\\") + _fileName, inFileName);
		}
	}
}

void RecycleFileOnWindows(const CString& inFileName)
{
	std::wstring path = inFileName;
	path.append(1, L'\0');        // path string must be double nul-terminated

	SHFILEOPSTRUCT shfos = {};
	shfos.hwnd = nullptr;       // handle to window that will own generated windows, if applicable
	shfos.wFunc = FO_DELETE;
	shfos.pFrom = path.c_str();
	shfos.pTo = nullptr;       // not used for deletion operations
	shfos.fFlags = FOF_ALLOWUNDO; // use the recycle bin

	const int retVal = SHFileOperation(&shfos);
	if (retVal != 0)
	{
		// The operation failed...
		if (shfos.fAnyOperationsAborted)
		{
			// ...but that's because the user canceled.
			MessageBox(nullptr, L"Operation was canceled", nullptr, MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			// ...for one of the other reasons given in the documentation.
			MessageBox(nullptr, L"Operation failed", nullptr, MB_OK | MB_ICONERROR);
		}
	}
}

void RecycleFilesOnWindows(const std::vector<CString> inFiles)
{
	std::wstring path;

	for(int i = 0;i < inFiles.size(); ++i)
	{
		const CString& FileName = inFiles[i];
		path.append(FileName);
		path.append(1, L'\0');
	}
	
	SHFILEOPSTRUCT shfos = {};
	shfos.hwnd = nullptr;       // handle to window that will own generated windows, if applicable
	shfos.wFunc = FO_DELETE;
	shfos.pFrom = path.c_str();
	shfos.pTo = nullptr;       // not used for deletion operations
	shfos.fFlags = FOF_ALLOWUNDO; // use the recycle bin

	const int retVal = SHFileOperation(&shfos);
	if (retVal != 0)
	{
		// The operation failed...
		if (shfos.fAnyOperationsAborted)
		{
			// ...but that's because the user canceled.
			MessageBox(nullptr, L"Operation was canceled", nullptr, MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			// ...for one of the other reasons given in the documentation.
			MessageBox(nullptr, L"Operation failed", nullptr, MB_OK | MB_ICONERROR);
		}
	}
}

DWORD GetMD5(const TCHAR* InFileName, TCHAR* OutMD5Characters, TCHAR* OutErrorMessage)
{
	const TCHAR rgbDigits[] = TEXT("0123456789abcdef");

	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	// Logic to check usage goes here.

	hFile = CreateFile(InFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwStatus = GetLastError();
		//_stprintf(OutErrorMessage, TEXT("Error opening file %s\nError: %d\n"), InFileName, dwStatus);
		return dwStatus;
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		//_stprintf(OutErrorMessage, TEXT("CryptAcquireContext failed: %d\n"), dwStatus);
		CloseHandle(hFile);
		return dwStatus;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		//_stprintf(OutErrorMessage, TEXT("CryptAcquireContext failed: %d\n"), dwStatus);
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return dwStatus;
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
		&cbRead, NULL))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			dwStatus = GetLastError();
			//_stprintf(OutErrorMessage, TEXT("CryptHashData failed: %d\n"), dwStatus);
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return dwStatus;
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		//_stprintf(OutErrorMessage, TEXT("ReadFile failed: %d\n"), dwStatus);
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return dwStatus;
	}

	cbHash = MD5LEN;
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		for (DWORD i = 0; i < cbHash; i++)
		{
			OutMD5Characters[2 * i + 0] = rgbDigits[rgbHash[i] >> 4];
			OutMD5Characters[2 * i + 1] = rgbDigits[rgbHash[i] & 0xf];
		}

		OutMD5Characters[cbHash * 2] = 0;
	}
	else
	{
		dwStatus = GetLastError();
		//_stprintf(OutErrorMessage, TEXT("CryptGetHashParam failed: %d\n"), dwStatus);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);

	return dwStatus;
}