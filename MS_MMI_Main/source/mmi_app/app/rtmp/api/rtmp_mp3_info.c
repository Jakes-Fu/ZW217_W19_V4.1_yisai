#include "rtmp_mp3_info.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

#define HDR_SIZE                    4
#define MAX_FREE_FORMAT_FRAME_SIZE  2304    /* more than ISO spec's */
#ifndef MAX_FRAME_SYNC_MATCHES
#define MAX_FRAME_SYNC_MATCHES      10
#endif /* MAX_FRAME_SYNC_MATCHES */

#define MAX_L3_FRAME_PAYLOAD_BYTES  MAX_FREE_FORMAT_FRAME_SIZE /* MUST be >= 320000/8/32000*1152 = 1440 */

#define MAX_BITRESERVOIR_BYTES      511
#define SHORT_BLOCK_TYPE            2
#define STOP_BLOCK_TYPE             3
#define MODE_MONO                   3
#define MODE_JOINT_STEREO           1
#define HDR_SIZE                    4
#define HDR_IS_MONO(h)              (((h[3]) & 0xC0) == 0xC0)
#define HDR_IS_MS_STEREO(h)         (((h[3]) & 0xE0) == 0x60)
#define HDR_IS_FREE_FORMAT(h)       (((h[2]) & 0xF0) == 0)
#define HDR_IS_CRC(h)               (!((h[1]) & 1))
#define HDR_TEST_PADDING(h)         ((h[2]) & 0x2)
#define HDR_TEST_MPEG1(h)           ((h[1]) & 0x8)
#define HDR_TEST_NOT_MPEG25(h)      ((h[1]) & 0x10)
#define HDR_TEST_I_STEREO(h)        ((h[3]) & 0x10)
#define HDR_TEST_MS_STEREO(h)       ((h[3]) & 0x20)
#define HDR_GET_STEREO_MODE(h)      (((h[3]) >> 6) & 3)
#define HDR_GET_STEREO_MODE_EXT(h)  (((h[3]) >> 4) & 3)
#define HDR_GET_LAYER(h)            (((h[1]) >> 1) & 3)
#define HDR_GET_BITRATE(h)          ((h[2]) >> 4)
#define HDR_GET_SAMPLE_RATE(h)      (((h[2]) >> 2) & 3)
#define HDR_GET_MY_SAMPLE_RATE(h)   (HDR_GET_SAMPLE_RATE(h) + (((h[1] >> 3) & 1) + ((h[1] >> 4) & 1))*3)
#define HDR_IS_FRAME_576(h)         ((h[1] & 14) == 2)
#define HDR_IS_LAYER_1(h)           ((h[1] & 6) == 6)

static int hdr_valid(const uint8_t *h)
{
	return h[0] == 0xff &&
		((h[1] & 0xF0) == 0xf0 || (h[1] & 0xFE) == 0xe2) &&
		(HDR_GET_LAYER(h) != 0) &&
		(HDR_GET_BITRATE(h) != 15) &&
		(HDR_GET_SAMPLE_RATE(h) != 3);
}

static int hdr_compare(const uint8_t *h1, const uint8_t *h2)
{
	return hdr_valid(h2) &&
		((h1[1] ^ h2[1]) & 0xFE) == 0 &&
		((h1[2] ^ h2[2]) & 0x0C) == 0 &&
		!(HDR_IS_FREE_FORMAT(h1) ^ HDR_IS_FREE_FORMAT(h2));
}

static unsigned hdr_bitrate_kbps(const uint8_t *h)
{
	static const uint8_t halfrate[2][3][15] = {
		{ { 0,4,8,12,16,20,24,28,32,40,48,56,64,72,80 }, { 0,4,8,12,16,20,24,28,32,40,48,56,64,72,80 }, { 0,16,24,28,32,40,48,56,64,72,80,88,96,112,128 } },
		{ { 0,16,20,24,28,32,40,48,56,64,80,96,112,128,160 }, { 0,16,24,28,32,40,48,56,64,80,96,112,128,160,192 }, { 0,16,32,48,64,80,96,112,128,144,160,176,192,208,224 } },
	};
	return 2 * halfrate[!!HDR_TEST_MPEG1(h)][HDR_GET_LAYER(h) - 1][HDR_GET_BITRATE(h)];
}

static unsigned hdr_sample_rate_hz(const uint8_t *h)
{
	static const unsigned g_hz[3] = { 44100, 48000, 32000 };
	return g_hz[HDR_GET_SAMPLE_RATE(h)] >> (int)!HDR_TEST_MPEG1(h) >> (int)!HDR_TEST_NOT_MPEG25(h);
}

static unsigned hdr_frame_samples(const uint8_t *h)
{
	return HDR_IS_LAYER_1(h) ? 384 : (1152 >> (int)HDR_IS_FRAME_576(h));
}

static int hdr_frame_bytes(const uint8_t *h, int free_format_size)
{
	int frameBytes = hdr_frame_samples(h)*hdr_bitrate_kbps(h) * 125 / hdr_sample_rate_hz(h);
	if (HDR_IS_LAYER_1(h))
	{
		frameBytes &= ~3; /* slot align */
	}
	return frameBytes ? frameBytes : free_format_size;
}

static int hdr_padding(const uint8_t *h)
{
	return HDR_TEST_PADDING(h) ? (HDR_IS_LAYER_1(h) ? 4 : 1) : 0;
}

static int mp3d_match_frame(const uint8_t *hdr, int mp3_bytes, int frameBytes)
{
	int i, nmatch;
	for (i = 0, nmatch = 0; nmatch < MAX_FRAME_SYNC_MATCHES; nmatch++)
	{
		i += hdr_frame_bytes(hdr + i, frameBytes) + hdr_padding(hdr + i);
		if (i + HDR_SIZE > mp3_bytes)
			return nmatch > 0;
		if (!hdr_compare(hdr, hdr + i))
			return 0;
	}
	return 1;
}

static int mp3d_find_frame(const uint8_t *mp3, int mp3_bytes, int *free_format_bytes, int *ptr_frame_bytes)
{
	int i, k;
	for (i = 0; i < mp3_bytes - HDR_SIZE; i++, mp3++)
	{
		if (hdr_valid(mp3))
		{
			int frameBytes = hdr_frame_bytes(mp3, *free_format_bytes);
			int frame_and_padding = frameBytes + hdr_padding(mp3);

			for (k = HDR_SIZE; !frameBytes && k < MAX_FREE_FORMAT_FRAME_SIZE && i + 2 * k < mp3_bytes - HDR_SIZE; k++)
			{
				if (hdr_compare(mp3, mp3 + k))
				{
					int fb = k - hdr_padding(mp3);
					int nextfb = fb + hdr_padding(mp3 + k);
					if (i + k + nextfb + HDR_SIZE > mp3_bytes || !hdr_compare(mp3, mp3 + k + nextfb))
						continue;
					frame_and_padding = k;
					frameBytes = fb;
					*free_format_bytes = fb;
				}
			}
			if ((frameBytes && i + frame_and_padding <= mp3_bytes &&
				mp3d_match_frame(mp3, mp3_bytes - i, frameBytes)) ||
				(!i && frame_and_padding == mp3_bytes))
			{
				*ptr_frame_bytes = frame_and_padding;
				return i;
			}
			*free_format_bytes = 0;
		}
	}
	*ptr_frame_bytes = 0;
	return mp3_bytes;
}

int rtmp_mp3_id3_length(const char *mp3, const int mp3_bytes)
{
	int totalSize = 0;

	if (mp3_bytes < 10) 
		return 0;

	if (mp3[0] != 'I' || mp3[1] != 'D' || mp3[2] != '3')
		return 0;


	totalSize =	(mp3[6] & 0x7F) * 0x200000 + (mp3[7] & 0x7F) * 0x4000 + (mp3[8] & 0x7F) * 0x80 + (mp3[9] & 0x7F) + 10;
	return totalSize;
}

int rtmp_mp3_decode_frame_info(const char *mp3, const int mp3_bytes, rtmp_mp3_frame_info_t *info)
{
	int i = 0, frame_size = 0, success = 1;
	const char *hdr;
	int free_format_bytes = 0;

	i = mp3d_find_frame(mp3, mp3_bytes, &free_format_bytes, &frame_size);
	if (!frame_size || i + frame_size > mp3_bytes)
	{
		info->frameBytes = i;
		return 0;
	}

	hdr = mp3 + i;
	info->frameBytes = i + frame_size;
	info->frameOffset = i;
	info->channels = HDR_IS_MONO(hdr) ? 1 : 2;
	info->hz = hdr_sample_rate_hz(hdr);
	info->layer = 4 - HDR_GET_LAYER(hdr);
	info->bitrateKbps = hdr_bitrate_kbps(hdr);

	return hdr_frame_samples(hdr);
}

#if 0

#define  _CRT_SECURE_NO_WARNINGS

int main(int argc, char ** argv)
{
	(void)argc;    /*Unused*/
	(void)argv;    /*Unused*/

	rtmp_mp3_frame_info_t info;
	int id3v2_length = 0;

	char buffer[8192 * 100];
	long filesize = 0;

	FILE* f;

	int samples = 0;
	int count = 0;
	float ms = 0;

	fopen_s(&f, "Z:\\1.mp3", "rb");
	fread(buffer, sizeof(char), 8192 * 100, f);

	if (0 == fseek(f, 0, SEEK_END)) {
		filesize = ftell(f);
	}
	fclose(f);

	id3v2_length = rtmp_mp3_id3_length(buffer, 8192 * 100);
	printf("file size = %d\n", filesize);
	printf("id3 total size = %d\n", id3v2_length);
	
	samples = rtmp_mp3_decode_frame_info(buffer, 8192 * 100, &info);
	// MPEG1 Layer3
	count = filesize / (info.frameBytes - info.frameOffset);
	ms = (float)samples * 1000 / (float)info.hz ;

	printf("count = %d frames\n", count);
	printf("frame time = %.2f ms\n", ms);
	printf("frame size = %d bytes\n", (info.frameBytes - info.frameOffset));
	printf("pcm size = %d bytes\n", (samples * info.channels));

	printf("\n");

	printf("duration = %.2f seconds\n", (count * ms)/1000);
	printf("channels = %d\n", info.channels);
	printf("bitrate = %d kbps\n", info.bitrateKbps);
	printf("sample rate = %d hz\n", info.hz);

	getchar();
}

#endif