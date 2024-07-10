/*****************************************************************************
** File Name:      mime_type.h                                               *
** Author:                                                                   *
** Date:           06/10/2008                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    OMA DRM v1.0: mime type                                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/18/2007     @681             Create                                    *
******************************************************************************/

#ifndef _MIME_TYPE_H_
#define _MIME_TYPE_H_



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Enum			                                 */
/*---------------------------------------------------------------------------*/
// mime type
typedef enum {        
    MIME_TYPE_ANY_ANY,          // "*/*"
    MIME_TYPE_TEXT_ANY,         // "text/*"
    MIME_TYPE_TEXT_HTML,        // "text/html"
    MIME_TYPE_TEXT_WML,        // "text/vnd.wap.wml"
    MIME_TYPE_TEXT_WMLS,
    MIME_TYPE_TEXT_WMLSC,
    MIME_TYPE_TEXT_WMLC,
    MIME_TYPE_TEXT_PLAIN,       // "text/plain"
    MIME_TYPE_TEXT_CSS,
    MIME_TYPE_TEXT_X_VCALENDAR, // "text/x-vcalendar
    MIME_TYPE_TEXT_X_VCARD,     // "text/x-vcard"
    MIME_TYPE_TEXT_XML,         // "text/xml"
    MIME_TYPE_APPLICATION_ANY,
    MIME_TYPE_APPLICATION_JAVA_VM,
    MIME_TYPE_APPLICATION_JAVA_JAR,
    MIME_TYPE_APPLICATION_JAVA_JAD,
    MIME_TYPE_APPLICATION_XML,    
    MIME_TYPE_APPLICATION_VND_WAP_WBXML,
    MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE,
    MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT,
    MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML,
    MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML,
    MIME_TYPE_APPLICATION_VND_OMA_DD,
    MIME_TYPE_IMAGE_ANY,                // image start
    MIME_TYPE_IMAGE_GIF,
    MIME_TYPE_IMAGE_JPEG,
    MIME_TYPE_IMAGE_TIFF,
    MIME_TYPE_IMAGE_PNG,
    MIME_TYPE_IMAGE_VND_WAP_WBMP,
    MIME_TYPE_IMAGE_SVG,
    MIME_TYPE_IMAGE_BMP,
    MIME_TYPE_IMAGE_X_UP_WPNG,    
    MIME_TYPE_IMAGE_IEF,
    MIME_TYPE_AUDIO_ANY,                // audio start
    MIME_TYPE_AUDIO_MID,
    MIME_TYPE_AUDIO_MPEG,
    MIME_TYPE_AUDIO_X_WAV,
    MIME_TYPE_AUDIO_AMR,
    MIME_TYPE_AUDIO_X_AAC,
    MIME_TYPE_AUDIO_MIDI,
    MIME_TYPE_AUDIO_X_MIDI,
    MIME_TYPE_AUDIO_SP_MIDI,
    MIME_TYPE_AUDIO_IMELODY,
    MIME_TYPE_AUDIO_WMA,
    MIME_TYPE_AUDIO_MPA,
    MIME_TYPE_AUDIO_MPA_ROBUST,
    MIME_TYPE_AUDIO_MPA_LATM,
    MIME_TYPE_AUDIO_TONE,
    MIME_TYPE_AUDIO_RM,
    MIME_TYPE_AUDIO_3GP,
    MIME_TYPE_TEXT_X_IMELODY,
    MIME_TYPE_APPLICATION_VND_SMAF,
    MIME_TYPE_APPLICATION_X_SMAF,
    MIME_TYPE_APPLICATION_X_MMF,
    MIME_TYPE_APPLICATION_SMIL,
    MIME_TYPE_VIDEO_MPEG,
    MIME_TYPE_VIDEO_3GPP,
    MIME_TYPE_VIDEO_AVI,
    MIME_TYPE_VIDEO_MP4,
    MIME_TYPE_VIDEO_KMV,
    MIME_TYPE_VIDEO_SMV,//smv_support
    MIME_TYPE_VIDEO_HMV,//hmv_support
    MIME_TYPE_VIDEO_WMV,
    MIME_TYPE_APPLICATION_X_SHOCKWAVE_FLASH,
    MIME_TYPE_APPLICATION_ZIP,   
    MIME_TYPE_APPLICATION_SDP,
    MIME_TYPE_APPLICATION_QBT,
    MIME_TYPE_APPLICATION_PNL,
    MIME_TYPE_APPLICATION_NATC,
    MIME_TYPE_UNKNOWN
}MIME_TYPE_E;




/*---------------------------------------------------------------------------*/
/*                          Struct				                             */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          Message				                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: convert mime string to uint32
// Global resource dependence: 
// Author: @681
// Retrun: mime type
// Note: 
/*****************************************************************************/
PUBLIC uint32 mime_to_uint( const char *mime_ptr );

/*****************************************************************************/
// Description: convert mime string to uint32
// Global resource dependence: 
// Author: @681
// Retrun: mime type
// Note: 
/*****************************************************************************/
PUBLIC uint32 mime_ext_to_uint( const char *ext_ptr );

/*****************************************************************************/
// Description: convert uint32 mime type to string
// Global resource dependence: 
// Author: @681
// Retrun: mime type string
// Note: 
/*****************************************************************************/
PUBLIC const char* uint_to_mime( uint32 mime_value);
 
/*****************************************************************************/
// Description: get extension of a special mime type
// Global resource dependence: 
// Author: @681
// Retrun: ext name
// Note: 
/*****************************************************************************/
PUBLIC const char *mime_to_ext( uint32 mime_value );

/*****************************************************************************/
// Description: convert mime string to ext
// Global resource dependence: 
// Author: Jiaoyou.wu
// Retrun: mime type
// Note: 
/*****************************************************************************/
PUBLIC const char *mime_to_mime_ext( const char *mime_ptr );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif //_MIME_TYPE_H_
