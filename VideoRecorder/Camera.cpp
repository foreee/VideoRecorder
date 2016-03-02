#include "stdafx.h"
#include "Camera.h"


CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void __stdcall CCamera::FrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CCamera* cam = (CCamera*)(pFrame->pUserParam);
	cam->pImage->imageData = (char*)pFrame->pImgBuf;

	cvShowImage(cam->camName, cam->pImage);

	if (cam->index == 1)	cvMoveWindow(cam->camName, 0, 0);
	if (cam->index == 2)	cvMoveWindow(cam->camName, 762, 0);
	cvWaitKey(1);
}
