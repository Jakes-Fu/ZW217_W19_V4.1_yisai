/**************************************************************
  General Information:
  File Name:      sysMemFile.h
  Author:         
  Description:    
  *************************************************************
  Revision History:
  06/22/2002   Initial Version

*/
#ifndef SYSMEMFILE_H
#define SYSMEMFILE_H

//#include "stdClib.h"
//#include "sysConfig.h"
//#include "mMmsSystem.h"
#include "sci_types.h"

#ifndef FILE_HANDLE
typedef			int32					FILE_HANDLE;
#endif

PUBLIC int32 sysVFileInitInfo(void);
PUBLIC void sysVFileDestroyInfo(void);
PUBLIC int32 sysVFrename(int8 *old_fname,int8 *new_fname);
PUBLIC FILE_HANDLE sysVFopen(int8 *fname,int8 *mode);
PUBLIC int32 sysVFread(int8 *buf,int32 size,int32 count,FILE_HANDLE fd);
PUBLIC int32 sysVFwrite(int8 *buf,int32 size,int32 count,FILE_HANDLE fd);
PUBLIC int32 sysVFflush(FILE_HANDLE fd);
PUBLIC int32 sysVFseek(FILE_HANDLE fd,int32 offset,int32 origin);
PUBLIC int32 sysVFputc(int32 c, FILE_HANDLE fd);
PUBLIC int32 sysVFgetc(FILE_HANDLE fd);
PUBLIC int32 sysVFtell(FILE_HANDLE fd);
PUBLIC int32	sysVFremove(int8 *fname);
PUBLIC int32 sysVFSetBufPointer(int8 *fname,int32 size);
PUBLIC int32 sysVFRemoveByHandle(FILE_HANDLE fd);
PUBLIC void sysVFclose(FILE_HANDLE fd);
PUBLIC int32 sysVFCreateByData(int8 *filename,int8 *data,int32 len);
PUBLIC int32 sysVFGetConstFileData(int8 *filename,int8 **data,int32 *len);
PUBLIC int32 sysVFileDuplicate(int8 *src_fname,int8 *des_fname);
PUBLIC int32 sysFileIsExist(int8 *fname);
PUBLIC int32 sysVMediaFileRefPlusOne(int8 *fname);
PUBLIC int32 sysVMediaFileRefSubOne(int8 *fname);
PUBLIC int32 sysVFGetMediaFileRefNum(int8 *fname);
PUBLIC int32 sysVFeof(FILE_HANDLE fd);
PUBLIC int32 sysVFGetFileInfo(int8 *name,uint8 **buf,int32 *buf_len);
PUBLIC int32 sysVFCreateFile(int8 *fname,uint8 *buf,int32 size);
PUBLIC int32	sysVFDestroyFile(int8 *fname);
PUBLIC int8* sysVFGetBufPointer( FILE_HANDLE fd );
PUBLIC int32 sysVFfileLength(FILE_HANDLE file);
PUBLIC BOOLEAN sysVFileEnd(FILE_HANDLE stream);

#endif


