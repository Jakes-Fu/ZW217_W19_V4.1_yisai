/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stddef.h>
#include "toctool.h"
#include "scsidefs.h"
#include "cdromioctl.h"
#include "processpil.h"
#include "atlstr.h"

#ifdef _USRDLL
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

#define __DRIVELST_C

static int getNumAdapters( void );
static int aspiGetNumDrives( void );
static int ntGetNumDrives( void );

unsigned long CDDBSum( unsigned long n );
DRIVELIST driveList;

#define __INIT_C

/*
 * Returns the current OS system, one of OS_WIN95, OS_WIN98,
 * OS_WINNT35, OS_WINNT4, OS_WIN2K, or if an error occurs,
 * OS_UNKNOWN
 */
int getOsVersion( void )
{
  OSVERSIONINFO os;
  
  ZeroMemory( &os, sizeof(os) );
  os.dwOSVersionInfoSize = sizeof(os);
  GetVersionEx( &os );

  if ( os.dwPlatformId == VER_PLATFORM_WIN32_NT )
  {
    if ( os.dwMajorVersion == 3 && os.dwMinorVersion >= 50 )
      return OS_WINNT35;
    else if ( os.dwMajorVersion == 4 )
      return OS_WINNT4;
    else if ( os.dwMajorVersion == 5 )
      return OS_WIN2K;
  }
  else if ( os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
  {
    if ( os.dwMinorVersion == 0 )
      return OS_WIN95;
    else
      return OS_WIN98;
  }
  
  return OS_UNKNOWN;  
}

static BOOL bInit = FALSE;

typedef struct {
  TCHAR name[32];
  BOOL avail;
} CDROM_INTERFACE;

static CDROM_INTERFACE cdif[NUM_INTERFACE];

static int iActiveInterface = 0;
static int iOSVer;

DWORD (*GetASPI32SupportInfo)(void);
DWORD (*SendASPI32Command)(LPSRB);
static HINSTANCE hDll;

/*
 * Initialize functions according to whether we're using ASPI (Win95/98)
 * or the CDROM ioctls (NT/2000);
 */
int InitCDInterface( int iInterface)
{
  
  if ( bInit )
    return 0;
  
  ZeroMemory( cdif, sizeof(cdif) );
  lstrcpy( cdif[INTERFACE_ASPI].name, "ASPI" );
  lstrcpy( cdif[INTERFACE_SPTI].name, "SPTI" );
  lstrcpy( cdif[INTERFACE_IOCTL_CDROM].name, "IOCTL_CDROM" );
  
  iOSVer = getOsVersion();
  if ( iOSVer == OS_UNKNOWN )
    return -1;

  if (iInterface == INTERFACE_ASPI) {
	// check if aspi is available
	hDll = LoadLibrary( "WNASPI32.DLL" );
	if (hDll) {
		GetASPI32SupportInfo = 
			(DWORD(*)(void))GetProcAddress( hDll, "GetASPI32SupportInfo" );
		SendASPI32Command = 
			(DWORD(*)(LPSRB))GetProcAddress( hDll, "SendASPI32Command" );

		// make sure that we've got both function addresses
		if ( GetASPI32SupportInfo && SendASPI32Command )
		{
			cdif[INTERFACE_ASPI].avail = TRUE;
		}
	} else {
		iInterface = INTERFACE_SPTI;
	}
  }
  if ( iOSVer >= OS_WINNT4 ) {
    cdif[INTERFACE_SPTI].avail = TRUE;
    cdif[INTERFACE_IOCTL_CDROM].avail = TRUE;
  }
  
  iActiveInterface = iInterface;
  bInit = TRUE;
  return 0;
}


void UninitCDInterface( void )
{
  if ( cdif[INTERFACE_ASPI].avail )
  {
    FreeLibrary( hDll );
  }
  ZeroMemory( cdif, sizeof(cdif) );
  
  bInit = FALSE;
}

void GetDriveDesc( int driveNo, TCHAR *szBuf, int bufLen )
{
  if ( !szBuf )
    return;
  
  ZeroMemory( szBuf, bufLen );
  
  if ( !bInit )
    return;
  
  if ( driveNo > driveList.num )
    return;

  switch( iActiveInterface )
  {
    case INTERFACE_ASPI:
      lstrcpyn( szBuf, driveList.drive[driveNo].a.desc, bufLen );
      break;
    case INTERFACE_SPTI:
    case INTERFACE_IOCTL_CDROM:
      lstrcpyn( szBuf, driveList.drive[driveNo].n.desc, bufLen );
      break;
  }
}


/************************************************************************
 * NT/2000 layer (common to IOCTL_CDROM and SPTI cdif
 ************************************************************************/
static int ntGetNumDrives( void )
{
  int idx;
  TCHAR szDrives[106];
  TCHAR *p;
  
  ZeroMemory( &driveList, sizeof(driveList) );
  
  ZeroMemory( szDrives, 106 );
  GetLogicalDriveStrings( 105, szDrives );
  
  for( p = szDrives; *p; p += (lstrlen( p ) + 1) )
  {
    if ( GetDriveType( p ) == DRIVE_CDROM )
    {
      idx = driveList.num++;
      driveList.drive[idx].n.driveLetter = *p;
      lstrcpy( driveList.drive[idx].n.desc, p );
    }
  }
  
  return driveList.num;
}


/************************************************************************
 * ASPI layer
 ************************************************************************/
static int getNumAdapters( void )
{
  DWORD d;
  BYTE bCount, bStatus;
  
  d = GetASPI32SupportInfo();
  bCount = LOBYTE(LOWORD(d));
  bStatus = HIBYTE(LOWORD(d));
  
  if ( bStatus != SS_COMP && bStatus != SS_NO_ADAPTERS )
    return -1;
  
  return (int)bCount;
}


static int aspiGetNumDrives( void )
{
  SRB_HAInquiry sh;
  SRB_GDEVBlock sd;
  BYTE numAdapters, maxTgt;
  BYTE i, j, k;
  int idx = 0;
  
  // initialize the drive list;
  ZeroMemory( &driveList, sizeof(driveList) );
  numAdapters = (BYTE)getNumAdapters();
  if ( numAdapters == 0 )
    return 0;
  
  for( i = 0; i < numAdapters; i++ )
  {
    ZeroMemory( &sh, sizeof(sh) );
    sh.SRB_Cmd = SC_HA_INQUIRY;
    sh.SRB_HaID = i;
    SendASPI32Command( (LPSRB)&sh );

    // in case of error, skip to next adapter
    if ( sh.SRB_Status != SS_COMP )
      continue;
    
    // determine the max target number for the adapter from offset 3
    // if it's zero, then assume the max is 8
    maxTgt = sh.HA_Unique[3];
    if ( maxTgt == 0 )
      maxTgt = 8;

    for( j = 0; j < maxTgt; j++ )
    {
      // try all 8 values for LUN
      for( k = 0; k < 8; k++ )
      {
        ZeroMemory( &sd, sizeof(sd) );
        sd.SRB_Cmd   = SC_GET_DEV_TYPE;
        sd.SRB_HaID  = i;
        sd.SRB_Target = j;
        sd.SRB_Lun   = k;
        SendASPI32Command( (LPSRB)&sd );
        if ( sd.SRB_Status == SS_COMP )
        {
          if ( sd.SRB_DeviceType == DTYPE_CDROM && driveList.num <= MAX_DRIVE_LIST )
          {
            idx = driveList.num++;
            driveList.drive[idx].a.ha = i;
            driveList.drive[idx].a.tgt = j;
            driveList.drive[idx].a.lun = k;
            wsprintf( driveList.drive[idx].a.desc, "ASPI[%d:%d:%d]", i, j, k );
          }
        }
      }
    }
  }
  
  return driveList.num;
}

unsigned long genCddbId( TOC *toc )
{
  unsigned long t, n;
  TOCTRACK *t1, *t2;
  int i, numTracks;
  
  if ( !toc )
    return 0;
  
  t = n = 0;
  numTracks = (int)(toc->lastTrack - toc->firstTrack + 1);
  for( i = 0; i < numTracks; i++ )
  {
    t1 = &(toc->tracks[i]);
    n += CDDBSum( 60 * t1->addr[1] + t1->addr[2] );
  }
  
  t2 = &(toc->tracks[numTracks]);
  t = 60 * t2->addr[1] + t2->addr[2];
  t2 = &(toc->tracks[0]);
  t -= ( 60 * t2->addr[1] + t2->addr[2] );
  
  return (unsigned long)( ((n % 0xFF) << 24) | 
                             (t << 8) | 
                             ((unsigned long)numTracks) );
}


unsigned long CDDBSum( unsigned long n )
{
  unsigned long retVal = 0;

  while( n > 0 )
    {
      retVal += ( n % 10 );
      n /= 10;
    }

  return retVal;
}


TCHAR *genCddbQuery( TOC *toc, TCHAR *buf, int bufLen )
{
  TCHAR *p = buf;
  int numTracks, i;
  TOCTRACK *t1;
  DWORD ofs;
  
  if ( !toc || !buf || (bufLen <= 0) )
    return "";
  
  ZeroMemory( buf, bufLen );
  bufLen--;
  
  numTracks = (int)(toc->lastTrack - toc->firstTrack + 1);

  _snprintf( p, bufLen, "cddb query %08x %d", genCddbId( toc ),
          numTracks );
  bufLen -= lstrlen( p );
  p += lstrlen( p );
  if ( bufLen == 0 )
    return buf;

  // track offsets (not including lead-out)
  for( i = 0; i < numTracks; i++ )
  {
    t1 = &(toc->tracks[i]);
    ofs = (((t1->addr[1]*60)+t1->addr[2])*75)+t1->addr[3];
    _snprintf( p, bufLen, " %ld", ofs );
    p += lstrlen( p );
    if ( bufLen == 0 )
      return buf;
  }
  
  // disc length
  t1 = &toc->tracks[i];
  ofs = t1->addr[1]*60 + t1->addr[2];
  _snprintf( p, bufLen, " %ld", ofs );
  
  return buf;
}

#define __TOC_C

int aspiReadTOC( int driveNo, TOC *t );
int ioctlCdromReadTOC( int driveNo, TOC *t );
int sptiReadTOC( int driveNo, TOC *t );


/*
 * reads the TOC of the CD in position driveNo
 */
int readTOC( int driveNo, TOC *t )
{
  if ( !bInit || (driveNo > driveList.num) )
    return -1;

  switch( iActiveInterface )
  {
    case INTERFACE_ASPI:
      return aspiReadTOC( driveNo, t );
    case INTERFACE_SPTI:
      return sptiReadTOC( driveNo, t );
    case INTERFACE_IOCTL_CDROM:
      return ioctlCdromReadTOC( driveNo, t );
  }
  return -1;
}


int sptiReadTOC( int driveNo, TOC *t )
{
  HANDLE hDrive;
  ULONG length;
  TCHAR buf[32];
  int retVal = 0;
  DWORD dwReturned;
  SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER swb;
  DWORD dwFlags = GENERIC_READ;
  
  if ( iOSVer >= OS_WIN2K )
    dwFlags |= GENERIC_WRITE;

  wsprintf( buf, "\\\\.\\%c:", driveList.drive[driveNo].n.driveLetter );
  hDrive = CreateFile( buf, dwFlags, FILE_SHARE_READ, NULL,
                        OPEN_EXISTING, 0, NULL );
  if ( hDrive == INVALID_HANDLE_VALUE )
    return -1;

  dwReturned = 0;
  length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);
  ZeroMemory( t, sizeof(TOC) );
  ZeroMemory( &swb, sizeof(swb) );
  swb.spt.Length          = sizeof(SCSI_PASS_THROUGH);
  swb.spt.CdbLength       = 10;
  swb.spt.DataIn          = SCSI_IOCTL_DATA_IN;
  swb.spt.DataTransferLength = sizeof(TOC);
  swb.spt.TimeOutValue    = 5;
  swb.spt.DataBuffer      = t;
  swb.spt.SenseInfoOffset =
    offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);
  swb.spt.Cdb[0]          = 0x43; // read TOC
  swb.spt.Cdb[1]          = 0x02; // MSF mode
  swb.spt.Cdb[7]          = 0x03;
  swb.spt.Cdb[8]          = 0x24;
  if ( !DeviceIoControl( hDrive,
                          IOCTL_SCSI_PASS_THROUGH_DIRECT,
                          &swb,
                          length,
                          &swb,
                          length,
                          &dwReturned,
                          NULL ) )
    retVal = -1;
  
  if ( retVal < 0 )
  {
    DWORD dwError = GetLastError();
    if ( dwError == 5 ) {
      printf( "    Error: you must have Administor priveleges to use SPTI under Win2000\n" );
    } else if ( dwError == 1117 ) {
      // catch the generic device IO error
      printf( "    Error: ERROR_IO_DEVICE reported by the drive (no CD in drive?)\n" );
      ZeroMemory( t, sizeof(TOC) );
    } else {
      printf( "    Error: %ld\n", dwError );
    }
  }
  
  CloseHandle( hDrive );
  return retVal;
}


int ioctlCdromReadTOC( int driveNo, TOC *t )
{
  HANDLE hDrive;
  TCHAR buf[32];
  int retVal = 0;
  DWORD dwReturned;
  
  wsprintf( buf, "\\\\.\\%c:", driveList.drive[driveNo].n.driveLetter );
  hDrive = CreateFile( buf, GENERIC_READ, FILE_SHARE_READ, NULL,
                        OPEN_EXISTING, 0, NULL );
  if ( hDrive == INVALID_HANDLE_VALUE )
    return -1;

  dwReturned = 0;
  ZeroMemory( t, sizeof(TOC) );
  if ( !DeviceIoControl( hDrive,
                         IOCTL_CDROM_READ_TOC,
                         NULL,
                         0,
                         t,
                         sizeof(*t),
                         &dwReturned,
                         NULL ) )
    retVal = -1;
  
  CloseHandle( hDrive );
  return retVal;
}


/*
 * 
 */
int aspiReadTOC( int driveNo, TOC *t )
{
  HANDLE hEvent;
  SRB_ExecSCSICmd s;
  DWORD dwStatus;
  
  if ( driveNo > driveList.num )
    return -2;
  
  hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
  
  ZeroMemory( &s, sizeof(s) );
  s.SRB_Cmd      = SC_EXEC_SCSI_CMD;
  s.SRB_HaID     = driveList.drive[driveNo].a.ha;
  s.SRB_Target   = driveList.drive[driveNo].a.tgt;
  s.SRB_Lun      = driveList.drive[driveNo].a.lun;
  s.SRB_Flags    = SRB_DIR_IN | SRB_EVENT_NOTIFY;
  s.SRB_BufLen   = 0x324;
  s.SRB_BufPointer = (BYTE FAR *)t;
  s.SRB_SenseLen = 0x0E;
  s.SRB_CDBLen   = 0x0A;
  s.SRB_PostProc = (LPVOID)hEvent;
  s.CDBByte[0]   = 0x43;   // read TOC command
  s.CDBByte[1]   = 0x02;   // MSF mode 
  s.CDBByte[7]   = 0x03;   // high-order byte of buffer len
  s.CDBByte[8]   = 0x24;   // low-order byte of buffer len
  
  ResetEvent( hEvent );
  dwStatus = SendASPI32Command( (LPSRB)&s );
  
  if ( dwStatus == SS_PENDING )
  {
    WaitForSingleObject( hEvent, 10000 );  // wait up to 10 secs
  }
  CloseHandle( hEvent );
  
  if ( s.SRB_Status != SS_COMP )
    return -3;
  
  return 0;
}

static int GetCDID(int drvno, TCHAR* msg)
{
	int numTracks;
	TOC toc;
	int cddbid = 0;

	if ( !bInit) return -1;
    if ( !cdif[iActiveInterface].avail )
    {
      if (msg) sprintf(msg, "%s interface not available.\n", cdif[iActiveInterface].name );
      return -1;
    }

      ZeroMemory( &toc, sizeof(toc) );
      readTOC( drvno, &toc );
      if ( !toc.firstTrack &&  !toc.lastTrack )
        numTracks = 0;
      else
        numTracks = (int)(toc.lastTrack - toc.firstTrack + 1 );
  
      if ( numTracks > 0 )
      {
        TCHAR buf[2048];
		cddbid = genCddbId( &toc );
        // calculate the cddbId
        genCddbQuery( &toc, buf, 2048 );
		if (msg) strcpy(msg, buf);
      }

	  /*
    if ( intNo == INTERFACE_IOCTL_CDROM )
    {
      printf( "\n" );
      printf( "    Warning: IOCTL_CDROM_READ_TOC should only be used as a last resort.\n" );
      printf( "    As it does not include data tracks from mixed-mode discs, it can\n" );
      printf( "    sometimes produce incorrect results.\n" );
    }
	*/
	return cddbid;
}

EXPORT TCHAR* mcGetDiscInfo(int disctype, int track, TCHAR* discroot)
{
	static char* info = 0;
	if (info) free(info);
	switch (disctype) {
	case 0: {
		TCHAR buf[512];
		buf[0] = 0;
		GetCDID(track, buf);
		info = strdup(buf);
		return info;
		} break;
	case 2: {
		CString cmd;
		SHELL_PARAM sp;
		memset(&sp, 0, sizeof(sp));
		cmd.Format("tools\\ifodump.exe %s %d", discroot, track);
		sp.pchCommandLine = cmd.GetBuffer();
		sp.flags = SHELL_ALLOC | SHELL_READ_STDOUT_ALL | SHELL_REDIRECT_STDOUT | SHELL_CONVERT_LF | SHELL_REDIRECT_STDERR;
		if (ShellExec(&sp)) return 0;
		ShellRead(&sp, 0);
		info = sp.buffer;
		sp.flags &= ~SHELL_ALLOC;
		ShellClean(&sp);
		return info;
		}
	}
	return 0;
}

EXPORT int mcGetDriveTrackCount(int drvno)
{
	TOC toc;
	ZeroMemory( &toc, sizeof(toc) );
	readTOC( drvno, &toc );
	return ( !toc.firstTrack &&  !toc.lastTrack ) ? 0 : toc.lastTrack - toc.firstTrack + 1;
}

EXPORT int mcGetDriveCount()
{
  if ( !bInit )
    return 0;

  switch( iActiveInterface )
  {
    case INTERFACE_ASPI:
      return aspiGetNumDrives();
    case INTERFACE_SPTI:
    case INTERFACE_IOCTL_CDROM:
      return ntGetNumDrives();
  }
  
  return 0;
}
