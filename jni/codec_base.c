/*
 * codec_base.c
 *
 *  Created on: 2013/11/20
 *      Author: Owner
 */


#include <string.h>
#include <math.h>

#ifndef UTIL_H_
#include "util.h"
#define UTIL_H_
#endif

#ifndef CODEC_BASE_H_
#include "codec_base.h"
#define CODEC_BAE_H_
#endif

#include <android/log.h>
#define LOG_TAG "NLiveRoid"
#define LOGD(...)  __android_log_print(3, LOG_TAG, __VA_ARGS__)

/* encoder management */
static AVCodec *first_avcodec = NULL;



const uint8_t sp5x_quant_table[20][64]=
{
    /* index 0, Q50 */
    {  16, 11, 12, 14, 12, 10, 16, 14, 13, 14, 18, 17, 16, 19, 24, 40,
       26, 24, 22, 22, 24, 49, 35, 37, 29, 40, 58, 51, 61, 60, 57, 51,
       56, 55, 64, 72, 92, 78, 64, 68, 87, 69, 55, 56, 80,109, 81, 87,
       95, 98,103,104,103, 62, 77,113,121,112,100,120, 92,101,103, 99 },
    {  17, 18, 18, 24, 21, 24, 47, 26, 26, 47, 99, 66, 56, 66, 99, 99,
       99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
       99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
       99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },

    /* index 1, Q70 */
    {  10,  7,  7,  8,  7,  6, 10,  8,  8,  8, 11, 10, 10, 11, 14, 24,
       16, 14, 13, 13, 14, 29, 21, 22, 17, 24, 35, 31, 37, 36, 34, 31,
       34, 33, 38, 43, 55, 47, 38, 41, 52, 41, 33, 34, 48, 65, 49, 52,
       57, 59, 62, 62, 62, 37, 46, 68, 73, 67, 60, 72, 55, 61, 62, 59 },
    {  10, 11, 11, 14, 13, 14, 28, 16, 16, 28, 59, 40, 34, 40, 59, 59,
       59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
       59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
       59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59 },

    /* index 2, Q80 */
    {   6,  4,  5,  6,  5,  4,  6,  6,  5,  6,  7,  7,  6,  8, 10, 16,
       10, 10,  9,  9, 10, 20, 14, 15, 12, 16, 23, 20, 24, 24, 23, 20,
       22, 22, 26, 29, 37, 31, 26, 27, 35, 28, 22, 22, 32, 44, 32, 35,
       38, 39, 41, 42, 41, 25, 31, 45, 48, 45, 40, 48, 37, 40, 41, 40 },
    {   7,  7,  7, 10,  8, 10, 19, 10, 10, 19, 40, 26, 22, 26, 40, 40,
       40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
       40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
       40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },

    /* index 3, Q85 */
    {   5,  3,  4,  4,  4,  3,  5,  4,  4,  4,  5,  5,  5,  6,  7, 12,
        8,  7,  7,  7,  7, 15, 11, 11,  9, 12, 17, 15, 18, 18, 17, 15,
       17, 17, 19, 22, 28, 23, 19, 20, 26, 21, 17, 17, 24, 33, 24, 26,
       29, 29, 31, 31, 31, 19, 23, 34, 36, 34, 30, 36, 28, 30, 31, 30 },
    {   5,  5,  5,  7,  6,  7, 14,  8,  8, 14, 30, 20, 17, 20, 30, 30,
       30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
       30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
       30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },

    /* index 4, Q90 */
    {   3,  2,  2,  3,  2,  2,  3,  3,  3,  3,  4,  3,  3,  4,  5,  8,
        5,  5,  4,  4,  5, 10,  7,  7,  6,  8, 12, 10, 12, 12, 11, 10,
       11, 11, 13, 14, 18, 16, 13, 14, 17, 14, 11, 11, 16, 22, 16, 17,
       19, 20, 21, 21, 21, 12, 15, 23, 24, 22, 20, 24, 18, 20, 21, 20 },
    {   3,  4,  4,  5,  4,  5,  9,  5,  5,  9, 20, 13, 11, 13, 20, 20,
       20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
       20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
       20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },

    /* index 5, Q60 */
    {  13,  9, 10, 11, 10,  8, 13, 11, 10, 11, 14, 14, 13, 15, 19, 32,
       21, 19, 18, 18, 19, 39, 28, 30, 23, 32, 46, 41, 49, 48, 46, 41,
       45, 44, 51, 58, 74, 62, 51, 54, 70, 55, 44, 45, 64, 87, 65, 70,
       76, 78, 82, 83, 82, 50, 62, 90, 97, 90, 80, 96, 74, 81, 82, 79 },
    {  14, 14, 14, 19, 17, 19, 38, 21, 21, 38, 79, 53, 45, 53, 79, 79,
       79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
       79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
       79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79 },

    /* index 6, Q25 */
    {  32, 22, 24, 28, 24, 20, 32, 28, 26, 28, 36, 34, 32, 38, 48, 80,
       52, 48, 44, 44, 48, 98, 70, 74, 58, 80,116,102,122,120,114,102,
      112,110,128,144,184,156,128,136,174,138,110,112,160,218,162,174,
      190,196,206,208,206,124,154,226,242,224,200,240,184,202,206,198 },
    {  34, 36, 36, 48, 42, 48, 94, 52, 52, 94,198,132,112,132,198,198,
      198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,
      198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,
      198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198 },

    /* index 7, Q95 */
    {   2,  1,  1,  1,  1,  1,  2,  1,  1,  1,  2,  2,  2,  2,  2,  4,
        3,  2,  2,  2,  2,  5,  4,  4,  3,  4,  6,  5,  6,  6,  6,  5,
        6,  6,  6,  7,  9,  8,  6,  7,  9,  7,  6,  6,  8, 11,  8,  9,
       10, 10, 10, 10, 10,  6,  8, 11, 12, 11, 10, 12,  9, 10, 10, 10 },
    {   2,  2,  2,  2,  2,  2,  5,  3,  3,  5, 10,  7,  6,  7, 10, 10,
       10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
       10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },

    /* index 8, Q93 */
    {   2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  3,  2,  2,  3,  3,  6,
        4,  3,  3,  3,  3,  7,  5,  5,  4,  6,  8,  7,  9,  8,  8,  7,
        8,  8,  9, 10, 13, 11,  9, 10, 12, 10,  8,  8, 11, 15, 11, 12,
       13, 14, 14, 15, 14,  9, 11, 16, 17, 16, 14, 17, 13, 14, 14, 14 },
    {   2,  3,  3,  3,  3,  3,  7,  4,  4,  7, 14,  9,  8,  9, 14, 14,
       14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
       14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
       14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },

    /* index 9, Q40 */
    {  20, 14, 15, 18, 15, 13, 20, 18, 16, 18, 23, 21, 20, 24, 30, 50,
       33, 30, 28, 28, 30, 61, 44, 46, 36, 50, 73, 64, 76, 75, 71, 64,
       70, 69, 80, 90,115, 98, 80, 85,109, 86, 69, 70,100,136,101,109,
      119,123,129,130,129, 78, 96,141,151,140,125,150,115,126,129,124 },
    {  21, 23, 23, 30, 26, 30, 59, 33, 33, 59,124, 83, 70, 83,124,124,
      124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,
      124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,
      124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124 }
};

AVCodecDescriptor codec_descriptors[] = {
    /* video codecs */
    {
        .id        = AV_CODEC_ID_MPEG1VIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mpeg1video",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-1 video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MPEG2VIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mpeg2video",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-2 video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MPEG2VIDEO_XVMC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mpegvideo_xvmc",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-1/2 video XvMC (X-Video Motion Compensation)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_H263,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "h263",
        .long_name = NULL_IF_CONFIG_SMALL("H.263 / H.263-1996, H.263+ / H.263-1998 / H.263 version 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RV10,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "rv10",
        .long_name = NULL_IF_CONFIG_SMALL("RealVideo 1.0"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RV20,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "rv20",
        .long_name = NULL_IF_CONFIG_SMALL("RealVideo 1.0"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MJPEG,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mjpeg",
        .long_name = NULL_IF_CONFIG_SMALL("Motion JPEG"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MJPEGB,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mjpegb",
        .long_name = NULL_IF_CONFIG_SMALL("Apple MJPEG-B"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SP5X,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "sp5x",
        .long_name = NULL_IF_CONFIG_SMALL("Sunplus JPEG (SP5X)"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MPEG4,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mpeg4",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-4 part 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RAWVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "rawvideo",
        .long_name = NULL_IF_CONFIG_SMALL("raw video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MSMPEG4V1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "msmpeg4v1",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-4 part 2 Microsoft variant version 1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MSMPEG4V2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "msmpeg4v2",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-4 part 2 Microsoft variant version 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MSMPEG4V3,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "msmpeg4v3",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-4 part 2 Microsoft variant version 3"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMV1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "wmv1",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Video 7"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMV2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "wmv2",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Video 8"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_H263P,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "h263p",
        .long_name = NULL_IF_CONFIG_SMALL("H.263+ / H.263-1998 / H.263 version 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_H263I,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "h263i",
        .long_name = NULL_IF_CONFIG_SMALL("Intel H.263"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FLV1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "flv1",
        .long_name = NULL_IF_CONFIG_SMALL("FLV / Sorenson Spark / Sorenson H.263 (Flash Video)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SVQ1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "svq1",
        .long_name = NULL_IF_CONFIG_SMALL("Sorenson Vector Quantizer 1 / Sorenson Video 1 / SVQ1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SVQ3,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "svq3",
        .long_name = NULL_IF_CONFIG_SMALL("Sorenson Vector Quantizer 3 / Sorenson Video 3 / SVQ3"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_DVVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dvvideo",
        .long_name = NULL_IF_CONFIG_SMALL("DV (Digital Video)"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_HUFFYUV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "huffyuv",
        .long_name = NULL_IF_CONFIG_SMALL("HuffYUV"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_CYUV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cyuv",
        .long_name = NULL_IF_CONFIG_SMALL("Creative YUV (CYUV)"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_H264,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "h264",
        .long_name = NULL_IF_CONFIG_SMALL("H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
        .props     = AV_CODEC_PROP_LOSSY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_INDEO3,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "indeo3",
        .long_name = NULL_IF_CONFIG_SMALL("Intel Indeo 3"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VP3,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vp3",
        .long_name = NULL_IF_CONFIG_SMALL("On2 VP3"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_THEORA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "theora",
        .long_name = NULL_IF_CONFIG_SMALL("Theora"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ASV1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "asv1",
        .long_name = NULL_IF_CONFIG_SMALL("ASUS V1"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ASV2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "asv2",
        .long_name = NULL_IF_CONFIG_SMALL("ASUS V2"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FFV1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ffv1",
        .long_name = NULL_IF_CONFIG_SMALL("FFmpeg video codec #1"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_4XM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "4xm",
        .long_name = NULL_IF_CONFIG_SMALL("4X Movie"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VCR1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vcr1",
        .long_name = NULL_IF_CONFIG_SMALL("ATI VCR1"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_CLJR,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cljr",
        .long_name = NULL_IF_CONFIG_SMALL("Cirrus Logic AccuPak"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MDEC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mdec",
        .long_name = NULL_IF_CONFIG_SMALL("Sony PlayStation MDEC (Motion DECoder)"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ROQ,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "roq",
        .long_name = NULL_IF_CONFIG_SMALL("id RoQ video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_INTERPLAY_VIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "interplayvideo",
        .long_name = NULL_IF_CONFIG_SMALL("Interplay MVE video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_XAN_WC3,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "xan_wc3",
        .long_name = NULL_IF_CONFIG_SMALL("Wing Commander III / Xan"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_XAN_WC4,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "xan_wc4",
        .long_name = NULL_IF_CONFIG_SMALL("Wing Commander IV / Xxan"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RPZA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "rpza",
        .long_name = NULL_IF_CONFIG_SMALL("QuickTime video (RPZA)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_CINEPAK,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cinepak",
        .long_name = NULL_IF_CONFIG_SMALL("Cinepak"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WS_VQA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ws_vqa",
        .long_name = NULL_IF_CONFIG_SMALL("Westwood Studios VQA (Vector Quantized Animation) video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MSRLE,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "msrle",
        .long_name = NULL_IF_CONFIG_SMALL("Microsoft RLE"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MSVIDEO1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "msvideo1",
        .long_name = NULL_IF_CONFIG_SMALL("Microsoft Video 1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_IDCIN,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "idcin",
        .long_name = NULL_IF_CONFIG_SMALL("id Quake II CIN video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_8BPS,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "8bps",
        .long_name = NULL_IF_CONFIG_SMALL("QuickTime 8BPS video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_SMC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "smc",
        .long_name = NULL_IF_CONFIG_SMALL("QuickTime Graphics (SMC)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FLIC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "flic",
        .long_name = NULL_IF_CONFIG_SMALL("Autodesk Animator Flic video"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_TRUEMOTION1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "truemotion1",
        .long_name = NULL_IF_CONFIG_SMALL("Duck TrueMotion 1.0"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VMDVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vmdvideo",
        .long_name = NULL_IF_CONFIG_SMALL("Sierra VMD video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MSZH,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mszh",
        .long_name = NULL_IF_CONFIG_SMALL("LCL (LossLess Codec Library) MSZH"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_ZLIB,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "zlib",
        .long_name = NULL_IF_CONFIG_SMALL("LCL (LossLess Codec Library) ZLIB"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_QTRLE,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "qtrle",
        .long_name = NULL_IF_CONFIG_SMALL("QuickTime Animation (RLE) video"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_SNOW,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "snow",
        .long_name = NULL_IF_CONFIG_SMALL("Snow"),
        .props     = AV_CODEC_PROP_LOSSY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_TSCC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tscc",
        .long_name = NULL_IF_CONFIG_SMALL("TechSmith Screen Capture Codec"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_ULTI,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ulti",
        .long_name = NULL_IF_CONFIG_SMALL("IBM UltiMotion"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_QDRAW,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "qdraw",
        .long_name = NULL_IF_CONFIG_SMALL("Apple QuickDraw"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_VIXL,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vixl",
        .long_name = NULL_IF_CONFIG_SMALL("Miro VideoXL"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_QPEG,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "qpeg",
        .long_name = NULL_IF_CONFIG_SMALL("Q-team QPEG"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PNG,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "png",
        .long_name = NULL_IF_CONFIG_SMALL("PNG (Portable Network Graphics) image"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PPM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ppm",
        .long_name = NULL_IF_CONFIG_SMALL("PPM (Portable PixelMap) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PBM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "pbm",
        .long_name = NULL_IF_CONFIG_SMALL("PBM (Portable BitMap) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PGM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "pgm",
        .long_name = NULL_IF_CONFIG_SMALL("PGM (Portable GrayMap) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PGMYUV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "pgmyuv",
        .long_name = NULL_IF_CONFIG_SMALL("PGMYUV (Portable GrayMap YUV) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PAM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "pam",
        .long_name = NULL_IF_CONFIG_SMALL("PAM (Portable AnyMap) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_FFVHUFF,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ffvhuff",
        .long_name = NULL_IF_CONFIG_SMALL("Huffyuv FFmpeg variant"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_RV30,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "rv30",
        .long_name = NULL_IF_CONFIG_SMALL("RealVideo 3.0"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RV40,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "rv40",
        .long_name = NULL_IF_CONFIG_SMALL("RealVideo 4.0"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VC1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vc1",
        .long_name = NULL_IF_CONFIG_SMALL("SMPTE VC-1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMV3,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "wmv3",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Video 9"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_LOCO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "loco",
        .long_name = NULL_IF_CONFIG_SMALL("LOCO"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_WNV1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "wnv1",
        .long_name = NULL_IF_CONFIG_SMALL("Winnov WNV1"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AASC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "aasc",
        .long_name = NULL_IF_CONFIG_SMALL("Autodesk RLE"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_INDEO2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "indeo2",
        .long_name = NULL_IF_CONFIG_SMALL("Intel Indeo 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FRAPS,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "fraps",
        .long_name = NULL_IF_CONFIG_SMALL("Fraps"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_TRUEMOTION2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "truemotion2",
        .long_name = NULL_IF_CONFIG_SMALL("Duck TrueMotion 2.0"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_BMP,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "bmp",
        .long_name = NULL_IF_CONFIG_SMALL("BMP (Windows and OS/2 bitmap)"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_CSCD,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cscd",
        .long_name = NULL_IF_CONFIG_SMALL("CamStudio"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MMVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mmvideo",
        .long_name = NULL_IF_CONFIG_SMALL("American Laser Games MM Video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ZMBV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "zmbv",
        .long_name = NULL_IF_CONFIG_SMALL("Zip Motion Blocks Video"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_AVS,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "avs",
        .long_name = NULL_IF_CONFIG_SMALL("AVS (Audio Video Standard) video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SMACKVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "smackvideo",
        .long_name = NULL_IF_CONFIG_SMALL("Smacker video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_NUV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "nuv",
        .long_name = NULL_IF_CONFIG_SMALL("NuppelVideo/RTJPEG"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_KMVC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "kmvc",
        .long_name = NULL_IF_CONFIG_SMALL("Karl Morton's video codec"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FLASHSV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "flashsv",
        .long_name = NULL_IF_CONFIG_SMALL("Flash Screen Video v1"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_CAVS,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cavs",
        .long_name = NULL_IF_CONFIG_SMALL("Chinese AVS (Audio Video Standard) (AVS1-P2, JiZhun profile)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_JPEG2000,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "jpeg2000",
        .long_name = NULL_IF_CONFIG_SMALL("JPEG 2000"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY |
                     AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_VMNC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vmnc",
        .long_name = NULL_IF_CONFIG_SMALL("VMware Screen Codec / VMware Video"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_VP5,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vp5",
        .long_name = NULL_IF_CONFIG_SMALL("On2 VP5"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VP6,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vp6",
        .long_name = NULL_IF_CONFIG_SMALL("On2 VP6"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VP6F,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vp6f",
        .long_name = NULL_IF_CONFIG_SMALL("On2 VP6 (Flash version)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TARGA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "targa",
        .long_name = NULL_IF_CONFIG_SMALL("Truevision Targa image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_DSICINVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dsicinvideo",
        .long_name = NULL_IF_CONFIG_SMALL("Delphine Software International CIN video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TIERTEXSEQVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tiertexseqvideo",
        .long_name = NULL_IF_CONFIG_SMALL("Tiertex Limited SEQ video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TIFF,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tiff",
        .long_name = NULL_IF_CONFIG_SMALL("TIFF image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_GIF,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "gif",
        .long_name = NULL_IF_CONFIG_SMALL("GIF (Graphics Interchange Format)"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_DXA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dxa",
        .long_name = NULL_IF_CONFIG_SMALL("Feeble Files/ScummVM DXA"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_DNXHD,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dnxhd",
        .long_name = NULL_IF_CONFIG_SMALL("VC3/DNxHD"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_THP,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "thp",
        .long_name = NULL_IF_CONFIG_SMALL("Nintendo Gamecube THP video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SGI,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "sgi",
        .long_name = NULL_IF_CONFIG_SMALL("SGI image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_SGIRLE,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "sgirle",
        .long_name = NULL_IF_CONFIG_SMALL("SGI RLE 8-bit"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_C93,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "c93",
        .long_name = NULL_IF_CONFIG_SMALL("Interplay C93"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_BETHSOFTVID,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "bethsoftvid",
        .long_name = NULL_IF_CONFIG_SMALL("Bethesda VID video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PTX,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ptx",
        .long_name = NULL_IF_CONFIG_SMALL("V.Flash PTX image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TXD,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "txd",
        .long_name = NULL_IF_CONFIG_SMALL("Renderware TXD (TeXture Dictionary) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VP6A,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vp6a",
        .long_name = NULL_IF_CONFIG_SMALL("On2 VP6 (Flash version, with alpha channel)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AMV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "amv",
        .long_name = NULL_IF_CONFIG_SMALL("AMV Video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VB,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vb",
        .long_name = NULL_IF_CONFIG_SMALL("Beam Software VB"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PCX,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "pcx",
        .long_name = NULL_IF_CONFIG_SMALL("PC Paintbrush PCX image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_SUNRAST,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "sunrast",
        .long_name = NULL_IF_CONFIG_SMALL("Sun Rasterfile image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_INDEO4,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "indeo4",
        .long_name = NULL_IF_CONFIG_SMALL("Intel Indeo Video Interactive 4"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_INDEO5,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "indeo5",
        .long_name = NULL_IF_CONFIG_SMALL("Intel Indeo Video Interactive 5"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MIMIC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mimic",
        .long_name = NULL_IF_CONFIG_SMALL("Mimic"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RL2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "rl2",
        .long_name = NULL_IF_CONFIG_SMALL("RL2 video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ESCAPE124,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "escape124",
        .long_name = NULL_IF_CONFIG_SMALL("Escape 124"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_DIRAC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dirac",
        .long_name = NULL_IF_CONFIG_SMALL("Dirac"),
        .props     = AV_CODEC_PROP_LOSSY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_BFI,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "bfi",
        .long_name = NULL_IF_CONFIG_SMALL("Brute Force & Ignorance"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_CMV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cmv",
        .long_name = NULL_IF_CONFIG_SMALL("Electronic Arts CMV video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MOTIONPIXELS,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "motionpixels",
        .long_name = NULL_IF_CONFIG_SMALL("Motion Pixels video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TGV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tgv",
        .long_name = NULL_IF_CONFIG_SMALL("Electronic Arts TGV video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TGQ,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tgq",
        .long_name = NULL_IF_CONFIG_SMALL("Electronic Arts TGQ video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TQI,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tqi",
        .long_name = NULL_IF_CONFIG_SMALL("Electronic Arts TQI video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AURA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "aura",
        .long_name = NULL_IF_CONFIG_SMALL("Auravision AURA"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AURA2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "aura2",
        .long_name = NULL_IF_CONFIG_SMALL("Auravision Aura 2"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_V210X,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "v210x",
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_TMV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tmv",
        .long_name = NULL_IF_CONFIG_SMALL("8088flex TMV"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_V210,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "v210",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed 4:2:2 10-bit"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_DPX,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dpx",
        .long_name = NULL_IF_CONFIG_SMALL("DPX image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MAD,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mad",
        .long_name = NULL_IF_CONFIG_SMALL("Electronic Arts Madcow Video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FRWU,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "frwu",
        .long_name = NULL_IF_CONFIG_SMALL("Forward Uncompressed"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_FLASHSV2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "flashsv2",
        .long_name = NULL_IF_CONFIG_SMALL("Flash Screen Video v2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_CDGRAPHICS,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cdgraphics",
        .long_name = NULL_IF_CONFIG_SMALL("CD Graphics video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_R210,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "r210",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed RGB 10-bit"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_ANM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "anm",
        .long_name = NULL_IF_CONFIG_SMALL("Deluxe Paint Animation"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_BINKVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "binkvideo",
        .long_name = NULL_IF_CONFIG_SMALL("Bink video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_IFF_ILBM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "iff_ilbm",
        .long_name = NULL_IF_CONFIG_SMALL("IFF ILBM"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_IFF_BYTERUN1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "iff_byterun1",
        .long_name = NULL_IF_CONFIG_SMALL("IFF ByteRun1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_KGV1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "kgv1",
        .long_name = NULL_IF_CONFIG_SMALL("Kega Game Video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_YOP,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "yop",
        .long_name = NULL_IF_CONFIG_SMALL("Psygnosis YOP Video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VP8,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vp8",
        .long_name = NULL_IF_CONFIG_SMALL("On2 VP8"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VP9,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vp9",
        .long_name = NULL_IF_CONFIG_SMALL("Google VP9"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PICTOR,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "pictor",
        .long_name = NULL_IF_CONFIG_SMALL("Pictor/PC Paint"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ANSI,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ansi",
        .long_name = NULL_IF_CONFIG_SMALL("ASCII/ANSI art"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_A64_MULTI,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "a64_multi",
        .long_name = NULL_IF_CONFIG_SMALL("Multicolor charset for Commodore 64"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_A64_MULTI5,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "a64_multi5",
        .long_name = NULL_IF_CONFIG_SMALL("Multicolor charset for Commodore 64, extended with 5th color (colram)"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_R10K,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "r10k",
        .long_name = NULL_IF_CONFIG_SMALL("AJA Kona 10-bit RGB Codec"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MVC1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mvc1",
        .long_name = NULL_IF_CONFIG_SMALL("Silicon Graphics Motion Video Compressor 1"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MVC2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mvc2",
        .long_name = NULL_IF_CONFIG_SMALL("Silicon Graphics Motion Video Compressor 2"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MXPEG,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mxpeg",
        .long_name = NULL_IF_CONFIG_SMALL("Mobotix MxPEG video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_LAGARITH,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "lagarith",
        .long_name = NULL_IF_CONFIG_SMALL("Lagarith lossless"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PRORES,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "prores",
        .long_name = NULL_IF_CONFIG_SMALL("Apple ProRes (iCodec Pro)"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_JV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "jv",
        .long_name = NULL_IF_CONFIG_SMALL("Bitmap Brothers JV video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_DFA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dfa",
        .long_name = NULL_IF_CONFIG_SMALL("Chronomaster DFA"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMV3IMAGE,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "wmv3image",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Video 9 Image"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VC1IMAGE,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vc1image",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Video 9 Image v2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_UTVIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "utvideo",
        .long_name = NULL_IF_CONFIG_SMALL("Ut Video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_BMV_VIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "bmv_video",
        .long_name = NULL_IF_CONFIG_SMALL("Discworld II BMV video"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_VBLE,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "vble",
        .long_name = NULL_IF_CONFIG_SMALL("VBLE Lossless Codec"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_DXTORY,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "dxtory",
        .long_name = NULL_IF_CONFIG_SMALL("Dxtory"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_V410,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "v410",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed 4:4:4 10-bit"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_XWD,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "xwd",
        .long_name = NULL_IF_CONFIG_SMALL("XWD (X Window Dump) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_CDXL,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cdxl",
        .long_name = NULL_IF_CONFIG_SMALL("Commodore CDXL video"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_XBM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "xbm",
        .long_name = NULL_IF_CONFIG_SMALL("XBM (X BitMap) image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_ZEROCODEC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "zerocodec",
        .long_name = NULL_IF_CONFIG_SMALL("ZeroCodec Lossless Video"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MSS1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mss1",
        .long_name = NULL_IF_CONFIG_SMALL("MS Screen 1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MSA1,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "msa1",
        .long_name = NULL_IF_CONFIG_SMALL("MS ATC Screen"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TSCC2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "tscc2",
        .long_name = NULL_IF_CONFIG_SMALL("TechSmith Screen Codec 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MTS2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mts2",
        .long_name = NULL_IF_CONFIG_SMALL("MS Expression Encoder Screen"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_CLLC,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cllc",
        .long_name = NULL_IF_CONFIG_SMALL("Canopus Lossless Codec"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MSS2,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "mss2",
        .long_name = NULL_IF_CONFIG_SMALL("MS Windows Media Video V9 Screen"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_Y41P,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "y41p",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed YUV 4:1:1 12-bit"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_ESCAPE130,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "escape130",
        .long_name = NULL_IF_CONFIG_SMALL("Escape 130"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_EXR,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "exr",
        .long_name = NULL_IF_CONFIG_SMALL("OpenEXR image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY |
                     AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_AVRP,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "avrp",
        .long_name = NULL_IF_CONFIG_SMALL("Avid 1:1 10-bit RGB Packer"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_012V,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "012v",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed 4:2:2 10-bit"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_G2M,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "g2m",
        .long_name = NULL_IF_CONFIG_SMALL("GoToMeeting"),
    },
    {
        .id        = AV_CODEC_ID_AVUI,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "avui",
        .long_name = NULL_IF_CONFIG_SMALL("Avid Meridien Uncompressed"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_AYUV,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "ayuv",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed packed MS 4:4:4:4"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_TARGA_Y216,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "targa_y216",
        .long_name = NULL_IF_CONFIG_SMALL("Pinnacle TARGA CineWave YUV16"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_V308,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "v308",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed packed 4:4:4"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_V408,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "v408",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed packed QT 4:4:4:4"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_YUV4,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "yuv4",
        .long_name = NULL_IF_CONFIG_SMALL("Uncompressed packed 4:2:0"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_SANM,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "sanm",
        .long_name = NULL_IF_CONFIG_SMALL("LucasArts SMUSH video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PAF_VIDEO,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "paf_video",
        .long_name = NULL_IF_CONFIG_SMALL("Amazing Studio Packed Animation File Video"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AVRN,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "avrn",
        .long_name = NULL_IF_CONFIG_SMALL("Avid AVI Codec"),
    },
    {
        .id        = AV_CODEC_ID_CPIA,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "cpia",
        .long_name = NULL_IF_CONFIG_SMALL("CPiA video format"),
    },
    {
        .id        = AV_CODEC_ID_XFACE,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "xface",
        .long_name = NULL_IF_CONFIG_SMALL("X-face image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_BRENDER_PIX,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "brender_pix",
        .long_name = NULL_IF_CONFIG_SMALL("BRender PIX image"),
        .props     = AV_CODEC_PROP_INTRA_ONLY | AV_CODEC_PROP_LOSSLESS,
    },

    /* various PCM "codecs" */
    {
        .id        = AV_CODEC_ID_PCM_S16LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s16le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 16-bit little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S16BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s16be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 16-bit big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_U16LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_u16le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM unsigned 16-bit little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_U16BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_u16be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM unsigned 16-bit big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S8,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s8",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 8-bit"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_U8,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_u8",
        .long_name = NULL_IF_CONFIG_SMALL("PCM unsigned 8-bit"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_MULAW,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_mulaw",
        .long_name = NULL_IF_CONFIG_SMALL("PCM mu-law / G.711 mu-law"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PCM_ALAW,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_alaw",
        .long_name = NULL_IF_CONFIG_SMALL("PCM A-law / G.711 A-law"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PCM_S32LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s32le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 32-bit little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S32BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s32be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 32-bit big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_U32LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_u32le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM unsigned 32-bit little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_U32BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_u32be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM unsigned 32-bit big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S24LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s24le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 24-bit little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S24BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s24be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 24-bit big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_U24LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_u24le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM unsigned 24-bit little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_U24BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_u24be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM unsigned 24-bit big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S24DAUD,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s24daud",
        .long_name = NULL_IF_CONFIG_SMALL("PCM D-Cinema audio signed 24-bit"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_ZORK,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_zork",
        .long_name = NULL_IF_CONFIG_SMALL("PCM Zork"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PCM_S16BE_PLANAR,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s16be_planar",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 16-bit big-endian planar"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S16LE_PLANAR,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s16le_planar",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 16-bit little-endian planar"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S24LE_PLANAR,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s24le_planar",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 24-bit little-endian planar"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_S32LE_PLANAR,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s32le_planar",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 32-bit little-endian planar"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_DVD,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_dvd",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 20|24-bit big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_F32BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_f32be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM 32-bit floating point big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_F32LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_f32le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM 32-bit floating point little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_F64BE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_f64be",
        .long_name = NULL_IF_CONFIG_SMALL("PCM 64-bit floating point big-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_F64LE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_f64le",
        .long_name = NULL_IF_CONFIG_SMALL("PCM 64-bit floating point little-endian"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_BLURAY,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_bluray",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 16|20|24-bit big-endian for Blu-ray media"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_PCM_LXF,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_lxf",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 20-bit little-endian planar"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_S302M,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "s302m",
        .long_name = NULL_IF_CONFIG_SMALL("SMPTE 302M"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_PCM_S8_PLANAR,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "pcm_s8_planar",
        .long_name = NULL_IF_CONFIG_SMALL("PCM signed 8-bit planar"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },

    /* various ADPCM codecs */
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_QT,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_qt",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA QuickTime"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_WAV,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_wav",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA WAV"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_DK3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_dk3",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Duck DK3"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_DK4,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_dk4",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Duck DK4"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_WS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_ws",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Westwood"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_SMJPEG,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_smjpeg",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Loki SDL MJPEG"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_MS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ms",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Microsoft"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_4XM,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_4xm",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM 4X Movie"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_XA,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_xa",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM CDROM XA"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_ADX,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_adx",
        .long_name = NULL_IF_CONFIG_SMALL("SEGA CRI ADX ADPCM"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_EA,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ea",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Electronic Arts"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_G726,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_g726",
        .long_name = NULL_IF_CONFIG_SMALL("G.726 ADPCM"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_CT,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ct",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Creative Technology"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_SWF,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_swf",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Shockwave Flash"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_YAMAHA,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_yamaha",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Yamaha"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_SBPRO_4,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_sbpro_4",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Sound Blaster Pro 4-bit"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_SBPRO_3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_sbpro_3",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Sound Blaster Pro 2.6-bit"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_SBPRO_2,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_sbpro_2",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Sound Blaster Pro 2-bit"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_THP,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_thp",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Nintendo Gamecube THP"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_AMV,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_amv",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA AMV"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_EA_R1,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ea_r1",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Electronic Arts R1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_EA_R3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ea_r3",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Electronic Arts R3"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_EA_R2,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ea_r2",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Electronic Arts R2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_EA_SEAD,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_ea_sead",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Electronic Arts SEAD"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_EA_EACS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_ea_eacs",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Electronic Arts EACS"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_EA_XAS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ea_xas",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Electronic Arts XAS"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_EA_MAXIS_XA,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ea_maxis_xa",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Electronic Arts Maxis CDROM XA"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_ISS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_iss",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Funcom ISS"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_G722,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_g722",
        .long_name = NULL_IF_CONFIG_SMALL("G.722 ADPCM"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_APC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_apc",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA CRYO APC"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_AFC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_afc",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM Nintendo Gamecube AFC"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ADPCM_IMA_OKI,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "adpcm_ima_oki",
        .long_name = NULL_IF_CONFIG_SMALL("ADPCM IMA Dialogic OKI"),
        .props     = AV_CODEC_PROP_LOSSY,
    },

    /* AMR */
    {
        .id        = AV_CODEC_ID_AMR_NB,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "amr_nb",
        .long_name = NULL_IF_CONFIG_SMALL("AMR-NB (Adaptive Multi-Rate NarrowBand)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AMR_WB,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "amr_wb",
        .long_name = NULL_IF_CONFIG_SMALL("AMR-WB (Adaptive Multi-Rate WideBand)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },

    /* RealAudio codecs*/
    {
        .id        = AV_CODEC_ID_RA_144,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "ra_144",
        .long_name = NULL_IF_CONFIG_SMALL("RealAudio 1.0 (14.4K)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RA_288,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "ra_288",
        .long_name = NULL_IF_CONFIG_SMALL("RealAudio 2.0 (28.8K)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },

    /* various DPCM codecs */
    {
        .id        = AV_CODEC_ID_ROQ_DPCM,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "roq_dpcm",
        .long_name = NULL_IF_CONFIG_SMALL("DPCM id RoQ"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_INTERPLAY_DPCM,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "interplay_dpcm",
        .long_name = NULL_IF_CONFIG_SMALL("DPCM Interplay"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_XAN_DPCM,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "xan_dpcm",
        .long_name = NULL_IF_CONFIG_SMALL("DPCM Xan"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SOL_DPCM,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "sol_dpcm",
        .long_name = NULL_IF_CONFIG_SMALL("DPCM Sol"),
        .props     = AV_CODEC_PROP_LOSSY,
    },

    /* audio codecs */
    {
        .id        = AV_CODEC_ID_MP2,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mp2",
        .long_name = NULL_IF_CONFIG_SMALL("MP2 (MPEG audio layer 2)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MP3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mp3",
        .long_name = NULL_IF_CONFIG_SMALL("MP3 (MPEG audio layer 3)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AAC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "aac",
        .long_name = NULL_IF_CONFIG_SMALL("AAC (Advanced Audio Coding)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AC3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "ac3",
        .long_name = NULL_IF_CONFIG_SMALL("ATSC A/52A (AC-3)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_DTS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "dts",
        .long_name  = NULL_IF_CONFIG_SMALL("DCA (DTS Coherent Acoustics)"),
        .props     = AV_CODEC_PROP_LOSSY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_VORBIS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "vorbis",
        .long_name  = NULL_IF_CONFIG_SMALL("Vorbis"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_DVAUDIO,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "dvaudio",
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMAV1,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "wmav1",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Audio 1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMAV2,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "wmav2",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Audio 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MACE3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mace3",
        .long_name = NULL_IF_CONFIG_SMALL("MACE (Macintosh Audio Compression/Expansion) 3:1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MACE6,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mace6",
        .long_name = NULL_IF_CONFIG_SMALL("MACE (Macintosh Audio Compression/Expansion) 6:1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VMDAUDIO,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "vmdaudio",
        .long_name = NULL_IF_CONFIG_SMALL("Sierra VMD audio"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FLAC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "flac",
        .long_name = NULL_IF_CONFIG_SMALL("FLAC (Free Lossless Audio Codec)"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MP3ADU,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mp3adu",
        .long_name = NULL_IF_CONFIG_SMALL("ADU (Application Data Unit) MP3 (MPEG audio layer 3)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MP3ON4,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mp3on4",
        .long_name = NULL_IF_CONFIG_SMALL("MP3onMP4"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SHORTEN,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "shorten",
        .long_name = NULL_IF_CONFIG_SMALL("Shorten"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_ALAC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "alac",
        .long_name = NULL_IF_CONFIG_SMALL("ALAC (Apple Lossless Audio Codec)"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_WESTWOOD_SND1,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "westwood_snd1",
        .long_name = NULL_IF_CONFIG_SMALL("Westwood Audio (SND1)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_GSM,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "gsm",
        .long_name = NULL_IF_CONFIG_SMALL("GSM"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_QDM2,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "qdm2",
        .long_name = NULL_IF_CONFIG_SMALL("QDesign Music Codec 2"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_COOK,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "cook",
        .long_name = NULL_IF_CONFIG_SMALL("Cook / Cooker / Gecko (RealAudio G2)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TRUESPEECH,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "truespeech",
        .long_name = NULL_IF_CONFIG_SMALL("DSP Group TrueSpeech"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TTA,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "tta",
        .long_name = NULL_IF_CONFIG_SMALL("TTA (True Audio)"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_SMACKAUDIO,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "smackaudio",
        .long_name = NULL_IF_CONFIG_SMALL("Smacker audio"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_QCELP,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "qcelp",
        .long_name = NULL_IF_CONFIG_SMALL("QCELP / PureVoice"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WAVPACK,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "wavpack",
        .long_name = NULL_IF_CONFIG_SMALL("WavPack"),
        .props     = AV_CODEC_PROP_LOSSY | AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_DSICINAUDIO,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "dsicinaudio",
        .long_name = NULL_IF_CONFIG_SMALL("Delphine Software International CIN audio"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_IMC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "imc",
        .long_name = NULL_IF_CONFIG_SMALL("IMC (Intel Music Coder)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MUSEPACK7,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "musepack7",
        .long_name = NULL_IF_CONFIG_SMALL("Musepack SV7"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MLP,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mlp",
        .long_name = NULL_IF_CONFIG_SMALL("MLP (Meridian Lossless Packing)"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_GSM_MS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "gsm_ms",
        .long_name = NULL_IF_CONFIG_SMALL("GSM Microsoft variant"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ATRAC3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "atrac3",
        .long_name = NULL_IF_CONFIG_SMALL("Atrac 3 (Adaptive TRansform Acoustic Coding 3)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VOXWARE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "voxware",
        .long_name = NULL_IF_CONFIG_SMALL("Voxware RT29 Metasound"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_APE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "ape",
        .long_name = NULL_IF_CONFIG_SMALL("Monkey's Audio"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_NELLYMOSER,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "nellymoser",
        .long_name = NULL_IF_CONFIG_SMALL("Nellymoser Asao"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MUSEPACK8,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "musepack8",
        .long_name = NULL_IF_CONFIG_SMALL("Musepack SV8"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SPEEX,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "speex",
        .long_name = NULL_IF_CONFIG_SMALL("Speex"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMAVOICE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "wmavoice",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Audio Voice"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMAPRO,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "wmapro",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Audio 9 Professional"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_WMALOSSLESS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "wmalossless",
        .long_name = NULL_IF_CONFIG_SMALL("Windows Media Audio Lossless"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_ATRAC3P,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "atrac3p",
        .long_name = NULL_IF_CONFIG_SMALL("Sony ATRAC3+"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_EAC3,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "eac3",
        .long_name = NULL_IF_CONFIG_SMALL("ATSC A/52B (AC-3, E-AC-3)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SIPR,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "sipr",
        .long_name = NULL_IF_CONFIG_SMALL("RealAudio SIPR / ACELP.NET"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_MP1,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mp1",
        .long_name = NULL_IF_CONFIG_SMALL("MP1 (MPEG audio layer 1)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TWINVQ,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "twinvq",
        .long_name = NULL_IF_CONFIG_SMALL("VQF TwinVQ"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TRUEHD,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "truehd",
        .long_name = NULL_IF_CONFIG_SMALL("TrueHD"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_MP4ALS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "mp4als",
        .long_name = NULL_IF_CONFIG_SMALL("MPEG-4 Audio Lossless Coding (ALS)"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_ATRAC1,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "atrac1",
        .long_name = NULL_IF_CONFIG_SMALL("Atrac 1 (Adaptive TRansform Acoustic Coding)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_BINKAUDIO_RDFT,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "binkaudio_rdft",
        .long_name = NULL_IF_CONFIG_SMALL("Bink Audio (RDFT)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_BINKAUDIO_DCT,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "binkaudio_dct",
        .long_name = NULL_IF_CONFIG_SMALL("Bink Audio (DCT)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_AAC_LATM,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "aac_latm",
        .long_name = NULL_IF_CONFIG_SMALL("AAC LATM (Advanced Audio Coding LATM syntax)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_QDMC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "qdmc",
        .long_name = NULL_IF_CONFIG_SMALL("QDesign Music"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_CELT,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "celt",
        .long_name = NULL_IF_CONFIG_SMALL("Constrained Energy Lapped Transform (CELT)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_G723_1,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "g723_1",
        .long_name = NULL_IF_CONFIG_SMALL("G.723.1"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_G729,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "g729",
        .long_name = NULL_IF_CONFIG_SMALL("G.729"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_8SVX_EXP,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "8svx_exp",
        .long_name = NULL_IF_CONFIG_SMALL("8SVX exponential"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_8SVX_FIB,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "8svx_fib",
        .long_name = NULL_IF_CONFIG_SMALL("8SVX fibonacci"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_BMV_AUDIO,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "bmv_audio",
        .long_name = NULL_IF_CONFIG_SMALL("Discworld II BMV audio"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_RALF,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "ralf",
        .long_name = NULL_IF_CONFIG_SMALL("RealAudio Lossless"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_IAC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "iac",
        .long_name = NULL_IF_CONFIG_SMALL("IAC (Indeo Audio Coder)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_ILBC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "ilbc",
        .long_name = NULL_IF_CONFIG_SMALL("iLBC (Internet Low Bitrate Codec)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_VIMA,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "vima",
        .long_name = NULL_IF_CONFIG_SMALL("LucasArts VIMA audio"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_FFWAVESYNTH,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "wavesynth",
        .long_name = NULL_IF_CONFIG_SMALL("Wave synthesis pseudo-codec"),
    },
    {
        .id        = AV_CODEC_ID_SONIC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "sonic",
        .long_name = NULL_IF_CONFIG_SMALL("Sonic"),
    },
    {
        .id        = AV_CODEC_ID_SONIC_LS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "sonicls",
        .long_name = NULL_IF_CONFIG_SMALL("Sonic lossless"),
    },
    {
        .id        = AV_CODEC_ID_PAF_AUDIO,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "paf_audio",
        .long_name = NULL_IF_CONFIG_SMALL("Amazing Studio Packed Animation File Audio"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_OPUS,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "opus",
        .long_name = NULL_IF_CONFIG_SMALL("Opus (Opus Interactive Audio Codec)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_COMFORT_NOISE,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "comfortnoise",
        .long_name = NULL_IF_CONFIG_SMALL("RFC 3389 Comfort Noise"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_TAK,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "tak",
        .long_name = NULL_IF_CONFIG_SMALL("TAK (Tom's lossless Audio Kompressor)"),
        .props     = AV_CODEC_PROP_LOSSLESS,
    },
    {
        .id        = AV_CODEC_ID_EVRC,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "evrc",
        .long_name = NULL_IF_CONFIG_SMALL("EVRC (Enhanced Variable Rate Codec)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },
    {
        .id        = AV_CODEC_ID_SMV,
        .type      = AVMEDIA_TYPE_AUDIO,
        .name      = "smv",
        .long_name = NULL_IF_CONFIG_SMALL("SMV (Selectable Mode Vocoder)"),
        .props     = AV_CODEC_PROP_LOSSY,
    },

    /* subtitle codecs */
    {
        .id        = AV_CODEC_ID_DVD_SUBTITLE,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "dvd_subtitle",
        .long_name = NULL_IF_CONFIG_SMALL("DVD subtitles"),
        .props     = AV_CODEC_PROP_BITMAP_SUB,
    },
    {
        .id        = AV_CODEC_ID_DVB_SUBTITLE,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "dvb_subtitle",
        .long_name = NULL_IF_CONFIG_SMALL("DVB subtitles"),
        .props     = AV_CODEC_PROP_BITMAP_SUB,
    },
    {
        .id        = AV_CODEC_ID_TEXT,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "text",
        .long_name = NULL_IF_CONFIG_SMALL("raw UTF-8 text"),
    },
    {
        .id        = AV_CODEC_ID_XSUB,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "xsub",
        .long_name = NULL_IF_CONFIG_SMALL("XSUB"),
        .props     = AV_CODEC_PROP_BITMAP_SUB,
    },
    {
        .id        = AV_CODEC_ID_SSA,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "ssa",
        .long_name = NULL_IF_CONFIG_SMALL("SSA (SubStation Alpha) / ASS (Advanced SSA) subtitle"),
    },
    {
        .id        = AV_CODEC_ID_MOV_TEXT,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "mov_text",
        .long_name = NULL_IF_CONFIG_SMALL("MOV text"),
    },
    {
        .id        = AV_CODEC_ID_HDMV_PGS_SUBTITLE,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "hdmv_pgs_subtitle",
        .long_name = NULL_IF_CONFIG_SMALL("HDMV Presentation Graphic Stream subtitles"),
        .props     = AV_CODEC_PROP_BITMAP_SUB,
    },
    {
        .id        = AV_CODEC_ID_DVB_TELETEXT,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "dvb_teletext",
        .long_name = NULL_IF_CONFIG_SMALL("DVB teletext"),
    },
    {
        .id        = AV_CODEC_ID_SRT,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "srt",
        .long_name = NULL_IF_CONFIG_SMALL("SubRip subtitle with embedded timing"),
    },
    {
        .id        = AV_CODEC_ID_SUBRIP,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "subrip",
        .long_name = NULL_IF_CONFIG_SMALL("SubRip subtitle"),
    },
    {
        .id        = AV_CODEC_ID_MICRODVD,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "microdvd",
        .long_name = NULL_IF_CONFIG_SMALL("MicroDVD subtitle"),
    },
    {
        .id        = AV_CODEC_ID_MPL2,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "mpl2",
        .long_name = NULL_IF_CONFIG_SMALL("MPL2 subtitle"),
    },
    {
        .id        = AV_CODEC_ID_EIA_608,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "eia_608",
        .long_name = NULL_IF_CONFIG_SMALL("EIA-608 closed captions"),
    },
    {
        .id        = AV_CODEC_ID_JACOSUB,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "jacosub",
        .long_name = NULL_IF_CONFIG_SMALL("JACOsub subtitle"),
    },
    {
        .id        = AV_CODEC_ID_PJS,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "pjs",
        .long_name = NULL_IF_CONFIG_SMALL("PJS (Phoenix Japanimation Society) subtitle"),
    },
    {
        .id        = AV_CODEC_ID_SAMI,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "sami",
        .long_name = NULL_IF_CONFIG_SMALL("SAMI subtitle"),
    },
    {
        .id        = AV_CODEC_ID_REALTEXT,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "realtext",
        .long_name = NULL_IF_CONFIG_SMALL("RealText subtitle"),
    },
    {
        .id        = AV_CODEC_ID_SUBVIEWER1,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "subviewer1",
        .long_name = NULL_IF_CONFIG_SMALL("SubViewer v1 subtitle"),
    },
    {
        .id        = AV_CODEC_ID_SUBVIEWER,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "subviewer",
        .long_name = NULL_IF_CONFIG_SMALL("SubViewer subtitle"),
    },
    {
        .id        = AV_CODEC_ID_VPLAYER,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "vplayer",
        .long_name = NULL_IF_CONFIG_SMALL("VPlayer subtitle"),
    },
    {
        .id        = AV_CODEC_ID_WEBVTT,
        .type      = AVMEDIA_TYPE_SUBTITLE,
        .name      = "webvtt",
        .long_name = NULL_IF_CONFIG_SMALL("WebVTT subtitle"),
    },
    {
        .id        = AV_CODEC_ID_BINTEXT,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "bintext",
        .long_name = NULL_IF_CONFIG_SMALL("Binary text"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_XBIN,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "xbin",
        .long_name = NULL_IF_CONFIG_SMALL("eXtended BINary text"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_IDF,
        .type      = AVMEDIA_TYPE_VIDEO,
        .name      = "idf",
        .long_name = NULL_IF_CONFIG_SMALL("iCEDraw text"),
        .props     = AV_CODEC_PROP_INTRA_ONLY,
    },
    {
        .id        = AV_CODEC_ID_SMPTE_KLV,
        .type      = AVMEDIA_TYPE_DATA,
        .name      = "klv",
        .long_name = NULL_IF_CONFIG_SMALL("SMPTE 336M Key-Length-Value (KLV) metadata"),
    },

};

static enum AVCodecID remap_deprecated_codec_id(enum AVCodecID id)
{
    switch(id){
        //This is for future deprecatec codec ids, its empty since
        //last major bump but will fill up again over time, please don't remove it
//         case AV_CODEC_ID_UTVIDEO_DEPRECATED: return AV_CODEC_ID_UTVIDEO;
        case AV_CODEC_ID_OPUS_DEPRECATED: return AV_CODEC_ID_OPUS;
        case AV_CODEC_ID_TAK_DEPRECATED : return AV_CODEC_ID_TAK;
        default                         : return id;
    }
}


inline AVCodec *avcodec_find_encoder(enum AVCodecID id)
{
    AVCodec *p, *experimental = NULL;
    p = first_avcodec;
    printf("first ID:%d\n",id);
    id= remap_deprecated_codec_id(id);
    while (p) {
        if (av_codec_is_encoder(p) &&
            p->id == id) {
            if ((p->capabilities & CODEC_CAP_EXPERIMENTAL) && !experimental) {
                experimental = p;
            } else
                return p;
        }
        printf("PID:%d\n",p->id);
        p = p->next;
    }
    return experimental;
}


AVCodec *avcodec_find_decoder(enum AVCodecID id, int encoder)
{
    AVCodec *p, *experimental = NULL;
    p = first_avcodec;
    id= remap_deprecated_codec_id(id);
    while (p) {
        if ((av_codec_is_decoder(p)) &&
            p->id == id) {
            if ((p->capabilities & CODEC_CAP_EXPERIMENTAL) && !experimental) {
                experimental = p;
            } else
                return p;
        }
        p = p->next;
    }
    return experimental;
}

static int is_hwaccel_pix_fmt(enum AVPixelFormat pix_fmt)
{
    const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(pix_fmt);
    return desc->flags & PIX_FMT_HWACCEL;
}

enum AVPixelFormat avcodec_default_get_format(struct AVCodecContext *s, const enum AVPixelFormat *fmt)
{
    while (*fmt != AV_PIX_FMT_NONE && is_hwaccel_pix_fmt(*fmt))
        ++fmt;
    return fmt[0];
}

const char *av_get_pix_fmt_name(enum AVPixelFormat pix_fmt)
{
    return (unsigned)pix_fmt < AV_PIX_FMT_NB ?
        av_pix_fmt_descriptors[pix_fmt].name : NULL;
}



int av_image_get_linesize(enum AVPixelFormat pix_fmt, int width, int plane)
{
    const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(pix_fmt);
    int max_step     [4];       /* max pixel step for each plane */
    int max_step_comp[4];       /* the component for each plane which has the max pixel step */

    if ((unsigned)pix_fmt >= AV_PIX_FMT_NB || (desc->flags & PIX_FMT_HWACCEL))
        return -1;

    av_image_fill_max_pixsteps(max_step, max_step_comp, desc);
    return image_get_linesize(width, plane, max_step[plane], max_step_comp[plane], desc);
}


void av_picture_copy(AVPicture *dst, const AVPicture *src,
                     enum AVPixelFormat pix_fmt, int width, int height)
{
    av_image_copy(dst->data, dst->linesize, (const uint8_t **)src->data,
                  src->linesize, pix_fmt, width, height);
}

int avcodec_default_reget_buffer(AVCodecContext *s, AVFrame *pic)
{
    AVFrame temp_pic;
    int i, ret;

//    av_assert0(s->codec_type == AVMEDIA_TYPE_VIDEO);

    if (pic->data[0] && (pic->width != s->width || pic->height != s->height || pic->format != s->pix_fmt)) {
        printf( "Picture changed from size:%dx%d fmt:%s to size:%dx%d fmt:%s in reget buffer()\n", pic->width, pic->height, av_get_pix_fmt_name(pic->format), s->width, s->height, av_get_pix_fmt_name(s->pix_fmt));
        s->release_buffer(s, pic);
    }

    ff_init_buffer_info(s, pic);

    /* If no picture return a new buffer */
    if (pic->data[0] == NULL) {
        /* We will copy from buffer, so must be readable */
        pic->buffer_hints |= FF_BUFFER_HINTS_READABLE;
        return ff_get_buffer(s, pic);
    }

//    assert(s->pix_fmt == pic->format);

    /* If internal buffer type return the same buffer */
    if (pic->type == FF_BUFFER_TYPE_INTERNAL) {
        return 0;
    }

    /*
     * Not internal type and reget_buffer not overridden, emulate cr buffer
     */
    temp_pic = *pic;
    for (i = 0; i < AV_NUM_DATA_POINTERS; i++)
        pic->data[i] = pic->base[i] = NULL;
    pic->opaque = NULL;
    /* Allocate new frame */
    if ((ret = ff_get_buffer(s, pic)))
        return ret;
    /* Copy image data from old buffer to new buffer */
    av_picture_copy((AVPicture *)pic, (AVPicture *)&temp_pic, s->pix_fmt, s->width,
                    s->height);
    s->release_buffer(s, &temp_pic); // Release old frame
    return 0;
}

int avcodec_default_execute(AVCodecContext *c, int (*func)(AVCodecContext *c2, void *arg2), void *arg, int *ret, int count, int size)
{
//	LOGD("avcodec_default_execute Called\n");
    int i;
    //第3引数のargがMpegEncContextにキャストされる
//    MpegEncContext *s = c->priv_data;
//    LOGD( "PPP %d \n",s->dsp.pix_sum);

    for (i = 0; i < count; i++) {
        int r = func(c, (char *)arg + i * size);
        if (ret)
            ret[i] = r;
    }
    return 0;
}

int avcodec_default_execute2(AVCodecContext *c, int (*func)(AVCodecContext *c2, void *arg2, int jobnr, int threadnr), void *arg, int *ret, int count)
{
	LOGD("codec_base.c/avcodec_default_execute2 Called\n");
    int i;

    for (i = 0; i < count; i++) {
        int r = func(c, arg, i, 0);
        if (ret)
            ret[i] = r;
    }
    return 0;
}



AVRational av_d2q(double d, int max)
{
    AVRational a;
#define LOG2  0.69314718055994530941723212145817656807550013436025
    int exponent;
    int64_t den;
    if (isnan(d))
        return (AVRational) { 0,0 };
    if (isinf(d))
        return (AVRational) { d < 0 ? -1 : 1, 0 };
    exponent = FFMAX( (int)(log(fabs(d) + 1e-20)/LOG2), 0);
    den = 1LL << (61 - exponent);
    av_reduce(&a.num, &a.den, (int64_t)(d * den + 0.5), den, max);

    return a;
}


int avcodec_set_context_defaults3(AVCodecContext *s, const AVCodec *codec)
{
//    int flags=0;
//
//    s->codec_type = codec ? codec->type : AVMEDIA_TYPE_UNKNOWN;
//    if(s->codec_type == AVMEDIA_TYPE_AUDIO)
//        flags= AV_OPT_FLAG_AUDIO_PARAM;
//    else if(s->codec_type == AVMEDIA_TYPE_VIDEO)
//        flags= AV_OPT_FLAG_VIDEO_PARAM;
//    else if(s->codec_type == AVMEDIA_TYPE_SUBTITLE)
//        flags= AV_OPT_FLAG_SUBTITLE_PARAM;
//    av_opt_set_defaults2(s, flags, flags);//ここはオプション入れるのが面倒なのでテストの仕方を考えながら代替策を実装

    s->get_buffer          = avcodec_default_get_buffer;
    s->release_buffer      = avcodec_default_release_buffer;
    s->get_format          = avcodec_default_get_format;
    s->execute             = avcodec_default_execute;
//    s->execute2            = avcodec_default_execute2;
    s->sample_aspect_ratio = (AVRational){0,1};
    s->pix_fmt             = AV_PIX_FMT_NONE;
    s->sample_fmt          = AV_SAMPLE_FMT_NONE;
    s->timecode_frame_start = -1;

    s->reget_buffer        = avcodec_default_reget_buffer;
    s->reordered_opaque    = AV_NOPTS_VALUE;

//    LOGD("avcodec_get_context_defaults3 codec : %x\n",codec);
//    LOGD("avcodec_get_context_defaults3 codec->defaults : %x\n",codec->defaults);
//    LOGD("avcodec_get_context_defaults3 codec->defaults : %x\n",codec->defaults->key);
//    LOGD("avcodec_get_context_defaults3 codec->defaults : %x\n",codec->defaults->value);
//    LOGD("avcodec_get_context_defaults3 priv_data_size : %d\n",codec->priv_data_size);
//    if(codec && codec->priv_data_size){
//        if(!s->priv_data){
//            s->priv_data= av_mallocz(codec->priv_data_size);
//            if (!s->priv_data) {
//                return -1;
//            }
//        }
//    }
    return 0;
}




