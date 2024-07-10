/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include <windows.h>
#include "datatypes.h"
#include "transcode.h"
#include "common.h"

T_ITEM itemqueue;
extern MC_STATE* pinst;

T_ITEM* mcMakeItem(TCHAR* filename)
{
	DWORD filesize = 0;
	int count = 0;
	bool fDvd;
	T_ITEM* item;

	item = (T_ITEM*)calloc(1, sizeof(T_ITEM));
	if (!item) return 0;

	if (!item->filename && filename)
		item->filename = strdup(filename);

	if (item->filename) {
		fDvd = (!strncmp(item->filename, "dvd://", 6));
		item->info.filesize = RetrieveFileSize(item->filename);
		item->info.filetype = mcIdentifyFileType(item->filename);
	}
	return item;
}

int mcAddItem(T_ITEM* itemtoadd)
{
	T_ITEM *item = &itemqueue;
	if (item->next) {
		for (item = item->next; ;item = item->next) {
			if (!strcmp(item->filename, itemtoadd->filename) && 
				item->info.begin == itemtoadd->info.begin &&
				item->info.track == itemtoadd->info.track && 
				item->info.chapter == itemtoadd->info.chapter)
				return -1;
			if (!item->next) break;
		}
	}
	if (!itemtoadd->info.tags.valid) {
		ReadFileTags(itemtoadd);
	}
	item->next = itemtoadd;
	return 0;
}

void mcRemoveAllItems()
{
	T_ITEM *item;
	for (item = itemqueue.next; item; ) {
		T_ITEM* t = item;
		item = item->next;
		PurgeTags(&t->info.tags);
		free(t->filename);
		free(t);
	}
	itemqueue.next = 0;
	mcinst.item = &itemqueue;
	mcinst.destfile = "";
}

int mcRemoveItem(T_ITEM* item)
{
	T_ITEM *ptr;
	int i = 0;
	for (ptr = &itemqueue; ptr && ptr->next != item; ptr = ptr->next, i++);
	if (!ptr) return -1;
	ptr->next = item->next;
	if (item == mcinst.item)
		mcinst.item = &itemqueue;
	mcFreeItem(item);
	free(item);
	return i;
}

void mcFreeItem(T_ITEM* item)
{
	PurgeTags(&item->info.tags);
	if (item->info.discroot) free(item->info.discroot);
	if (item->filename) free(item->filename);
	if (item->destfile) free(item->destfile);
	if (item->info.container) free(item->info.container);
	if (item->info.audio.codec) free(item->info.audio.codec);
	if (item->info.video.codec) free(item->info.video.codec);
}

T_ITEM* mcGetNextItem(T_ITEM* prev)
{
	if (!prev)
		return itemqueue.next;
	else
		return prev->next;
}

T_ITEM* mcGetItemByIndex(int index)
{
	T_ITEM *item;
	int i = 0;
	for (item = itemqueue.next; item && i < index; item = item->next, i++);
	return i == index ? item : 0;
}


T_ITEM* mcGetItemByFileName(TCHAR* filename)
{
	T_ITEM *item;
	for (item = itemqueue.next; item; item = item->next) {
		if (!strcmp(filename, item->filename)) {
			return item;
		}
	}
	return NULL;	
}
