/*
* amuse_main.c
* Copyright (C) 2011 Microrapid Inc
* Author: gentoo <gentoo@Microrapid.com>
* 
* This file implementes amuse main play flow 
*   for mtk platform.
*/

#include <stdio.h>
#include"string.h"
#include <amuse.h>
#include <amuse_file.h>
#include <amuse_input.h>
#include <amuse_key_device.h>
#include <amuse_config_device.h>
#include "kal_release.h"
#include "reg_base.h"
#include "config_hw.h"
#include "init.h"
#include <setjmp.h>

jmp_buf amuse_mark; 

#define USE_MALLOC 0
#define MEMORY_SIZE 	0x1800000
#define STACK_SIZE (32*1024)

#if USE_MALLOC
char *amuse_memory = NULL;
#else
char amuse_memory[MEMORY_SIZE];
#endif

static int key_event = 0;
static char*memory = 0;
char ascii_file_name[64];
short unicode_file_name[64];

void set_key_event(int value)
{
    key_event = value;
}

int get_key_event(void)
{
    return key_event;
}

unsigned int get_key_index_from_hard_code(unsigned int hard);
unsigned char* amuse_get_swf_from_mem(char *filename,int *pswf_size);
void clean_file_handle(void);
void clean_canvas(void);


int print_system_info()
{
	amuse_printf("MCUCLK_CON:%x\n", *MCUCLK_CON);
	amuse_printf("CLK_CON:%x\n", *CLK_CON);	
	amuse_printf("mtk version:%d\n", INT_ecoVersion());
}

static void move_mouse(int *x,int *y)
{
    if(*x < 0)
      *x = 0;
    if(*x > 240)
      *x= (int)240 -1; 
    		        
    if(*y < 0)
    	*y = 0;
    if(*y > 320)
    	*y = 320-1;
  //execMouseCmd(CMD_MOUSE_MOVE,x,y);    				    
}

enum 
{ 
  CMD_MOUSE_PRESS, 
  CMD_MOUSE_RELEASE, 
  CMD_MOUSE_MOVE, 
}; 

enum
{
CMD_KEY_PRESS,
CMD_KEY_RELEASE,
};

typedef struct _mouse_msg{
	int event;
	unsigned short x;
	unsigned short y;
	int flag;
}mouse_msg;

typedef struct _key_msg{
	int event;
	int key_code;
	int flag;
}key_msg;

#define STORED_NUM 16
static  key_msg key_msg_queue[STORED_NUM];
static int key_index_read = 0, key_index_write = 0;
static unsigned short last_key_value = 0; 

static  mouse_msg mouse_msg_queue[STORED_NUM];
static int mouse_index_read = 0, mouse_index_write = 0;

static void init_msg_queue(void)
{
	memset(key_msg_queue,0,sizeof(key_msg)*STORED_NUM);
	memset(mouse_msg_queue,0,sizeof(mouse_msg)*STORED_NUM);
	key_index_read = 0, key_index_write = 0;	mouse_index_read = 0, mouse_index_write = 0;
	last_key_value = 0; 
}

int get_one_msg(int *p_event,int *p_code)
{
	if(key_index_read >= STORED_NUM)
		key_index_read = 0;
	if(key_msg_queue[key_index_read].flag != 1)
		return -1;
	
	*p_event = key_msg_queue[key_index_read].event ;
	*p_code = key_msg_queue[key_index_read].key_code ;
	
	key_msg_queue[key_index_read].flag = 0 ;
	key_index_read ++;
	return 0;
}

int get_one_msg_mouse(int *p_event,unsigned short *px,unsigned short *py)
{
	if(mouse_index_read >= STORED_NUM)
		mouse_index_read = 0;
	if(mouse_msg_queue[mouse_index_read].flag != 1)
		return -1;
	*p_event = mouse_msg_queue[mouse_index_read].event ;
	*px = mouse_msg_queue[mouse_index_read].x ;
	*py = mouse_msg_queue[mouse_index_read].y ;
	
	mouse_msg_queue[mouse_index_read].flag = 0 ; 
	mouse_index_read ++;
	return 0;
}

int push_one_msg(int event,int code)
{
	if(key_index_write >= STORED_NUM)
		key_index_write = 0;
	key_msg_queue[key_index_write].event= event;
	key_msg_queue[key_index_write].key_code= code;
	
	key_msg_queue[key_index_write].flag = 1;
	key_index_write ++;
	return 0;
}

int push_one_msg_mouse(int event,unsigned short x,unsigned short y)
{
	if(mouse_index_write >= STORED_NUM)
		mouse_index_write = 0;

	mouse_msg_queue[mouse_index_write].event = event;
	mouse_msg_queue[mouse_index_write].x = x;
	mouse_msg_queue[mouse_index_write].y = y;
	
	mouse_msg_queue[mouse_index_write].flag = 1;
	mouse_index_write ++;
	return 0;
}
static void convert_mouse_msg(int *event)
{
	if(*event == CMD_MOUSE_PRESS)
		*event = AMUSE_MOUSEDOWN;
	else if(*event == CMD_MOUSE_RELEASE)
		*event = AMUSE_MOUSEUP;
	else if(*event == CMD_MOUSE_MOVE)
		*event = AMUSE_MOUSEMOTION;
	else
		*event = 0;
}
static void convert_key_msg(int *event)
{
	if(*event == CMD_KEY_PRESS)
		*event = AMUSE_KEYDOWN;
	else if(*event == CMD_MOUSE_RELEASE)
		*event = AMUSE_KEYUP;
	else
		*event = 0;
}

//将鼠标和按键信息从队列中获取，并分发给Flash引擎
static int
amuse_event_process(key_map* keymap)
{
	int event;
	int code;
	unsigned short x,y;
	amuse_input_event input;
	while(get_one_msg_mouse(&event, &x, &y) != -1)
	{
		input.mouse.x = x;
		input.mouse.y = y;
		input.mouse.button = AMUSE_MOUSE_BUTTON_LEFT;

		convert_mouse_msg(&event);
		if(event == AMUSE_MOUSEDOWN)
		{
			input.type = AMUSE_MOUSEMOTION;
			amuse_process_input(&input);
		}
		input.type = event;
		amuse_process_input(&input);
	}
	

	while(get_one_msg(&event,&code)!= -1)
	{
		convert_key_msg(&event);
		input.type = event;
		input.key.code = amuse_key_getAmuseKeyCode(keymap,code);
		input.key.mod = AMUSE_KMOD_NONE;		
		amuse_process_input(&input);
		amuse_printf("hard key code:%d\n",code);
		
		if(input.type == AMUSE_KEYDOWN && input.key.code == AMUSE_KEY_ESCAPE)
		{
			input.type = AMUSE_QUIT;
			amuse_process_input(&input);
			return -1;//quit
		}
	}


	return 1;
}
/*********************************************************
* For touch screen
**********************************************************/
void play_flash_pen_event(int event, unsigned short x, unsigned short y)
{
	push_one_msg_mouse(event,x,y);
}

/*********************************************************
* For key board
**********************************************************/
void play_flash_key_event(int event, int keycode)
{
	push_one_msg(event,keycode);
}

void resume_flash(void)
{
}
void  pause_flash(void)
{
}

void finalize_flash(void)
{
}




static struct key_map* keymap = 0;

int
play_flash(short* file_name)
{
	int count = 10;
	
	amuse_printf("amuse start\n");
	
#if USE_MALLOC
	amuse_memory = (char*)amuse_malloc_device(MEMORY_SIZE);
#endif
	if(!amuse_memory)
	{
		amuse_printf("malloc error!");
		return 0;
	}
	else
	{
		amuse_printf("malloced memory address:%x\n", amuse_memory);
	}
	init_msg_queue();
	
	clean_file_handle();
	amuse_stack_init(STACK_SIZE);

	amuse_config_setDefault();
	//keymap = amuse_key_createMap(4);

	memory = (char*)((unsigned int)amuse_memory&(~0xf));
	if(amuse_player_create((char*)file_name, (void*)memory, MEMORY_SIZE- 0x10, keymap) == -1){
		amuse_printf("create failed\n");
		return 0;
	}
	
	clean_canvas();
	amuse_printf("init success\n");

	while(count--)
	{
		amuse_play_frame();
		amuse_printf("running!");
	}

	amuse_player_destory();
	amuse_display_destory();
	
#if USE_MALLOC	
	amuse_free_device(amuse_memory);
#endif

	return 0;
}

void amuse_start_ui(void)
{
	flashtask_start_ui();
}


int
play_flash_init(short* file_name)
{
	amuse_printf("amuse start\n");	

	amuse_stack_init(STACK_SIZE);

	amuse_config_setDefault();
	
	memory = (char*)((unsigned int)amuse_memory&(~0xf));
	amuse_printf("amuse_memory:%08x,memory:%08x\n",amuse_memory,memory);
	if(amuse_player_create((char*)file_name, (void*)memory, MEMORY_SIZE- 0x10, keymap) == -1){
		amuse_printf("create failed\n");
		return 0;
	}

	register_as_api();
	
	return 1;
}

int play_flash_run()
{
	amuse_play_frame();
}

int play_flash_destroy()
{
	if(keymap) amuse_key_destroyMap(keymap);
	amuse_player_destory();
}

int  stop_flash(void)
{
	return 1;
}

void play_flash_next_step(void)
{
	amuse_play_frame();
	if(amuse_event_process(keymap) == -1)
	{
		return;
	}
}
