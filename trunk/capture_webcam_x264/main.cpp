extern "C"{

#ifdef __cplusplus
 #define __STDC_CONSTANT_MACROS
 #ifdef _STDINT_H
  #undef _STDINT_H
 #endif
 # include <stdint.h>
#endif

}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "capture.h"
#include "vcompress.h"
#include "sender.h"

extern "C"
{
#include "avilib.h"
}

/** webcam_server: 打开 /dev/video0, 获取图像, 压缩, 发送到 localhost:3020 端口
 *
 * 	使用 320x240, fps=10
 */
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define VIDEO_FPS 30.0

#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 3030

#define DEFAULT_VIDEO "/dev/video1"

int record_flag = 1;

void hand_sig(int i)
{
    printf("recv sigint");
    record_flag = 0;
}

int main (int argc, char **argv)
{
    signal(SIGINT, hand_sig);

	void *capture = capture_open(DEFAULT_VIDEO, VIDEO_WIDTH, VIDEO_HEIGHT, PIX_FMT_YUV420P);
	if (!capture) {
		fprintf(stderr, "ERR: can't open %s \n", DEFAULT_VIDEO);
		exit(-1);
	}

	void *encoder = vc_open(VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_FPS);
	if (!encoder) {
		fprintf(stderr, "ERR: can't open x264 encoder\n");
		exit(-1);
	}

#if 0
	void *sender = sender_open(TARGET_IP, TARGET_PORT);
	if (!sender) {
		fprintf(stderr, "ERR: can't open sender for %s:%d\n", TARGET_IP, TARGET_PORT);
		exit(-1);
	}
#endif

    avi_t* avi_handle = AVI_open_output_file("record.avi");
    AVI_set_video(avi_handle, VIDEO_WIDTH, VIDEO_HEIGHT, 30, "x264");
	int tosleep = 1000000 / VIDEO_FPS;

    FILE *fp_save = fopen("./record.264", "wb");

    int frame_no = 0;
    //for(;;)
    while(record_flag)
    {
		// 抓
		Picture pic;
		capture_get_picture(capture, &pic);

		// 压
		const void *outdata;
		int outlen;
		int rc = vc_compress(encoder, pic.data, pic.stride, &outdata, &outlen);
		if (rc < 0) continue;
		
		// 发
		//sender_send(sender, outdata, outlen);
		fwrite(outdata, 1, outlen, fp_save);
        AVI_write_frame(avi_handle, (char*)outdata, outlen);
		// 等
		//usleep(tosleep);
        printf("frame_no=%d\n", frame_no++);
	}

    AVI_close(avi_handle);
	fclose(fp_save);

#if 0
	sender_close(sender);
#endif

	vc_close(encoder);
	capture_close(capture);

	return 0;
}

