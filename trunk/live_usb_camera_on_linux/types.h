#ifndef _types__hh
#define _types__hh

extern "C" {
#include <libswscale/swscale.h>
}

// yuv
struct Picture
{
    PixelFormat fmt;
    unsigned char *data[4];
    int stride[4];
};
typedef struct Picture Picture;

#endif // types.h

