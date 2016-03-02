
// VideoRecorderDlg.h : ͷ�ļ�
#include "GxIAPI.h"
#include <iostream>
#include <string>
#include <sstream>
#include "Camera.h"
#include <vector>
#define CAMERA_WIDTH 752
#define CAMERA_HEIGHT 480
#define CAMERA_EXPOTIME 65000
//

#pragma once


// CVideoRecorderDlg �Ի���
class CVideoRecorderDlg : public CDialogEx
{
// ����
public:
	CVideoRecorderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VIDEORECORDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	uint camNum;
	std::vector<CCamera> camQue;
	void Display();
};
