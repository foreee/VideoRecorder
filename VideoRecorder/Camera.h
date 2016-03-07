#pragma once
#include "GxIAPI.h"
#include <cv.h>
#include "cxcore.h"
#include "highgui.h"
#include <string>
#include <iostream>
#include <string>
#include <sstream>

class CCamera
{
public:
	CCamera();
	~CCamera();
public:
	int index;
	char camName[50];
	GX_DEV_HANDLE hDevice;
	IplImage *pImage;
	static void __stdcall FrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
};

