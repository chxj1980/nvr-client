#ifndef _NVR_RTSP_AAC_H_
#define _NVR_RTSP_AAC_H_

#include "nvr_rtsp_struct.h"

/*
* Copyright (c) 2010
* All rights reserved.
*
* filename:nvr_rtsp_aac.h
*
* func:本文件实现了对rtsp aac音频的封装
*
* author : nvr
* createdate: 2011-01-27
*
* modify list:
*
*/
#ifdef __cplusplus
extern "C" {
#endif

	int rtsp_aac_audio_send(RTSP_SESSION_S *pSess, const char *szBuff, int nFrameLen,
	                        unsigned long u32TimeStamp, unsigned long u32SampleRate);



#ifdef __cplusplus
}
#endif

#endif

