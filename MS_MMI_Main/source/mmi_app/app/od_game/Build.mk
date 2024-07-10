ifeq ($(strip $(OD_GAME_SUPPORT)),TRUE)
ifeq ($(findstring 6531,$(strip $(CHIP_VER))),6531)
    MCFLAG_OPT		+=  -D__OD_SC6531_SWITCH__   
    MMI_PATH := MS_MMI
 else
	    
    ifeq ($(findstring 6530,$(strip $(CHIP_VER))),6530)
        MCFLAG_OPT		+=  -D__OD_SC6530_SWITCH__
	MMI_PATH := MS_MMI
    else
        ifeq ($(findstring 7701,$(strip $(CHIP_VER))),7701)
            MCFLAG_OPT		+=  -D__OD_SC6531_SWITCH__  -D__OD_SC7701__
	    MMI_PATH := MS_MMI_Main
	else
	    $(error this project does not support OD_GAME_SUPPORT)
	endif
    endif		
endif

OD_PROJECT_BUILD_TIME := $(shell $(PERL) $(MMI_PATH)/source/mmi_app/app/od_game/scripts/time.pl)
MCFLAG_OPT	+=  -DOD_PROJECT_BUILD_TIME=$(strip $(OD_PROJECT_BUILD_TIME)) #"${OD_PROJECT_BUILD_TIME}"
#$(error ${PHY_PROJECT_BUILD_TIME})

MCFLAG_OPT		+=  -DOD_GAME_SUPPORT
ifeq ($(strip $(OD_GAME_LIB_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_GAME_LIB_SUPPORT
endif
OD_GAME_PATH := $(strip $(MMI_PATH))/source/mmi_app/app/od_game/menu
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/adapter
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/custom

ifeq ($(strip $(OD_GAME_LIB_SUPPORT)),FALSE)
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/core
endif

#计费部分
ifeq ($(strip $(OD_GAME_BILLING_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_GAME_BILLING_SUPPORT
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/billing
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/aes
endif

#涂鸦跳跃
ifeq ($(strip $(OD_DOODLE_JUMP_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_DOODLE_JUMP_SUPPORT
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/games/doodle_jump
endif
#天天过马路
ifeq ($(strip $(OD_CROSSY_ROAD_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_CROSSY_ROAD_SUPPORT
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/games/crossy_road
endif
#棍子英雄
ifeq ($(strip $(OD_STICK_HERO_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_STICK_HERO_SUPPORT
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/games/stick_hero
endif
#泡泡
ifeq ($(strip $(OD_BUBBLE_PARTY_MIX_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_BUBBLE_PARTY_MIX_SUPPORT
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/games/bubble_party_mix
endif
#宠物
ifeq ($(strip $(OD_SUPER_PET_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_SUPER_PET_SUPPORT
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/games/super_pet
endif
ifeq ($(strip $(OD_RUNNER_SUPPORT)),TRUE)
MCFLAG_OPT		+=  -DOD_RUNNER_SUPPORT
OD_GAME_PATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/games/runner
endif

MSRCPATH += $(foreach E,$(OD_GAME_PATH),$(E)/src)
MINCPATH += $(foreach E,$(OD_GAME_PATH),$(E)/inc)
SOURCES  += $(foreach E,$(OD_GAME_PATH),$(notdir ${wildcard $(E)/src/*.c}))
MINCPATH += $(strip $(MMI_PATH))/source/mmi_app/app/od_game/inc
endif
