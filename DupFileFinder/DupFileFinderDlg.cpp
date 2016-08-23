
// DupFileFinderDlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CDupFileFinderDlg ��ȭ ����



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


// CDupFileFinderDlg �޽��� ó����

BOOL CDupFileFinderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDupFileFinderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
			//������ �̸�
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

// ���ϸ�, outFIles�� ����.
void RecursiveFileFind(std::set<CString>& outFiles, const CString& inPath, const CString& inRelPath, const CString& inFileName)
{
	CFileFind finder;

	BOOL bWorking = (inRelPath != TEXT("")) ? finder.FindFile(inPath + TEXT("\\") + inRelPath + TEXT("\\") + inFileName) : finder.FindFile(inPath + TEXT("\\") + inFileName);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			//������ �̸�
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

		// set���� ������ ����, ����� ��� path�� �����ͼ�,
		// find �� ��, ���Լ��� �����ؼ�, �Ѵ�.
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
	// ������ ���ؼ�, �����Ѵ�.

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