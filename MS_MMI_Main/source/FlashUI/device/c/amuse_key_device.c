/*
* amuse_key_device.c
* Copyright (C) 2011 Microrapid Inc
* Author: gentoo <gentoo@Microrapid.com>
* 
* This file implements the key device interface. for mtk
*/


#include "amuse_key.h"
#include "amuse_file_device.h"
#include "amuse_key_device.h"
#include "string.h"

#include "Caf.h"


#define AMUSE_TOSTRING(X)	#X

#define MAX_KEY_NUM  256

static unsigned int key_reindex[MAX_KEY_NUM];
static key_map_item key_map_arr[MAX_KEY_NUM];
static unsigned int key_num_mapped = 0;
static key_map amuse_key_map;

unsigned int key_device_get_num(void)
{
	return  key_num_mapped;   
}

char*
key_device_get_item(int index, int* default_key, int* softkey)
{
	if((unsigned int)index >= key_num_mapped || index < 0){
		return 0;
	}
	*default_key = key_map_arr[index].default_soft_key;
	*softkey = key_map_arr[index].soft_key;
	return key_map_arr[index].key_name;
}

void
key_device_set_item(int code_default, int code_set)
{
	unsigned int i, j;

	for(i = 0; i < key_num_mapped; i++)
	{
		if(key_map_arr[i].default_soft_key == code_default)
		{
			for(j = 0;j < key_num_mapped;j++)
			{
				if (key_map_arr[j].soft_key == code_set)
				{
					key_map_arr[j].soft_key = 0;
				}
			}
			key_map_arr[i].soft_key = code_set;
			break;
		}
	}
}

unsigned int get_key_index_from_hard_code(unsigned int hard){
	unsigned int i;
	if(!hard)
		return 0;
	for(i = 0; i < key_num_mapped; i++){
		if(hard == key_reindex[i])
			return i;
	}	
	return MAX_KEY_NUM;
}

static void register_key(unsigned int hard,unsigned int soft,char*key_name){
	unsigned int i;
	if(key_num_mapped >= MAX_KEY_NUM)
	{
		return;
	}

	for(i = 0; i < key_num_mapped; i++){
		if(hard == key_reindex[i]){
			amuse_printf("the hardcode:%d,had been registered!\n",hard);
			return;
		}
	}
	
	key_reindex[key_num_mapped] = hard;
	key_map_arr[key_num_mapped].default_soft_key = soft;
	key_map_arr[key_num_mapped].soft_key = soft;
	key_map_arr[key_num_mapped].key_name = key_name;
	key_num_mapped ++;
}


//Map the keys between Amuse with Mocor
static void BuildKeysMap(void)
{

	//arrow
	register_key(KEY_UP, AMUSE_KEY_UP, AMUSE_TOSTRING(KEY_UP));
 	register_key(KEY_DOWN, AMUSE_KEY_DOWN, AMUSE_TOSTRING(KEY_DOWN));
 	register_key(KEY_LEFT, AMUSE_KEY_LEFT, AMUSE_TOSTRING(KEY_LEFT));
 	register_key(KEY_RIGHT, AMUSE_KEY_RIGHT, AMUSE_TOSTRING(KEY_RIGHT));

	//number
 	register_key(KEY_0, AMUSE_KEY_0, AMUSE_TOSTRING(KEY_0));
 	register_key(KEY_1, AMUSE_KEY_1, AMUSE_TOSTRING(KEY_1));
 	register_key(KEY_2, AMUSE_KEY_2, AMUSE_TOSTRING(KEY_2));
 	register_key(KEY_3, AMUSE_KEY_3, AMUSE_TOSTRING(KEY_3));
 	register_key(KEY_4, AMUSE_KEY_4, AMUSE_TOSTRING(KEY_4));
 	register_key(KEY_5, AMUSE_KEY_5, AMUSE_TOSTRING(KEY_5));
 	register_key(KEY_6, AMUSE_KEY_6, AMUSE_TOSTRING(KEY_6));
 	register_key(KEY_7, AMUSE_KEY_7, AMUSE_TOSTRING(KEY_7));
 	register_key(KEY_8, AMUSE_KEY_8, AMUSE_TOSTRING(KEY_8));
 	register_key(KEY_9, AMUSE_KEY_9, AMUSE_TOSTRING(KEY_9));

	//function key
	register_key(KEY_WEB, AMUSE_KEY_ENTER, AMUSE_TOSTRING(KEY_WEB));
	register_key(KEY_OK, AMUSE_KEY_SPACE, AMUSE_TOSTRING(KEY_OK));
	register_key(KEY_CANCEL, AMUSE_KEY_CLEAR, AMUSE_TOSTRING(KEY_CANCEL));
	register_key(KEY_STAR, AMUSE_KEY_STAR , AMUSE_TOSTRING(KEY_STAR));
	register_key(KEY_HASH, AMUSE_KEY_POUND, AMUSE_TOSTRING(KEY_HASH));
	register_key(KEY_GREEN, AMUSE_KEY_GREEN , AMUSE_TOSTRING(KEY_GREEN));
	register_key(KEY_RED, AMUSE_KEY_RED, AMUSE_TOSTRING(KEY_RED));

	//update keys count
	amuse_key_map.keynum = key_num_mapped;
}

key_map* 
amuse_key_createMap(int keynum)
{
	/*  here should not distinguish upper and lower */
	memset(key_reindex,0,sizeof(unsigned int)*MAX_KEY_NUM);
	key_num_mapped = 0;
	memset(key_map_arr,0,sizeof(key_map_item)*MAX_KEY_NUM);

	
	amuse_key_map.keys = key_map_arr;

	BuildKeysMap();
	
	return &amuse_key_map;
}


int amuse_key_rotate_arrow()
{
	return 0;
}

int amuse_key_rostore_arrow()
{
	return 0;
}

void 
amuse_key_destroyMap(key_map* keymap)
{
	key_num_mapped = 0;
	memset(key_map_arr,0,sizeof(key_map_item)*MAX_KEY_NUM);
	keymap->keynum = 0;
}

int is_valid_key(int hard_key)
{
	int index = get_key_index_from_hard_code(hard_key);
	return (index < MAX_KEY_NUM);
}

int 
amuse_getKeyCode(key_map* keymap, int hardkey)
{
	unsigned int index = get_key_index_from_hard_code(hardkey);

	if( index >= MAX_KEY_NUM ) return 0;

	return key_map_arr[index].soft_key;	
}

//call back function
void checkKeyRepeat_cb(void)
{
	// may empty
}

void handle_key_mouse_event(struct key_map* keymap)
{
}
