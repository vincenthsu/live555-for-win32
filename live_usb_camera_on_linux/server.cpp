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

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "capture.h"
#include "vcompress.h"
#include "sender.h"

/** webcam_server: 打开 /dev/video0, 获取图像, 压缩, 发送到 localhost:3020 端口
 *
 * 	使用 320x240, fps=10
 */
#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define VIDEO_FPS 10.0

#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 3030

int main (int argc, char **argv)
{
	void *capture = capture_open("/dev/video0", VIDEO_WIDTH, VIDEO_HEIGHT, PIX_FMT_YUV420P);
	if (!capture) {
		fprintf(stderr, "ERR: can't open '/dev/video0'\n");
		exit(-1);
	}

	void *encoder = vc_open(VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_FPS);
	if (!encoder) {
		fprintf(stderr, "ERR: can't open x264 encoder\n");
		exit(-1);
	}

	void *sender = sender_open(TARGET_IP, TARGET_PORT);
	if (!sender) {
		fprintf(stderr, "ERR: can't open sender for %s:%d\n", TARGET_IP, TARGET_PORT);
		exit(-1);
	}

	int tosleep = 1000000 / VIDEO_FPS;

	FILE *fp_save = fopen("./my1.264", "wb");

	for (; ; ) {
		// 抓
		Picture pic;
		capture_get_picture(capture, &pic);

		// 压
		const void *outdata;
		int outlen;
		int rc = vc_compress(encoder, pic.data, pic.stride, &outdata, &outlen);
		if (rc < 0) continue;
		
		// 发
		sender_send(sender, outdata, outlen);
		fwrite(outdata, 1, outlen, fp_save);

		// 等
		//usleep(tosleep);
	}

	fclose(fp_save);

	sender_close(sender);
	vc_close(encoder);
	capture_close(capture);

	return 0;
}

