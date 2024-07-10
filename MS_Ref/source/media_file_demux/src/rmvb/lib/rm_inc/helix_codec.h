

#ifndef HELIX_CODEC_H
#define HELIX_CODEC_H

#include "helix_types.h"
#include "helix_result.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

/* HX_MOF base Media object format struct */
typedef struct tag_HX_MOF
{
    UINT32      cbLength;    /* size of structure in bytes        */
    HX_MOFTAG   moftag;      /* identifier of media format        */
    HX_MOFTAG   submoftag;   /* identifier of object format       */
} HX_MOF;

/* Media Native HX_MOF struct */
typedef struct tag_HX_FORMAT_NATIVE
{
    UINT32      cbLength;    /* the size of this struct in bytes  */
    HX_MOFTAG   moftag;      /* always == HX_MEDIA_NATIVE         */
    HX_MOFTAG   submoftag;   /* media format identifier           */
    UINT8       data[1];     /* format native initialization data */
} HX_FORMAT_NATIVE;

/* Generic Audio HX_MOF struct */
typedef struct tag_HX_FORMAT_AUDIO
{
    UINT32      cbLength;           /* the size of this struct in bytes   */
    HX_MOFTAG   moftag;             /* always == HX_MEDIA_AUDIO           */
    HX_MOFTAG   submoftag;          /* audio format identifier            */
/* General attributes of the audio stream independent of bitwise encoding */
    UINT16      uiBitsPerSample;    /* number of bits per audio sample    */
    UINT16      uiNumChannels;      /* number of audio channels in stream */
    UINT16      uiBytesPerSample;   /* number of bytes per sample         */
    UINT32      ulSampleRate;       /* audio sample rate                  */
    UINT32      ulAvgBytesPerSec;   /* average bytes/sec of audio stream  */
} HX_FORMAT_AUDIO;

/* Generic Image HX_MOF struct */
typedef struct tag_HX_FORMAT_IMAGE
{
    UINT32        cbLength;         /* the size of this struct in bytes   */
    HX_MOFTAG     moftag;           /* always == HX_MEDIA_IMAGE           */
    HX_MOFTAG     submoftag;        /* image format identifier            */
/* General attributes of the image stream independent of bitwise encoding */
    UINT16        uiWidth;          /* width of the image in pixels       */
    UINT16        uiHeight;         /* height of the image in pixels      */
    UINT16        uiBitCount;       /* color depth in bits                */
    UINT16        uiPadWidth;       /* # of padded columns for codecs that */
                                    /* need certian block sizes e.g. 8x8  */
    UINT16        uiPadHeight;      /* # of padded rows for codecs that   */
                                    /* need certian block sizes e.g. 8x8  */
} HX_FORMAT_IMAGE;

/* Generic Image HX_MOF struct */
typedef struct tag_HX_FORMAT_IMAGE2
{
    UINT32        cbLength;         /* the size of this struct in bytes   */
    HX_MOFTAG     moftag;           /* always == HX_MEDIA_IMAGE           */
    HX_MOFTAG     submoftag;        /* image format identifier            */
/* General attributes of the image stream independent of bitwise encoding */
    UINT16        uiWidth;          /* width of the image in pixels       */
    UINT16        uiHeight;         /* height of the image in pixels      */
    UINT16        uiBitCount;       /* color depth in bits                */
    UINT16        uiPadWidthLeft;   /* # of padded columns for codecs that */
                                    /* need certian block sizes e.g. 8x8  */
    UINT16        uiPadHeightTop;   /* # of padded rows for codecs that   */
    UINT16        uiPadWidthRight;  /* # of padded columns for codecs that */
                                    /* need certian block sizes e.g. 8x8  */
    UINT16        uiPadHeightBottom;/* # of padded rows for codecs that   */
                                    /* need certian block sizes e.g. 8x8  */
} HX_FORMAT_IMAGE2;

/* Generic Video HX_MOF struct */
typedef struct tag_HX_FORMAT_VIDEO
{
    UINT32        cbLength;         /* the size of this struct in bytes   */
    HX_MOFTAG     moftag;           /* always == HX_MEDIA_VIDEO           */
    HX_MOFTAG     submoftag;        /* video format identifier            */
/* General attributes of the video stream independent of bitwise encoding */
    UINT16        uiWidth;          /* width of the image in pixels       */
    UINT16        uiHeight;         /* height of the image in pixels      */
    UINT16        uiBitCount;       /* color depth in bits                */
    UINT16        uiPadWidth;       /* # of padded columns for codecs that */
                                    /* need certian block sizes e.g. 8x8  */
    UINT16        uiPadHeight;      /* # of padded rows for codecs that   */
                                    /* need certian block sizes e.g. 8x8  */
    UFIXED32      framesPerSecond;  /* frames per second                  */

} HX_FORMAT_VIDEO;


/* Unfortunately we did not 4 byte align the HX_FORMAT_VIDEO struct. Since
 * this struct must be 26 bytes in size we cannot do sizeof(HX_FORMAT_VIDEO)
 * on several UNIX platforms and PowerPC macs. We are therefore going to
 * hardcode the size to 26 bytes.
 */
#define SIZE_OF_HX_FORMAT_VIDEO 26


/* Segment info struct */
typedef struct tag_HXCODEC_SEGMENTINFO
{
    INT32  bIsValid;
    UINT32 ulSegmentOffset;
} HXCODEC_SEGMENTINFO;

#define HXCODEC_SEGMENTINFO_SIZE    (8)


/* HXCODEC_DATA struct */
typedef struct tag_HXCODEC_DATA
{
    UINT32      dataLength;
    UCHAR      *data;
    UINT32      timestamp;
    UINT16      sequenceNum;
    UINT16      flags;
    BOOL        lastPacket;
    UINT32      numSegments;
    HXCODEC_SEGMENTINFO *Segments;
} HXCODEC_DATA;

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef HELIX_CODEC_H */
