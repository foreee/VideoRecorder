
// VideoRecorderDlg.h : 头文件
//#include "GxIAPI.h"
//#include <iostream>
//#include <string>
//#include <sstream>
#include "Camera.h"
#include <vector>
#define CAMERA_WIDTH 752
#define CAMERA_HEIGHT 480
#define CAMERA_EXPOTIME 65000
#define camnum camNum
//

#pragma once


// CVideoRecorderDlg 对话框
class CVideoRecorderDlg : public CDialogEx
{
// 构造
public:
	CVideoRecorderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VIDEORECORDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	uint camNum;
	CvVideoWriter *writer[50];
	std::vector<CCamera> camQue;
	void Display();
	afx_msg void OnClickedFilepath();
	afx_msg void OnClickedRecord();
	CString begin_path;
};
