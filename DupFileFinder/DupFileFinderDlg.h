
// DupFileFinderDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#include <map>
#include <vector>


// CDupFileFinderDlg ��ȭ ����
class CDupFileFinderDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDupFileFinderDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUPFILEFINDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	std::map<CString, CString> DupFilesMap;

	bool GetExts(std::vector<CString>& OutExts);

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSrcPathBrowserButton();
	afx_msg void OnBnClickedDestPathBrowserButton();
	CEdit SrcPathEdit;
	CEdit DestPathEdit;
	CEdit ExtsEdit;
	CListBox ResultListCtrl;
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnBnClickedRemvoeButton();
};
