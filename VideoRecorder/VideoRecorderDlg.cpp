
// VideoRecorderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRecorder.h"
#include "VideoRecorderDlg.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CVideoRecorderDlg 对话框



CVideoRecorderDlg::CVideoRecorderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoRecorderDlg::IDD, pParent)
	, begin_path(_T("C:\\Users"))
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//GetDlgItem(IDC_PATH)->SetWindowText(_T("C:\\Users"));
}

void CVideoRecorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH, begin_path);
}

BEGIN_MESSAGE_MAP(CVideoRecorderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILEPATH, &CVideoRecorderDlg::OnClickedFilepath)
	ON_BN_CLICKED(IDC_RECORD, &CVideoRecorderDlg::OnClickedRecord)
END_MESSAGE_MAP()


// CVideoRecorderDlg 消息处理程序

BOOL CVideoRecorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	Display();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVideoRecorderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVideoRecorderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVideoRecorderDlg::Display()
{
	GX_STATUS status = GX_STATUS_SUCCESS;
	GXInitLib();
	//获得设备个数,逐个填入信息
	GXUpdateDeviceList(&camNum, 1000);
	for (int i = 0; i < camNum; ++i)
	{
		CCamera cam;
		cam.index = i+1;
		sprintf(cam.camName, "%d", i+1);
		GXOpenDeviceByIndex(i+1, &cam.hDevice);
		GXSetInt(cam.hDevice, GX_INT_WIDTH, CAMERA_WIDTH);
		GXSetInt(cam.hDevice, GX_INT_HEIGHT, CAMERA_HEIGHT);
		GXSetEnum(cam.hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);//设置采集模式为连续采集
		GXSetInt(cam.hDevice, GX_INT_ACQUISITION_SPEED_LEVEL, 8);//设置采集速度，范围(0 - 12)
		GXSetInt(cam.hDevice, GX_INT_GAIN, 8);//设置增益,增益范围(0-63)
		GXSetFloat(cam.hDevice, GX_FLOAT_EXPOSURE_TIME, CAMERA_EXPOTIME);//曝光时间
		cam.pImage = cvCreateImage(CvSize(CAMERA_WIDTH, CAMERA_HEIGHT), 8, 1);
		camQue.push_back(cam);
	}
	for (int i = 0; i < camNum; ++i)
	{
		status = GXRegisterCaptureCallback(camQue[i].hDevice, &camQue[i], CCamera::FrameCallbackFun);//注册回调函数
		status = GXSendCommand(camQue[i].hDevice, GX_COMMAND_ACQUISITION_START);
	}
	//停止时使用如下内容
	/*for (uint i = 0; i < camQue.size(); ++i)
	{
		GXSendCommand(camQue[i].hDevice, GX_COMMAND_ACQUISITION_STOP);
		GXUnregisterCaptureCallback(camQue[i].hDevice);
		GXCloseDevice(camQue[i].hDevice);
	}
	GXCloseLib();*/
}



void CVideoRecorderDlg::OnClickedFilepath()
{
	// TODO: 在此添加控件通知处理程序代码
	 TCHAR szPath[MAX_PATH];
	 BROWSEINFO br;
	 ITEMIDLIST* pItem;
	 CString strDir;
 

	 br.hwndOwner = this->GetSafeHwnd();
	 br.pidlRoot = 0;
	 br.pszDisplayName = 0;
	 br.lpszTitle = (LPCWSTR )" ";
	 br.ulFlags = BIF_USENEWUI;
	 br.lpfn = 0;
	 br.iImage = 0;
	 br.lParam = 0;
 
	 pItem = SHBrowseForFolder(&br);
	 if(pItem != NULL)
	 {
		if(SHGetPathFromIDList(pItem,szPath) == TRUE)
		{
			 strDir = szPath;
		}

	}
	 SetDlgItemText (IDC_PATH ,strDir);
}


void CVideoRecorderDlg::OnClickedRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CString Path;
	//wchar_t *path;
	//size_t len = 0;
	//size_t converted = 0;
	//char *Cpath;
	uint FPS=30;

	//char path[100];
	//int j=0;
	GetDlgItem(IDC_PATH)->GetWindowText(Path);
    GetDlgItem(IDC_RECORD)->GetWindowText(str);
	
    if (str == "开始")
    {
	   GetDlgItem(IDC_RECORD)->SetWindowText(_T("停止"));
	   //开始录制
	 
	   for(char i=0;i<camnum;++i)
	  {
		   
		  //Path=Path+_T ("\\video")+i; 
		  //Path=Path+_T (".avi");
		  
		 //path = Path.GetBuffer(Path.GetLength() + 1);//数据类型转换cstring转wchar_t
		 // Cpath=(char*)malloc(len*sizeof(char));
		 // len = wcslen(path) + 1;
		 // wcstombs_s(&converted, Cpath, len, path, _TRUNCATE);//wchar_t转char
		  
		   writer[i]=cvCreateVideoWriter( i+"out.avi" ,
			   CV_FOURCC ('D','I','V','X'),FPS,cvSize (CAMERA_WIDTH ,CAMERA_HEIGHT ),0);
	     // free (Cpath );
	   }
	   char key=0;
	  // IplImage *pImagebuff;
	   while(key!=27)
	   {
		   for(char i=0;i<camnum;i++)
		   {
			   IplImage *pImagebuff = cvCreateImage(cvGetSize(camQue[i].pImage), camQue[i].pImage->depth, 3);
			   //IplImage *pImagebuff_r = cvCreateImage(cvGetSize(camQue[i].pImage), camQue[i].pImage->depth, 1);
			  // IplImage *pImagebuff_g = cvCreateImage(cvGetSize(camQue[i].pImage), camQue[i].pImage->depth, 1);
			   //IplImage *pImagebuff_b = cvCreateImage(cvGetSize(camQue[i].pImage), camQue[i].pImage->depth, 1);
			   //cvSplit (pImagebuff ,pImagebuff_b ,pImagebuff_g ,pImagebuff_r ,0);
			   //for(int z=0;z<CAMERA_HEIGHT;z++)
				//   for(int y=0;y<CAMERA_WIDTH ;y++)
			    //      *(pImagebuff_g->imageData+z*pImagebuff_g->widthStep+y) = *(pImagebuff_g->imageData+z*pImagebuff_g->widthStep+y)/2;

			   //cvMerge (pImagebuff_b ,pImagebuff_g ,pImagebuff_r,0,pImagebuff );			
			   cvCvtColor(camQue[i].pImage ,pImagebuff ,CV_BayerBG2BGR);
			   cvCvtColor(pImagebuff,camQue[i].pImage  ,CV_RGB2GRAY);
			   cvWriteFrame (writer [i],camQue[i].pImage );
			   cvReleaseImage(&pImagebuff );
			   //cvReleaseImage(&pImagebuff_r );
			   //cvReleaseImage(&pImagebuff_g );
			   //cvReleaseImage(&pImagebuff_b );
		   }
		   key=cvWaitKey(20); 
	   }
	}
    else
	{
       GetDlgItem(IDC_RECORD)->SetWindowText(_T("开始"));
	   //停止录制，保存视频
	   for(char i=0;i<camnum;i++)
	      cvReleaseVideoWriter (&writer[i]);
	}
}
