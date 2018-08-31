/*******************************************************************************
--                                                                            --
--                    CedarX Multimedia Framework                             --
--                                                                            --
--          the Multimedia Framework for Linux/Android System                 --
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                         Softwinner Products.                               --
--                                                                            --
--                   (C) COPYRIGHT 2011 SOFTWINNER PRODUCTS                   --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
*******************************************************************************/

#include "render.h"
#define ARGS_NUM (4)
#define	SCREEN_NUM	2


VIDEO_RENDER_CONTEXT_TYPE *hnd_video_render[SCREEN_NUM] = {0};
#define CONFIG_DFBCEDAR

//int image_width,image_height;

#define SCREEN_IDX	1

static int config_de_parameter(int scn, unsigned int width, unsigned int height, __disp_pixel_fmt_t format) {
	__disp_layer_info_t tmpLayerAttr;
	int ret;
	unsigned long args[ARGS_NUM];
//	image_width = width;
//	image_height = height;

	args[0] = scn;
	args[1] = hnd_video_render[scn]->de_layer_hdl;
	args[2] = (unsigned long)(&tmpLayerAttr);
	ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_GET_PARA,args);

	//set color space
	if (height/*image_height*/ < 720) {
		tmpLayerAttr.fb.cs_mode = DISP_BT601;
	} else {
		tmpLayerAttr.fb.cs_mode = DISP_BT709;
	}
	//tmpFrmBuf.fmt.type = FB_TYPE_YUV;
	//printf("format== DISP_FORMAT_ARGB8888 %d\n", format == DISP_FORMAT_ARGB8888 ? 1 : 0);
	if(format == DISP_FORMAT_ARGB8888){
		tmpLayerAttr.fb.mode 	= DISP_MOD_NON_MB_PLANAR;
		tmpLayerAttr.fb.format 	= format; //
		tmpLayerAttr.fb.br_swap = 0;
		tmpLayerAttr.fb.cs_mode	= DISP_YCC;
		tmpLayerAttr.fb.seq 	= DISP_SEQ_P3210;
	}
	else
	{
#if 1
		tmpLayerAttr.fb.mode 	=  DISP_MOD_MB_UV_COMBINED;
#else
		tmpLayerAttr.fb.mode 	=  DISP_MOD_NON_MB_UV_COMBINED;
#endif
		tmpLayerAttr.fb.format 	= format; //DISP_FORMAT_YUV420;
		tmpLayerAttr.fb.br_swap = 0;
		tmpLayerAttr.fb.seq 	= DISP_SEQ_UVUV;
	}

	tmpLayerAttr.fb.addr[0] = 0;
	tmpLayerAttr.fb.addr[1] = 0;
	tmpLayerAttr.fb.size.width 	= width;//image_width;
	tmpLayerAttr.fb.size.height = height;//image_height;

	//set video layer attribute
	tmpLayerAttr.mode = DISP_LAYER_WORK_MODE_SCALER;
	//tmpLayerAttr.ck_mode = 0xff;
	//tmpLayerAttr.ck_eanble = 0;
	tmpLayerAttr.alpha_en = 1;
	tmpLayerAttr.alpha_val =0xff;//0xff;
#ifdef CONFIG_DFBCEDAR
	tmpLayerAttr.pipe = 1;
#else
	tmpLayerAttr.pipe = 0;
#endif
	//tmpLayerAttr.prio = 0xff;
	//screen window information
	tmpLayerAttr.scn_win.x = hnd_video_render[scn]->video_window.x;
	tmpLayerAttr.scn_win.y = hnd_video_render[scn]->video_window.y;
	tmpLayerAttr.scn_win.width  = hnd_video_render[scn]->video_window.width;
	tmpLayerAttr.scn_win.height = hnd_video_render[scn]->video_window.height;
	tmpLayerAttr.prio           = 0xff;
	//frame buffer pst and size information
	tmpLayerAttr.src_win.x = 0;//tmpVFrmInf->dst_rect.uStartX;
	tmpLayerAttr.src_win.y = 0;//tmpVFrmInf->dst_rect.uStartY;
	tmpLayerAttr.src_win.width = width;//image_width;//tmpVFrmInf->dst_rect.uWidth;
	tmpLayerAttr.src_win.height = height;//image_height;//tmpVFrmInf->dst_rect.uHeight;
	DEBUG_PF("width %d, height %d",tmpLayerAttr.src_win.width ,tmpLayerAttr.src_win.height );
	hnd_video_render[scn]->src_frm_rect.x = tmpLayerAttr.src_win.x;
	hnd_video_render[scn]->src_frm_rect.y = tmpLayerAttr.src_win.y;
	hnd_video_render[scn]->src_frm_rect.width = tmpLayerAttr.src_win.width;
	hnd_video_render[scn]->src_frm_rect.height = tmpLayerAttr.src_win.height;


	
	//tmpLayerAttr.fb.b_trd_src		= 0;
	//tmpLayerAttr.b_trd_out			= 0;
	//tmpLayerAttr.fb.trd_mode 		=  (__disp_3d_src_mode_t)3;
	//tmpLayerAttr.out_trd_mode		= DISP_3D_OUT_MODE_FP;
	//tmpLayerAttr.b_from_screen 		= 0;
	//set channel
	//tmpLayerAttr.channel = DISP_LAYER_OUTPUT_CHN_DE_CH1;
	//FIOCTRL(hnd_video_render[scn][scn]->de_fd, DISP_CMD_LAYER_SET_PARA, hnd_video_render[scn][scn]->de_layer_hdl, &tmpLayerAttr);
	DEBUG_PF("set video layer param");
	args[0] = scn;
	args[1] = hnd_video_render[scn]->de_layer_hdl;
	args[2] = (unsigned long) (&tmpLayerAttr);
	args[3] = 0;
	ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_SET_PARA, args);
	args[0]							= scn;
	args[1]                 		= hnd_video_render[scn]->de_layer_hdl;
	args[2]                 		= 0;
	args[3]                 		= 0;
	ret                     		= ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_BOTTOM,args);// ?????
	if(ret != 0)
	{		//open display layer failed, need send play end command, and exit
		OSA_ERROR("Open video display layer failed!");
		return -1;
	}
	
	args[0] = scn;
	args[1] = hnd_video_render[scn]->de_layer_hdl;
	args[2] = 0;
	args[3] = 0;
	ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_CK_ON, args);
	
	__disp_colorkey_t ck_mode;
	memset(&ck_mode, 0, sizeof(ck_mode));
	ck_mode.ck_max.alpha = 0xff;
	ck_mode.ck_max.red = 34;
	ck_mode.ck_max.green = 86;
	ck_mode.ck_max.blue = 106;
	ck_mode.ck_min.alpha = 0xff;
	ck_mode.ck_min.red = 34;
	ck_mode.ck_min.green = 86;
	ck_mode.ck_min.blue = 106;
	ck_mode.red_match_rule = 2;
	ck_mode.green_match_rule = 2;
	ck_mode.blue_match_rule = 2;
	args[0] = scn;
	args[1] = (__u32)&ck_mode;
	ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_SET_COLORKEY, args);
	
	return 0;
}

int render_init() 
{
	int scn;
	unsigned long args[ARGS_NUM];
	for(scn = 0; scn < SCREEN_NUM; scn++)
	{
		if (hnd_video_render[scn] != (VIDEO_RENDER_CONTEXT_TYPE *) 0) {
			OSA_ERROR("Cedar:vply: video play back has been opended already!");
			return -1;
		}

		hnd_video_render[scn] = (VIDEO_RENDER_CONTEXT_TYPE *) malloc(sizeof(VIDEO_RENDER_CONTEXT_TYPE ));
		if (hnd_video_render[scn] == (VIDEO_RENDER_CONTEXT_TYPE *) 0) {
			OSA_ERROR("Cedar:vply: malloc hnd_video_render[scn] error!");
			return -1;
		}
		memset(hnd_video_render[scn], 0, sizeof(VIDEO_RENDER_CONTEXT_TYPE ));

		hnd_video_render[scn]->first_frame_flag = 1;

		hnd_video_render[scn]->de_fd = open("/dev/disp", O_RDWR);
		if (hnd_video_render[scn]->de_fd < 0) {
			OSA_ERROR("Open display driver failed!");
			return -1;
		}

		args[0] = scn;
		args[1] = DISP_LAYER_WORK_MODE_NORMAL;//DISP_LAYER_WORK_MODE_SCALER;
		args[2] = 0;
		args[3] = 0;
		hnd_video_render[scn]->de_layer_hdl = ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_REQUEST,	args);
		if (hnd_video_render[scn]->de_layer_hdl == 0) {
			OSA_ERROR("Open display layer failed! de fd:%d ", hnd_video_render[scn]->de_fd);
			return -1;
		}


		//set video window information to default value, full screen
		if(scn == 0)
		{
			hnd_video_render[scn]->video_window.x = 0;
			hnd_video_render[scn]->video_window.y = 0;
			args[0] = scn;
			args[1] = hnd_video_render[scn]->de_layer_hdl;
			args[2] = 0;
			args[3] = 0;
			hnd_video_render[scn]->video_window.width = ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_SCN_GET_WIDTH, args);
			hnd_video_render[scn]->video_window.height = ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_SCN_GET_HEIGHT, args);
		}
		else
		{
			hnd_video_render[scn]->video_window.x = 0;
			hnd_video_render[scn]->video_window.y = 0;//24;
			args[0] = scn;
			args[1] = hnd_video_render[scn]->de_layer_hdl;
			args[2] = 0;
			args[3] = 0;
			hnd_video_render[scn]->video_window.width = 1280;//ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_SCN_GET_WIDTH, args);
			hnd_video_render[scn]->video_window.height = 720;//ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_SCN_GET_HEIGHT, args);
		}

		DEBUG_PF("de---w:%d,h:%d", hnd_video_render[scn]->video_window.width, hnd_video_render[scn]->video_window.height);
#if 0
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ hdmi on");
		args[0]=scn;
		ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_HDMI_OFF,args);
		player_waitMsecs(3000);//HDMI的屏幕分辩率设置需要休眠一两秒时间，如果是设置vga输出格式，不需要休眠也行
		//ioctl(disp_fd,DISP_CMD_HDMI_SET_MODE,arg);

		args[1]=DISP_TV_MOD_720P_60HZ;//需要什么分辨率在这里设置
		ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_HDMI_SET_MODE,args);
		player_waitMsecs(3000);

		ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_HDMI_ON,args);
		printf("hdmi mode:%d\r",ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_HDMI_GET_MODE,args));
#endif
	}
	return 0;
}

void render_exit(void) {
	int scn;
	unsigned long args[ARGS_NUM];
	for(scn = 0; scn < SCREEN_NUM; scn++)
	{
		if (hnd_video_render[scn] == NULL) {
			OSA_ERROR("video playback has been closed already!");
			return;
		}
		int			ret;

		//close displayer driver context
		if (hnd_video_render[scn]->de_fd) {
			args[0] = scn;
			args[1] = hnd_video_render[scn]->de_layer_hdl;
			args[2] = 0;
			args[3] = 0;
			ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_VIDEO_STOP, args);

			args[0] = scn;
			args[1] = hnd_video_render[scn]->de_layer_hdl;
			args[2] = 0;
			args[3] = 0;
			ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_RELEASE, args);

			args[0]	= scn;
			ret = ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_GET_OUTPUT_TYPE, args);
		DEBUG_PF("$$$$$$$$$$$$$$$$$$$$$$$$$$ ret: %d", ret);
#if 0		
			if(ret == DISP_OUTPUT_TYPE_HDMI)
			{
				printf("$$$$$$$$$$$$$$$$$$$$$$$$\n");
				args[0] 					= scn;
				args[1] 					= 0;
				args[2] 					= 0;
				args[3] 					= 0;
				ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_HDMI_OFF,(unsigned long)args);

				args[0] 					= scn;
				args[1] 					= 4;//hnd_video_render[scn]->hdmi_mode;
				args[2] 					= 0;
				args[3] 					= 0;
				ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_HDMI_SET_MODE, args);

				args[0] 					= scn;
				args[1] 					= 0;
				args[2] 					= 0;
				args[3] 					= 0;
				ioctl(hnd_video_render[scn]->de_fd,DISP_CMD_HDMI_ON,(unsigned long)args);

			}
#endif
			close(hnd_video_render[scn]->de_fd);
			hnd_video_render[scn]->de_fd = 0;
		}

		if (hnd_video_render[scn]) {
			free(hnd_video_render[scn]);
			hnd_video_render[scn] = NULL;
		}
	}
}

int render_render(void *frame_info, int frame_id)
{
	cedarv_picture_t *display_info = (cedarv_picture_t *) frame_info;
	__disp_video_fb_t tmpFrmBufAddr;
	// __disp_layer_info_t         layer_info;
	int ret;
	unsigned long args[ARGS_NUM];
	memset(&tmpFrmBufAddr, 0, sizeof(__disp_video_fb_t ));
	tmpFrmBufAddr.interlace 		= display_info->is_progressive? 0: 1;
	tmpFrmBufAddr.top_field_first 	= display_info->top_field_first;
	tmpFrmBufAddr.frame_rate 		= display_info->frame_rate;
	//tmpFrmBufAddr.first_frame = 0;//first_frame_flg;
	//first_frame_flg = 0;
	
	tmpFrmBufAddr.addr[0] = (__u32)display_info->y;
	tmpFrmBufAddr.addr[1] = (__u32)display_info->u;	

	tmpFrmBufAddr.id = frame_id;
//	printf("interlace :%d,top:%d,rate:%d,w1:%d,w2:%d\r\n",tmpFrmBufAddr.interlace,tmpFrmBufAddr.top_field_first,tmpFrmBufAddr.frame_rate ,display_info->display_width,display_info->width);
//	printf("y addr:%x,u addr:%x\r\n",tmpFrmBufAddr.addr[0],tmpFrmBufAddr.addr[1]);
	
	int scn;
	for(scn = 0; scn < SCREEN_NUM; scn++)
	{
	if (hnd_video_render[scn]->first_frame_flag == 1) 
	{
		__disp_layer_info_t         layer_info;
		__disp_pixel_fmt_t			pixel_format;
		
		pixel_format = display_info->pixel_format==CEDARV_PIXEL_FORMAT_AW_YUV422 ? DISP_FORMAT_YUV422 : DISP_FORMAT_YUV420;
		
		//printf("config de parameter!\n");
		if(display_info->display_width && display_info->display_height)
			config_de_parameter(scn, display_info->display_width, display_info->display_height, pixel_format);
		else
			config_de_parameter(scn, display_info->width, display_info->height,pixel_format);

		//set_display_mode(display_info, &layer_info);

	    args[0] = scn;
	    args[1] = hnd_video_render[scn]->de_layer_hdl;
	    args[2] = (unsigned long) (&layer_info);
	    args[3] = 0;
	    ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_GET_PARA, args);
	
	    layer_info.fb.addr[0] 	= tmpFrmBufAddr.addr[0];
	    layer_info.fb.addr[1] 	= tmpFrmBufAddr.addr[1];
	    
	    args[0] 				= scn;
	    args[1] 				= hnd_video_render[scn]->de_layer_hdl;
	    args[2] 				= (unsigned long) (&layer_info);
	    args[3] 				= 0;
	    ret = ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_SET_PARA, args);
	    	
		args[0] = scn;
		args[1] = hnd_video_render[scn]->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ret = ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_OPEN, args);
		DEBUG_PF("layer open hdl:%d,ret:%d w:%d h:%d", hnd_video_render[scn]->de_layer_hdl, ret,display_info->width,display_info->height);
		if (ret != 0){
			//open display layer failed, need send play end command, and exit
			OSA_ERROR("Open video display layer failed!");
			return -1;
		}
		args[0] = scn;
		args[1] = hnd_video_render[scn]->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ret = ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_VIDEO_START, args);
		if(ret != 0) {
			OSA_ERROR("Start video layer failed!");
			return -1;
		}
		
		hnd_video_render[scn]->layer_open_flag = 1;
		hnd_video_render[scn]->first_frame_flag = 0;	
	}
	else
	{
		args[0] = scn;
		args[1] = hnd_video_render[scn]->de_layer_hdl;
		args[2] = (unsigned long) (&tmpFrmBufAddr);
		args[3] = 0;
		ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_VIDEO_SET_FB, args);
	}	
	}
	return 0;
}
/*
int render_get_disp_frame_id(void)
{
	return ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_VIDEO_GET_FRAME_ID, args);
}
*/
int render_video_start_en(u8 en)
{
	int scn;	
	unsigned long args[ARGS_NUM];
	for(scn = 0; scn < SCREEN_NUM; scn++)
	{
		args[0] = scn;
		args[1] = hnd_video_render[scn]->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		if(en)
		{
			ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_VIDEO_START, args);
		}
		else
		{
			ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_VIDEO_STOP, args);
		}
	}
	return 0;
}
int render_set_screen_window(__disp_rect_t *rect)
{
	int scn;
	unsigned long args[ARGS_NUM];	
	for(scn = 0; scn < SCREEN_NUM; scn++)
	{
		args[0] = scn;
		args[1] = hnd_video_render[scn]->de_layer_hdl;
		args[2] = (unsigned long) rect;
		args[3] = 0;
		ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_SET_SCN_WINDOW, args);
	}
	return 0;
}
int render_set_src_window(__disp_rect_t *rect)
{
	int scn;	
	unsigned long args[ARGS_NUM];
	for(scn = 0; scn < SCREEN_NUM; scn++)
	{
		args[0] = scn;
		args[1] = hnd_video_render[scn]->de_layer_hdl;
		args[2] = (unsigned long) rect;
		args[3] = 0;
		ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_SET_SRC_WINDOW, args);
	}
	return 0;
}
void render_set_fb_alpha(unsigned char alpha)
{
	int fb_layer,scn;
	unsigned long args[ARGS_NUM];
	int fb = open("/dev/fb0", O_RDWR);
	ioctl(fb, FBIOGET_LAYER_HDL_0, &fb_layer);
	for(scn=0;scn<SCREEN_NUM;scn++)
	{
		args[0]=scn;
		args[1]=fb_layer;
		ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_ALPHA_ON, args);
		args[0]=scn;
		args[1]=fb_layer;
		args[2]=alpha;
		ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_LAYER_SET_ALPHA_VALUE, args);
	}
	close(fb);
}
void render_set_colorkey(unsigned char r,unsigned char g,unsigned char b)
{
	__disp_colorkey_t ck_mode;
	int scn=0;	
	unsigned long args[ARGS_NUM];
	for(scn=0;scn<SCREEN_NUM;scn++)
	{
		memset(&ck_mode, 0, sizeof(ck_mode));
		ck_mode.ck_max.alpha = 0xff;
		ck_mode.ck_max.red = r;
		ck_mode.ck_max.green = g;
		ck_mode.ck_max.blue = b;
		ck_mode.ck_min.alpha = 0xff;
		ck_mode.ck_min.red = r;
		ck_mode.ck_min.green = g;
		ck_mode.ck_min.blue = b;
		ck_mode.red_match_rule = 2;
		ck_mode.green_match_rule = 2;
		ck_mode.blue_match_rule = 2;
		args[0] = scn;
		args[1] = (__u32)&ck_mode;
		ioctl(hnd_video_render[scn]->de_fd, DISP_CMD_SET_COLORKEY, args);
	}
}
