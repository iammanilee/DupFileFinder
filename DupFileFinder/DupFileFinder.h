
// DupFileFinder.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDupFileFinderApp:
// �� Ŭ������ ������ ���ؼ��� DupFileFinder.cpp�� �����Ͻʽÿ�.
//

class CDupFileFinderApp : public CWinApp
{
public:
	CDupFileFinderApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDupFileFinderApp theApp;