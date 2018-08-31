#ifndef _NVR_RTSP_GLOBAL_H_
#define _NVR_RTSP_GLOBAL_H_

/*
* Copyright (c) 2010
* All rights reserved.
*
* filename:nvr_rtsp_global.h
*
* func:本文件实现了对rtsp 的初始化和rtsp 回调函数定义
*
* author : nvr
* createdate: 2011-01-27
*
* modify list:
*
*/

#include "nvr_rtsp_struct.h"

#ifdef __cplusplus
extern "C" {
#endif


	/*RTP Payload type define*/
	typedef enum _RTP_PT_E_
	{
		RTP_PT_ULAW             = 0,        /* mu-law  PCMU*/
		RTP_PT_GSM              = 3,        /* GSM */
		RTP_PT_G723             = 4,        /* G.723 */
		RTP_PT_ALAW             = 8,        /* a-law  PCMA*/
		RTP_PT_G722             = 9,        /* G.722 */
		RTP_PT_S16BE_STEREO     = 10,       /* linear 16, 44.1khz, 2 channel */
		RTP_PT_S16BE_MONO       = 11,       /* linear 16, 44.1khz, 1 channel */
		RTP_PT_MPEGAUDIO        = 14,       /* mpeg audio */
		RTP_PT_JPEG             = 26,       /* jpeg */
		RTP_PT_H261             = 31,       /* h.261 */
		RTP_PT_MPEGVIDEO        = 32,       /* mpeg video */
		RTP_PT_MPEG2TS          = 33,       /* mpeg2 TS stream */
		RTP_PT_H263             = 34,       /* old H263 encapsulation */


		RTP_PT_H264             = 96,       /* */
		RTP_PT_G726             = 97,       /* */
		RTP_PT_AAC              = 96,
		RTP_PT_ADPCM			= 99,

		         RTP_PT_INVALID          = 127
	}
	                                   RTP_PT_E;

	typedef enum _RTSP_MSG_NOTIFY_
	{
	    RTSP_MSG_PREVIEW_LINK		= 0x1,
	    RTSP_MSG_PREVIEW_UNLINK  	= 0x2,
	} RTSP_MSG_NOTIFY;

	//用户检测回调
	typedef int (*CheckUserCallback)(char *szUser, char *szPsw, int nGetPsw);

	typedef int (*RequestIDRCallback)(int nCh, int nSub, long lPriData);

	typedef int (*GetAudioCallback)(int nCh, char *lpFrameBuf,
	                                unsigned long *pU32Pts,
	                                unsigned long *pU32Playload,
	                                unsigned long *pU32Bits,
	                                unsigned long *pU32Sample,
	                                int nTimeOut,
	                                long lPriData);

	typedef int (*GetVideoCallback)(int nCh, int nStream,
		                       char **lpFrameBuf,
		                       unsigned long *pU32Pts,
		                       unsigned long *pU32PlayLoad,
		                       unsigned long *pU32FrameType,
		                       int nTimeOut,
		                       long lPriData);

	typedef int (*MsgCallback)(RTSP_MSG_NOTIFY eMsgNotify, int nCh, int nStream, long *lPriData);

	typedef void *(*AllocMemCallback)(int size);

	typedef void (*FreeMemCallback)(void *mem);

	//============================================================================
	//函数:nvr_rtsp_set_cb
	//
	//功能:设置rtsp服务回调函数
	//
	//=============================================================================

	int     nvr_rtsp_set_cb(CheckUserCallback checkUserCb,
	                        RequestIDRCallback reqIdrCb,
	                        GetVideoCallback  getVideoCb,
	                        GetAudioCallback  getAudioCb,
	                        MsgCallback	  msgCb,
	                        AllocMemCallback allocMemCb,
	                        FreeMemCallback freeMemCb);

	//============================================================================
	//函数:nvr_rtsp_start
	//
	//功能:启动rtsp 服务
	//
	//nMaxCh:设备支持的视频最大通道数
	//
	//nStreamNum:设备一个通道支持的码流数量 (1.单码流 2.双码流 3.三码流 以此类推)
	//
	//nRtspPort:rtsp 服务端口默认为554
	//
	//bUseAuth: 是否启用校验 0 不启用 1启用
	//
	//bRtspMode: rtsp工作模式 0 被动上传视频 1 主动上传视频
	//
	//=============================================================================
	int		nvr_rtsp_start(int nMaxCh, int nStreamNum, int nRtspPort, int bUseAuth, int bRtspMode);

	//============================================================================
	//函数:nvr_rtsp_stop
	//
	//功能:停止rtsp 服务
	//
	//=============================================================================
	int		nvr_rtsp_stop();


	extern NVR_RTSP_SVR	         gRtspSvr;
	extern pthread_mutex_t		 gRtspMutex;

	extern CheckUserCallback    gCheckUserCb;
	extern RequestIDRCallback	gRequestIdrCb;
	extern GetVideoCallback     gGetVideoCb ;
	extern GetAudioCallback     gGetAudioCb ;
	extern MsgCallback     		gMsgCb ;
	extern AllocMemCallback     	gAllocMemCb ;
	extern FreeMemCallback     	gFreeMemCb ;

#ifdef __cplusplus
}
#endif

#endif
