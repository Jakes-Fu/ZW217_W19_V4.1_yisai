/*****************************************************************************
** File Name:      mmicom_trace.h                                            *
** Author:         chunjuan.liang                                            *
** Date:           2/20/2020                                                 *
** Copyright:                                                                *
** Description:    This file is used to define common part for king          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2020       chunjuan.liang       Create
******************************************************************************/
#ifndef _MMICOM_TRACE_H_
#define _MMICOM_TRACE_H_

#ifndef WIN32
#define TRACE_APP_CAMERA(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Camera", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_SETTINGS(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Settings", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_LAUNCHER(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Launcher", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)


#define TRACE_APP_SLIDEPAGE(_format,...) \
        do \
        { \
            SCI_TraceLow("{%s}[%s]<%d> "_format, "SlidePage", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        }while(0)


#define TRACE_APP_ENGINEERMODE(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Engineer Mode", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_GPS(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "GPS", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_MESSAGES(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Messages", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_VOICERECORDER(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "TRACE_APP_VoiceRecorder", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_CALLCTRL(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Call Ctrl", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_VIDEOS(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "VIDEOS", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_COMMON(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Common", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_GALLERY(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Gallery", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_STOPWATCH(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "StopWatch", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_STARTUP(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Startup", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_WLAN(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Wlan", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_NOTIFYCENTER(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "NotifyCenter", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_OLD_PUPWIN(_format,...) \
        do \
        { \
            SCI_TraceLow("############################################################");\
            SCI_TraceLow("{%s}[%s]<%d> "_format, "Old pupwin of feature phone pop up!", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        }while(0)

#define TRACE_APP_ALARM(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "WatchAlarm", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_BT(_format,...) \
        do \
        { \
            SCI_TraceLow("{%s}[%s]<%d> "_format, "BT APP", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        }while(0)
#define TRACE_APP_SENSOR(_format,...) \
        do \
        { \
            SCI_TraceLow("{%s}[%s]<%d> "_format, "Sensor APP", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        }while(0)

#ifdef ESIM_SUPPORT_HB
#define TRACE_APP_ESIM_HB(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "ESIM_HB", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#endif

#ifdef SFR_SUPPORT_CMCC
#ifndef WIN32
#define TRACE_SFR_CMCC(_format,...) \
        do \
        { \
            SCI_TraceLow("{%s}[%s]<%d> "_format, "SFR_CMCC", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        }while(0)

#else
#define TRACE_SFR_CMCC(_format,...) \
        do \
        { \
            printf("{%s}<%d>"_format, "SFR_CMCC",  __LINE__, ##__VA_ARGS__);\
        }while(0)
#endif
#endif

#ifdef BBM_ROLE_SUPPORT
#define TRACE_APP_BBM(_format,...) \
                    do \
                    { \
                        SCI_TraceLow("{%s}[%s]<%d> "_format, "BBM", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
                    }while(0)
#endif
#define TRACE_JSON_PARSER(_format,...) \
            do \
            { \
                SCI_TraceLow("{%s}[%s]<%d> "_format, "JSON_PARSER", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
            }while(0)
#define TRACE_WATCHFACE(_format,...) \
            do \
            { \
                SCI_TraceLow("{%s}[%s]<%d> "_format, "WATCHFACE", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
            }while(0)

#else
extern  void SCI_Trace(const char*, ...);

#define TRACE_APP_CAMERA(_format,...)     SCI_Trace("{%s}[%s]<%d> "_format, "CAMERA", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define TRACE_APP_SETTINGS(_format,...)     SCI_Trace("{%s}[%s]<%d> "_format, "SETTING", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define TRACE_APP_LAUNCHER(_format,...)     SCI_Trace("{%s}[%s]<%d> "_format, "LAUNCHER", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define TRACE_APP_SLIDEPAGE(_format,...)     SCI_Trace("{%s}[%s]<%d> "_format, "SlidePage", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define TRACE_APP_ENGINEERMODE  SCI_TRACE_LOW
#define TRACE_APP_GPS           SCI_TRACE_LOW
#define TRACE_APP_MESSAGES      SCI_TRACE_LOW
#define TRACE_APP_VOICERECORDER SCI_TRACE_LOW
#define TRACE_APP_CALLCTRL      SCI_TRACE_LOW
#define TRACE_APP_STOPWATCH     SCI_TRACE_LOW
#define TRACE_APP_VIDEOS        SCI_TRACE_LOW
#define TRACE_APP_COMMON        SCI_TRACE_LOW
#define TRACE_APP_GALLERY       SCI_TRACE_LOW
#define TRACE_APP_STOPWATCH     SCI_TRACE_LOW
#define TRACE_APP_STARTUP       SCI_TRACE_LOW
#define TRACE_APP_WLAN          SCI_TRACE_LOW
#define TRACE_APP_NOTIFYCENTER  SCI_TRACE_LOW
#define TRACE_APP_OLD_PUPWIN    SCI_TRACE_LOW
#ifdef ESIM_SUPPORT_HB
#define TRACE_APP_ESIM_HB       SCI_TRACE_LOW
#endif

#define TRACE_APP_ALARM    SCI_TRACE_LOW
#define TRACE_APP_BT            SCI_TRACE_LOW
#define TRACE_APP_SENSOR        SCI_TRACE_LOW

#ifdef BBM_ROLE_SUPPORT
#define TRACE_APP_BBM          SCI_TRACE_LOW
#endif
#define TRACE_JSON_PARSER      SCI_TRACE_LOW
#define TRACE_WATCHFACE(_format,...)     SCI_Trace("{%s}[%s]<%d> "_format, "WATCHFACE", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#endif
