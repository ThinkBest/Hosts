
// HOSTS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHOSTSApp:
// �йش����ʵ�֣������ HOSTS.cpp
//

class CHOSTSApp : public CWinApp
{
public:
	CHOSTSApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHOSTSApp theApp;