
// VideoRecorder.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoRecorderApp: 
// �йش����ʵ�֣������ VideoRecorder.cpp
//

class CVideoRecorderApp : public CWinApp
{
public:
	CVideoRecorderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoRecorderApp theApp;