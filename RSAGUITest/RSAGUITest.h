
// RSAGUITest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRSAGUITestApp: 
// �йش����ʵ�֣������ RSAGUITest.cpp
//

class CRSAGUITestApp : public CWinApp
{
public:
	CRSAGUITestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRSAGUITestApp theApp;