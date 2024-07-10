#include "read_res_img.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#define MAX_RES_IMG		4
const RES_IMG_T s_res_img[MAX_RES_IMG] =
{
	//res image 0
	{
		"..\\input\\texture_0_argb.raw",
		640,
		480,
		0,
		640*480*4
	},

	//res image 1
	{
		"..\\input\\texture_1.raw",
		120,
		160,
		1,
		160*120*2
	}
};

int ReadFromFile(const char *file_name, void *data_ptr,unsigned int data_size)
{
	FILE *fp = fopen(file_name, "r+b");
	int32 read_size = 0;
		
	if (fp)
	{
		read_size = fread(data_ptr, 1, data_size, fp);
		fclose(fp);
	}

	return read_size;
}

BOOLEAN ReadResImageFromID(unsigned int id, void *data_ptr, unsigned int data_size,
						   RES_IMG_T *res_img_ptr)
{
	unsigned int size = 0;
	unsigned int bytes_per_pixel = 0;

	if (id >= MAX_RES_IMG)
	{
		return FALSE;
	}

	*res_img_ptr = s_res_img[id];

	if (NULL != data_ptr && data_size > 0)
	{		
		size = ReadFromFile(s_res_img[id].file_name, data_ptr, data_size);
		if (size == 0)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return TRUE;
	}
}