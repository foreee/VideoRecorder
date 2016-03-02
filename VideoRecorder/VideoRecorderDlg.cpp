
// VideoRecorderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoRecorder.h"
#include "VideoRecorderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoRecorderDlg �Ի���



CVideoRecorderDlg::CVideoRecorderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoRecorderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoRecorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoRecorderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CVideoRecorderDlg ��Ϣ�������

BOOL CVideoRecorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	Display();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVideoRecorderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoRecorderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoRecorderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVideoRecorderDlg::Display()
{
	GX_STATUS status = GX_STATUS_SUCCESS;
	GXInitLib();
	//����豸����,���������Ϣ
	GXUpdateDeviceList(&camNum, 1000);
	for (int i = 0; i < camNum; ++i)
	{
		CCamera cam;
		cam.index = i+1;
		sprintf(cam.camName, "%d", i+1);
		GXOpenDeviceByIndex(i+1, &cam.hDevice);
		GXSetInt(cam.hDevice, GX_INT_WIDTH, CAMERA_WIDTH);
		GXSetInt(cam.hDevice, GX_INT_HEIGHT, CAMERA_HEIGHT);
		GXSetEnum(cam.hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);//���òɼ�ģʽΪ�����ɼ�
		GXSetInt(cam.hDevice, GX_INT_ACQUISITION_SPEED_LEVEL, 8);//���òɼ��ٶȣ���Χ(0 - 12)
		GXSetInt(cam.hDevice, GX_INT_GAIN, 8);//��������,���淶Χ(0-63)
		GXSetFloat(cam.hDevice, GX_FLOAT_EXPOSURE_TIME, CAMERA_EXPOTIME);//�ع�ʱ��
		cam.pImage = cvCreateImage(CvSize(CAMERA_WIDTH, CAMERA_HEIGHT), 8, 1);
		camQue.push_back(cam);
	}
	for (int i = 0; i < camNum; ++i)
	{
		status = GXRegisterCaptureCallback(camQue[i].hDevice, &camQue[i], CCamera::FrameCallbackFun);//ע��ص�����
		status = GXSendCommand(camQue[i].hDevice, GX_COMMAND_ACQUISITION_START);
	}
	//ֹͣʱʹ����������
	/*for (uint i = 0; i < camQue.size(); ++i)
	{
		GXSendCommand(camQue[i].hDevice, GX_COMMAND_ACQUISITION_STOP);
		GXUnregisterCaptureCallback(camQue[i].hDevice);
		GXCloseDevice(camQue[i].hDevice);
	}
	GXCloseLib();*/
}

