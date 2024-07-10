
#include "xnes_init.h"
#include "os_api.h"

G_NES_INTERFACE_CTX g_nes_ctx = {0};
int Xnes_All_Interface_init(G_NES_INTERFACE_CTX *g_nes, int size)
{
	if (size != sizeof(G_NES_INTERFACE_CTX))
	{
		return -1;
	}
	memcpy(&g_nes_ctx, g_nes, size);//!pclint
	if (NULL == g_nes_ctx.xnes_peer_fopen_4_read) return -1;
	if (NULL == g_nes_ctx.xnes_peer_fopen_4_read) return -1;
	if (NULL == g_nes_ctx.xnes_peer_fopen_4_write) return -1;
	if (NULL == g_nes_ctx.xnes_peer_file_close) return -1;
	if (NULL == g_nes_ctx.xnes_peer_file_read) return -1;
	if (NULL == g_nes_ctx.xnes_peer_file_write) return -1;
	if (NULL == g_nes_ctx.xnes_peer_read_joypad) return -1;

	if (NULL == g_nes_ctx.xnes_peer_video_initialize) return -1;
	if (NULL == g_nes_ctx.xnes_peer_video_finalize) return -1;
	if (NULL == g_nes_ctx.xnes_peer_video_update_screen) return -1;
	
	if (NULL == g_nes_ctx.xnes_peer_sound_initialize) return -1;
	if (NULL == g_nes_ctx.xnes_peer_sound_set_param) return -1;
	if (NULL == g_nes_ctx.xnes_peer_sound_finalize) return -1;
	if (NULL == g_nes_ctx.xnes_peer_set_sound) return -1;
	if (NULL == g_nes_ctx.xnes_peer_sound_write) return -1;

	if (NULL == g_nes_ctx.NesMalloc)  return -1;
	if (NULL == g_nes_ctx.NesFree)  return -1;
	return 0;
}

