/*
   miniunz.c
   Version 1.1, February 14h, 2010
   sample part of the MiniZip project - (
   http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) (
   http://www.winimage.com/zLibDll/minizip.html )

         Modifications of Unzip for Zip64
         Copyright (C) 2007-2008 Even Rouault

         Modifications for Zip64 support on both zip and unzip
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )
*/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/*lint -save -e746 -e718 -e716 -e628 -e578 -e516 -e515 */
#include <stdio.h>
#include <stdlib.h>

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

// For lint issue
#ifndef uLong
#define uLong unsigned long
#endif /* uLong */

#include "unzip.h"
#include "miniunzip.h"

#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (1024 * 4)
#define MAXFILENAME (256)

/**---------------------------------------------------------------------------*
 **                         Local Functions                                   *
 **---------------------------------------------------------------------------*/

static int do_extract_currentfile(unzFile uf, const char *dst_dir)
{
    char filename_inzip[MAXFILENAME] = {0};
    char filename_withpath[MAXFILENAME] = {0};
    char *filename_withoutpath = NULL;

    char *p = NULL;
    int err = UNZ_OK;
    FILE *fout = NULL;
    void *buf = NULL;
    unsigned int size_buf = 0;

    unz_file_info64 file_info = {0};
    err = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip,
                                  sizeof(filename_inzip), NULL, 0, NULL, 0);

    if (err != UNZ_OK)
    {
        printf("error %d with zipfile in unzGetCurrentFileInfo\n", err);
        return err;
    }

    size_buf = WRITEBUFFERSIZE;
    buf = (void *)malloc(size_buf);
    if (buf == NULL)
    {
        printf("Error allocating memory\n");
        return UNZ_INTERNALERROR;
    }

    // add dst dir path
    strcat(filename_withpath, dst_dir);
    strcat(filename_withpath, "\\");
    strcat(filename_withpath, filename_inzip);
    // printf("filename_withpath: %s\n", filename_withpath);

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p) == '/') || ((*p) == '\\'))
        {
            filename_withoutpath = p + 1;
        }
        p++;
    }

    // check if dir
    if ((*filename_withoutpath) == '\0')
    {
        // printf("creating directory: %s\n", filename_withpath);
        mkdir(filename_withpath);
    }
    else
    {
        const char *write_filename = filename_withpath;
        err = unzOpenCurrentFile(uf);
        if (err == UNZ_OK)
        {
            fout = FOPEN_FUNC(write_filename, "wb");
            /* some zipfile don't contain directory alone before file */
            if ((fout == NULL)
                && (filename_withoutpath != (char *)filename_inzip))
            {
                char c = *(filename_withoutpath - 1);
                *(filename_withoutpath - 1) = '\0';
                mkdirs(write_filename);
                *(filename_withoutpath - 1) = c;
                fout = FOPEN_FUNC(write_filename, "wb");
            }

            if (fout == NULL)
            {
                printf("error opening %s\n", write_filename);
            }
        }

        if (fout != NULL)
        {
            // printf(" extracting: %s\n", write_filename);
            do
            {
                err = unzReadCurrentFile(uf, buf, size_buf);
                if (err < 0)
                {
                    printf("error %d with zipfile in unzReadCurrentFile\n",
                           err);
                    break;
                }
                if (err > 0)
                {
                    if (fwrite(buf, (unsigned)err, 1, fout) != 1)
                    {
                        printf("error in writing extracted file\n");
                        err = UNZ_ERRNO;
                        break;
                    }
                }

            } while (err > 0);
            fclose(fout);
        }

        if (err == UNZ_OK)
        {
            err = unzCloseCurrentFile(uf);
            if (err != UNZ_OK)
            {
                printf("error %d with zipfile in unzCloseCurrentFile\n", err);
            }
        }
        else
        {
            unzCloseCurrentFile(uf); /* don't lose the error */
        }
    }

    free(buf);
    return err;
}

static int do_extract(unzFile uf, const char *dst_dir)
{
    uLong i = 0;
    unz_global_info64 gi = {0};
    int err = UNZ_ERRNO;

    err = unzGetGlobalInfo64(uf, &gi);
    if (err != UNZ_OK)
    {
        printf("error %d with zipfile in unzGetGlobalInfo \n", err);
    }

    for (i = 0; i < gi.number_entry; i++)
    {
        if (do_extract_currentfile(uf, dst_dir) != UNZ_OK)
        {
            break;
        }

        if ((uLong)(i + 1) < gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err != UNZ_OK)
            {
                printf("error %d with zipfile in unzGoToNextFile\n", err);
                break;
            }
        }
    }

    return err;
}

static int do_extract_single_file(unzFile uf, const char *dst_dir,
                              const char *filename)
{
    int err = UNZ_ERRNO;
    err = unzLocateFile(uf, filename, CASESENSITIVITY);
    if (err != UNZ_OK)
    {
        printf("file %s not found in the zipfile\n", filename);
        return -1;
    }

    err = do_extract_currentfile(uf, dst_dir);
    return err;
}

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

int mini_unzip(const char *src_zip_file, const char *dst_dir)
{
    int ret = -1;
    unzFile uf = NULL;
    if (NULL == src_zip_file)
    {
        return -1;
    }

    // open zip file
    uf = unzOpen64(src_zip_file);
    // open error
    if (NULL == uf)
    {
        printf("Cannot open %s\n", src_zip_file);
        return -1;
    }
    // make dst dir
    mkdirs(dst_dir);
    // do extract work
    ret = do_extract(uf, dst_dir);
    // close uz
    unzClose(uf);
    return ret;
}

int mini_unzip_single_file(const char *src_zip_file, const char *dst_dir,
                       const char *filename)
{
    int ret = -1;
    unzFile uf = NULL;
    if (NULL == src_zip_file || NULL == filename)
    {
        return -1;
    }

    // open zip file
    uf = unzOpen64(src_zip_file);
    // open error
    if (NULL == uf)
    {
        printf("Cannot open %s\n", src_zip_file);
        return -1;
    }
    // make dst dir
    mkdirs(dst_dir);
    // do extract onefile
    ret = do_extract_single_file(uf, dst_dir, filename);
    // close uz
    unzClose(uf);
    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*lint -restore */