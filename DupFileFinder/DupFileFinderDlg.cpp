
// DupFileFinderDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DupFileFinder.h"
#include "DupFileFinderDlg.h"
#include "afxdialogex.h"

#include <vector>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDupFileFinderDlg 대화 상자



CDupFileFinderDlg::CDupFileFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DUPFILEFINDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDupFileFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SRC_PATH_EDIT, SrcPathEdit);
	DDX_Control(pDX, IDC_DEST_PATH_EDIT, DestPathEdit);
	DDX_Control(pDX, IDC_EXTS_EDIT, ExtsEdit);
	DDX_Control(pDX, IDC_RESULT_LIST, ResultListCtrl);
}

BEGIN_MESSAGE_MAP(CDupFileFinderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SRC_PATH_BROWSER_BUTTON, &CDupFileFinderDlg::OnBnClickedSrcPathBrowserButton)
	ON_BN_CLICKED(IDC_DEST_PATH_BROWSER_BUTTON, &CDupFileFinderDlg::OnBnClickedDestPathBrowserButton)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CDupFileFinderDlg::OnBnClickedFindButton)
	ON_BN_CLICKED(IDC_REMVOE_BUTTON, &CDupFileFinderDlg::OnBnClickedRemvoeButton)
END_MESSAGE_MAP()


// CDupFileFinderDlg 메시지 처리기

BOOL CDupFileFinderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDupFileFinderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDupFileFinderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDupFileFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDupFileFinderDlg::OnBnClickedSrcPathBrowserButton()
{
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = TEXT("Find Folder");

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != 0)
	{
		// get the name of the folder
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			SrcPathEdit.SetWindowTextW(path);
		}

		// free memory used
		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}
}


void CDupFileFinderDlg::OnBnClickedDestPathBrowserButton()
{
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = TEXT("Find Folder");

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != 0)
	{
		// get the name of the folder
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			DestPathEdit.SetWindowTextW(path);
		}

		// free memory used
		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}
}


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

// 파일명만, outFIles에 담음.
void RecursiveFileFind(std::set<CString>& outFiles, const CString& inPath, const CString& inRelPath, const CString& inFileName)
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

void CDupFileFinderDlg::OnBnClickedFindButton()
{
	DupFilesMap.clear();

	ResultListCtrl.ResetContent();

	CString SrcFilePath;
	SrcPathEdit.GetWindowText(SrcFilePath);

	CString DestFilePath;
	DestPathEdit.GetWindowText(DestFilePath);

	std::vector<CString> DestFiles;
	std::vector<CString> Exts;
	if (!GetExts(Exts))
	{
		return;
	}

	for (int i = 0; i < Exts.size(); ++i)
	{
		const CString& Ext = Exts[i];
		RecursiveFileFind(DestFiles, DestFilePath, TEXT(""), TEXT("*.") + Ext);
	}

	std::set<CString> SrcFiles;
	for (int i = 0; i < Exts.size(); ++i)
	{
		const CString& Ext = Exts[i];
		RecursiveFileFind(SrcFiles, SrcFilePath, TEXT(""), TEXT("*.") + Ext);
	}

	for (int i = 0; i < DestFiles.size(); ++i)
	{
		CString& RelDestFileName = DestFiles[i];

		int FindPos = RelDestFileName.ReverseFind(TEXT('\\'));

		CString DestFileName = (FindPos == -1) ? RelDestFileName : RelDestFileName.Mid(FindPos + 1);

		// set으로 가져올 때도, 사실은 상대 path로 가져와서,
		// find 할 때, 비교함수를 정의해서, 한다.
		auto finditer = SrcFiles.find(DestFileName);
		if (finditer != SrcFiles.end())
		{
			ResultListCtrl.AddString(RelDestFileName);

			DupFilesMap.insert(std::pair<CString, CString>(
				));
		}
	}
}


void CDupFileFinderDlg::OnBnClickedRemvoeButton()
{
	// 파일을 비교해서, 삭제한다.

}

bool CDupFileFinderDlg::GetExts(std::vector<CString>& OutExts)
{
	CString ExtsLine;
	ExtsEdit.GetWindowText(ExtsLine);

	int curPos = 0;

	CString Ext = ExtsLine.Tokenize(_T(" "), curPos);
	while (Ext != _T(""))
	{
		OutExts.push_back(Ext);
		Ext = ExtsLine.Tokenize(_T(" "), curPos);
	};

	return OutExts.size() != 0;
}