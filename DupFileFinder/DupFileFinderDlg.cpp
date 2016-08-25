
// DupFileFinderDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DupFileFinder.h"
#include "DupFileFinderDlg.h"
#include "afxdialogex.h"

#include <vector>
#include <set>
#include <algorithm>

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
	DDX_Control(pDX, IDC_SRC_PATH_STATIC, InfoTextStatic);
	DDX_Control(pDX, IDC_FIND_PROGRESS, FindProgressBar);
	DDX_Control(pDX, IDC_FIND_BUTTON, FindButton);
	DDX_Control(pDX, IDC_REMVOE_BUTTON, RemoveButton);
}

BEGIN_MESSAGE_MAP(CDupFileFinderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SRC_PATH_BROWSER_BUTTON, &CDupFileFinderDlg::OnBnClickedSrcPathBrowserButton)
	ON_BN_CLICKED(IDC_DEST_PATH_BROWSER_BUTTON, &CDupFileFinderDlg::OnBnClickedDestPathBrowserButton)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CDupFileFinderDlg::OnBnClickedFindButton)
	ON_BN_CLICKED(IDC_REMVOE_BUTTON, &CDupFileFinderDlg::OnBnClickedRemoveButton)
	ON_MESSAGE(WM_USER_UPDATE_PROGRESS, &CDupFileFinderDlg::OnUserEventUpdateProgressBar)
	ON_MESSAGE(WM_USER_FIND_COMPLETE, &CDupFileFinderDlg::OnUserEventFindCompleted)
	ON_MESSAGE(WM_USER_FIND_DEST_COMPLETED, &CDupFileFinderDlg::OnUserEventFindDestCompleted)
	ON_MESSAGE(WM_USER_FIND_SRC_FILE, &CDupFileFinderDlg::OnUserEventFindSrcFile)
	ON_LBN_SELCHANGE(IDC_RESULT_LIST, &CDupFileFinderDlg::OnLbnSelchangeResultList)
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

void CDupFileFinderDlg::OnBnClickedFindButton()
{
	FindFilesParam.Exts.clear();
	FindFilesParam.DestFileList.clear();
	FindFilesParam.DupFilesMap.clear();

	ResultListCtrl.ResetContent();

	CString SrcFilePath;
	SrcPathEdit.GetWindowText(SrcFilePath);

	CString DestFilePath;
	DestPathEdit.GetWindowText(DestFilePath);

	FindFilesParam.SrcFilePath = SrcFilePath;
	FindFilesParam.DestFilePath = DestFilePath;

	FindFilesParam.hwnd = GetSafeHwnd();

	if (!GetExts(FindFilesParam.Exts))
	{
		return;
	}

	hFindThread = CreateThread(NULL, 0, FindDuplicatedFunc, &FindFilesParam, 0, &FindThreadID);

	FindButton.EnableWindow(FALSE);
	RemoveButton.EnableWindow(FALSE);
}

void CDupFileFinderDlg::OnBnClickedRemoveButton()
{
	// 메세지로 확인
	if (MessageBox(TEXT("Caution!. All selected files will be deleted!!!"), TEXT("Confirm"), MB_YESNO) != IDYES)
	{
		return;
	}

	CString DestFilePath;
	DestPathEdit.GetWindowText(DestFilePath);

	std::vector<CString> DeleteFiles;

	// List Ctrl에서 선택된 파일만 삭제한다.
	int maxItemCount = ResultListCtrl.GetCount() + 1;
	int* pSelectedItems = new int[maxItemCount];

	int selectedCount = ResultListCtrl.GetSelItems(maxItemCount, pSelectedItems);
	std::sort(pSelectedItems, pSelectedItems + selectedCount, std::less<int>());
	
	for (int i = 0; i < selectedCount; ++i)
	{
		CString RelDestFileName;
		ResultListCtrl.GetText(pSelectedItems[i], RelDestFileName);

		int FindPosDest = RelDestFileName.ReverseFind(TEXT('\\'));

		CString DestFileFullPath = (FindPosDest != -1) ? DestFilePath + RelDestFileName : DestFilePath + TEXT("\\") + RelDestFileName;

		DeleteFiles.push_back(DestFileFullPath);
	}

	// 휴지통으로 이동한다.
	RecycleFilesOnWindows(DeleteFiles);

	// 삭제한 파일을 List Ctrl에서 지운다.
	for (int i = selectedCount - 1; i >= 0; --i)
	{
		if (pSelectedItems[i] < ResultListCtrl.GetCount())
			ResultListCtrl.DeleteString(pSelectedItems[i]);
	}
}

bool CDupFileFinderDlg::GetExts(std::vector<CString>& OutExts)
{
	// 확장자를 가져온다.
	CString ExtsLine;
	ExtsEdit.GetWindowText(ExtsLine);

	int curPos = 0;

	CString Ext = ExtsLine.Tokenize(_T(" ;"), curPos);
	while (Ext != _T(""))
	{
		OutExts.push_back(Ext);
		Ext = ExtsLine.Tokenize(_T(" ;"), curPos);
	};

	return OutExts.size() != 0;
}

void CDupFileFinderDlg::SetInfoText(const CString& InText)
{
	InfoTextStatic.SetWindowText(InText);
}

LRESULT CDupFileFinderDlg::OnUserEventUpdateProgressBar(WPARAM wParam, LPARAM lParam)
{
	int DestIndex = (int)lParam;
	if (DestIndex < FindFilesParam.DestFileList.size())
	{
		FindProgressBar.SetPos(DestIndex + 1);
	}
	
	return TRUE;
}

LRESULT CDupFileFinderDlg::OnUserEventFindCompleted(WPARAM wParam, LPARAM lParam)
{
	FindButton.EnableWindow(TRUE);
	RemoveButton.EnableWindow(TRUE);

	CString InfoTextMessage;
	InfoTextMessage.Format(TEXT("%d Files Found"), ResultListCtrl.GetCount());
	SetInfoText(InfoTextMessage);

	for (int i = 0; i < ResultListCtrl.GetCount(); i++)
	{
		ResultListCtrl.SetSel(i, true);
	}

	return TRUE;
}

LRESULT CDupFileFinderDlg::OnUserEventFindDestCompleted(WPARAM wParam, LPARAM lParam)
{
	FindProgressBar.SetRange(0, (short)FindFilesParam.DestFileList.size());
	FindProgressBar.SetPos(0);
	return TRUE;
}

LRESULT CDupFileFinderDlg::OnUserEventFindSrcFile(WPARAM wParam, LPARAM lParam)
{
	int DestIndex = (int)lParam;
	if (DestIndex < FindFilesParam.DestFileList.size())
	{
		ResultListCtrl.AddString(FindFilesParam.DestFileList[DestIndex]);
	}
	
	return TRUE;
}


void CDupFileFinderDlg::OnLbnSelchangeResultList()
{
	CString InfoTextMessage;
	InfoTextMessage.Format(TEXT("%d Files are selected"), ResultListCtrl.GetSelCount());
	SetInfoText(InfoTextMessage);
}
