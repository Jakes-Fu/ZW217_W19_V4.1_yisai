/******************************************************************************
 ** File Name:      mmi_app_game_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:180
#ifndef _MMI_APP_GAME_TRC_H_
#define _MMI_APP_GAME_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIGAME_BOXMAN_WINTAB_656_112_2_18_2_21_19_0 "MMIAPIGMBOXMAN_OpenGameWin"
#define MMIGAME_BOXMAN_WINTAB_815_112_2_18_2_21_19_1 "RefreshCurrentMapData error boxman_current_map_num = %d"
#define MMIGAME_BOXMAN_WINTAB_1320_112_2_18_2_21_21_2 "[mmigame_boxman_wintab.c]:SetBoxmanTextStyle(PNULL == style_ptr)"
#define MMIGAME_BOXMAN_WINTAB_2257_112_2_18_2_21_22_3 "mmi GameBoxApplet_HandleEvent msg_id = 0x%04x"
#define MMIGAME_LINK_FUNC_487_112_2_18_2_21_24_4 "[mmigame_link_func.c] SetLinkTextStyle (PNULL == style_ptr)"
#define MMIGAME_LINK_FUNC_794_112_2_18_2_21_24_5 "[mmigame_link_func.c] InitStage s_links_current_level = %d is error!"
#define MMIGAME_LINK_FUNC_1159_112_2_18_2_21_25_6 "[mmigame_link_func.c]:GetImageID tile.id = %d is error!"
#define MMIGAME_LINK_FUNC_1216_112_2_18_2_21_25_7 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_MOVEDOWN error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1240_112_2_18_2_21_25_8 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_MOVELEFT error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1280_112_2_18_2_21_25_9 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_DISPERSE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1292_112_2_18_2_21_25_10 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_DISPERSE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1343_112_2_18_2_21_26_11 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_CENTRALIZE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1355_112_2_18_2_21_26_12 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_CENTRALIZE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1407_112_2_18_2_21_26_13 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_DISPERSE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1419_112_2_18_2_21_26_14 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_DISPERSE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1471_112_2_18_2_21_26_15 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_CENTRALIZE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_1483_112_2_18_2_21_26_16 "[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_CENTRALIZE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
#define MMIGAME_LINK_FUNC_2364_112_2_18_2_21_28_17 "[mmigame_link_func.c]: GetConnectPath path == NULL"
#define MMIGAME_LINK_FUNC_2369_112_2_18_2_21_28_18 "[mmigame_link_func.c]: GetConnectPath tile1.id = %d,tile2.id = %d"
#define MMIGAME_LINK_FUNC_2435_112_2_18_2_21_29_19 "[mmigame_link_func.c]:GetLeftRightConnectPath error"
#define MMIGAME_LINK_FUNC_2557_112_2_18_2_21_29_20 "[mmigame_link_func.c]:GetTopBottomConnectPath error"
#define MMIGAME_LINK_WINTAB_340_112_2_18_2_21_33_21 "MMIAPIGMLINK_OpenGameWin"
#define MMIGAME_LINK_WINTAB_1248_112_2_18_2_21_35_22 "mmi GameLinkApplet_HandleEvent msg_id = 0x%04x"
#define MMIGAME_WINTAB_177_112_2_18_2_21_36_23 "HandleGameMainMenuWindow menu_id is error:%d"
#define MMIGAME_WINTAB_269_112_2_18_2_21_36_24 "MMIAPIGAME_OpenMainMenuWindow:mainmenu_style %d is error!"
#define MMIGAME_QUINTET_WINTAB_607_112_2_18_2_21_42_25 "MMIGMQUT_AppendListBoxItem param error!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_GAME_TRC)
TRACE_MSG(MMIGAME_BOXMAN_WINTAB_656_112_2_18_2_21_19_0,"MMIAPIGMBOXMAN_OpenGameWin")
TRACE_MSG(MMIGAME_BOXMAN_WINTAB_815_112_2_18_2_21_19_1,"RefreshCurrentMapData error boxman_current_map_num = %d")
TRACE_MSG(MMIGAME_BOXMAN_WINTAB_1320_112_2_18_2_21_21_2,"[mmigame_boxman_wintab.c]:SetBoxmanTextStyle(PNULL == style_ptr)")
TRACE_MSG(MMIGAME_BOXMAN_WINTAB_2257_112_2_18_2_21_22_3,"mmi GameBoxApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIGAME_LINK_FUNC_487_112_2_18_2_21_24_4,"[mmigame_link_func.c] SetLinkTextStyle (PNULL == style_ptr)")
TRACE_MSG(MMIGAME_LINK_FUNC_794_112_2_18_2_21_24_5,"[mmigame_link_func.c] InitStage s_links_current_level = %d is error!")
TRACE_MSG(MMIGAME_LINK_FUNC_1159_112_2_18_2_21_25_6,"[mmigame_link_func.c]:GetImageID tile.id = %d is error!")
TRACE_MSG(MMIGAME_LINK_FUNC_1216_112_2_18_2_21_25_7,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_MOVEDOWN error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1240_112_2_18_2_21_25_8,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_MOVELEFT error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1280_112_2_18_2_21_25_9,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_DISPERSE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1292_112_2_18_2_21_25_10,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_DISPERSE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1343_112_2_18_2_21_26_11,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_CENTRALIZE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1355_112_2_18_2_21_26_12,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_CENTRALIZE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1407_112_2_18_2_21_26_13,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_DISPERSE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1419_112_2_18_2_21_26_14,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_DISPERSE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1471_112_2_18_2_21_26_15,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_CENTRALIZE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_1483_112_2_18_2_21_26_16,"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_CENTRALIZE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)")
TRACE_MSG(MMIGAME_LINK_FUNC_2364_112_2_18_2_21_28_17,"[mmigame_link_func.c]: GetConnectPath path == NULL")
TRACE_MSG(MMIGAME_LINK_FUNC_2369_112_2_18_2_21_28_18,"[mmigame_link_func.c]: GetConnectPath tile1.id = %d,tile2.id = %d")
TRACE_MSG(MMIGAME_LINK_FUNC_2435_112_2_18_2_21_29_19,"[mmigame_link_func.c]:GetLeftRightConnectPath error")
TRACE_MSG(MMIGAME_LINK_FUNC_2557_112_2_18_2_21_29_20,"[mmigame_link_func.c]:GetTopBottomConnectPath error")
TRACE_MSG(MMIGAME_LINK_WINTAB_340_112_2_18_2_21_33_21,"MMIAPIGMLINK_OpenGameWin")
TRACE_MSG(MMIGAME_LINK_WINTAB_1248_112_2_18_2_21_35_22,"mmi GameLinkApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIGAME_WINTAB_177_112_2_18_2_21_36_23,"HandleGameMainMenuWindow menu_id is error:%d")
TRACE_MSG(MMIGAME_WINTAB_269_112_2_18_2_21_36_24,"MMIAPIGAME_OpenMainMenuWindow:mainmenu_style %d is error!")
TRACE_MSG(MMIGAME_QUINTET_WINTAB_607_112_2_18_2_21_42_25,"MMIGMQUT_AppendListBoxItem param error!")
END_TRACE_MAP(MMI_APP_GAME_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_GAME_TRC_H_

