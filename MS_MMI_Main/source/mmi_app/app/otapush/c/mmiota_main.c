/*****************************************************************************
** File Name:      mmipushota_main.c                                                 *
** Author:                                                                   *
** Date:           11/2008                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pushota                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008        minghumao       Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_otapush_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiidle_export.h"
#include "mmiota_main.h"
#include "mmiota_export.h"
#include "wbxml_api.h"
#include "wsp_header_api.h"
#include "gui_ucs2b_converter.h"
#include "mmimms_push.h"
#include "mmi_common.h"
#ifdef DCD_SUPPORT
#include "mmidcd_export.h"
#include "mmidcd_main.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm.h" 
#endif

#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmiset_text.h"
#include "mmipub.h"
#ifdef MMI_OTA_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define  OTA_STACK_OBJECT          unsigned long           //stack objects
#define  OTA_STACK_DEFAULT_GROW   (20)                     //stack defaut growth
#define  OTA_STACK_SIZE            10                      //stack size
#define  OTA_STACK_ALLOW_GROW      1                       //stack allow grow

#ifdef MMI_OTA_SUPPORT
#define MMIOTA_PUSHLIST_NAME        (s_mmiota_pushlist_name)
#define MMIOTA_PUSHLIST_NAME_LEN    12
#define MMIOTA_INVAILD_INDEX        0xffff
#endif

#if defined(OMA_CP_SEC_SUPPORT)&&defined(TRACE_INFO_SUPPORT)
//#define OMA_CP_SEC_DEBUG
#endif
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#ifdef MMI_OTA_SUPPORT
MMIOTA_LONG_PUSH_T *s_long_ota_push = PNULL;
const wchar s_mmiota_pushlist_name[] = { 'p', 'u', 's', 'h', 'l', 'i', 's', 't', '.', 'i', 'n', 'i', 0 };
//const wchar g_mmiota_dir_wstr[] = {'M', 'M', 'S', 0 };
extern const wchar g_mmiota_dir_wstr[];

#ifdef WIN32
const uint8 nokia_bookmark_setting[] = {
        0x01,0x06,
        0x2c,
        0x1f,0x2a,
        0x61,0x70,0x70,0x6c,0x69,0x63,0x61,0x74,0x69,0x6f,0x6e,0x2f,0x78,0x2d, //14
        0x77,0x61,0x70,0x2d,0x70,0x72,0x6f,0x76,0x2e,0x62,0x72,0x6f,0x77,0x73,0x65,0x72, //16
        0x2d,0x73,0x65,0x74,0x74,0x69,0x6e,0x67,0x73,0x00,                     //10
        0x81,0xea,
        0x01,0x01,0x6A,0x00,0x45,0xC6,0x7F,0x01,0x87,0x15,
        0x11,0x03,0xE6,0x88,0x91,0xE7,0x9A,0x84,0x77,0x61,
        0x70,0x00,0x01,0x87,0x17,0x11,0x03,0x87,0x68,0x74,
        0x74,0x70,0x3A,0x2F,0x2F,0x6D,0x79,0x77,0x61,0x70,
        0x2E,0x63,0x6F,0x6D,0x00,0x01,0x01,0x01
};
    
const uint8 vodafone_nokia_ota_data[] = {
        0x91, 0x06, 0x2c, 0x1f, 0x2a, 0x61, 0x70, 0x70,
        0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e,
        0x2f, 0x78, 0x2d, 0x77, 0x61, 0x70, 0x2d, 0x70,
        0x72, 0x6f, 0x76, 0x2e, 0x62, 0x72, 0x6f, 0x77,
        0x73, 0x65, 0x72, 0x2d, 0x73, 0x65, 0x74, 0x74,
        0x69, 0x6e, 0x67, 0x73, 0x00, 0x81, 0xea, 
        0x01,
        0x01, 0x6a, 0x00, 0x45, 0xc6, 0x06, 0x01, 0x87,
        0x12, 0x49, 0x01, 0x87, 0x13, 0x11, 0x03, 0x31,
        0x30, 0x2e, 0x31, 0x30, 0x2e, 0x31, 0x2e, 0x31,
        0x30, 0x30, 0x00, 0x01, 0x87, 0x14, 0x61, 0x01,
        0x87, 0x1c, 0x11, 0x03, 0x70, 0x6f, 0x72, 0x74,
        0x61, 0x6c, 0x6e, 0x6d, 0x6d, 0x73, 0x00, 0x01,
        0x87, 0x7e, 0x11, 0x03, 0x56, 0x6f, 0x64, 0x61,
        0x66, 0x6f, 0x6e, 0x65, 0x20, 0x4d, 0x4d, 0x53,
        0x00, 0x01, 0x01, 0x86, 0x7c, 0x11, 0x03, 0x68,
        0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x6d, 0x6d,
        0x73, 0x31, 0x2e, 0x6c, 0x69, 0x76, 0x65, 0x2e,
        0x76, 0x6f, 0x64, 0x61, 0x66, 0x6f, 0x6e, 0x65,
        0x2e, 0x69, 0x6e, 0x2f, 0x6d, 0x6d, 0x73, 0x2f,
        0x00, 0x01, 0xc6, 0x08, 0x01, 0x87, 0x15, 0x11,
        0x03, 0x56, 0x6f, 0x64, 0x61, 0x66, 0x6f, 0x6e,
        0x65, 0x20, 0x4d, 0x4d, 0x53, 0x00, 0x01, 0x01,
        0x01        
}; 

const uint8 wap_push_si[] = {
        0xdc, 0x06, 0x01, 0xae, 0x02, 0x05, 0x6a, 0x00, 
        0x45, 0xc6, 0x0c, 0x03, 0x77, 0x61, 0x70, 0x2e, 
        0x31, 0x32, 0x35, 0x33, 0x30, 0x2e, 0x63, 0x6f, 
        0x6d, 0x2f, 0x73, 0x2f, 0x64, 0x2f, 0x31, 0x33, 
        0x34, 0x36, 0x2f, 0x70, 0x69, 0x63, 0x32, 0x2f, 
        0x32, 0x30, 0x38, 0x00, 0x01, 0x03, 0xe6, 0x89, 
        0x8b, 0xe6, 0x9c, 0xba, 0xe7, 0x85, 0xa7, 0xe7, 
        0x89, 0x87, 0xe4, 0xb8, 0x8b, 0xe8, 0xbd, 0xbd, 
        0x3a, 0xe7, 0x8e, 0x8b, 0xe8, 0x8f, 0xb2, 0x00, 
        0x01, 0x01
};
#endif
#endif

#ifdef MMS_SUPPORT
extern uint8 *mms_sc_address_ptr;
extern MN_DUAL_SYS_E s_push_dual_sys;      //??àíMMS?aμ??à1??ó?úoˉêyoó?éò?è￥μ?′?±?á?
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct {
    char     *name;   
    OTA_MIME_TYPE_E type;
}OTA_MIME_CONTENT_TYPE_T;

typedef struct {
    uint32              allow_grow;
    uint32              size;
    uint32              len;
    OTA_STACK_OBJECT*   stack;
}OTA_STACK_T;

typedef struct ota_characteristic_item_t{
    OTA_TAG_ITEM_T   *item_ptr;
    struct ota_characteristic_item_t   *next_ptr;
}OTA_CHARACTERISTIC_ITEM_T;

typedef struct {
    OTA_CHARACTERISTIC_ITEM_T   *bootstarp_ptr;
    OTA_CHARACTERISTIC_ITEM_T   *pxlogical_ptr;
    OTA_CHARACTERISTIC_ITEM_T   *napdef_ptr;
    OTA_CHARACTERISTIC_ITEM_T   *application_ptr;
}OTA_SETTING_MAP_T;

typedef struct ota_application_map_t {
    OTA_CHARACTERISTIC_ITEM_T   *pxlogical_ptr;
    OTA_CHARACTERISTIC_ITEM_T   *napdef_ptr;
    OTA_CHARACTERISTIC_ITEM_T   *application_ptr;
    struct ota_application_map_t *next_ptr;
}OTA_APPLICATION_MAP_T;

typedef enum{        
        CON_BOOTSTRAP = 0,
        CON_BOOTSTRAP_NAME,
        CON_BOOTSTRAP_NETWORK,
        CON_BOOTSTRAP_COUNTRY,
        CON_BOOTSTRAP_PROXYID,
        CON_BOOTSTRAP_PROVISION_URL,
        CON_BOOTSTRAP_CONTEXT_ALLOW,

        CON_PXLOGICAL,        
        CON_PXLOGICAL_PROXYID,
        CON_PXLOGICAL_PROXY_PROVIDERID,
        CON_PXLOGICAL_NAME,
        CON_PXLOGICAL_DOMAIN,
        CON_PXLOGICAL_TRUST,
        CON_PXLOGICAL_MASTER,
        CON_PXLOGICAL_STARTPAGE,
        CON_PXLOGICAL_BASAUTH_ID,
        CON_PXLOGICAL_BASAUTH_PW,
        CON_PXLOGICAL_WSP_VERSION,
        CON_PXLOGICAL_PUSHENABLED,
        CON_PXLOGICAL_PULLENABLED,
        CON_PXLOGICAL_PXAUTHINFO,
        CON_PXLOGICAL_PXAUTHINFO_PXAUTHTPYE,
        CON_PXLOGICAL_PXAUTHINFO_PXAUTHID,          //username
        CON_PXLOGICAL_PXAUTHINFO_PXAUTHPW,          //password
        CON_PXLOGICAL_PORT,
        CON_PXLOGICAL_PORT_PORTNBR,
        CON_PXLOGICAL_PORT_SERVICE,
        CON_PXLOGICAL_PXPHYSICAL,
        CON_PXLOGICAL_PXPHYSICAL_PHYSICALPROXYID,
        CON_PXLOGICAL_PXPHYSICAL_DOMAIN,
        CON_PXLOGICAL_PXPHYSICAL_PXADDR, 
        CON_PXLOGICAL_PXPHYSICAL_PXADDRTYPE,
        CON_PXLOGICAL_PXPHYSICAL_WSP_VERSION,
        CON_PXLOGICAL_PXPHYSICAL_PUSHENABLED,
        CON_PXLOGICAL_PXPHYSICAL_PULLENABLED,
        CON_PXLOGICAL_PXPHYSICAL_TONAPID,        
        CON_PXLOGICAL_PXPHYSICAL_PORT,
        CON_PXLOGICAL_PXPHYSICAL_PORT_PORTNBR,
        CON_PXLOGICAL_PXPHYSICAL_PORT_SERVICE,  
        
        CON_NAPDEF,
        CON_NAPDEF_NAPID,
        CON_NAPDEF_BEARER,
        CON_NAPDEF_NAME,
        CON_NAPDEF_INTERNET,
        CON_NAPDEF_NAPADDRESS,
        CON_NAPDEF_NAPADDRTYPE,
        CON_NAPDEF_CALLTYPE,
        CON_NAPDEF_LOCALADDRESS,
        CON_NAPDEF_LOCALADDRTYPE,
        CON_NAPDEF_DNSADDRESS,
        CON_NAPDEF_LINKSPEED,
        CON_NAPDEF_DNLINKSPEED,
        CON_NAPDEF_LINGER,
        CON_NAPDEF_DELIVERY_ERR_SDU,
        CON_NAPDEF_DELIVERY_ORDER,
        CON_NAPDEF_TRAFFIC_CLASS,
        CON_NAPDEF_MAX_SDU_SIZE,
        CON_NAPDEF_MAX_BITRATE_UPLINK,
        CON_NAPDEF_MAX_BITRATE_DNLINK,
        CON_NAPDEF_RESIDUAL_BEARER,
        CON_NAPDEF_SDU_ERROR_RATIO,
        CON_NAPDEF_TRAFFIC_HANDL_PRIORITY,
        CON_NAPDEF_TRANSFER_DELAY,
        CON_NAPDEF_GUARANTEED_BITRATE_UPLINK,
        CON_NAPDEF_GUARANTEED_BITRATE_DNLINK,
        CON_NAPDEF_NAPAUTHINFO,
        CON_NAPDEF_NAPAUTHINFO_AUTHTYPE,
        CON_NAPDEF_NAPAUTHINFO_AUTHNAME,
        CON_NAPDEF_NAPAUTHINFO_AUTHSECRET,
        CON_NAPDEF_VALIDATY,
        CON_NAPDEF_VALIDATY_COUNTRY,
        CON_NAPDEF_VALIDATY_NETWORK,
        CON_NAPDEF_VALIDATY_SID,
        CON_NAPDEF_VALIDATY_SOC,
        CON_NAPDEF_VALIDATY_UNTIL,
        CON_NAPDEF_WLAN,

        CON_APPLICATION,
        CON_APPLICATION_NAME,
        CON_APPLICATION_APPID, 
        CON_APPLICATION_TONAPID,
        CON_APPLICATION_TOPROXY,
        CON_APPLICATION_ADDR,
        CON_APPLICATION_RESOURCE,
        CON_APPLICATION_RESOURCE_NAME,
        CON_APPLICATION_RESOURCE_URI,
                
        CON_MAX                        
}OTA_CONNECTIVITY_NAME_TPYE_E;

typedef struct {
    char     *name;   
    OTA_CONNECTIVITY_NAME_TPYE_E type;
}OTA_CONNECTIVITY_NAME_T;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL const OTA_MIME_CONTENT_TYPE_T s_ota_mime_content_types[] = {
    {"application/x-wap-prov.browser-settings",      MIME_BROWERSETTING},
    {"application/x-wap-prov.browser-bookmarks",     MIME_BROWERBOOKMARK},
    {"application/vnd.wap.connectivity-wbxml",       MIME_OTACONNECTIVITY},
    {"application/vnd.wap.sic",                      MIME_WAP_PUSH_SI},
    {"application/vnd.wap.coc",                      MIME_WAP_PUSH_CO},
	{"application/vnd.wap.slc",                      MIME_WAP_PUSH_SL},
    {"application/x-oma-DCD:DCD.ua",                 MIME_OMA_DCD},
	{"application/vnd.oma.drm.rights+wbxml",          MIME_DRM_RIGHTS},
    {"unknow",                                       MIME_MAX}
};

LOCAL const OTA_ELEMENT_PATH_T s_oma_ota_wap_map[] ={
    {"APPLICATION","APPID",MMIOTA_SETTING_TYPE},
    {"","",MMIOTA_ACCESS_TYPE},
    {"","",MMIOTA_NEED_SECURITY_CONNECT},
    {"PXLOGICAL/PXPHYSICAL/PORT","PORTNBR",MMIOTA_PROXY_PORT},
    {"BOOTSTRAP","NAME",MMIOTA_SEETTING_NAME},
    {"PXLOGICAL/PXPHYSICAL","PXADDR",MMIOTA_PROXY_ADDRESS},
    {"NAPDEF/NAPAUTHINFO","AUTHNAME",MMIOTA_APN_USER_NAME},
    {"NAPDEF/NAPAUTHINFO","AUTHSECRET",MMIOTA_APN_PASSWORD},
    {"NAPDEF","NAP-ADDRESS",MMIOTA_APN},
    {"APPLICATION/RESOURCE","URI",MMIOTA_HOMEPAGE},
    {"APPLICATION/RESOURCE","NAME",MMIOTA_BOOMARK_NAME},
    {"APPLICATION/RESOURCE","URI",MMIOTA_BOOKMARK_URL},
    {"","",MMIOTA_PARAMETER_MAX}
};

LOCAL const OTA_ELEMENT_PATH_T s_oma_ota_mms_map[] ={
    {"APPLICATION","APPID",MMIOTA_SETTING_TYPE},
    {"","",MMIOTA_ACCESS_TYPE},
    {"","",MMIOTA_NEED_SECURITY_CONNECT},
    {"PXLOGICAL/PXPHYSICAL/PORT","PORTNBR",MMIOTA_PROXY_PORT},
    {"BOOTSTRAP","NAME",MMIOTA_SEETTING_NAME},
    {"PXLOGICAL/PXPHYSICAL","PXADDR",MMIOTA_PROXY_ADDRESS},
    {"NAPDEF/NAPAUTHINFO","AUTHNAME",MMIOTA_APN_USER_NAME},
    {"NAPDEF/NAPAUTHINFO","AUTHSECRET",MMIOTA_APN_PASSWORD},
    {"NAPDEF","NAP-ADDRESS",MMIOTA_APN},
    {"APPLICATION","ADDR",MMIOTA_HOMEPAGE},
    {"APPLICATION","NAME",MMIOTA_BOOMARK_NAME},
    {"APPLICATION","ADDR",MMIOTA_BOOKMARK_URL},
    {"","",MMIOTA_PARAMETER_MAX}
};

LOCAL const OTA_ELEMENT_PATH_T s_oma_ota_setting_type_map = {"APPLICATION","APPID",MMIOTA_SETTING_TYPE};
LOCAL const OTA_ELEMENT_PATH_T s_oma_ota_setting_port_map = {"PXLOGICAL/PORT","PORTNBR",MMIOTA_PROXY_PORT};
LOCAL const OTA_ELEMENT_PATH_T s_oma_ota_setting_name_map = {"NAPDEF","NAME",MMIOTA_SEETTING_NAME};
LOCAL const OTA_ELEMENT_PATH_T s_oma_ota_setting_homepage_map = {"PXLOGICAL","STARTPAGE",MMIOTA_HOMEPAGE};
static int16 s_oma_napdef_index = 0;
static int16 s_oma_application_index = 0;
static uint16 s_oma_phisical_add_index = 0;

LOCAL const OTA_ELEMENT_PATH_T s_nokia_wap_ota_map[] ={
    {"NAME","NAME",MMIOTA_SETTING_TYPE},
    {"","",MMIOTA_ACCESS_TYPE},
    {"","",MMIOTA_NEED_SECURITY_CONNECT},
    {"ADDRESS","PORT",MMIOTA_PROXY_PORT},
    {"NAME","NAME",MMIOTA_SEETTING_NAME},
    {"ADDRESS","PROXY",MMIOTA_PROXY_ADDRESS},
    {"ADDRESS","PROXY_AUTHNAME",MMIOTA_APN_USER_NAME},
    {"ADDRESS","PROXY_AUTHSECRET",MMIOTA_APN_PASSWORD},
    {"ADDRESS","GPRS_ACCESSPOINTNAME",MMIOTA_APN},
    {"URL","",MMIOTA_HOMEPAGE},
    {"BOOKMARK","NAME",MMIOTA_BOOMARK_NAME},
    {"BOOKMARK","URL",MMIOTA_BOOKMARK_URL},
    {"","",MMIOTA_PARAMETER_MAX}
};

LOCAL const OTA_ELEMENT_PATH_T s_nokia_mms_ota_map[] ={
    {"NAME","NAME",MMIOTA_SETTING_TYPE},
    {"","",MMIOTA_ACCESS_TYPE},
    {"","",MMIOTA_NEED_SECURITY_CONNECT},
    {"ADDRESS","PORT",MMIOTA_PROXY_PORT},
    {"NAME","NAME",MMIOTA_SEETTING_NAME},
    {"ADDRESS","PROXY",MMIOTA_PROXY_ADDRESS},
    {"ADDRESS","PROXY_AUTHNAME",MMIOTA_APN_USER_NAME},
    {"ADDRESS","PROXY_AUTHSECRET",MMIOTA_APN_PASSWORD},
    {"ADDRESS","GPRS_ACCESSPOINTNAME",MMIOTA_APN},
    {"MMSURL","",MMIOTA_HOMEPAGE},
    {"BOOKMARK","NAME",MMIOTA_BOOMARK_NAME},
    {"BOOKMARK","URL",MMIOTA_BOOKMARK_URL},
    {"","",MMIOTA_PARAMETER_MAX}
};

LOCAL const OTA_ELEMENT_PATH_T s_wap_si_ota_map[] ={
    {"","",MMIOTA_WAP_SI_NAME},
    {"href","",MMIOTA_WAP_SI_URL},
    {"si-id","",MMIOTA_WAP_SI_ID}, 
    {"created","",MMIOTA_WAP_SI_CREATED_TIME}, 
    {"si-expires","",MMIOTA_WAP_SI_EXPIRED_TIME}, 
    {"action","",MMIOTA_WAP_SI_SIGNAL_ACTION}, 
    {"","",MMIOTA_PARAMETER_MAX}
};

LOCAL const OTA_ELEMENT_PATH_T s_wap_co_ota_map[] ={
    {"","",MMIOTA_WAP_CO_NAME},
    {"uri","",MMIOTA_WAP_CO_URL},
    {"service","",MMIOTA_WAP_CO_SERVICE}, 
    {"","",MMIOTA_PARAMETER_MAX}
};

LOCAL const OTA_ELEMENT_PATH_T s_wap_sl_ota_map[] ={
    {"","",MMIOTA_WAP_SL_NAME},
    {"href","",MMIOTA_WAP_SL_URL}, 
    {"action","",MMIOTA_WAP_SL_EXECUTE_ACITON},
    {"","",MMIOTA_PARAMETER_MAX}
};


LOCAL const OTA_CONNECTIVITY_NAME_T s_ota_connectivity_name[] = {
    {"BOOTSTRAP",             CON_BOOTSTRAP},
    {"NAME",                  CON_BOOTSTRAP_NAME},
    {"NETWORK",               CON_BOOTSTRAP_NETWORK},
    {"COUNTRY",               CON_BOOTSTRAP_COUNTRY},
    {"PROXY-ID",              CON_BOOTSTRAP_PROXYID},
    {"PROVURL",               CON_BOOTSTRAP_PROVISION_URL},
    {"CONTEXT-ALLOW",         CON_BOOTSTRAP_CONTEXT_ALLOW},
    
    {"PXLOGICAL",             CON_PXLOGICAL},    
	{"PROXY-ID",              CON_PXLOGICAL_PROXYID},
    {"PROXY-PROVIDER-ID",     CON_PXLOGICAL_PROXY_PROVIDERID},
    {"NAME",                  CON_PXLOGICAL_NAME},
    {"DOMAIN",                CON_PXLOGICAL_DOMAIN},
    {"TRUST",                 CON_PXLOGICAL_TRUST},
    {"MASTER",                CON_PXLOGICAL_MASTER},
    {"STARTPAGE",             CON_PXLOGICAL_STARTPAGE},
    {"BASAUTH-ID",            CON_PXLOGICAL_BASAUTH_ID},
    {"BASAUTH-PW",            CON_PXLOGICAL_BASAUTH_PW},
    {"WSP-VERSION",           CON_PXLOGICAL_WSP_VERSION},
    {"PUSHENABLED",           CON_PXLOGICAL_PUSHENABLED},
    {"PULLENABLED",           CON_PXLOGICAL_PULLENABLED},
    {"PXAUTHINFO",            CON_PXLOGICAL_PXAUTHINFO},
    {"PXAUTH-TYPE",           CON_PXLOGICAL_PXAUTHINFO_PXAUTHTPYE},
	{"PXAUTH-ID",             CON_PXLOGICAL_PXAUTHINFO_PXAUTHID},
    {"PXAUTH-PW",             CON_PXLOGICAL_PXAUTHINFO_PXAUTHPW},
    {"PORT",                  CON_PXLOGICAL_PORT},
    {"PORTNBR",               CON_PXLOGICAL_PORT_PORTNBR},
	{"SERVICE",               CON_PXLOGICAL_PORT_SERVICE},
    {"PXPHYSICAL",            CON_PXLOGICAL_PXPHYSICAL},
    {"PHYSICAL-PROXY-ID",     CON_PXLOGICAL_PXPHYSICAL_PHYSICALPROXYID},
    {"DOMAIN",                CON_PXLOGICAL_PXPHYSICAL_DOMAIN},
    {"PXADDR",                CON_PXLOGICAL_PXPHYSICAL_PXADDR},
    {"PXADDRTYPE",            CON_PXLOGICAL_PXPHYSICAL_PXADDRTYPE},	
    {"WSP-VERSION",           CON_PXLOGICAL_PXPHYSICAL_WSP_VERSION},
    {"PUSHENABLED",           CON_PXLOGICAL_PXPHYSICAL_PUSHENABLED},
    {"PULLENABLED",           CON_PXLOGICAL_PXPHYSICAL_PULLENABLED},
    {"TO-NAPID",              CON_PXLOGICAL_PXPHYSICAL_TONAPID},
    {"PORT",                  CON_PXLOGICAL_PXPHYSICAL_PORT},
    {"PORTNBR",               CON_PXLOGICAL_PXPHYSICAL_PORT_PORTNBR},
	{"SERVICE",               CON_PXLOGICAL_PXPHYSICAL_PORT_SERVICE},
    
    {"NAPDEF",                CON_NAPDEF},    
    {"NAPID",                 CON_NAPDEF_NAPID},
	{"BEARER",                CON_NAPDEF_BEARER},
    {"NAME",                  CON_NAPDEF_NAME},
    {"INTERNET",              CON_NAPDEF_INTERNET},
    {"NAP-ADDRESS",           CON_NAPDEF_NAPADDRESS},
    {"NAP-ADDRTYPE",          CON_NAPDEF_NAPADDRTYPE},
    {"DNS-ADDR",              CON_NAPDEF_DNSADDRESS},
    {"CALLTYPE",              CON_NAPDEF_CALLTYPE},
    {"LOCAL-ADDR",            CON_NAPDEF_LOCALADDRESS},
    {"LOCAL-ADDRTYPE",        CON_NAPDEF_LOCALADDRTYPE},
    {"LINKSPEED",             CON_NAPDEF_LINKSPEED},
    {"DNLINKSPEED",           CON_NAPDEF_DNLINKSPEED},
    {"LINGER",                CON_NAPDEF_LINGER},
    {"DELIVERY-ERR-SDU",      CON_NAPDEF_DELIVERY_ERR_SDU},
    {"DELIVERY-ORDER",        CON_NAPDEF_DELIVERY_ORDER},
    {"TRAFFIC-CALSS",         CON_NAPDEF_TRAFFIC_CLASS},
    {"MAX-SDU-SIZE",          CON_NAPDEF_MAX_SDU_SIZE},
    {"MAX-BITRATE-UPLINK",    CON_NAPDEF_MAX_BITRATE_UPLINK},
    {"MAX-BITRATE-DNLINK",    CON_NAPDEF_MAX_BITRATE_DNLINK},
    {"RESIDUAL-BER",          CON_NAPDEF_RESIDUAL_BEARER},
    {"SDU-ERROR-RATIO",       CON_NAPDEF_SDU_ERROR_RATIO},
    {"TRAFFIC-HANDL-PRIO",    CON_NAPDEF_TRAFFIC_HANDL_PRIORITY},
    {"TRANSFER-DELAY",        CON_NAPDEF_TRANSFER_DELAY},
    {"GUARANTEED-BITRATE-UPLINK", CON_NAPDEF_GUARANTEED_BITRATE_UPLINK},
    {"GUARANTEED-BITRATE-DNLINK", CON_NAPDEF_GUARANTEED_BITRATE_DNLINK},
    {"NAPAUTHINFO",           CON_NAPDEF_NAPAUTHINFO},
	{"AUTHTYPE",              CON_NAPDEF_NAPAUTHINFO_AUTHTYPE},
    {"AUTHNAME",              CON_NAPDEF_NAPAUTHINFO_AUTHNAME},
    {"AUTHSECRET",            CON_NAPDEF_NAPAUTHINFO_AUTHSECRET},
    {"VALIDATY",              CON_NAPDEF_VALIDATY},
    {"COUNTRY",               CON_NAPDEF_VALIDATY_COUNTRY},
    {"NETWORK",               CON_NAPDEF_VALIDATY_NETWORK},
    {"SID",                   CON_NAPDEF_VALIDATY_SID},
    {"SOC",                   CON_NAPDEF_VALIDATY_SOC},
    {"VALIDUNTIL",            CON_NAPDEF_VALIDATY_UNTIL},
	{"WLAN",                  CON_NAPDEF_WLAN},
    
    {"APPLICATION",           CON_APPLICATION},
    {"NAME",                  CON_APPLICATION_NAME},
    {"APPID",                 CON_APPLICATION_APPID},
    {"TO-PROXY",              CON_APPLICATION_TOPROXY},
	{"TO-NAPID",              CON_APPLICATION_TONAPID},
    {"ADDR",                  CON_APPLICATION_ADDR},
    {"RESOURCE",              CON_APPLICATION_RESOURCE},
	{"NAME",                  CON_APPLICATION_RESOURCE_NAME},
    {"URI",                   CON_APPLICATION_RESOURCE_URI},
    
    {"unknow",                CON_MAX}
};
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
OTA_STACK_T* g_ota_stack = PNULL;                       //stack

OTA_TAG_ITEM_T *g_parsered_xml_root = PNULL;        //parsed xml root

OTA_ELEMENT_PATH_T *  g_ota_map = PNULL;             //ota map 
uint8    g_ota_map_len = 0;                         //ota map len

WBXML_PARSER_T * g_wbxml_parser_ptr = PNULL;            //parser struct

wchar * g_ota_charater_ptr = PNULL; 

OTA_SETTING_MAP_T *g_ota_connectivity_map_ptr = PNULL;
OTA_APPLICATION_MAP_T *g_ota_connectivity_application_map_ptr = PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         DividedOTADataToWSPHeaderAndWBXML
* Function:     把ota数据分解成为wsp数据和wbxml数据
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN DividedOTADataToWSPHeaderAndWBXML(const uint8* pdu_ptr , 
                                                uint32 pdu_len, 
                                                OTA_DATA_T *ota_data_ptr
                                                );

/* ----------------------------------------------------------------------------
* Name:         GetOTAMIMEFromWSPHeader
* Function:     根据wsp数据得到ota setting的MIME Type类型
* Author:       minghu.mao
* Note:         返回值暂时设置为本模块可以处理的三种类型
* ----------------------------------------------------------------------------*/ 
#if defined(OMA_CP_SEC_SUPPORT)
LOCAL OTA_MIME_TYPE_E GetOTAMIMEFromWSPHeader(uint8* header_ptr , 
                                                uint32  header_len,int* sec,uint8* mac);
#else
LOCAL OTA_MIME_TYPE_E GetOTAMIMEFromWSPHeader(uint8* header_ptr, 
                                              uint32  header_len
                                              );
#endif
/* ----------------------------------------------------------------------------
* Name:         CallocOTAMem
* Function:     ota模块封装SCI_ALLOC_APP函数和MEMSET函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void * CallocOTAMem(uint32 size);

/* ----------------------------------------------------------------------------
* Name:         FreeOTAMem
* Function:     ota封装的内存释放函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void FreeOTAMem(void *ptr);

/* ----------------------------------------------------------------------------
* Name:         MallocAndCopyOTAMem
* Function:     ota封装的内存拷贝函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/  
LOCAL BOOLEAN MallocAndCopyOTAMem(void **dst, 
                                  const void *src
                                  );

/* ----------------------------------------------------------------------------
* Name:         AddXMLTreeChild
* Function:     生成WBXML解析tree操作函数，添加子节点
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN AddXMLTreeChild(OTA_TAG_ITEM_T *parent_ptr, 
                              OTA_TAG_ITEM_T *child_ptr
                              );

/* ----------------------------------------------------------------------------
* Name:         AddXMLTreeAtrr
* Function:     添加元素属性
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/
LOCAL BOOLEAN AddXMLTreeAtrr(OTA_TAG_ITEM_T * item_ptr, 
                             const uint8 *att_name_ptr, 
                             const uint8 *att_value_ptr
                             );

/* ----------------------------------------------------------------------------
* Name:         ParserRootDestroy
* Function:     g_parsered_xml_root 销毁
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParserRootDestroy(OTA_TAG_ITEM_T * item_ptr);

/* ----------------------------------------------------------------------------
* Name:         OTAStackRelloc
* Function:     WBXML解析栈操作函数
* Author:       minghu.mao
* Note:         该栈为自动增加的栈，增加BRW_STACK_DEFAULT_GROW为20个元素
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN OTAStackRelloc(OTA_STACK_T* stack_ptr);

/* ----------------------------------------------------------------------------
* Name:         OTAStackCreate
* Function:     创建栈结构
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL OTA_STACK_T* OTAStackCreate(uint32 size, uint32 allow_grow);

/* ----------------------------------------------------------------------------
* Name:         OTAStackPush
* Function:     压栈
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN OTAStackPush(OTA_STACK_T* stack_ptr, 
                           OTA_STACK_OBJECT sz_elem
                           );

/* ----------------------------------------------------------------------------
* Name:         OTAStackIsEmpty
* Function:     判断栈是否空
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8 OTAStackIsEmpty(OTA_STACK_T* stack_ptr);

/* ----------------------------------------------------------------------------
* Name:         OTAStackPop
* Function:     出栈
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN OTAStackPop(OTA_STACK_T* stack_ptr, 
                          OTA_STACK_OBJECT *object_ptr
                          );

/* ----------------------------------------------------------------------------
* Name:         OTAStackDestroy
* Function:     栈销毁
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void OTAStackDestroy(OTA_STACK_T**stack_pptr);

/* ----------------------------------------------------------------------------
* Name:         StartParseWBXMLElement
* Function:     元素解析开始
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void StartParseWBXMLElement(void *user_data_ptr, 
                                  WBXML_TAG_T *element_ptr, 
                                  WBXML_ATTR_T **atts_pptr, 
                                  BOOLEAN empty
                                  );

/* ----------------------------------------------------------------------------
* Name:         EndParseWBXMLElement
* Function:     元素解析结束
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void EndParseWBXMLElement(void *user_data_ptr, 
                                WBXML_TAG_T *element, 
                                BOOLEAN empty
                                );

/* ----------------------------------------------------------------------------
* Name:         ParseWBXMLCharacters
* Function:     解析utf-8数据
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void ParseWBXMLCharacters(void *user_data_ptr, 
                                uint8 *data_ptr, 
                                uint32 start,
                                uint32 length
                                );

/* ----------------------------------------------------------------------------
* Name:         GetOTAUnitVariableValue 
* Function:     push消息数据取得变长数据的长度
* Author:       minghu.mao
* Note:         该函数的标准来至wbxml
* ----------------------------------------------------------------------------*/ 
LOCAL uint32 GetOTAUnitVariableValue(const uint8* pdu_ptr , 
                                     uint32 pdu_len,  uint8* 
                                     offset
                                     );

/* ----------------------------------------------------------------------------
* Name:         DividedOTADataToWSPHeaderAndWBXML
* Function:     把ota数据分解成为wsp数据和wbxml数据
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void DestroyOTAData(OTA_DATA_T *ota_data_ptr);

/* ----------------------------------------------------------------------------
* Name:         TagNodeCreate
* Function:     初始化标签节点
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void TagNodeCreate(OTA_TAG_PATH_T * tag_path_ptr, 
                         BOOLEAN is_get_setting_type
                         );

/* ----------------------------------------------------------------------------
* Name:         SettingPathCreate
* Function:     初始化取得设置参数的具体标签位置
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN SettingPathCreate(OTA_SETTING_PATH_T *setting_path_ptr);

/* ----------------------------------------------------------------------------
* Name:         SettingPathDestroy
* Function:     设置路径销毁
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN SettingPathDestroy(OTA_SETTING_PATH_T *setting_path_ptr);

/* ----------------------------------------------------------------------------
* Name:         MMISettingDestroy
* Function:     销毁MMI setting
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN MMISettingDestroy(MMIOTA_SETTING_T *mmiota_setting_ptr);

/* ----------------------------------------------------------------------------
* Name:         GetSettingVlue
* Function:     取得标签值
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8* GetSettingVlue(OTA_SETTING_PATH_T *setting_path_ptr, 
                            MMIOTA_SETTING_PARAMETER_E  parameter_type
                            );

/* ----------------------------------------------------------------------------
* Name:         GetOTASettingValue 
* Function:     取得设置参数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL MMIOTA_SETTING_TYPE_E GetOTAType(OTA_MIME_TYPE_E mime_type);

/* ----------------------------------------------------------------------------
* Name:         GetOTASettingValue 
* Function:     解析nokia格式的ota类型（彩信，wap，或者bookmark）
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL MMIOTA_SETTING_TYPE_E GetNokiaOtaType(OTA_MIME_TYPE_E mime_type);

/* ----------------------------------------------------------------------------
* Name:         GetOTASettingValue 
* Function:     取得设置参数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN GetOTASettingValue(MMIOTA_SETTING_T  *mmiota_setting_ptr, 
                                 OTA_MIME_TYPE_E mime_type
                                 );

/* ----------------------------------------------------------------------------
* Name:         WbxmlParserCreate
* Function:     创建wbxml parser
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN WbxmlParserCreate(void);

/* ----------------------------------------------------------------------------
* Name:         OTAWbxmlParserDestroy
* Function:     销毁wbxml parser
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void WbxmlParserDestroy(void);

/* ----------------------------------------------------------------------------
* Name:         ParseOTAWbxml
* Function:     初始化wbxml parser中的parse handler，language,user data , 协议map，解析wbxml
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseOTAWbxml(OTA_MIME_TYPE_E mime_type, 
                            OTA_DATA_T  *ota_data_ptr
                            );

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_InitSettingMap 
* Function:     获取application结构
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN MMIOTA_InitSettingMap(OTA_MIME_TYPE_E mime_type);

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_FreeSettingMap 
* Function:     释放setting map空间
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void MMIOTA_FreeSettingMap(OTA_SETTING_MAP_T *setting_map_ptr);

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_FreeSettingMap 
* Function:     释放setting application空间
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void MMIOTA_FreeApplicationMap(OTA_APPLICATION_MAP_T *app_map_ptr);

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_FreeCharMap 
* Function:     释放setting char空间
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void MMIOTA_FreeCharMap(OTA_CHARACTERISTIC_ITEM_T *char_ptr);

/* ----------------------------------------------------------------------------
* Name:         get value by name 
* Function:     根据名称获得相应的值
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8 * MMIOTA_GetValueFromChar(OTA_TAG_ITEM_T *tag_itme_ptr, OTA_CONNECTIVITY_NAME_TPYE_E name_type, uint16 char_num);

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_GetConSettingValue 
* Function:     获取oma ota配置值
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN MMIOTA_GetConSettingValue(OTA_APPLICATION_MAP_T *app_map_ptr, MMIOTA_SETTING_T  *mmiota_setting_ptr);

/* ----------------------------------------------------------------------------
* Name:        MMIOTA_GetItmeNumber 
* Function:     根据名称to napdef和to proxy id的个数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint16 MMIOTA_GetItmeNumber(OTA_TAG_ITEM_T *tag_item_ptr, OTA_CONNECTIVITY_NAME_TPYE_E name_type);

#ifdef MMI_OTA_SUPPORT
/*****************************************************************************/
//  Description : get push list full path
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetOtaPushListPath(
                             wchar          *full_path_name_ptr,    //[OUT]
                             uint16         *full_path_len_ptr      //[OUT]
                             );
                             
/*****************************************************************************/
//  Description : concat push
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConcatOtaPush(
                         MMIOTA_PUSH_DETAIL_T* push_item_ptr,
                         MMIOTA_USER_VALID_DATA_T* mms_data
                         );                             
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         CallocOTAMem
* Function:     ota模块封装SCI_ALLOC_APP函数和MEMSET函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void * CallocOTAMem(uint32 size)  //malloc memory for ota
{
    void *ptr = PNULL;
    if (0 == size)
    {
        return PNULL;
    }
    ptr = SCI_ALLOC_APP(size);   
    if(PNULL != ptr) 
    {
        SCI_MEMSET(ptr,0,size);
    }
    return ptr;
}//end CallocOTAMem

/* ----------------------------------------------------------------------------
* Name:         FreeOTAMem
* Function:     ota封装的内存释放函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void FreeOTAMem(void *ptr)  //free memory for ota
{
    if (ptr != PNULL)
    {
        SCI_FREE(ptr);
        ptr = PNULL;
    }
}//end FreeOTAMem

/* ----------------------------------------------------------------------------
* Name:         MallocAndCopyOTAMem
* Function:     ota封装的内存拷贝函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/  
LOCAL BOOLEAN MallocAndCopyOTAMem(void **dst, const void *src) //malloc and copy attr
{
    int len = 0;
    if(PNULL == dst || PNULL == src)
    {
        return FALSE;    
    }
       
    if(PNULL != *dst)
    {
        FreeOTAMem(*dst);
        *dst = PNULL;
    }     
    
    len = SCI_STRLEN(src);
    *dst = CallocOTAMem(len+1);
    if(PNULL != *dst) 
    {
        SCI_STRCPY(*dst,src);
        return TRUE;
    }
    return FALSE;
}//end MallocAndCopyOTAMem

/* ----------------------------------------------------------------------------
* Name:         AddXMLTreeChild
* Function:     生成WBXML解析tree操作函数，添加子节点
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN AddXMLTreeChild(OTA_TAG_ITEM_T *parent_ptr, OTA_TAG_ITEM_T *child_ptr)
{
    OTA_TAG_ITEM_T *last_ptr = PNULL;
    OTA_TAG_ITEM_T *temp_ptr = PNULL;   
    int32	counter = 0;
    
    if(parent_ptr == PNULL || child_ptr == PNULL)
    {
        return FALSE;
    }
    temp_ptr = parent_ptr->child;
    if(temp_ptr != PNULL)
    {
        while(temp_ptr != PNULL) 
        {
            last_ptr = temp_ptr;
            temp_ptr = temp_ptr->next;
            ++counter;                         //brother index
        }
        last_ptr->next = child_ptr;
    }
    else
    {
        parent_ptr->child = child_ptr;
    }    
    child_ptr->index = counter;
    child_ptr->parent  = parent_ptr;
 
    return  TRUE;
}//end AddXMLTreeChild

/* ----------------------------------------------------------------------------
* Name:         AddXMLTreeAtrr
* Function:     添加元素属性
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/
LOCAL BOOLEAN AddXMLTreeAtrr(OTA_TAG_ITEM_T * item_ptr, const uint8 *att_name_ptr, const uint8 *att_value_ptr)
{
    OTA_ATTR_T *temp_pro_ptr = PNULL;
    OTA_ATTR_T *big_brother_ptr = PNULL; 
    
    if (att_name_ptr == PNULL || att_value_ptr == PNULL)
    {
        return FALSE;
    }
    
    if(item_ptr != PNULL)
    {
        temp_pro_ptr = item_ptr->tag_property;
        if(temp_pro_ptr == PNULL)	
        {
            temp_pro_ptr = (OTA_ATTR_T*)CallocOTAMem(sizeof(OTA_ATTR_T));
            if (temp_pro_ptr == PNULL)
            {
                return FALSE;
            }
            if (!MallocAndCopyOTAMem(&temp_pro_ptr->att_name,(char *)att_name_ptr) || !MallocAndCopyOTAMem(&temp_pro_ptr->att_value,(char*)att_value_ptr))
            {
                FreeOTAMem(temp_pro_ptr);
                temp_pro_ptr = PNULL;
            }
            item_ptr->tag_property = temp_pro_ptr;
            return TRUE;
        }//end if(temp_pro_ptr==PNULL)
        else
        {           
            big_brother_ptr = temp_pro_ptr;
            while(temp_pro_ptr != PNULL)
            {
                if( !strcmp((char*)temp_pro_ptr->att_name, (char*)att_name_ptr) )
                {
                    if(att_value_ptr != PNULL)	
                    {
                        FreeOTAMem(temp_pro_ptr->att_value);
                        temp_pro_ptr->att_value = PNULL;
                        MallocAndCopyOTAMem(&temp_pro_ptr->att_value, (char *)att_value_ptr);
                    }
                    return TRUE;
                }
                big_brother_ptr = temp_pro_ptr;
                temp_pro_ptr = temp_pro_ptr->next;  
            }
            temp_pro_ptr = (OTA_ATTR_T *)CallocOTAMem(sizeof(OTA_ATTR_T));
            if(temp_pro_ptr == PNULL)
            {
                return FALSE;
            }
            if (!MallocAndCopyOTAMem(&temp_pro_ptr->att_name, (char *)att_name_ptr) || !MallocAndCopyOTAMem(&temp_pro_ptr->att_value, (char *)att_value_ptr))
            {
                FreeOTAMem(temp_pro_ptr);
                temp_pro_ptr = PNULL;
            }      
            if(PNULL != temp_pro_ptr)
            {
                temp_pro_ptr->next  = PNULL;
                big_brother_ptr->next  = temp_pro_ptr;
            }
            return TRUE;            
        }//end else 	
    }//end if(item_ptr)

    return FALSE;    
}//end XMLTreeAddAtrr

/* ----------------------------------------------------------------------------
* Name:         ParserRootDestroy
* Function:     g_parsered_xml_root 销毁
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParserRootDestroy(OTA_TAG_ITEM_T * item_ptr)
{
    OTA_ATTR_T *attr_ptr = PNULL;
    if (item_ptr == PNULL)
    {
        return TRUE;
    }
    while (item_ptr->tag_property != PNULL)
    {
        attr_ptr = item_ptr->tag_property;
        item_ptr->tag_property = item_ptr->tag_property->next;
        FreeOTAMem(attr_ptr->att_name);
        attr_ptr->att_name = PNULL;
        FreeOTAMem(attr_ptr->att_value);
        attr_ptr->att_value =PNULL;
        FreeOTAMem(attr_ptr);
        attr_ptr = PNULL;        
    }
    if (item_ptr->tag_name != PNULL)
    {
        FreeOTAMem(item_ptr->tag_name);
        item_ptr->tag_name = PNULL;
    }
    
    if (item_ptr->tag_add_content != PNULL)
    {
        FreeOTAMem(item_ptr->tag_add_content);
        item_ptr->tag_add_content = PNULL;
    }   
    
    if (item_ptr->child)
    {
        ParserRootDestroy(item_ptr->child);
    }
    
    if (item_ptr->next)
    {
        ParserRootDestroy(item_ptr->next);
    }
    
    
    FreeOTAMem(item_ptr);
    item_ptr = PNULL;

    return TRUE;
}//end ParserRootDestroy

/* ----------------------------------------------------------------------------
* Name:         OTAStackRelloc
* Function:     WBXML解析栈操作函数
* Author:       minghu.mao
* Note:         该栈为自动增加的栈，增加BRW_STACK_DEFAULT_GROW为20个元素
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN OTAStackRelloc(OTA_STACK_T* stack_ptr)
{
    OTA_STACK_OBJECT*  old_stack_ptr = PNULL;
    uint16  length                   = 0;

    old_stack_ptr = stack_ptr->stack;
    stack_ptr->size += OTA_STACK_DEFAULT_GROW;
    stack_ptr->stack = (OTA_STACK_OBJECT*)CallocOTAMem(stack_ptr->size*sizeof(OTA_STACK_OBJECT));

    if(stack_ptr->stack == PNULL) 
    {
        stack_ptr->stack = old_stack_ptr;
        stack_ptr->size -= OTA_STACK_DEFAULT_GROW;
        return FALSE;
    }
    length = stack_ptr->size*sizeof(OTA_STACK_OBJECT);
    SCI_MEMSET(stack_ptr->stack,0,length);
    SCI_MEMCPY(stack_ptr->stack, old_stack_ptr, sizeof(OTA_STACK_OBJECT)*stack_ptr->len);

    return TRUE;
}//end OTAStackRelloc

/* ----------------------------------------------------------------------------
* Name:         OTAStackCreate
* Function:     创建栈结构
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL OTA_STACK_T* OTAStackCreate(uint32 size, uint32 allow_grow)
{
    OTA_STACK_T* stack_ptr = (OTA_STACK_T*)CallocOTAMem(sizeof(OTA_STACK_T));
    if(stack_ptr != PNULL)
    {
        stack_ptr->size = size;
        stack_ptr->allow_grow = allow_grow;
        stack_ptr->stack = (OTA_STACK_OBJECT*)CallocOTAMem(sizeof(OTA_STACK_OBJECT) * size);
        stack_ptr->len = 0;
        if(stack_ptr->stack == PNULL) 
        {
            FreeOTAMem(&stack_ptr);
            stack_ptr = PNULL;
        }
    }

    return stack_ptr;
}//end OTAStackCreate

/* ----------------------------------------------------------------------------
* Name:         OTAStackPush
* Function:     压栈
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN OTAStackPush(OTA_STACK_T* stack_ptr, OTA_STACK_OBJECT sz_elem)
{
    if( PNULL == stack_ptr) 
    {
        return FALSE;
    }
    if(stack_ptr->len >= stack_ptr->size) 
    {
        if(!stack_ptr->allow_grow || !OTAStackRelloc(stack_ptr)) 
        {
            return FALSE;
        }
    }

    *(stack_ptr->stack+stack_ptr->len) = sz_elem;
    ++stack_ptr->len;
    
    return TRUE;
}//end  OTAStackPush

/* ----------------------------------------------------------------------------
* Name:         OTAStackIsEmpty
* Function:     判断栈是否空
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8 OTAStackIsEmpty(OTA_STACK_T* stack_ptr)
{
    if(stack_ptr == PNULL)
    {
        return TRUE;
    }
    return (0 == stack_ptr->len);
}//end OTAStackIsEmpty

/* ----------------------------------------------------------------------------
* Name:         OTAStackPop
* Function:     出栈
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN OTAStackPop(OTA_STACK_T* stack_ptr, OTA_STACK_OBJECT *object_ptr)
{
    if(!stack_ptr || OTAStackIsEmpty(stack_ptr) || object_ptr == PNULL) 
    {
        return FALSE;
    }
    --stack_ptr->len;
    *object_ptr = *(stack_ptr->stack+stack_ptr->len);
    
    return TRUE;
}//end OTAStackPop

/* ----------------------------------------------------------------------------
* Name:         OTAStackTop
* Function:     取栈顶元素
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN OTAStackTop(OTA_STACK_T* stack_ptr, OTA_STACK_OBJECT *object_ptr)
{
    if(!stack_ptr || OTAStackIsEmpty(stack_ptr) || object_ptr == PNULL)
    {
        return FALSE;
    }
    *object_ptr = *(stack_ptr->stack+stack_ptr->len-1);
    return TRUE;
}//end OTAStackTop

/* ----------------------------------------------------------------------------
* Name:         OTAStackDestroy
* Function:     栈销毁
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void OTAStackDestroy(OTA_STACK_T**stack_pptr)
{
    if(stack_pptr == PNULL || (*stack_pptr) == PNULL )
    {
        return;
    }

    FreeOTAMem((*stack_pptr)->stack);
    (*stack_pptr)->stack = PNULL;
    FreeOTAMem(*stack_pptr);
    *stack_pptr = PNULL;
}//end OTAtackDestroy

/* ----------------------------------------------------------------------------
* Name:         StartParseWBXMLDocument
* Function:     WBXML解析文档开始
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
//{    
//}

/* ----------------------------------------------------------------------------
* Name:         EndParseWBXMLDocument
* Function:     WBXML解析文档结束
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
//void EndParseWBXMLDocument(void *user_data)
//{
//}

/* ----------------------------------------------------------------------------
* Name:         StartParseWBXMLElement
* Function:     元素解析开始
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void StartParseWBXMLElement(void *user_data_ptr, WBXML_TAG_T *element_ptr, WBXML_ATTR_T **atts_pptr, BOOLEAN empty)
{   
    OTA_STACK_OBJECT top = 0;
    OTA_TAG_ITEM_T *item_ptr = PNULL;
    OTA_TAG_ITEM_T *user_data_tem_ptr = user_data_ptr;    
    uint8 attr_index = 0;    
   
    //if (user_data_ptr == PNULL || element_ptr == PNULL || atts_pptr == PNULL)
    if (user_data_ptr == PNULL)
    {
        return;
    }
//    if (*atts_pptr == PNULL)
//    {
//        return;
//    }
    item_ptr = (OTA_TAG_ITEM_T*)CallocOTAMem(sizeof(OTA_TAG_ITEM_T));
    if (item_ptr == PNULL)
    {
        return;
    }
#ifdef PLATFORM_ANTISW3

    //set the tag name
    if (!MallocAndCopyOTAMem(&item_ptr->tag_name,(char*)WBXML_TagGetXmlName(element_ptr)))
    {
        return;
    }
    if(atts_pptr != PNULL)  
    {
        while (atts_pptr[attr_index] != PNULL)
        {
            //add the attr
            AddXMLTreeAtrr(item_ptr, (uint8*)WBXML_TagAttrGetXmlName(atts_pptr[attr_index]), (uint8*)WBXML_TagAttrGetXmlValue(atts_pptr[attr_index]));
            attr_index++;
        }        
    }
#elif defined (PLATFORM_UWS6121E)
    if (!MallocAndCopyOTAMem(&item_ptr->tag_name,(char*)wbxml_tag_get_xml_name(element_ptr)))
    {
        return;
    }
    if(atts_pptr != PNULL)  
    {
        while (atts_pptr[attr_index] != PNULL)
        {
            //add the attr
            AddXMLTreeAtrr(item_ptr, (uint8*)wbxml_attribute_get_xml_name(atts_pptr[attr_index]), (uint8*)wbxml_attribute_get_xml_value(atts_pptr[attr_index]));
            attr_index++;
        }        
    }
#endif
    //get the top item and add new item to it
    if(OTAStackTop(g_ota_stack,&top))     
    {
        AddXMLTreeChild((OTA_TAG_ITEM_T*)top, item_ptr);
    }
    if(user_data_tem_ptr->next == PNULL) 
    {
        user_data_tem_ptr->next = item_ptr;
    }
    //push new item into stack
    OTAStackPush(g_ota_stack, (OTA_STACK_OBJECT)item_ptr);
    
}//end StartParseWBXMLElement

/* ----------------------------------------------------------------------------
* Name:         EndParseWBXMLElement
* Function:     元素解析结束
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void EndParseWBXMLElement(void *user_data_ptr, WBXML_TAG_T *element, BOOLEAN empty)
{
    OTA_STACK_OBJECT top;

    OTAStackPop(g_ota_stack, &top);    
}//end EndParseWBXMLElement

/* ----------------------------------------------------------------------------
* Name:         ParseWBXMLCharacters
* Function:     解析utf-8数据
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void ParseWBXMLCharacters(void *user_data_ptr, 
                                uint8 *data_ptr, 
                                uint32 start,
                                uint32 length
                                )
{
    wchar * wstr_ptr = PNULL;
    uint16 wstr_len = 0;

    if (PNULL != g_ota_charater_ptr)
    {
        SCI_FREE(g_ota_charater_ptr);
        g_ota_charater_ptr = PNULL;
    }
    wstr_ptr = (wchar *)SCI_ALLOCA((length + 1) * sizeof(wchar));
    if (PNULL == wstr_ptr)
    {
        return;
    }
    SCI_MEMSET(wstr_ptr, 0, ((length + 1) * sizeof(wchar)));
    wstr_len = GUI_UTF8ToWstr(wstr_ptr, length, data_ptr, length);

    g_ota_charater_ptr = (wchar *)SCI_ALLOCA((wstr_len + 1) * sizeof(wchar));
    if (PNULL == g_ota_charater_ptr)
    {
        SCI_FREE(wstr_ptr);
        wstr_ptr = PNULL;
        return;
    }
    SCI_MEMSET((char *)g_ota_charater_ptr, 0, ((wstr_len + 1) * sizeof(wchar)));
    SCI_MEMCPY((char *)g_ota_charater_ptr, (char *)wstr_ptr, (wstr_len * sizeof(wchar)));

    SCI_FREE(wstr_ptr);
    wstr_ptr = PNULL;
}

/* ----------------------------------------------------------------------------
* Name:         ParseWBXMLCharacters
* Function:     解析characters
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
//void ParseWBXMLCharacters(void *user_data, WB_UTINY *ch, WB_ULONG start, WB_ULONG length)
//{   
//}

/* ----------------------------------------------------------------------------
* Name:         GetOTAUnitVariableValue 
* Function:     push消息数据取得变长数据的长度
* Author:       minghu.mao
* Note:         该函数的标准来至wbxml
* ----------------------------------------------------------------------------*/ 
LOCAL uint32 GetOTAUnitVariableValue(const uint8* pdu_ptr , uint32 pdu_len,  uint8* offset)
{
    int   tempc = 0;
    uint8 value_index = 0;
    uint32 value_index_max = 0;
    unsigned long variableValueLen = 0;
    uint32 value = 0;
    
    if (pdu_ptr == PNULL || pdu_len == 0)
    {
        return value;
    }
    if (offset == PNULL)
    {
        return value;
    }
    *offset = 0;
    value_index_max = pdu_len > WBXML_VARIABLE_VALUE_MAX_LENGTH ? WBXML_VARIABLE_VALUE_MAX_LENGTH : pdu_len;
    for (value_index = 0; value_index < value_index_max; value_index++) 
    {
        tempc = *(pdu_ptr+(*offset) + value_index);
        if (tempc < 0)
        {        
            value = 0;   
        }            
        variableValueLen = (variableValueLen << 7) | (tempc & 0x7f);//0x7f = 0111 1111表示把tempc的低7为赋给variableValueLen
        if (!(tempc & 0x80)) //0x80 = 1000 0000 该if表示当tempc最高为为0分支
        {
            *offset = value_index+1;
            value = variableValueLen;
            break;
        }
    }  

    return value;
}//end  GetOTAUnitVariableValue

/* ----------------------------------------------------------------------------
* Name:         DividedOTADataToWSPHeaderAndWBXML
* Function:     把ota数据分解成为wsp数据和wbxml数据
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN DividedOTADataToWSPHeaderAndWBXML(const uint8* pdu_ptr , uint32 pdu_len,  OTA_DATA_T *ota_data_ptr)
{
    uint8    offset = 0;
    uint8    pos    = 0;
    uint32   mimecontenttype_len =0;

    if (pdu_ptr == PNULL || pdu_len == 0)
    {
        return FALSE;
    }

    pos += 2;                      //pos位置移动到描述mime的变长变量长度的位置
    if (pos < pdu_len)
    {
        mimecontenttype_len = GetOTAUnitVariableValue(&(pdu_ptr[pos]), pdu_len-pos, &offset);
        if (mimecontenttype_len == 0)
        {
            return FALSE;
        }
        else
        {
            pos += offset;                 //pos移动到变长变量长度标识位置后面    
            pos += mimecontenttype_len;    //pos移动到ota body数据位置
        }        
    }    
    
    if (ota_data_ptr != PNULL &&  (pos <= pdu_len))//当数据只有头的时候等于产生
    {
        ota_data_ptr->wsp.length = pos;
        ota_data_ptr->wsp.ota_wsp_header_data_arr = (uint8*)CallocOTAMem(ota_data_ptr->wsp.length+1);
        SCI_MEMCPY(ota_data_ptr->wsp.ota_wsp_header_data_arr, pdu_ptr,ota_data_ptr->wsp.length);
        ota_data_ptr->body.length = OTA_USER_DATA_MAX_LEN > (pdu_len-pos)?(pdu_len-pos):OTA_USER_DATA_MAX_LEN;
        ota_data_ptr->body.ota_user_data_arr = (uint8*)CallocOTAMem(ota_data_ptr->body.length+1);
        SCI_MEMCPY(ota_data_ptr->body.ota_user_data_arr, pdu_ptr+pos,ota_data_ptr->body.length);
        return TRUE;
    }    
    
    return FALSE;
}//end DividedOTADataToWSPHeaderAndWBXML
/* ----------------------------------------------------------------------------
* Name:         DividedOTADataToWSPHeaderAndWBXML
* Function:     把ota数据分解成为wsp数据和wbxml数据
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void DestroyOTAData(OTA_DATA_T *ota_data_ptr)
{
    if (ota_data_ptr->wsp.ota_wsp_header_data_arr != PNULL)
    {
        FreeOTAMem(ota_data_ptr->wsp.ota_wsp_header_data_arr);
        ota_data_ptr->wsp.ota_wsp_header_data_arr = PNULL;
        ota_data_ptr->wsp.length = 0;
        ota_data_ptr->wsp.ota_wsp_header_data_arr = PNULL;
    }
    if (ota_data_ptr->body.ota_user_data_arr != PNULL)
    {
        FreeOTAMem(ota_data_ptr->body.ota_user_data_arr);
        ota_data_ptr->body.ota_user_data_arr = PNULL;
        ota_data_ptr->body.length = 0;
        ota_data_ptr->body.ota_user_data_arr = PNULL;
    }
}

/* ----------------------------------------------------------------------------
* Name:         GetOTAMIMEFromWSPHeader
* Function:     根据wsp数据得到ota setting的MIME Type类型
* Author:       minghu.mao
* Note:         返回值暂时设置为本模块可以处理的三种类型
* ----------------------------------------------------------------------------*/ 
#if defined(OMA_CP_SEC_SUPPORT)
LOCAL OTA_MIME_TYPE_E GetOTAMIMEFromWSPHeader(uint8* header_ptr , uint32  header_len,int* sec,uint8* mac)
#else
LOCAL OTA_MIME_TYPE_E GetOTAMIMEFromWSPHeader(uint8* header_ptr , uint32  header_len)
#endif
{
    HTTP_HEADER  *wspheader_ptr = PNULL;
    HTTP_HEADER  *temp_wspheader_ptr = PNULL;
    uint8    pos    = 0;
    uint8    offset = 0;
    uint8    mime_index = 0;
    uint8    ota_mime_number = 0;
    uint8    *contenttype_ptr = PNULL;
    uint32   mimecontenttype_len = 0;    
    OTA_MIME_TYPE_E  eOTAtype = MIME_MAX;

    if (header_ptr == 0 || header_len == 0)
    {
        return MIME_MAX;
    }
    
    pos += 2;                      //pos位置移动到描述mime的变长变量长度的位置
    SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader pos:%d,header_len=%d",pos,header_len);
    if (pos < header_len)
    {
        mimecontenttype_len = GetOTAUnitVariableValue(&(header_ptr[pos]), header_len-pos,&offset);
    }
    else
    {
        return MIME_MAX;
    }
    SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader mimecontenttype_len:%d ,offset=%d",mimecontenttype_len,offset);
    if (mimecontenttype_len == 0 || mimecontenttype_len > (header_len - pos - offset))
    {
        return MIME_MAX;
    }
    pos += offset;                 //pos移动到变长变量长度标识位置后面
    if (pos >= header_len)
    {
        return MIME_MAX;
    }
	
    wspheader_ptr = WSP_HeadersUnpack_Open(header_ptr+pos,mimecontenttype_len);        //解析wsp头，得到WSP结构

    #ifdef OMA_CP_SEC_DEBUG
    if (wspheader_ptr != PNULL)
    {
        contenttype_ptr = (uint8 *)wspheader_ptr->value;
        SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader contenttype_ptr:%s ",contenttype_ptr);
        temp_wspheader_ptr = wspheader_ptr->next;
        while (PNULL != temp_wspheader_ptr)
        {
            contenttype_ptr = (uint8 *)temp_wspheader_ptr->value;
            SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader contenttype_ptr:%s ",contenttype_ptr);
            temp_wspheader_ptr = wspheader_ptr->next;
        }
    }
    #endif

    if (wspheader_ptr != PNULL)
    {
        contenttype_ptr = (uint8 *)wspheader_ptr->value;
        SCI_TRACE_BUF("zhangqiang test contenttype_ptr",(char*)contenttype_ptr,SCI_STRLEN((char*)contenttype_ptr));
        ota_mime_number = sizeof(s_ota_mime_content_types)/sizeof(s_ota_mime_content_types[0]);
        for (mime_index = 0; mime_index < ota_mime_number; mime_index++)
        {
            if (!MMIAPICOM_CompareTwoUint8String((uint8 *)s_ota_mime_content_types[mime_index].name, strlen((char *)s_ota_mime_content_types[mime_index].name),contenttype_ptr, strlen((char *)contenttype_ptr)))
            {
                eOTAtype = s_ota_mime_content_types[mime_index].type;
                if (MIME_WAP_PUSH_SL == eOTAtype)
                {
                    temp_wspheader_ptr = wspheader_ptr->next;
                    while (PNULL != temp_wspheader_ptr)
                    {
                        contenttype_ptr = (uint8 *)temp_wspheader_ptr->value;
                        if (!MMIAPICOM_CompareTwoUint8String((uint8 *)"application/x-oma-DCD:DCD.ua", strlen((char *)"application/x-oma-DCD:DCD.ua"),contenttype_ptr, strlen((char *)contenttype_ptr)))
                        {
                            eOTAtype = MIME_OMA_DCD;
                            break;
                        }
                        temp_wspheader_ptr = temp_wspheader_ptr->next;
                    }
                } 
                break;
            }
        }

        #if defined(OMA_CP_SEC_SUPPORT)
        SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader eOTAtype=%d",eOTAtype);
        if (MIME_OTACONNECTIVITY == eOTAtype)
        {
            if ((PNULL != wspheader_ptr->parameter))
            {
                SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader parameter->name=%s",wspheader_ptr->parameter->name);
                if(!MMIAPICOM_StrCmp(wspheader_ptr->parameter->name, strlen(wspheader_ptr->parameter->name), "sec", 3))
                {
                    *sec = atoi((char*)wspheader_ptr->parameter->value);
                    SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader secValue=%d",*sec);
                }
                SCI_TRACE_LOW("feeme GetOTAMIMEFromWSPHeader parameter->next->name=%s",wspheader_ptr->parameter->next->name);
                if(!MMIAPICOM_StrCmp(wspheader_ptr->parameter->next->name, strlen(wspheader_ptr->parameter->next->name), "mac", 3))
                {
                    strncpy(mac, wspheader_ptr->parameter->next->value, strlen(wspheader_ptr->parameter->next->value));
                    SCI_TRACE_LOW("[OTA] GetOTAMIMEFromWSPHeader macValue=%s",mac);
                }        
            }
        }
        #endif
        
        WSP_HeadersUnpack_Close(wspheader_ptr);
    }
    
    return eOTAtype;
}//end GetOTAMIMEFromWSPHeader

/* ----------------------------------------------------------------------------
* Name:         TagNodeCreate
* Function:     初始化标签节点
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void TagNodeCreate(OTA_TAG_PATH_T * tag_path_ptr, BOOLEAN is_get_setting_type)
{
    OTA_TAG_NODE_T  *temp_tag_node_ptr = PNULL;
    OTA_TAG_NODE_T  *new_node_ptr = PNULL;
    uint8  *characteristic_ptr = PNULL;
    uint8  *parm_ptr = PNULL;
    uint8  *tempch_ptr = PNULL;
    uint8  map_index = 0;                            //used for ota map index
    uint8  charac_index = 0;                         //used for characteristic string position
    uint8  charac_begin_pos = 0;                     //characteristic begin position
    uint8  charac_len = 0;                           //characteristic length
    uint8  parm_len = 0;                             //parm length 

    if (tag_path_ptr == PNULL)
    {
        return;
    }
    //find the characteristic and parm path by the parameter in the g_OTA_map, characteristic expressed as 'aa/bb/cc'
    if (is_get_setting_type == TRUE)
    {
        if (tag_path_ptr->parameter_name == s_oma_ota_setting_type_map.parameter_name)
        {
            characteristic_ptr = (uint8*)s_oma_ota_setting_type_map.characteristic;
            parm_ptr = (uint8*)s_oma_ota_setting_type_map.parm;
        }
        else if (tag_path_ptr->parameter_name == s_oma_ota_setting_port_map.parameter_name)
        {
            characteristic_ptr = (uint8*)s_oma_ota_setting_port_map.characteristic;
            parm_ptr = (uint8*)s_oma_ota_setting_port_map.parm;
        }
        else if (tag_path_ptr->parameter_name == s_oma_ota_setting_name_map.parameter_name)
        {
            characteristic_ptr = (uint8*)s_oma_ota_setting_name_map.characteristic;
            parm_ptr = (uint8*)s_oma_ota_setting_name_map.parm;
        }
        else if (tag_path_ptr->parameter_name == s_oma_ota_setting_homepage_map.parameter_name)
        {
            characteristic_ptr = (uint8*)s_oma_ota_setting_homepage_map.characteristic;
            parm_ptr = (uint8*)s_oma_ota_setting_homepage_map.parm;
        }
    }
    else
    {
        for ( map_index = 0; map_index < g_ota_map_len; map_index++ )
        {
            if (tag_path_ptr->parameter_name == g_ota_map[map_index].parameter_name)
            {
                characteristic_ptr = (uint8*)g_ota_map[map_index].characteristic;
                parm_ptr = (uint8*)g_ota_map[map_index].parm;
                break;
            }
        }    
    }
    
    //divided characteristic 'aa/bb/cc' into 'aa','bb','cc'
    if (PNULL != characteristic_ptr)
    {
        while (characteristic_ptr[charac_index] != '\0')
        {
            if (characteristic_ptr[charac_index] == '/' || characteristic_ptr[charac_index+1] == '\0')
            {
                if (characteristic_ptr[charac_index+1] == '\0')
                {
                    charac_len++;
                }
                tempch_ptr = (uint8*)CallocOTAMem(charac_len+1);
                if (tempch_ptr != PNULL)
                {
                    SCI_MEMCPY(tempch_ptr,characteristic_ptr+charac_begin_pos,charac_len);
                    tempch_ptr[charac_len] = '\0';
                }            
                
                new_node_ptr = (OTA_TAG_NODE_T*)CallocOTAMem(sizeof(OTA_TAG_NODE_T));
                if (new_node_ptr != PNULL)
                {
                    new_node_ptr->element_type = CHARACTERISTIC;
                    new_node_ptr->tag_name = tempch_ptr;
                }            
                
                if (tag_path_ptr->path_node == PNULL)
                {                  
                    tag_path_ptr->path_node = new_node_ptr; 
                    new_node_ptr = PNULL;
                }
                else
                {
                    temp_tag_node_ptr = tag_path_ptr->path_node;
                    while (temp_tag_node_ptr->next != PNULL)
                    {
                        temp_tag_node_ptr = temp_tag_node_ptr->next;
                    }
                    temp_tag_node_ptr->next = new_node_ptr;
                }                                 
                
                temp_tag_node_ptr = PNULL;
                tempch_ptr = PNULL;
                charac_begin_pos = charac_index+1;
                charac_len = 0;
                charac_index++;
            }
            else
            {    
                charac_index++;
                charac_len++;
            }    
        }
    }
    
    //add parm to last path node
    if (PNULL != parm_ptr)
    {
        if (parm_ptr[0] != '\0')
        {
            tempch_ptr = PNULL;
            parm_len = strlen((char*)parm_ptr);
            tempch_ptr = (uint8*)CallocOTAMem(parm_len+1);
            if (tempch_ptr != PNULL)
            {
                SCI_MEMCPY(tempch_ptr,parm_ptr,parm_len);
                tempch_ptr[parm_len] = '\0'; 
            }               
            
            new_node_ptr = (OTA_TAG_NODE_T*)CallocOTAMem(sizeof(OTA_TAG_NODE_T));
            if (new_node_ptr != PNULL)
            {
                new_node_ptr->element_type = PARM;
                new_node_ptr->tag_name = tempch_ptr;            
            }
            else
            {
                FreeOTAMem(tempch_ptr);    
                tempch_ptr = PNULL;
            }
            tempch_ptr = PNULL;
            
            
            if (tag_path_ptr->path_node == PNULL)
            {                  
                tag_path_ptr->path_node = new_node_ptr; 
                new_node_ptr = PNULL;
            }
            else
            {
                temp_tag_node_ptr = tag_path_ptr->path_node;
                while (temp_tag_node_ptr->next != PNULL)
                {
                    temp_tag_node_ptr = temp_tag_node_ptr->next;
                }
                temp_tag_node_ptr->next = new_node_ptr;
            }
        } 
    }       
}//end TagNodeCreate

/* ----------------------------------------------------------------------------
* Name:         SettingPathCreate
* Function:     初始化取得设置参数的具体标签位置
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN SettingPathCreate(OTA_SETTING_PATH_T *setting_path_ptr)
{
    OTA_TAG_PATH_T      *new_tag_path_ptr = PNULL;
    OTA_SETTING_PATH_T  *tem_setting_path_ptr = PNULL;
    OTA_SETTING_PATH_T  *new_setting_path_ptr = PNULL;
    MMIOTA_SETTING_PARAMETER_E para_type = MMIOTA_SETTING_TYPE;
    
    if (PNULL == setting_path_ptr)
    {
        return FALSE;
    }
    while (para_type != MMIOTA_PARAMETER_MAX)
    { 
        //alloc for the setting path it include tag node path
        new_setting_path_ptr = (OTA_SETTING_PATH_T *)CallocOTAMem(sizeof(OTA_SETTING_PATH_T));
        //alloc for tag node path
        new_tag_path_ptr = (OTA_TAG_PATH_T *)CallocOTAMem(sizeof(OTA_TAG_PATH_T));
        if (new_tag_path_ptr != PNULL)
        {
            new_tag_path_ptr->parameter_name = para_type;
            //create the tag node path by the parameter
            TagNodeCreate(new_tag_path_ptr, FALSE);   
            
            if (new_setting_path_ptr != PNULL && new_tag_path_ptr->path_node != PNULL)
            {
                //if the first setting path node
                if (setting_path_ptr->tag_path == PNULL)
                {
                    setting_path_ptr->tag_path = new_tag_path_ptr;
                    new_tag_path_ptr = PNULL;
                    FreeOTAMem(new_setting_path_ptr);
                    new_setting_path_ptr = PNULL;
                }
                else
                {
                    new_setting_path_ptr->tag_path = new_tag_path_ptr;
                    
                    tem_setting_path_ptr = setting_path_ptr;
                    while (tem_setting_path_ptr->next != PNULL)
                    {
                        tem_setting_path_ptr = tem_setting_path_ptr->next;
                    }
                    tem_setting_path_ptr->next = new_setting_path_ptr;
                    new_tag_path_ptr = PNULL;
                    new_setting_path_ptr = PNULL;
                } 
            }
            else if (new_tag_path_ptr->path_node == PNULL)
            {
                FreeOTAMem(new_tag_path_ptr);
                new_tag_path_ptr = PNULL;
                FreeOTAMem(new_setting_path_ptr);
                new_setting_path_ptr = PNULL;
            }
            para_type++;
        }
        else
        {
            return FALSE;
        }
    }
   return TRUE;
}//end SettingPathCreate

/* ----------------------------------------------------------------------------
* Name:         SettingPathDestroy
* Function:     设置路径销毁
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN SettingPathDestroy(OTA_SETTING_PATH_T *setting_path_ptr)
{
    OTA_SETTING_PATH_T *temp_setting_path_ptr = PNULL;
    OTA_TAG_PATH_T *tag_path_ptr = PNULL;
    OTA_TAG_NODE_T *tag_node_ptr = PNULL;
    if (setting_path_ptr == PNULL)
    {
        return TRUE;
    }
    
    while (setting_path_ptr != PNULL)
    {
        temp_setting_path_ptr = setting_path_ptr;
        if (temp_setting_path_ptr->tag_path != PNULL)
        {
            tag_path_ptr = temp_setting_path_ptr->tag_path;
            if (tag_path_ptr->path_node != PNULL)
            {
                while (tag_path_ptr->path_node != PNULL)
                {
                    tag_node_ptr = tag_path_ptr->path_node;
                    if (tag_node_ptr != PNULL)
                    {
                        tag_path_ptr->path_node = tag_path_ptr->path_node->next;
                        FreeOTAMem(tag_node_ptr);
                        tag_node_ptr = PNULL;                        
                    }                    
                }                
            }      

            FreeOTAMem(tag_path_ptr);
            tag_path_ptr = PNULL;
        }
        setting_path_ptr = setting_path_ptr->next;

        FreeOTAMem(temp_setting_path_ptr);
        temp_setting_path_ptr = PNULL;        
    }
    return TRUE;
}//end SettingPathDestroy

/* ----------------------------------------------------------------------------
* Name:         MMISettingDestroy
* Function:     销毁MMI setting
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN MMISettingDestroy(MMIOTA_SETTING_T *mmiota_setting_ptr)
{
    MMIOTA_SETTING_T *temp_setting_ptr = mmiota_setting_ptr;
    while(PNULL != mmiota_setting_ptr)
    {
        temp_setting_ptr = mmiota_setting_ptr;
        mmiota_setting_ptr = mmiota_setting_ptr->next;
        FreeOTAMem(temp_setting_ptr);
        temp_setting_ptr = PNULL;
    } 
    return TRUE;
}//end MMISettingDestroy

/* ----------------------------------------------------------------------------
* Name:         GetSettingVlue
* Function:     取得标签值
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8* GetSettingVlue(OTA_SETTING_PATH_T *setting_path_ptr, MMIOTA_SETTING_PARAMETER_E  parameter_type)
{
    uint8               *value_ptr = PNULL;
    uint8               *tag_name_ptr = PNULL;
    OTA_SETTING_PATH_T  *tem_path_ptr = setting_path_ptr;
    OTA_TAG_PATH_T      *tag_path_ptr = PNULL;
    OTA_TAG_ITEM_T      *tag_item_ptr = PNULL;   //xml parsered result
    OTA_TAG_NODE_T      *tag_node_ptr = PNULL;
    char                napdef[] = {"NAPDEF"};
    char                application[] = {"APPLICATION"};
    char                phisical[] = {"PXLOGICAL"};
    uint16              oma_phisical_add_index = s_oma_phisical_add_index;
    int16               oma_napdef_index = s_oma_napdef_index;
    int16               oma_application_index = s_oma_application_index;
    uint16              temp_len = 0;
    if ( g_parsered_xml_root != PNULL && 
        (MMIOTA_WAP_SI_NAME == parameter_type 
        || MMIOTA_WAP_SL_NAME == parameter_type
        || MMIOTA_WAP_CO_NAME == parameter_type
        )
        )
    {
        value_ptr = (uint8*) CallocOTAMem(strlen((char*)g_parsered_xml_root->tag_name)+1);
        if (value_ptr != PNULL)
        {
            temp_len = SCI_STRLEN((char*)g_parsered_xml_root->tag_name);
            SCI_MEMCPY(value_ptr,(char*)g_parsered_xml_root->tag_name,temp_len);
            value_ptr[temp_len] = '\0';                        
        }
        return value_ptr;
    }
    if (g_parsered_xml_root != PNULL && g_parsered_xml_root->child != PNULL)
    {
        tag_item_ptr = g_parsered_xml_root->child;
    }
    else if (g_parsered_xml_root != PNULL)
    {
        tag_item_ptr = g_parsered_xml_root;
    }
    else
    {
        return value_ptr;
    }

    //while find the tag path by parameter 
    while (tem_path_ptr != PNULL)
    {
        if (tem_path_ptr->tag_path->parameter_name == parameter_type)
        {
            tag_path_ptr = tem_path_ptr->tag_path;
            break;
        }
        tem_path_ptr = tem_path_ptr->next;
    }
    //find  value by tag path in xml parsered result g_parsered_xml_root
    if (tag_path_ptr != PNULL)
    {
        tag_node_ptr = tag_path_ptr->path_node; //get the first tag path node
        while (tag_node_ptr != PNULL)
        {
            tag_name_ptr = tag_node_ptr->tag_name;  //get the tag name,such as 'NAME','PORT' etc.
            while (tag_item_ptr != PNULL)       //find the ota item in g_parsered_xml_root
            {
                //find the ota item and the item is parm(have value)
                if (tag_item_ptr->tag_property != PNULL)
                {
                    if (parameter_type >= 12)//for si
                    {
                        OTA_ATTR_T *temp_pro_ptr = PNULL;
                        temp_pro_ptr = tag_item_ptr->tag_property;
                        if (temp_pro_ptr != PNULL)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
                        {
                            while (temp_pro_ptr != PNULL)
                            {
                                if (0 == strcmp((char*)temp_pro_ptr->att_name,(char*)tag_name_ptr) && temp_pro_ptr->att_value != PNULL)
                                {
                                    value_ptr = (uint8*) CallocOTAMem(strlen((char*)temp_pro_ptr->att_value)+1);
                                    if (value_ptr != PNULL)
                                    {
                                        temp_len = SCI_STRLEN((char*)temp_pro_ptr->att_value);
                                        SCI_MEMCPY(value_ptr,(char*)temp_pro_ptr->att_value,temp_len);
                                        value_ptr[temp_len] = '\0';                        
                                    }
                                    return value_ptr;
                                }
                                temp_pro_ptr = temp_pro_ptr->next;
                            }
                        }
                        tag_item_ptr = tag_item_ptr->next;
                    }
                    else
                    {
                        if (0 == strcmp((char*)tag_item_ptr->tag_property->att_value,(char*)tag_name_ptr) && tag_item_ptr->tag_property->next != PNULL)
                        {
                            value_ptr = (uint8*) CallocOTAMem(strlen((char*)tag_item_ptr->tag_property->next->att_value)+1);
                            if (value_ptr != PNULL)
                            {
                                temp_len = SCI_STRLEN((char*)tag_item_ptr->tag_property->next->att_value);
                                SCI_MEMCPY(value_ptr,(char*)tag_item_ptr->tag_property->next->att_value,temp_len);
                                value_ptr[temp_len] = '\0';                        
                            }
                            return value_ptr;                    
                        }//end if                
                        //find the ota item but item is characteristic(have not value),then find in child
                        else if ( 0 == strcmp((char*)tag_item_ptr->tag_property->att_value,(char*)tag_name_ptr))
                        {
                            if ((0 == strcmp(napdef,(char*)tag_name_ptr)) && oma_napdef_index > 0)
                            {
                                tag_item_ptr = tag_item_ptr->next;
                                oma_napdef_index--;
                            }
                            else if((0 == strcmp(application,(char*)tag_name_ptr)) && oma_application_index > 0)
                            {
                                tag_item_ptr = tag_item_ptr->next;
                                oma_application_index--;
                            }
                            else if ((0 == strcmp(phisical,(char*)tag_name_ptr)) && oma_phisical_add_index > 0)
                            {
                                tag_item_ptr = tag_item_ptr->next;
                                oma_phisical_add_index--;
                            }
                            else
                            {
                                tag_item_ptr = tag_item_ptr->child;
                                break;
                            }                        
                        } 
                        else
                        {
                            tag_item_ptr = tag_item_ptr->next;
                        }
                    }                    
                }
                else
                {
                    tag_item_ptr = tag_item_ptr->next;
                }                
            }//end while
            //find next tag node path until the parm
            if (tag_item_ptr != PNULL)
            {
                tag_node_ptr = tag_node_ptr->next;
            }
            //stop find
            else
            {
                break;
            }
        }//end tag node path
    }//end if
    
    return value_ptr;
}//end GetSettingVlue

/* ----------------------------------------------------------------------------
* Name:         GetOTASettingValue 
* Function:     解析nokia格式的ota类型（彩信，wap，或者bookmark）
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL MMIOTA_SETTING_TYPE_E GetNokiaOtaType(OTA_MIME_TYPE_E mime_type)
{
    MMIOTA_SETTING_TYPE_E result = MMIOTA_SETTING_MAX;
    char *adress_ptr = "ADDRESS";
    BOOLEAN if_have_address = FALSE;
    char *url_ptr    = "URL";
    BOOLEAN if_have_url = FALSE;
    char *mmsurl_ptr = "MMSURL";
    BOOLEAN if_have_mmsurl = FALSE;
    char *bookmark_ptr = "BOOKMARK";
    BOOLEAN if_have_bookmark = FALSE;
    OTA_TAG_ITEM_T      *tag_item_ptr = PNULL;   //xml parsered result
    if (g_parsered_xml_root != PNULL && g_parsered_xml_root->child != PNULL)
    {
        tag_item_ptr = g_parsered_xml_root->child;
    }
    else
    {
        return result;
    }
    while (PNULL != tag_item_ptr)
    {
        if (0 == strcmp((char *)tag_item_ptr->tag_property->att_value, adress_ptr))
        {
            if_have_address = TRUE;
        }
        if (0 == strcmp((char *)tag_item_ptr->tag_property->att_value, mmsurl_ptr))
        {
            if_have_mmsurl = TRUE;
        }
        if (0 == strcmp((char *)tag_item_ptr->tag_property->att_value, url_ptr))
        {
            if_have_url = TRUE;
        }
        if (0 == strcmp((char *)tag_item_ptr->tag_property->att_value, bookmark_ptr))
        {
            if_have_bookmark = TRUE;
        }
        tag_item_ptr = tag_item_ptr->next;
    }
    if (TRUE == if_have_address)
    {
        if (TRUE == if_have_mmsurl)
        {
            result = MMIOTA_MMS_SETTING;
        }
        else
        {
            result = MMIOTA_BRW_SETTING;
        }
    }
    else if (TRUE == if_have_bookmark)
    {
        result = MMIOTA_BRW_BOOKMARK;
    }
    return result;
}

/* ----------------------------------------------------------------------------
* Name:         GetOTASettingValue 
* Function:     取得设置参数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL MMIOTA_SETTING_TYPE_E GetOTAType(OTA_MIME_TYPE_E mime_type)
{
    MMIOTA_SETTING_TYPE_E ota_type = MMIOTA_SETTING_MAX;
    OTA_TAG_PATH_T  tag_path = {0};
    uint8  *setting_type_ptr = PNULL;
    OTA_SETTING_PATH_T setting_path = {0};

    if (mime_type == MIME_MAX)
    {
        return ota_type;
    }
    //get setting type
    switch(mime_type) 
    {
    case MIME_BROWERSETTING :
        ota_type = GetNokiaOtaType(MIME_BROWERSETTING);
        break;
    case MIME_BROWERBOOKMARK :
        ota_type = MMIOTA_BRW_BOOKMARK;
        break;
    case MIME_OTACONNECTIVITY :
        tag_path.parameter_name = MMIOTA_SETTING_TYPE;
        TagNodeCreate(&tag_path, TRUE);
        setting_path.tag_path = &tag_path;
        setting_type_ptr = GetSettingVlue(&setting_path, MMIOTA_SETTING_TYPE);
        if (setting_type_ptr != PNULL)
        {
            if (0 == strcmp((char *)setting_type_ptr, "w4"))
            {
                ota_type = MMIOTA_MMS_SETTING;
            }
            else if(0 == strcmp((char *)setting_type_ptr, "w2"))
            {
                ota_type = MMIOTA_BRW_SETTING;
            }
            else
            {
                ota_type = MMIOTA_SETTING_MAX;
            }
        }
        break;
    case MIME_WAP_PUSH_SI:
        ota_type = MMIOTA_BRW_WAP_SI;
        break;
    case MIME_WAP_PUSH_CO:
        ota_type = MMIOTA_BRW_WAP_CO;
        break;
    case MIME_WAP_PUSH_SL:
        ota_type = MMIOTA_BRW_WAP_SL;
        break;
    default:
        ota_type = MMIOTA_SETTING_MAX;
        break;
    }
    if (setting_type_ptr != PNULL)
    {
        FreeOTAMem(setting_type_ptr);
        setting_type_ptr = PNULL;  
    }    
    
    return ota_type;
}

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_FreeCharMap 
* Function:     释放setting char空间
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void MMIOTA_FreeCharMap(OTA_CHARACTERISTIC_ITEM_T *char_ptr)
{
    OTA_CHARACTERISTIC_ITEM_T * char_item_ptr = char_ptr;
    OTA_CHARACTERISTIC_ITEM_T * temp_item_ptr = PNULL;
    if (PNULL == char_ptr)
    {
        return;
    }
    while (PNULL != char_item_ptr)
    {
        temp_item_ptr = char_item_ptr;
        char_item_ptr = char_item_ptr->next_ptr;
        SCI_FREE(temp_item_ptr);
        temp_item_ptr = PNULL;
    }
}

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_FreeSettingMap 
* Function:     释放setting map空间
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void MMIOTA_FreeSettingMap(OTA_SETTING_MAP_T *setting_map_ptr)
{
    if (PNULL == setting_map_ptr)
    {
        return;
    }    
    MMIOTA_FreeCharMap(setting_map_ptr->application_ptr);
    setting_map_ptr->application_ptr = PNULL;
    MMIOTA_FreeCharMap(setting_map_ptr->bootstarp_ptr);
    setting_map_ptr->bootstarp_ptr = PNULL;
    MMIOTA_FreeCharMap(setting_map_ptr->napdef_ptr);
    setting_map_ptr->napdef_ptr = PNULL;
    MMIOTA_FreeCharMap(setting_map_ptr->pxlogical_ptr);
    setting_map_ptr->pxlogical_ptr = PNULL;
    SCI_FREE(setting_map_ptr);
    setting_map_ptr = PNULL;
}

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_FreeSettingMap 
* Function:     释放setting application空间
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void MMIOTA_FreeApplicationMap(OTA_APPLICATION_MAP_T *app_map_ptr)
{
    OTA_APPLICATION_MAP_T * f_app_map_ptr    = app_map_ptr;
    OTA_APPLICATION_MAP_T * temp_app_map_ptr = PNULL;
    if (PNULL == f_app_map_ptr)
    {
        return;
    }
    while (PNULL != f_app_map_ptr)
    {
        temp_app_map_ptr = f_app_map_ptr;
        f_app_map_ptr = f_app_map_ptr->next_ptr;
        FreeOTAMem(temp_app_map_ptr);
        temp_app_map_ptr = PNULL;
    }
}

/* ----------------------------------------------------------------------------
* Name:         get value by name 
* Function:     根据名称获得相应的值
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8 * MMIOTA_GetValueFromChar(OTA_TAG_ITEM_T *tag_item_ptr, OTA_CONNECTIVITY_NAME_TPYE_E name_type, uint16 char_num)
{
    uint8 *value_ptr  = PNULL;
    char *name_ptr   = PNULL;
    uint16 index      = 0;
    uint16 temp_str_len = 0;
    
    if (PNULL == tag_item_ptr)
    {
        return value_ptr;
    }
    for (index = CON_BOOTSTRAP; index < CON_MAX; index++)
    {
        if (s_ota_connectivity_name[index].type == name_type)
        {
            name_ptr = s_ota_connectivity_name[index].name;
        }
    }
    if (PNULL == name_ptr)
    {
        return value_ptr;
    }
    while (PNULL != tag_item_ptr)
    {
        if (PNULL != tag_item_ptr->tag_property && 0 == strcmp(name_ptr, (char *)tag_item_ptr->tag_property->att_value))
        {
            if (char_num == 0)//如果搜索取的第一个就可以返回了
            {
                if (PNULL != tag_item_ptr->tag_property->next)
                {
                    value_ptr = (uint8*) CallocOTAMem(strlen((char*)tag_item_ptr->tag_property->next->att_value)+1);
                    if (value_ptr != PNULL)
                    {
                        temp_str_len = strlen((char*)tag_item_ptr->tag_property->next->att_value);
                        SCI_MEMCPY(value_ptr,(char*)tag_item_ptr->tag_property->next->att_value,temp_str_len);
                        value_ptr[temp_str_len] = '\0';                        
                    }
                    return value_ptr;
                } 
            }
            else
            {
                char_num--;
            }
        }
        if (PNULL != tag_item_ptr->child)
        {
            value_ptr = MMIOTA_GetValueFromChar(tag_item_ptr->child, name_type, char_num);
            if (PNULL != value_ptr)
            {
                if (char_num == 0)
                {
                    return value_ptr;
                }
                else
                {
                    char_num--;
                }                
            }
        }

        tag_item_ptr = tag_item_ptr->next;
    }
    return value_ptr;
}

/* ----------------------------------------------------------------------------
* Name:        MMIOTA_GetItmeNumber 
* Function:     根据名称to napdef和to proxy id的个数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint16 MMIOTA_GetItmeNumber(OTA_TAG_ITEM_T *tag_item_ptr, OTA_CONNECTIVITY_NAME_TPYE_E name_type)
{
    // uint8 *value_ptr  = PNULL;
    char *name_ptr    = PNULL;
    uint16 index      = 0;
    uint16 item_num   = 0;

    if (PNULL == tag_item_ptr)
    {
        return 0;
    }
    for (index = CON_BOOTSTRAP; index < CON_MAX; index++)
    {
        if (s_ota_connectivity_name[index].type == name_type)
        {
            name_ptr = s_ota_connectivity_name[index].name;
        }
    }
    if (PNULL == name_ptr)
    {
        return 0;
    }
    while (PNULL != tag_item_ptr)
    {
        if (PNULL != tag_item_ptr->tag_property && 0 == strcmp(name_ptr, (char *)tag_item_ptr->tag_property->att_value))
        {
            item_num++;
        }
        if (PNULL != tag_item_ptr->child)
        {
            item_num = item_num + MMIOTA_GetItmeNumber(tag_item_ptr->child, name_type);
        }

        tag_item_ptr = tag_item_ptr->next;
    }
    return item_num;
}

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_InitSettingMap 
* Function:     初始化app
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN MMIOTA_InitSettingMap(OTA_MIME_TYPE_E mime_type)
{
    OTA_CHARACTERISTIC_ITEM_T * temp_item_ptr = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * char_item_ptr = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * sear_item_ptr = PNULL;
    OTA_TAG_ITEM_T * root_item_ptr            = PNULL;
    OTA_APPLICATION_MAP_T *temp_app_item_ptr  = PNULL;
    OTA_APPLICATION_MAP_T *new_app_item_ptr   = PNULL;
    char  bootstarp[]                         = {"BOOTSTRAP"};
    char  napdef[]                            = {"NAPDEF"};
    char  application[]                       = {"APPLICATION"};
    char  phisical[]                          = {"PXLOGICAL"};
    char  to_napid_internet[]                 = {"INTERNET"};
    uint8 *app_toproxy_ptr                    = PNULL;
    uint8 *app_toproxy_find_ptr               = PNULL;
    uint8 *app_tonapid_ptr                    = PNULL;
    uint8 *app_tonapid_find_ptr               = PNULL;
    BOOLEAN is_find                           = FALSE;
    uint16 pisical_id_num                     = 0;      //该app有几个pisical id 对应的应该生成几个app
    uint16 napdef_id_num                      = 0;      //该pisical有几个napdef，对应的应该生成几个app
    uint16 pisical_id_find                    = 0;
    uint16 napdef_id_find                     = 0;
    BOOLEAN is_duplicate                      = FALSE;
    BOOLEAN is_type_same                      = FALSE;
    BOOLEAN is_phisicalid_same                = FALSE;
    BOOLEAN is_napdefid_same                  = FALSE;
    OTA_APPLICATION_MAP_T *temp_unique_app    = PNULL;
    OTA_APPLICATION_MAP_T *temp_be_unique_app = PNULL;
    OTA_APPLICATION_MAP_T *temp_ba_unique_app = PNULL;
    uint8 *ota_type_ptr                       = PNULL;
    uint8 *be_ota_type_ptr                    = PNULL;
    uint8 *ba_ota_type_ptr                    = PNULL;
    uint8 *be_phisicalid_ptr                  = PNULL;
    uint8 *ba_phisicalid_ptr                  = PNULL;
    uint8 *be_napdef_ptr                      = PNULL;
    uint8 *ba_napdef_ptr                      = PNULL;

    if (g_parsered_xml_root != PNULL && g_parsered_xml_root->child != PNULL)
    {
        root_item_ptr = g_parsered_xml_root->child;
    }
    else if(g_parsered_xml_root != PNULL)
    {
        root_item_ptr = g_parsered_xml_root;
    }
    //SCI_TRACE_LOW:"MMIOTA_InitSettingMap"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_2191_112_2_18_2_42_41_1,(uint8*)"");
    if (MIME_OTACONNECTIVITY != mime_type || PNULL == root_item_ptr)
    {
        return FALSE;
    }
    g_ota_connectivity_application_map_ptr = PNULL;
    g_ota_connectivity_map_ptr = PNULL;
    
    g_ota_connectivity_map_ptr = (OTA_SETTING_MAP_T *)SCI_ALLOCAZ(sizeof(OTA_SETTING_MAP_T));
    if (PNULL == g_ota_connectivity_map_ptr)
    {
        //SCI_TRACE_LOW:"MMIOTA_InitSettingMap all memery failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_2202_112_2_18_2_42_41_2,(uint8*)"");
        return FALSE;
    }
    //SCI_MEMSET((char *)g_ota_connectivity_map_ptr, 0, sizeof(OTA_SETTING_MAP_T));

    while (PNULL != root_item_ptr)
    {
        if (PNULL != root_item_ptr->tag_property && 0 == strcmp(bootstarp, (char *)root_item_ptr->tag_property->att_value))
        {
            char_item_ptr = (OTA_CHARACTERISTIC_ITEM_T *)SCI_ALLOCAZ(sizeof(OTA_CHARACTERISTIC_ITEM_T));
            if (PNULL == char_item_ptr)
            {
                MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
                g_ota_connectivity_map_ptr = PNULL;
                return FALSE;
            }
            //SCI_MEMSET((char *)char_item_ptr, 0, sizeof(OTA_CHARACTERISTIC_ITEM_T));
            char_item_ptr->item_ptr = root_item_ptr;
            
            if (PNULL == g_ota_connectivity_map_ptr->bootstarp_ptr)
            {                
                g_ota_connectivity_map_ptr->bootstarp_ptr = char_item_ptr;
            }
            else
            {
                temp_item_ptr = g_ota_connectivity_map_ptr->bootstarp_ptr;
                while (PNULL != temp_item_ptr->next_ptr)
                {
                    temp_item_ptr = temp_item_ptr->next_ptr;
                }
                temp_item_ptr->next_ptr = char_item_ptr;
            }
        }
        else if (PNULL != root_item_ptr->tag_property && 0 == strcmp(phisical, (char *)root_item_ptr->tag_property->att_value))
        {
            char_item_ptr = (OTA_CHARACTERISTIC_ITEM_T *)SCI_ALLOCAZ(sizeof(OTA_CHARACTERISTIC_ITEM_T));
            if (PNULL == char_item_ptr)
            {
                MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
                g_ota_connectivity_map_ptr = PNULL;
                return FALSE;
            }
            //SCI_MEMSET((char *)char_item_ptr, 0, sizeof(OTA_CHARACTERISTIC_ITEM_T));
            char_item_ptr->item_ptr = root_item_ptr;

            if (PNULL == g_ota_connectivity_map_ptr->pxlogical_ptr)
            {
                g_ota_connectivity_map_ptr->pxlogical_ptr = char_item_ptr;
            }
            else
            {
                temp_item_ptr = g_ota_connectivity_map_ptr->pxlogical_ptr;
                while (PNULL != temp_item_ptr->next_ptr)
                {
                    temp_item_ptr = temp_item_ptr->next_ptr;
                }
                temp_item_ptr->next_ptr = char_item_ptr;
            }
        }
        else if (PNULL != root_item_ptr->tag_property && 0 == strcmp(napdef, (char *)root_item_ptr->tag_property->att_value))
        {
            char_item_ptr = (OTA_CHARACTERISTIC_ITEM_T *)SCI_ALLOCAZ(sizeof(OTA_CHARACTERISTIC_ITEM_T));
            if (PNULL == char_item_ptr)
            {
                MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
                g_ota_connectivity_map_ptr = PNULL;
                return FALSE;
            }
            //SCI_MEMSET((char *)char_item_ptr, 0, sizeof(OTA_CHARACTERISTIC_ITEM_T));
            char_item_ptr->item_ptr = root_item_ptr;

            if (PNULL == g_ota_connectivity_map_ptr->napdef_ptr)
            {
                g_ota_connectivity_map_ptr->napdef_ptr = char_item_ptr;
            }
            else
            {
                temp_item_ptr = g_ota_connectivity_map_ptr->napdef_ptr;
                while (PNULL != temp_item_ptr->next_ptr)
                {
                    temp_item_ptr = temp_item_ptr->next_ptr;
                }
                temp_item_ptr->next_ptr = char_item_ptr;
            }
        }
        else if (PNULL != root_item_ptr->tag_property && 0 == strcmp(application, (char *)root_item_ptr->tag_property->att_value))
        {
            char_item_ptr = (OTA_CHARACTERISTIC_ITEM_T *)SCI_ALLOCAZ(sizeof(OTA_CHARACTERISTIC_ITEM_T));
            if (PNULL == char_item_ptr)
            {
                MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
                g_ota_connectivity_map_ptr = PNULL;
                return FALSE;
            }
            //SCI_MEMSET((char *)char_item_ptr, 0, sizeof(OTA_CHARACTERISTIC_ITEM_T));
            char_item_ptr->item_ptr = root_item_ptr;

            if (PNULL == g_ota_connectivity_map_ptr->application_ptr)
            {
                g_ota_connectivity_map_ptr->application_ptr = char_item_ptr;
            }
            else
            {
                temp_item_ptr = g_ota_connectivity_map_ptr->application_ptr;
                while (PNULL != temp_item_ptr->next_ptr)
                {
                    temp_item_ptr = temp_item_ptr->next_ptr;
                }
                temp_item_ptr->next_ptr = char_item_ptr;
            }
        }
        root_item_ptr = root_item_ptr->next;
    }

    if (PNULL != g_ota_connectivity_map_ptr->pxlogical_ptr || PNULL != g_ota_connectivity_map_ptr->napdef_ptr)
    {
        if (PNULL != g_ota_connectivity_application_map_ptr)//如果没有释放，要释放空间
        {
            MMIOTA_FreeApplicationMap(g_ota_connectivity_application_map_ptr);
            g_ota_connectivity_application_map_ptr = PNULL;
        }
        
        
        if (PNULL == g_ota_connectivity_map_ptr->application_ptr)//如果没有app，参考mtk，应该默认为是wap设置，把napdef和proxy取出来
        {
            new_app_item_ptr = (OTA_APPLICATION_MAP_T *)SCI_ALLOCAZ(sizeof(OTA_APPLICATION_MAP_T));
            if (PNULL == new_app_item_ptr)
            {
                //SCI_TRACE_LOW:"MMIOTA_InitSettingMap all memery failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_2330_112_2_18_2_42_41_3,(uint8*)"");
                MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
                g_ota_connectivity_map_ptr = PNULL;
                MMIOTA_FreeApplicationMap(g_ota_connectivity_application_map_ptr);
                g_ota_connectivity_application_map_ptr = PNULL;
                return FALSE;
            }
            //SCI_MEMSET((char *)new_app_item_ptr, 0, sizeof(OTA_APPLICATION_MAP_T));
            new_app_item_ptr->napdef_ptr = g_ota_connectivity_map_ptr->napdef_ptr;
            new_app_item_ptr->pxlogical_ptr = g_ota_connectivity_map_ptr->pxlogical_ptr;
            g_ota_connectivity_application_map_ptr = new_app_item_ptr;
        }
        else//如果有app，形成app链表
        {
            temp_item_ptr = g_ota_connectivity_map_ptr->application_ptr;             
            while (PNULL != temp_item_ptr)
            {
                new_app_item_ptr = (OTA_APPLICATION_MAP_T *)SCI_ALLOCAZ(sizeof(OTA_APPLICATION_MAP_T));
                if (PNULL == new_app_item_ptr)
                {
                    //SCI_TRACE_LOW:"MMIOTA_InitSettingMap all memery failed!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_2350_112_2_18_2_42_41_4,(uint8*)"");
                    MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
                    g_ota_connectivity_map_ptr = PNULL;
                    MMIOTA_FreeApplicationMap(g_ota_connectivity_application_map_ptr);
                    g_ota_connectivity_application_map_ptr = PNULL;
                    return FALSE;
                }
                //SCI_MEMSET((char *)new_app_item_ptr, 0, sizeof(OTA_APPLICATION_MAP_T));
                new_app_item_ptr->application_ptr = temp_item_ptr;//确认一个app
                //ota type:mms,wap have toproxyid, csd has tonapid
                ota_type_ptr = MMIOTA_GetValueFromChar(new_app_item_ptr->application_ptr->item_ptr->child, CON_APPLICATION_APPID, 0);
                if (PNULL != ota_type_ptr)//mms or wap
                {
                    pisical_id_num = MMIOTA_GetItmeNumber(temp_item_ptr->item_ptr->child, CON_APPLICATION_TOPROXY);
                    app_toproxy_ptr = MMIOTA_GetValueFromChar(temp_item_ptr->item_ptr->child, CON_APPLICATION_TOPROXY, pisical_id_find);                
                    if (PNULL != app_toproxy_ptr)//如果该app有,to_proxy，找到该id所在的pxlogical，根据pxlogical找到napdef
                    {                    
                        sear_item_ptr = g_ota_connectivity_map_ptr->pxlogical_ptr;//从第一个logical proxy开始查找
                        is_find = FALSE;
                        while (PNULL != sear_item_ptr)
                        {
                            app_toproxy_find_ptr = MMIOTA_GetValueFromChar(sear_item_ptr->item_ptr->child, CON_PXLOGICAL_PROXYID, 0);
                            if (PNULL != app_toproxy_find_ptr && 0 == strcmp((char *)app_toproxy_find_ptr, (char *)app_toproxy_ptr))
                            {
                                is_find = TRUE;
                                break;
                            }
                            if (PNULL != app_toproxy_find_ptr)
                            {
                                SCI_FREE(app_toproxy_find_ptr);
                                app_toproxy_find_ptr = PNULL;
                            }
                            sear_item_ptr =sear_item_ptr->next_ptr;
                        }
                        if (PNULL != app_toproxy_find_ptr)
                        {
                            SCI_FREE(app_toproxy_find_ptr);
                            app_toproxy_find_ptr = PNULL;
                        }
                        if (is_find && PNULL != sear_item_ptr && PNULL != sear_item_ptr->item_ptr)
                        {
                            new_app_item_ptr->pxlogical_ptr = sear_item_ptr;
                            napdef_id_num = MMIOTA_GetItmeNumber(sear_item_ptr->item_ptr->child, CON_PXLOGICAL_PXPHYSICAL_TONAPID);
                            app_tonapid_ptr = MMIOTA_GetValueFromChar(sear_item_ptr->item_ptr->child, CON_PXLOGICAL_PXPHYSICAL_TONAPID, napdef_id_find);
                            is_find = FALSE;
                            if (PNULL != app_tonapid_ptr)
                            {
                                sear_item_ptr = g_ota_connectivity_map_ptr->napdef_ptr;
                                if (PNULL != sear_item_ptr && 0 == strcmp((char *)app_tonapid_ptr, to_napid_internet))
                                {
                                    new_app_item_ptr->napdef_ptr = sear_item_ptr;
                                }
                                else
                                {
                                    while (PNULL != sear_item_ptr)
                                    {
                                        app_tonapid_find_ptr = MMIOTA_GetValueFromChar(sear_item_ptr->item_ptr->child, CON_NAPDEF_NAPID, 0);
                                        if (PNULL != app_tonapid_find_ptr && 0 == strcmp((char *)app_tonapid_ptr, (char *)app_tonapid_find_ptr))
                                        {
                                            is_find = TRUE;
                                            break;
                                        }
                                        if (PNULL != app_tonapid_find_ptr)
                                        {
                                            SCI_FREE(app_tonapid_find_ptr);
                                            app_tonapid_find_ptr = PNULL;
                                        }
                                        sear_item_ptr = sear_item_ptr->next_ptr;
                                    }
                                    if (PNULL != app_tonapid_find_ptr)
                                    {
                                        SCI_FREE(app_tonapid_find_ptr);
                                        app_tonapid_find_ptr = PNULL;
                                    }
                                    if (is_find)
                                    {
                                        new_app_item_ptr->napdef_ptr = sear_item_ptr;
                                    }    
                                    else
                                    {
                                        SCI_FREE(new_app_item_ptr);
                                        new_app_item_ptr = PNULL;
                                    }
                                }                            
                            }
                            else 
                            {
                                SCI_FREE(new_app_item_ptr);
                                new_app_item_ptr = PNULL;
                            }
                        }
                        else
                        {
                            SCI_FREE(new_app_item_ptr);
                            new_app_item_ptr = PNULL;
                        }                   
                        
                }
                else   //如果没有to_proxy，则根据TO-NAPID找到napdef
                {
                    app_tonapid_ptr = MMIOTA_GetValueFromChar(temp_item_ptr->item_ptr->child, CON_APPLICATION_TONAPID, 0);
                    if (PNULL != app_tonapid_ptr)
                    {
                        sear_item_ptr = g_ota_connectivity_map_ptr->napdef_ptr;
                        is_find = FALSE;
                        while (PNULL != sear_item_ptr)
                        {
                            app_tonapid_find_ptr = MMIOTA_GetValueFromChar(sear_item_ptr->item_ptr->child, CON_NAPDEF_NAPID, 0);
                            if (PNULL != app_tonapid_find_ptr && 0 == strcmp((char *)app_tonapid_ptr, (char *)app_tonapid_find_ptr))
                            {
                                is_find = TRUE;
                                break;
                            }
                            sear_item_ptr = sear_item_ptr->next_ptr;
                        }
                        if (is_find)
                        {
                            new_app_item_ptr->napdef_ptr = sear_item_ptr;
                        }
                        else
                        {
                            SCI_FREE(new_app_item_ptr);
                            new_app_item_ptr = PNULL;
                        }
                        if (PNULL != app_tonapid_find_ptr)
                        {
                            SCI_FREE(app_tonapid_find_ptr);
                            app_tonapid_find_ptr = PNULL;
                        }
                    }
                    else
                    {
                        SCI_FREE(new_app_item_ptr);
                        new_app_item_ptr = PNULL;
                    }
                }
                if (PNULL != app_toproxy_ptr)
                {
                    SCI_FREE(app_toproxy_ptr);
                    app_toproxy_ptr = PNULL;
                }
                if (PNULL != app_tonapid_ptr)
                {
                    SCI_FREE(app_tonapid_ptr);
                    app_tonapid_ptr = PNULL;
                }
                if (PNULL == new_app_item_ptr)//如果没有找到匹配则不生成app，处理下一个
                {
                    if (napdef_id_find < napdef_id_num - 1)
                    {
                        napdef_id_find ++;
                    }
                    else if (pisical_id_find < pisical_id_num - 1)
                    {
                        pisical_id_find++;
                    }
                    else
                    {
                        temp_item_ptr = temp_item_ptr->next_ptr;
                    }
                }
                else
                {
                    if (PNULL == g_ota_connectivity_application_map_ptr)
                    {
                        g_ota_connectivity_application_map_ptr = new_app_item_ptr;
                    }
                    else
                    {
                        temp_app_item_ptr = g_ota_connectivity_application_map_ptr;
                        while (PNULL != temp_app_item_ptr->next_ptr)
                        {
                            temp_app_item_ptr = temp_app_item_ptr->next_ptr;
                        }
                        temp_app_item_ptr->next_ptr = new_app_item_ptr;
                    }                
                    
                    if (napdef_id_find < napdef_id_num - 1)
                    {
                        napdef_id_find ++;
                    }
                    else if (pisical_id_find < pisical_id_num - 1)
                    {
                        pisical_id_find++;
                    }
                    else
                    {
                        temp_item_ptr = temp_item_ptr->next_ptr;
                    }  
                } 
                SCI_FREE(ota_type_ptr);
                ota_type_ptr = PNULL;
              }
              else //other ota type
              {
                  SCI_FREE(new_app_item_ptr);
                  new_app_item_ptr = PNULL;
                  temp_item_ptr = temp_item_ptr->next_ptr;
              }
            }
        }
    }    
    //处理相同的app，如果一个app的type，phsicalid，napdefid都相同，认为是相同的app，要删除掉
    temp_unique_app = g_ota_connectivity_application_map_ptr;
    if (PNULL != temp_unique_app && PNULL != temp_unique_app->next_ptr)//两个以上的app才有比较的必要
    {
        while (PNULL != temp_unique_app)//每一个的app都要做比较，这个是当前要比较的app
        {
            //获取当前的比较值
            if (PNULL != temp_unique_app->application_ptr)
            {
                be_ota_type_ptr = MMIOTA_GetValueFromChar(temp_unique_app->application_ptr->item_ptr->child, CON_APPLICATION_APPID, 0);
            }
            if (PNULL != temp_unique_app->pxlogical_ptr)
            {
                be_phisicalid_ptr = MMIOTA_GetValueFromChar(temp_unique_app->pxlogical_ptr->item_ptr->child, CON_PXLOGICAL_PROXYID, 0);
            }
            if (PNULL != temp_unique_app->napdef_ptr)
            {
                be_napdef_ptr = MMIOTA_GetValueFromChar(temp_unique_app->napdef_ptr->item_ptr->child, CON_NAPDEF_NAPID, 0);
            }
            temp_ba_unique_app = temp_unique_app->next_ptr;
            while(PNULL != temp_ba_unique_app)
            {
                is_type_same = FALSE;
                is_phisicalid_same = FALSE;
                is_napdefid_same = FALSE;
                is_duplicate = FALSE;
                if (PNULL != temp_ba_unique_app->application_ptr)
                {
                    ba_ota_type_ptr = MMIOTA_GetValueFromChar(temp_ba_unique_app->application_ptr->item_ptr->child, CON_APPLICATION_APPID, 0);
                    if (PNULL != be_ota_type_ptr && PNULL != ba_ota_type_ptr && 0 == strcmp((char *)be_ota_type_ptr, (char *)ba_ota_type_ptr))
                    {
                        is_type_same = TRUE;
                    }
                }
                if (PNULL != temp_ba_unique_app->pxlogical_ptr)
                {
                    ba_phisicalid_ptr = MMIOTA_GetValueFromChar(temp_ba_unique_app->pxlogical_ptr->item_ptr->child, CON_PXLOGICAL_PROXYID, 0);
                    if (PNULL == be_phisicalid_ptr && PNULL == ba_phisicalid_ptr)
                    {
                        is_phisicalid_same = TRUE;
                    }
                    else if (PNULL != be_phisicalid_ptr && PNULL != ba_phisicalid_ptr && 0 == strcmp((char *)be_phisicalid_ptr, (char *)ba_phisicalid_ptr))
                    {
                        is_phisicalid_same = TRUE;
                    }
                }
                if (PNULL != temp_ba_unique_app->napdef_ptr)
                {
                    ba_napdef_ptr = MMIOTA_GetValueFromChar(temp_ba_unique_app->napdef_ptr->item_ptr->child, CON_NAPDEF_NAPID, 0);
                    if (PNULL == be_napdef_ptr && PNULL == ba_napdef_ptr)
                    {
                        is_napdefid_same = TRUE;
                    }
                    else if (PNULL != be_napdef_ptr && PNULL != ba_napdef_ptr && 0 == strcmp((char *)be_napdef_ptr, (char *)ba_napdef_ptr))
                    {
                        is_napdefid_same = TRUE;
                    }
                }
                if (is_type_same && is_phisicalid_same && is_napdefid_same)
                {
                    is_duplicate = TRUE;
                }
                if (is_duplicate)//相同的app
                {
                    temp_be_unique_app = temp_ba_unique_app;
                    temp_unique_app->next_ptr = temp_ba_unique_app->next_ptr;
                    temp_ba_unique_app = temp_unique_app->next_ptr;
                    SCI_FREE(temp_be_unique_app);
                    temp_be_unique_app = PNULL;
                }
                else
                {
                    temp_ba_unique_app = temp_ba_unique_app->next_ptr;
                }                
            }
            temp_unique_app = temp_unique_app->next_ptr;
        }
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_GetConSettingValue 
* Function:     获取oma ota配置值
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN MMIOTA_GetConSettingValue(OTA_APPLICATION_MAP_T *app_map_ptr, MMIOTA_SETTING_T  *mmiota_setting_ptr)
{
    uint8 *value_ptr = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * app_item_ptr     = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * pisical_item_ptr = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * napdef_item_ptr  = PNULL;

    if (PNULL == app_map_ptr || PNULL == mmiota_setting_ptr)
    {
        return FALSE;
    }
    app_item_ptr = app_map_ptr->application_ptr;
    pisical_item_ptr = app_map_ptr->pxlogical_ptr;
    napdef_item_ptr = app_map_ptr->napdef_ptr;

    if (PNULL == pisical_item_ptr && PNULL == napdef_item_ptr)//如果没有配置信箱，只有app，没有配置的价值了
    {
        return FALSE;
    }

    //type 如果没有app 就认为是browser配置,如果有根据appid获取，只处理mms和wap，将来会扩展
    if (PNULL == app_item_ptr)
    {
        mmiota_setting_ptr->setting_type = MMIOTA_BRW_SETTING;
    }
    else
    {
        value_ptr = MMIOTA_GetValueFromChar(app_map_ptr->application_ptr->item_ptr->child, CON_APPLICATION_APPID, 0);
        SCI_TRACE_BUF("zhangqiang test CON_APPLICATION_APPID value_ptr",value_ptr,5);
        if (PNULL != value_ptr)
        {
            if (0 == strcmp((char *)value_ptr, "w4"))
            {
                mmiota_setting_ptr->setting_type = MMIOTA_MMS_SETTING;
            }
            else if(0 == strcmp((char *)value_ptr, "w2"))
            {
                mmiota_setting_ptr->setting_type = MMIOTA_BRW_SETTING;
            }
            else
            {
                mmiota_setting_ptr->setting_type = MMIOTA_SETTING_MAX;
            }
            FreeOTAMem(value_ptr);
            value_ptr = PNULL;
        }
        else
        {
            mmiota_setting_ptr->setting_type = MMIOTA_SETTING_MAX;
        }
    }

    //判断wap配置是不是GPRS，我们只支持GPRS
    if(MMIOTA_BRW_SETTING == mmiota_setting_ptr->setting_type && PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_BEARER, 0);
        if(0 != strcmp((char *)value_ptr, "GSM-GPRS"))
        {
            mmiota_setting_ptr->setting_type = MMIOTA_SETTING_MAX;
			//131782
            MMIPUB_OpenAlertFailWin(TXT_SMS_PUSH_IVALID_CFG);
            return FALSE;
        }
    }

    //name 如果没有app，在bootstarp里面的name获取配置名称，有则在app获取，如果app里面没有name，logical里面获取名称
    if (PNULL == app_item_ptr)
    {
        if (PNULL != g_ota_connectivity_map_ptr->bootstarp_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(g_ota_connectivity_map_ptr->bootstarp_ptr->item_ptr->child, CON_BOOTSTRAP_NAME, 0);
        }        
    }
    else
    {
        value_ptr = MMIOTA_GetValueFromChar(app_item_ptr->item_ptr->child, CON_APPLICATION_NAME, 0);
        if (PNULL == value_ptr && PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_NAME, 0);
        }
        if (PNULL == value_ptr && PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_PXLOGICAL_NAME, 0);
        }
    }
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->name_len = SCI_STRLEN((char *)value_ptr);
        mmiota_setting_ptr->name_len = MIN(mmiota_setting_ptr->name_len, MMIOTA_SETTING_MAX_NAME_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->name, value_ptr, mmiota_setting_ptr->name_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //gateway获取网关
    if(PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_PXPHYSICAL_PXADDR, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->gateway_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->gateway_len = MIN(mmiota_setting_ptr->gateway_len, MMIOTA_SETTING_MAX_GATEWAY_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->gateway, value_ptr, mmiota_setting_ptr->gateway_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //port获取port
    if(PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_PXPHYSICAL_PORT_PORTNBR, 0);
    }    
    if (PNULL != value_ptr)
    {   
        uint32 temp_port = 80;
        temp_port = atoi((char *)value_ptr);
        if(65535 < temp_port)
        {
            mmiota_setting_ptr->port = 80;
        }
        else
        {
            mmiota_setting_ptr->port = temp_port;
        }
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }
    else
    {
        mmiota_setting_ptr->port = 80;
    }

    //apn获取apn
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPADDRESS, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->apn_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->apn_len= MIN(mmiota_setting_ptr->apn_len, MMIOTA_SETTING_MAX_APN_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->apn, value_ptr, mmiota_setting_ptr->apn_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //dns获取dns
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_DNSADDRESS, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->dnsaddress_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->dnsaddress_len= MIN(mmiota_setting_ptr->dnsaddress_len, MMIOTA_SETTING_MAX_DNS_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->dnsaddress, value_ptr, mmiota_setting_ptr->dnsaddress_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //是否安全连接security link
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPAUTHINFO_AUTHTYPE, 0);
    }    
    if (PNULL != value_ptr && 0 == strcmp((char *)value_ptr, "CHAP"))
    {
        mmiota_setting_ptr->auth_type = MN_CHAP;
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //username用户名
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPAUTHINFO_AUTHNAME, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->username_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->username_len= MIN(mmiota_setting_ptr->username_len, MMIOTA_SETTING_MAX_USERNAME_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->username, value_ptr, mmiota_setting_ptr->username_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //password密码
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPAUTHINFO_AUTHSECRET, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->password_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->password_len= MIN(mmiota_setting_ptr->password_len, MMIOTA_SETTING_MAX_PASSWORD_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->password, value_ptr, mmiota_setting_ptr->password_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }
    
    //homepage主页
    if (MMIOTA_BRW_SETTING == mmiota_setting_ptr->setting_type)
    {
        if (PNULL != app_item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(app_item_ptr->item_ptr->child, CON_APPLICATION_RESOURCE_URI, 0);
        }
    }
    else if(MMIOTA_MMS_SETTING == mmiota_setting_ptr->setting_type)
    {
        if (PNULL != app_item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(app_item_ptr->item_ptr->child, CON_APPLICATION_ADDR, 0);
        }
    }
    if (PNULL == value_ptr && PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_STARTPAGE, 0);
    }
    if (PNULL != value_ptr)
    {
        //mmiota_setting_ptr->homepage_len = SCI_STRLEN((char*)value_ptr);
        //mmiota_setting_ptr->homepage_len = MIN(mmiota_setting_ptr->homepage_len, MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN);
        //SCI_MEMCPY(mmiota_setting_ptr->homepage, value_ptr, mmiota_setting_ptr->homepage_len);
        MMIAPICOM_NormalizeUrl(value_ptr, mmiota_setting_ptr->homepage, MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN, FALSE);
        mmiota_setting_ptr->homepage_len = SCI_STRLEN((char *)mmiota_setting_ptr->homepage);
        
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
* Name:         MMIOTA_GetConSettingValue 
* Function:     获取oma ota配置值
* Author:       
* Note:         为不影响老代码相互修改merge，故增加该函数达到多app 名称的情况for CR131778
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN MMIOTA_GetConSetting_MultAppAcc(OTA_APPLICATION_MAP_T *app_map_ptr, MMIOTA_SETTING_T  *mmiota_setting_ptr)
{
    uint8 *value_ptr = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * pisical_item_ptr = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * napdef_item_ptr  = PNULL;
    OTA_CHARACTERISTIC_ITEM_T * application_item_ptr = PNULL;
    if (PNULL == app_map_ptr || PNULL == mmiota_setting_ptr)
    {
        return FALSE;
    }
    pisical_item_ptr = app_map_ptr->pxlogical_ptr;
    napdef_item_ptr = app_map_ptr->napdef_ptr;
    application_item_ptr = app_map_ptr->application_ptr;
    if (PNULL == pisical_item_ptr && PNULL == napdef_item_ptr)//如果没有配置信箱，只有app，没有配置的价值了
    {
        return FALSE;
    }

    //type 如果没有app 就认为是browser配置,如果有根据appid获取，只处理mms和wap，将来会扩展
    if (PNULL == application_item_ptr)
    {
        mmiota_setting_ptr->setting_type = MMIOTA_BRW_SETTING;
    }
    else
    {
        value_ptr = MMIOTA_GetValueFromChar(application_item_ptr->item_ptr->child, CON_APPLICATION_APPID, 0);
        if (PNULL != value_ptr)
        {
            if (0 == strcmp((char *)value_ptr, "w4"))
            {
                mmiota_setting_ptr->setting_type = MMIOTA_MMS_SETTING;
            }
            else if(0 == strcmp((char *)value_ptr, "w2"))
            {
                mmiota_setting_ptr->setting_type = MMIOTA_BRW_SETTING;
            }
            else
            {
                mmiota_setting_ptr->setting_type = MMIOTA_SETTING_MAX;
            }
            FreeOTAMem(value_ptr);
            value_ptr = PNULL;
        }
        else
        {
            mmiota_setting_ptr->setting_type = MMIOTA_SETTING_MAX;
        }
    }

    //判断wap配置是不是GPRS，我们只支持GPRS
    if(MMIOTA_BRW_SETTING == mmiota_setting_ptr->setting_type && PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_BEARER, 0);
    if(PNULL !=value_ptr)
    {
        if(0 != strcmp((char *)value_ptr, "GSM-GPRS"))
        {
            mmiota_setting_ptr->setting_type = MMIOTA_SETTING_MAX;
            //131782
            MMIPUB_OpenAlertFailWin(TXT_SMS_PUSH_IVALID_CFG);
            return FALSE;
        }
    }
    }

    //name 如果没有app，在bootstarp里面的name获取配置名称，有则在app获取，如果app里面没有name，logical里面获取名称
    if (PNULL == application_item_ptr)
    {
        if (PNULL != g_ota_connectivity_map_ptr->bootstarp_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(g_ota_connectivity_map_ptr->bootstarp_ptr->item_ptr->child, CON_BOOTSTRAP_NAME, 0);
        }        
    }
    else
    {
        value_ptr = MMIOTA_GetValueFromChar(application_item_ptr->item_ptr->child, CON_APPLICATION_NAME, 0);
        if (PNULL == value_ptr && PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_NAME, 0);
        }
        if (PNULL == value_ptr && PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_PXLOGICAL_NAME, 0);
        }
    }
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->name_len = SCI_STRLEN((char *)value_ptr);
        mmiota_setting_ptr->name_len = MIN(mmiota_setting_ptr->name_len, MMIOTA_SETTING_MAX_NAME_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->name, value_ptr, mmiota_setting_ptr->name_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //gateway获取网关
    if(PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_PXPHYSICAL_PXADDR, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->gateway_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->gateway_len = MIN(mmiota_setting_ptr->gateway_len, MMIOTA_SETTING_MAX_GATEWAY_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->gateway, value_ptr, mmiota_setting_ptr->gateway_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //port获取port
    if(PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_PXPHYSICAL_PORT_PORTNBR, 0);
    }    
    if (PNULL != value_ptr)
    {
        uint32 temp_port = 80;
        temp_port = atoi((char *)value_ptr);
        if(65535 < temp_port)
        {
            mmiota_setting_ptr->port = 80;
        }
        else
        {
            mmiota_setting_ptr->port = temp_port;
        }
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }
    else
    {
        mmiota_setting_ptr->port = 80;
    }

    //apn获取apn
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPADDRESS, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->apn_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->apn_len= MIN(mmiota_setting_ptr->apn_len, MMIOTA_SETTING_MAX_APN_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->apn, value_ptr, mmiota_setting_ptr->apn_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //dns获取dns
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_DNSADDRESS, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->dnsaddress_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->dnsaddress_len= MIN(mmiota_setting_ptr->dnsaddress_len, MMIOTA_SETTING_MAX_DNS_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->dnsaddress, value_ptr, mmiota_setting_ptr->dnsaddress_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //是否安全连接security link
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPAUTHINFO_AUTHTYPE, 0);
    }    
    if (PNULL != value_ptr && 0 == strcmp((char *)value_ptr, "CHAP"))
    {
        mmiota_setting_ptr->auth_type = MN_CHAP;
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //username用户名
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPAUTHINFO_AUTHNAME, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->username_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->username_len= MIN(mmiota_setting_ptr->username_len, MMIOTA_SETTING_MAX_USERNAME_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->username, value_ptr, mmiota_setting_ptr->username_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    //password密码
    if (PNULL != napdef_item_ptr && PNULL != napdef_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(napdef_item_ptr->item_ptr->child, CON_NAPDEF_NAPAUTHINFO_AUTHSECRET, 0);
    }    
    if (PNULL != value_ptr)
    {
        mmiota_setting_ptr->password_len = SCI_STRLEN((char*)value_ptr);
        mmiota_setting_ptr->password_len= MIN(mmiota_setting_ptr->password_len, MMIOTA_SETTING_MAX_PASSWORD_LEN);
        SCI_MEMCPY(mmiota_setting_ptr->password, value_ptr, mmiota_setting_ptr->password_len);
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }
    
    //homepage主页
    if (MMIOTA_BRW_SETTING == mmiota_setting_ptr->setting_type)
    {
        if (PNULL != application_item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(application_item_ptr->item_ptr->child, CON_APPLICATION_RESOURCE_URI, 0);
        }
    }
    else if(MMIOTA_MMS_SETTING == mmiota_setting_ptr->setting_type)
    {
        if (PNULL != application_item_ptr)
        {
            value_ptr = MMIOTA_GetValueFromChar(application_item_ptr->item_ptr->child, CON_APPLICATION_ADDR, 0);
        }
    }
    if (PNULL == value_ptr && PNULL != pisical_item_ptr && PNULL != pisical_item_ptr->item_ptr)
    {
        value_ptr = MMIOTA_GetValueFromChar(pisical_item_ptr->item_ptr->child, CON_PXLOGICAL_STARTPAGE, 0);
    }
    if (PNULL != value_ptr)
    {
        //mmiota_setting_ptr->homepage_len = SCI_STRLEN((char*)value_ptr);
        //mmiota_setting_ptr->homepage_len = MIN(mmiota_setting_ptr->homepage_len, MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN);
        //SCI_MEMCPY(mmiota_setting_ptr->homepage, value_ptr, mmiota_setting_ptr->homepage_len);
        MMIAPICOM_NormalizeUrl(value_ptr, mmiota_setting_ptr->homepage, MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN, FALSE);
        mmiota_setting_ptr->homepage_len = SCI_STRLEN((char *)mmiota_setting_ptr->homepage);
        
        FreeOTAMem(value_ptr);
        value_ptr = PNULL;
    }

    return TRUE;
}


/* ----------------------------------------------------------------------------
* Name:         GetOTASettingValue 
* Function:     取得设置参数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN GetOTASettingValue(MMIOTA_SETTING_T  *mmiota_setting_ptr, OTA_MIME_TYPE_E mime_type)
{
    OTA_SETTING_PATH_T  *setting_path_ptr = PNULL;
    MMIOTA_SETTING_TYPE_E ota_type = MMIOTA_SETTING_MAX;
    uint8  *access_type_ptr = PNULL;
    uint8  *need_security_connect_ptr = PNULL;
    uint8  *port_ptr = PNULL;
    uint16 port = 0;
    uint8  *name_ptr = PNULL;    
    uint8  *gateway_ptr = PNULL;
    uint8  *username_ptr = PNULL;
    uint8  *password_ptr = PNULL;
    uint8  *apn_ptr = PNULL;
    uint8  *homepage_ptr = PNULL;
    uint8  *bookmark_name_ptr = PNULL;
    uint8  *bookmark_url_ptr = PNULL;
    uint8  *wap_push_name_ptr = PNULL;
    uint8  *wap_push_url_ptr = PNULL;
    uint8  *wap_push_action_ptr = PNULL;
    uint8 * value_ptr  = PNULL;

    char   *mms_char    = " MMS";
    char   *wap_char    = " BROWSER";
    char   temp_buf[50]       = {0}; 

    if (mmiota_setting_ptr == PNULL)
    {
        return FALSE;
    }
    
    //setting map
    ota_type = mmiota_setting_ptr->setting_type;
    switch(ota_type){
    case MMIOTA_MMS_SETTING:
        if (mime_type == MIME_OTACONNECTIVITY)
        {
            g_ota_map = (OTA_ELEMENT_PATH_T *)s_oma_ota_mms_map;
            g_ota_map_len = sizeof(s_oma_ota_mms_map)/sizeof(s_oma_ota_mms_map[0]);
        }
        else
        {
            g_ota_map = (OTA_ELEMENT_PATH_T *)s_nokia_mms_ota_map;
            g_ota_map_len = sizeof(s_nokia_mms_ota_map)/sizeof(s_nokia_mms_ota_map[0]);
        }
        break;
    case MMIOTA_BRW_SETTING:
        if (mime_type == MIME_OTACONNECTIVITY)
        {
            g_ota_map = (OTA_ELEMENT_PATH_T *)s_oma_ota_wap_map;
            g_ota_map_len = sizeof(s_oma_ota_wap_map)/sizeof(s_oma_ota_wap_map[0]);
        }
        else
        {
            g_ota_map = (OTA_ELEMENT_PATH_T *)s_nokia_wap_ota_map;
            g_ota_map_len = sizeof(s_nokia_wap_ota_map)/sizeof(s_nokia_wap_ota_map[0]);
        }
        break;
    case MMIOTA_BRW_BOOKMARK:
        g_ota_map = (OTA_ELEMENT_PATH_T *)s_nokia_wap_ota_map;
        g_ota_map_len = sizeof(s_nokia_wap_ota_map)/sizeof(s_nokia_wap_ota_map[0]);
        break;
    case MMIOTA_BRW_WAP_SI:
        g_ota_map = (OTA_ELEMENT_PATH_T *)s_wap_si_ota_map;
        g_ota_map_len = sizeof(s_wap_si_ota_map)/sizeof(s_wap_si_ota_map[0]);
        break;
    case MMIOTA_BRW_WAP_CO:
        g_ota_map = (OTA_ELEMENT_PATH_T *)s_wap_co_ota_map;
        g_ota_map_len = sizeof(s_wap_co_ota_map)/sizeof(s_wap_co_ota_map[0]);
        break;
    case MMIOTA_BRW_WAP_SL:
        g_ota_map = (OTA_ELEMENT_PATH_T *)s_wap_sl_ota_map;
        g_ota_map_len = sizeof(s_wap_sl_ota_map)/sizeof(s_wap_sl_ota_map[0]);
        break;
    default:
        return FALSE;
    };    

    setting_path_ptr = (OTA_SETTING_PATH_T*)CallocOTAMem(sizeof(OTA_SETTING_PATH_T));
    if (setting_path_ptr != PNULL)
    {
        SettingPathCreate(setting_path_ptr);
    }
    else
    {
        return FALSE;
    }   

    //when setting_type is not bookmark setting
    if (ota_type == MMIOTA_MMS_SETTING || ota_type == MMIOTA_BRW_SETTING)
    {
        //get access type
        access_type_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_ACCESS_TYPE);
        if (access_type_ptr != PNULL)
        {
            FreeOTAMem(access_type_ptr);
            access_type_ptr = PNULL;
        }
        
        //judge is security connect
        need_security_connect_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_NEED_SECURITY_CONNECT);
        if (need_security_connect_ptr != PNULL)
        {
            FreeOTAMem(need_security_connect_ptr);
            need_security_connect_ptr = PNULL;
        }
        
        //get port
        port_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_PROXY_PORT);
        if (PNULL == port_ptr && MIME_OTACONNECTIVITY == mime_type)
        {
            OTA_TAG_PATH_T  tag_path = {0};
            OTA_SETTING_PATH_T setting_path = {0};
            
            tag_path.parameter_name = MMIOTA_PROXY_PORT;
            TagNodeCreate(&tag_path, TRUE);
            setting_path.tag_path = &tag_path;
            port_ptr = GetSettingVlue(&setting_path, MMIOTA_PROXY_PORT);
        }
        if (port_ptr != PNULL)
        {
            port = atoi((char *)port_ptr);
            mmiota_setting_ptr->port = port;
            FreeOTAMem(port_ptr);
            port_ptr = PNULL;
        }
        //get setting name
        name_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_SEETTING_NAME);
        
        if (PNULL == name_ptr && MIME_OTACONNECTIVITY == mime_type)//容错处理，如果bootbar没有name就到其他地方找名称
        {
            OTA_TAG_PATH_T  tag_path = {0};
            OTA_SETTING_PATH_T setting_path = {0};
            
            tag_path.parameter_name = MMIOTA_SEETTING_NAME;
            TagNodeCreate(&tag_path, TRUE);
            setting_path.tag_path = &tag_path;
            name_ptr = GetSettingVlue(&setting_path, MMIOTA_SEETTING_NAME);
        }

        if (name_ptr != PNULL)
        {
            uint16  name_char_len = 0,mms_char_len = 0,wap_char_len =0;
            name_char_len = SCI_STRLEN((char*)name_ptr);
            SCI_MEMCPY(temp_buf, (char*)name_ptr, name_char_len);
            if (mime_type == MIME_OTACONNECTIVITY)
            {
                switch(ota_type) 
                {
                case MMIOTA_MMS_SETTING:
                    mms_char_len = SCI_STRLEN((char*)mms_char);
                    SCI_MEMCPY(temp_buf + name_char_len, mms_char, mms_char_len);
                    break;
                case MMIOTA_BRW_SETTING:
                    wap_char_len = SCI_STRLEN((char*)wap_char);
                    SCI_MEMCPY(temp_buf + name_char_len, wap_char, wap_char_len);
                    break;
                default:
                    break;
                }      
            }        
            mmiota_setting_ptr->name_len = SCI_STRLEN(temp_buf);
            mmiota_setting_ptr->name_len = MIN(mmiota_setting_ptr->name_len, MMIOTA_SETTING_MAX_NAME_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->name, temp_buf, mmiota_setting_ptr->name_len);
            FreeOTAMem(name_ptr);
            name_ptr = PNULL;
        }        
        
        //get gateway
        gateway_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_PROXY_ADDRESS);
        if (gateway_ptr != PNULL)
        {
            mmiota_setting_ptr->gateway_len = SCI_STRLEN((char*)gateway_ptr);
            mmiota_setting_ptr->gateway_len = MIN(mmiota_setting_ptr->gateway_len, MMIOTA_SETTING_MAX_GATEWAY_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->gateway, gateway_ptr, mmiota_setting_ptr->gateway_len);
            FreeOTAMem(gateway_ptr);
            gateway_ptr = PNULL;
        }        
        
        //get user name (authname)
        username_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_APN_USER_NAME);
        if (username_ptr != PNULL)
        {
            mmiota_setting_ptr->username_len = SCI_STRLEN((char*)username_ptr);
            mmiota_setting_ptr->username_len = MIN(mmiota_setting_ptr->username_len, MMIOTA_SETTING_MAX_USERNAME_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->username, username_ptr, mmiota_setting_ptr->username_len);
            FreeOTAMem(username_ptr);
            username_ptr = PNULL;
        }        
        
        //get password (auth secret)
        password_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_APN_PASSWORD);
        if (password_ptr != PNULL)
        {
            mmiota_setting_ptr->password_len = SCI_STRLEN((char*)password_ptr);
            mmiota_setting_ptr->password_len = MIN(mmiota_setting_ptr->password_len, MMIOTA_SETTING_MAX_PASSWORD_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->password, password_ptr, mmiota_setting_ptr->password_len);
            FreeOTAMem(password_ptr);
            password_ptr = PNULL;
        }        
        
        //get access point 
        apn_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_APN);
        if (apn_ptr != PNULL)
        {
            mmiota_setting_ptr->apn_len = SCI_STRLEN((char*)apn_ptr);
            mmiota_setting_ptr->apn_len = MIN(mmiota_setting_ptr->apn_len, MMIOTA_SETTING_MAX_APN_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->apn, apn_ptr, mmiota_setting_ptr->apn_len);
            FreeOTAMem(apn_ptr);
            apn_ptr = PNULL;
        }        
        
        //get homepage,mms is for mmsc
        homepage_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_HOMEPAGE);
        if (PNULL == homepage_ptr && MIME_OTACONNECTIVITY == mime_type)
        {
            OTA_TAG_PATH_T  tag_path = {0};
            OTA_SETTING_PATH_T setting_path = {0};
            
            tag_path.parameter_name = MMIOTA_HOMEPAGE;
            TagNodeCreate(&tag_path, TRUE);
            setting_path.tag_path = &tag_path;
            homepage_ptr = GetSettingVlue(&setting_path, MMIOTA_HOMEPAGE);
        }
        if (homepage_ptr != PNULL)
        {
            //mmiota_setting_ptr->homepage_len = SCI_STRLEN((char*)homepage_ptr);
            //mmiota_setting_ptr->homepage_len = MIN(mmiota_setting_ptr->homepage_len, MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN);
            //SCI_MEMCPY(mmiota_setting_ptr->homepage, homepage_ptr, mmiota_setting_ptr->homepage_len);
            MMIAPICOM_NormalizeUrl(value_ptr, mmiota_setting_ptr->homepage, MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN, FALSE);
            mmiota_setting_ptr->homepage_len = SCI_STRLEN((char *)mmiota_setting_ptr->homepage);

            FreeOTAMem(homepage_ptr);
            homepage_ptr = PNULL;
        }        
    } 
    else if (ota_type == MMIOTA_BRW_BOOKMARK)
    {
        //get bookmark name
        bookmark_name_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_BOOMARK_NAME);
        if (bookmark_name_ptr != PNULL)
        {
            mmiota_setting_ptr->bookmark.bookmark_name_len = SCI_STRLEN((char*)bookmark_name_ptr);
            mmiota_setting_ptr->bookmark.bookmark_name_len = MIN(mmiota_setting_ptr->bookmark.bookmark_name_len, MMIOTA_BOOKMARK_NAME_MAX_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->bookmark.bookmark_name, bookmark_name_ptr, mmiota_setting_ptr->bookmark.bookmark_name_len);
            FreeOTAMem(bookmark_name_ptr);
            bookmark_name_ptr = PNULL;
        }    
        
        //get book mark url
        bookmark_url_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_BOOKMARK_URL);
        if (bookmark_url_ptr != PNULL)
        {
            //mmiota_setting_ptr->bookmark.bookmark_url_len = SCI_STRLEN((char*)bookmark_url_ptr);
            //mmiota_setting_ptr->bookmark.bookmark_url_len = MIN(mmiota_setting_ptr->bookmark.bookmark_url_len, MMIOTA_BOOKMARK_URL_MAX_LEN);
            //SCI_MEMCPY(mmiota_setting_ptr->bookmark.bookmark_url, bookmark_url_ptr, mmiota_setting_ptr->bookmark.bookmark_url_len);
            MMIAPICOM_NormalizeUrl(bookmark_url_ptr, mmiota_setting_ptr->bookmark.bookmark_url, MMIOTA_BOOKMARK_URL_MAX_LEN, FALSE);
            mmiota_setting_ptr->bookmark.bookmark_url_len = SCI_STRLEN((char *)mmiota_setting_ptr->bookmark.bookmark_url);
            FreeOTAMem(bookmark_url_ptr);
            bookmark_url_ptr = PNULL;
        } 
    } 
    else if (ota_type == MMIOTA_BRW_WAP_SI)
    {
        //get si name
        wap_push_name_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SI_NAME);
        if (wap_push_name_ptr != PNULL)
        {
             mmiota_setting_ptr->wap_si.si_name_len = SCI_STRLEN((char*)wap_push_name_ptr);
            mmiota_setting_ptr->wap_si.si_name_len = MIN( mmiota_setting_ptr->wap_si.si_name_len, MMIOTA_WAP_SI_NAME_MAX_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->wap_si.si_name, wap_push_name_ptr, mmiota_setting_ptr->wap_si.si_name_len);
            FreeOTAMem(wap_push_name_ptr);
            wap_push_name_ptr = PNULL;
        }    
        
        //get si url
        wap_push_url_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SI_URL);
        if (wap_push_url_ptr != PNULL)
        {
            //mmiota_setting_ptr->wap_si.si_herf_len = SCI_STRLEN((char*)wap_push_url_ptr);
            //mmiota_setting_ptr->wap_si.si_herf_len = MIN(mmiota_setting_ptr->wap_si.si_herf_len, MMIOTA_BOOKMARK_URL_MAX_LEN);
            //SCI_MEMCPY(mmiota_setting_ptr->wap_si.si_herf, wap_push_url_ptr, mmiota_setting_ptr->wap_si.si_herf_len);
            MMIAPICOM_NormalizeUrl(wap_push_url_ptr, mmiota_setting_ptr->wap_si.si_herf, MMIOTA_WAP_SI_HERF_MAX_LEN, FALSE);
            mmiota_setting_ptr->wap_si.si_herf_len = SCI_STRLEN((char *)mmiota_setting_ptr->wap_si.si_herf);
            FreeOTAMem(wap_push_url_ptr);
            wap_push_url_ptr = PNULL;
        } 
        if (PNULL != g_ota_charater_ptr)
        {
            mmiota_setting_ptr->wap_si.si_content_len = MMIAPICOM_Wstrlen(g_ota_charater_ptr);
            mmiota_setting_ptr->wap_si.si_content_len = MIN(mmiota_setting_ptr->wap_si.si_content_len, MMIOTA_WAP_SI_CONTENT_MAX_LEN);
            MMI_WSTRNCPY(mmiota_setting_ptr->wap_si.si_content, MMIOTA_WAP_SI_CONTENT_MAX_LEN, 
                g_ota_charater_ptr, mmiota_setting_ptr->wap_si.si_content_len, mmiota_setting_ptr->wap_si.si_content_len);
            SCI_FREE(g_ota_charater_ptr);
            g_ota_charater_ptr = PNULL;
        }
        //get si signal action
        wap_push_action_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SI_SIGNAL_ACTION);
        if (wap_push_action_ptr != PNULL)
        {
            if (0 == strcmp((char *)wap_push_action_ptr, "signal-none"))
            {
                mmiota_setting_ptr->wap_si.signal_action = MMIOTA_SI_SIGNAL_ACTION_NONE;
            }
            else if (0 == strcmp((char *)wap_push_action_ptr, "signal-low"))
            {
                mmiota_setting_ptr->wap_si.signal_action = MMIOTA_SI_SIGNAL_ACTION_LOW;
            }
            else if (0 == strcmp((char *)wap_push_action_ptr, "signal-high"))
            {
                mmiota_setting_ptr->wap_si.signal_action = MMIOTA_SI_SIGNAL_ACTION_HIGH;
            }
            else if (0 == strcmp((char *)wap_push_action_ptr, "delete"))
            {
                mmiota_setting_ptr->wap_si.signal_action = MMIOTA_SI_SIGNAL_ACTION_DELETE;
            }
            else
            {
                mmiota_setting_ptr->wap_si.signal_action = MMIOTA_SI_SIGNAL_ACTION_MEDIUM;
            }
            FreeOTAMem(wap_push_action_ptr);
            wap_push_action_ptr = PNULL;
        }
        else
        {
            mmiota_setting_ptr->wap_si.signal_action = MMIOTA_SI_SIGNAL_ACTION_MEDIUM;
        }
        //si id
        value_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SI_ID);
        if (value_ptr != PNULL)
        {
            mmiota_setting_ptr->wap_si.si_id_len = SCI_STRLEN((char*)value_ptr);
            mmiota_setting_ptr->wap_si.si_id_len = MIN(mmiota_setting_ptr->wap_si.si_id_len, MMIOTA_WAP_SI_NAME_MAX_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->wap_si.si_id, value_ptr, mmiota_setting_ptr->wap_si.si_id_len);
            FreeOTAMem(value_ptr);
            value_ptr = PNULL;
        }
        //si created
        value_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SI_CREATED_TIME);
        if (value_ptr != PNULL)
        {
            mmiota_setting_ptr->wap_si.si_created_time_len = SCI_STRLEN((char*)value_ptr);
            mmiota_setting_ptr->wap_si.si_created_time_len = MIN(mmiota_setting_ptr->wap_si.si_created_time_len, MMIOTA_WAP_SI_NAME_MAX_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->wap_si.si_created_time, value_ptr, mmiota_setting_ptr->wap_si.si_created_time_len);
            FreeOTAMem(value_ptr);
            value_ptr = PNULL;
        }
        //si expired
        value_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SI_EXPIRED_TIME);
        if (value_ptr != PNULL)
        {
            mmiota_setting_ptr->wap_si.si_expired_time_len = SCI_STRLEN((char*)value_ptr);
            mmiota_setting_ptr->wap_si.si_expired_time_len = MIN(mmiota_setting_ptr->wap_si.si_expired_time_len, MMIOTA_WAP_SI_NAME_MAX_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->wap_si.si_expired_time, value_ptr, mmiota_setting_ptr->wap_si.si_expired_time_len);
            FreeOTAMem(value_ptr);
            value_ptr = PNULL;
        }
    }
    else if (ota_type == MMIOTA_BRW_WAP_SL)
    {
        //get sl name
        wap_push_name_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SL_NAME);
        if (wap_push_name_ptr != PNULL)
        {
            mmiota_setting_ptr->wap_sl.sl_name_len = SCI_STRLEN((char*)wap_push_name_ptr);
            mmiota_setting_ptr->wap_sl.sl_name_len = MIN(mmiota_setting_ptr->wap_sl.sl_name_len, MMIOTA_BOOKMARK_NAME_MAX_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->wap_sl.sl_name, wap_push_name_ptr, mmiota_setting_ptr->wap_sl.sl_name_len);
            FreeOTAMem(wap_push_name_ptr);
            wap_push_name_ptr = PNULL;
        }    
        
        //get sl url
        wap_push_url_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SL_URL);
        if (wap_push_url_ptr != PNULL)
        {
            //mmiota_setting_ptr->wap_sl.sl_herf_len = SCI_STRLEN((char*)wap_push_url_ptr);
            //mmiota_setting_ptr->wap_sl.sl_herf_len = MIN(mmiota_setting_ptr->wap_sl.sl_herf_len , MMIOTA_BOOKMARK_URL_MAX_LEN);
            //SCI_MEMCPY(mmiota_setting_ptr->wap_sl.sl_herf, wap_push_url_ptr, mmiota_setting_ptr->wap_sl.sl_herf_len);
            MMIAPICOM_NormalizeUrl(wap_push_url_ptr, mmiota_setting_ptr->wap_sl.sl_herf, MMIOTA_WAP_SI_HERF_MAX_LEN, FALSE);
            mmiota_setting_ptr->wap_sl.sl_herf_len = SCI_STRLEN((char *)mmiota_setting_ptr->wap_sl.sl_herf);
            FreeOTAMem(wap_push_url_ptr);
            wap_push_url_ptr = PNULL;
        }
        wap_push_action_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_SL_EXECUTE_ACITON);
        if (wap_push_action_ptr != PNULL)
        {
            if (0 == strcmp((char *)wap_push_action_ptr, "execute-high"))
            {
                mmiota_setting_ptr->wap_sl.execute_action = MMIOTA_SL_EXECUTE_ACTION_HIGH;
            }
            else if (0 == strcmp((char *)wap_push_action_ptr, "cache"))
            {
                mmiota_setting_ptr->wap_sl.execute_action = MMIOTA_SL_EXECUTE_ACTION_CACHE;
            }
            else
            {
                mmiota_setting_ptr->wap_sl.execute_action = MMIOTA_SL_EXECUTE_ACTION_LOW;
            }
            FreeOTAMem(wap_push_action_ptr);
            wap_push_action_ptr = PNULL;
        }
        else
        {
            mmiota_setting_ptr->wap_sl.execute_action = MMIOTA_SL_EXECUTE_ACTION_LOW;
        }
    }
    else if (ota_type == MMIOTA_BRW_WAP_CO)
    {
        //get sl name
        wap_push_name_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_CO_NAME);
        if (wap_push_name_ptr != PNULL)
        {
            mmiota_setting_ptr->wap_co.co_name_len = SCI_STRLEN((char*)wap_push_name_ptr);
            mmiota_setting_ptr->wap_co.co_name_len = MIN(mmiota_setting_ptr->wap_co.co_name_len, MMIOTA_BOOKMARK_NAME_MAX_LEN);
            SCI_MEMCPY(mmiota_setting_ptr->wap_co.co_name, wap_push_name_ptr, mmiota_setting_ptr->wap_co.co_name_len);
            FreeOTAMem(wap_push_name_ptr);
            wap_push_name_ptr = PNULL;
        }    
        
        //get sl url
        wap_push_url_ptr = GetSettingVlue(setting_path_ptr, MMIOTA_WAP_CO_URL);
        if (wap_push_url_ptr != PNULL)
        {
            //mmiota_setting_ptr->wap_co.co_herf_len = SCI_STRLEN((char*)wap_push_url_ptr);
            //mmiota_setting_ptr->wap_co.co_herf_len = MIN(mmiota_setting_ptr->wap_co.co_herf_len, MMIOTA_BOOKMARK_URL_MAX_LEN);
            //SCI_MEMCPY(mmiota_setting_ptr->wap_co.co_herf, wap_push_url_ptr, mmiota_setting_ptr->wap_co.co_herf_len);
            MMIAPICOM_NormalizeUrl(wap_push_url_ptr, mmiota_setting_ptr->wap_co.co_herf, MMIOTA_WAP_SI_HERF_MAX_LEN, FALSE);
            mmiota_setting_ptr->wap_co.co_herf_len = SCI_STRLEN((char *)mmiota_setting_ptr->wap_co.co_herf);
            FreeOTAMem(wap_push_url_ptr);
            wap_push_url_ptr = PNULL;
        }                
    }
    
    //destroy setting path
    SettingPathDestroy(setting_path_ptr);
    setting_path_ptr = PNULL;
    
   return TRUE;
}//end GetOTASettingValue

/* ----------------------------------------------------------------------------
* Name:         WbxmlParserCreate
* Function:     创建wbxml parser
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN WbxmlParserCreate(void)
{
#ifdef PLATFORM_ANTISW3
    g_wbxml_parser_ptr = WBXML_CreateParser();
#elif defined (PLATFORM_UWS6121E)
    g_wbxml_parser_ptr = wbxml_parser_create();
#endif
    if (g_wbxml_parser_ptr == PNULL)
    {
        return FALSE;
    }
    return TRUE;
}//end WbxmlParserCreate

/* ----------------------------------------------------------------------------
* Name:         OTAWbxmlParserDestroy
* Function:     销毁wbxml parser
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void WbxmlParserDestroy(void)
{
#ifdef PLATFORM_ANTISW3
    WBXML_DestroyParser(g_wbxml_parser_ptr);
#elif defined (PLATFORM_UWS6121E)
    wbxml_parser_destroy(g_wbxml_parser_ptr);
#endif
}//end WbxmlParserDestroy

/* ----------------------------------------------------------------------------
* Name:         ParseOTAWbxml
* Function:     初始化wbxml parser中的parse handler，language,user data , 协议map，解析wbxml
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseOTAWbxml(OTA_MIME_TYPE_E mime_type, OTA_DATA_T  *ota_data_ptr)
{
    WBXML_ERR_E ret = WBXML_OK;
    OTA_TAG_ITEM_T user_data = {0};
    WBXML_LANG_E language = WBXML_LANG_OTA_SETTINGS; 
    WBXML_CONTENT_HANDLER parse_handler = 
    {
            PNULL,//WBXMLParseStartDocument,
            PNULL,//WBXMLParseEndDocument,
            StartParseWBXMLElement,
            EndParseWBXMLElement,
            ParseWBXMLCharacters,//WBXMLParseCharacters,
            PNULL
    };
#ifdef PLATFORM_ANTISW3

    WBXML_ParserSetContentHandler(g_wbxml_parser_ptr, &parse_handler); 
    SCI_TRACE_LOW("zhangqiang test mime_type =%d",mime_type);
    //Set  wbxml_parser Parse Language
    if ( mime_type == MIME_BROWERBOOKMARK || mime_type == MIME_BROWERSETTING )
    {
        // Nokia Use,Other Use Default Language
        WBXML_ParserSetLanguage(g_wbxml_parser_ptr,language);        
    }   

    //Set  wbxml_parser User Data
    WBXML_ParserSetUserData(g_wbxml_parser_ptr, &user_data);

    //Parse WBXML document
    if ((ret = WBXML_ParserParse(g_wbxml_parser_ptr, ota_data_ptr->body.ota_user_data_arr, ota_data_ptr->body.length)) != WBXML_OK)
    {
        return FALSE;        
    }
    g_parsered_xml_root = user_data.next;
#elif defined(PLATFORM_UWS6121E)
    wbxml_parser_set_content_handler(g_wbxml_parser_ptr, &parse_handler);
    SCI_TRACE_LOW("zhangqiang test mime_type =%d",mime_type);
    //Set  wbxml_parser Parse Language
    if ( mime_type == MIME_BROWERBOOKMARK || mime_type == MIME_BROWERSETTING )
    {
        // Nokia Use,Other Use Default Language
        wbxml_parser_set_language(g_wbxml_parser_ptr,language);
    }   

    //Set  wbxml_parser User Data
    wbxml_parser_set_user_data(g_wbxml_parser_ptr, &user_data);
    
    //Parse WBXML document
    if ((ret = wbxml_parser_parse(g_wbxml_parser_ptr, ota_data_ptr->body.ota_user_data_arr, ota_data_ptr->body.length)) != WBXML_OK)
    {
        return FALSE;        
    }
    g_parsered_xml_root = user_data.next;
#endif

    return TRUE;
}//end ParseOTAWbxml
#if defined(OMA_CP_SEC_SUPPORT)
PUBLIC BOOLEAN  MMIAPIOTA_ParseWBXMLWithoutSecurityCheck(const uint8  *push_data_ptr, uint32 push_data_len, MMIOTA_CBF provision_setting, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys)
{
    OTA_DATA_T  ota_data = {0};
    OTA_MIME_TYPE_E mime_type = MIME_MAX;         
    MMIOTA_SETTING_T  *mmiota_setting_ptr = PNULL;
    P_MMIOTA_SETTING_T temp_setting_ptr = PNULL;
    // P_MMIOTA_SETTING_T temp_setting_ptr1 = PNULL;
    // P_MMIOTA_BOOKMARK_T temp_bookmark_ptr = PNULL;
    MMIOTA_SETTING_TYPE_E ota_setting_type = MMIOTA_SETTING_MAX;
    uint8 setting_num = 0;
#ifdef DCD_SUPPORT
    char* dcd_page_ptr = PNULL;
#endif
	int sec = -1;
	uint8 mac[20*2+1]={0};


    
    if (push_data_ptr == PNULL || push_data_len == 0)
    {
        return FALSE;
    }
    if (provision_setting == PNULL)
    {
        return FALSE;
    }
    
    //init the get value index
    s_oma_napdef_index = 0;   
    s_oma_application_index = 0;
    s_oma_phisical_add_index = 0;
    //Create Stack /
    g_ota_stack = OTAStackCreate(OTA_STACK_SIZE, OTA_STACK_ALLOW_GROW); 
    
    //Create WBXML Parser
    if (FALSE == WbxmlParserCreate())
    {
        return FALSE;
    }

    //Divided Push Data 
     DividedOTADataToWSPHeaderAndWBXML(push_data_ptr,push_data_len, &ota_data);   
    
    //Get MIME 
    mime_type = GetOTAMIMEFromWSPHeader(ota_data.wsp.ota_wsp_header_data_arr,ota_data.wsp.length,&sec,mac);

    switch(mime_type) 
    {
    case MIME_OTACONNECTIVITY:
    case MIME_BROWERSETTING:
    case MIME_BROWERBOOKMARK:
    case MIME_WAP_PUSH_SI:
    case MIME_WAP_PUSH_CO:
    case MIME_WAP_PUSH_SL:
    	break;

 #ifdef DCD_SUPPORT   
     case MIME_OMA_DCD:
         //SCI_TRACE_LOW:"MMIOTA_ParseWBXML DCD 2"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_3412_112_2_18_2_42_44_5,(uint8*)"");
         MMIAPIDCD_WappushUpdate();		
         return TRUE;
 #endif
#ifdef DRM_SUPPORT 
    case MIME_DRM_RIGHTS:
        return MMIDRM_InstallRoFile((char*)ota_data.body.ota_user_data_arr, ota_data.body.length, TRUE, TRUE);
#endif         
    default:
        return FALSE;
    }

    //Parse WBXML document
    if (!ParseOTAWbxml(mime_type, &ota_data))
    {
        return FALSE;
    }       

    //Destroy g_ota_stack
    OTAStackDestroy(&g_ota_stack); 
    
    //destroy ota data
    DestroyOTAData(&ota_data);

    //为了不影响其他的功能，实现兼容，对oma 的ota单独的处理如下
    if (MIME_OTACONNECTIVITY == mime_type)
    {
        OTA_APPLICATION_MAP_T *temp_app_item_ptr  = PNULL;

        //得到application结构
        MMIOTA_InitSettingMap(mime_type);
        temp_setting_ptr = (MMIOTA_SETTING_T *)CallocOTAMem(sizeof(MMIOTA_SETTING_T));
        if (PNULL == temp_setting_ptr)
        {            
            MMIOTA_FreeApplicationMap(g_ota_connectivity_application_map_ptr);
            g_ota_connectivity_application_map_ptr = PNULL;
            MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
            g_ota_connectivity_map_ptr = PNULL;
            ParserRootDestroy(g_parsered_xml_root);
            WbxmlParserDestroy();
            //SCI_TRACE_LOW:"MMIAPIOTA_ParseWBXML all memery failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_3452_112_2_18_2_42_44_6,(uint8*)"");
            return FALSE;
        }        
        temp_app_item_ptr = g_ota_connectivity_application_map_ptr;
        if(PNULL != temp_app_item_ptr)    //cr153733
        {
            SCI_MEMSET(temp_setting_ptr, 0, sizeof(MMIOTA_SETTING_T));            
            
            if (MMIOTA_GetConSettingValue(temp_app_item_ptr, temp_setting_ptr) && 
                (MMIOTA_MMS_SETTING == temp_setting_ptr->setting_type
                || MMIOTA_BRW_SETTING == temp_setting_ptr->setting_type))
            {
                provision_setting(temp_setting_ptr, sender_ptr, dual_sys);
            }
            
//only for multi application account name getting config for cr131778
            if(MMIOTA_MMS_SETTING == temp_setting_ptr->setting_type
                || MMIOTA_BRW_SETTING == temp_setting_ptr->setting_type)
            {
                //OTA_CHARACTERISTIC_ITEM_T * temp_application_ptr = temp_app_item_ptr->application_ptr;
                while (PNULL != temp_app_item_ptr->next_ptr)
                {
                    SCI_MEMSET(temp_setting_ptr, 0, sizeof(MMIOTA_SETTING_T));
                    if(MMIOTA_GetConSetting_MultAppAcc(temp_app_item_ptr->next_ptr, temp_setting_ptr) && //cr167880
                        (MMIOTA_MMS_SETTING == temp_setting_ptr->setting_type
                        || MMIOTA_BRW_SETTING == temp_setting_ptr->setting_type))
                    {
                        provision_setting(temp_setting_ptr, sender_ptr, dual_sys);
                    }
                    temp_app_item_ptr = temp_app_item_ptr->next_ptr;
                }
            }
            
            //temp_app_item_ptr = temp_app_item_ptr->next_ptr;
        }
        else //when config parameter g_ota_connectivity_application_map_ptr is null, pubwin show error. fixed 168831
        {
            MMIPUB_OpenAlertFailWin(TXT_SET_SETTING_FAIL);
        }
        MMISettingDestroy(mmiota_setting_ptr);
        MMIOTA_FreeApplicationMap(g_ota_connectivity_application_map_ptr);
        g_ota_connectivity_application_map_ptr = PNULL;
        MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
        g_ota_connectivity_map_ptr = PNULL;
        ParserRootDestroy(g_parsered_xml_root);
        WbxmlParserDestroy();
        SCI_FREE(temp_setting_ptr);
        temp_setting_ptr = PNULL;
        return TRUE;
    }    
    //GetOTASettingValue  取得设置的值
    ota_setting_type = GetOTAType(mime_type);
    s_oma_napdef_index = 0;
    s_oma_application_index = 0;
    s_oma_phisical_add_index = 0;

    temp_setting_ptr = (MMIOTA_SETTING_T *)CallocOTAMem(sizeof(MMIOTA_SETTING_T));
    if (temp_setting_ptr == PNULL)
    {
        ParserRootDestroy(g_parsered_xml_root);
        MMISettingDestroy(mmiota_setting_ptr);
        WbxmlParserDestroy();
        return FALSE;
    }

    while (ota_setting_type != MMIOTA_SETTING_MAX)
    {        
        SCI_MEMSET(temp_setting_ptr, 0, sizeof(MMIOTA_SETTING_T));         

        temp_setting_ptr->setting_type = ota_setting_type;
        if (!GetOTASettingValue(temp_setting_ptr, mime_type))
        {
            ParserRootDestroy(g_parsered_xml_root);
            MMISettingDestroy(mmiota_setting_ptr);
            WbxmlParserDestroy();
            return FALSE;
        }
        setting_num++;
        s_oma_napdef_index = setting_num;
        s_oma_application_index = setting_num;
        s_oma_phisical_add_index = setting_num;
        if (mime_type == MIME_OTACONNECTIVITY)//when it is oma
        {
            ota_setting_type = GetOTAType(mime_type);
        }
        else
        {
            ota_setting_type = MMIOTA_SETTING_MAX;
        }        
        mmiota_setting_ptr = temp_setting_ptr;
        //调用回调函数，驱动ota设置
        if (MIME_WAP_PUSH_SL == mime_type)
        {
#ifdef DCD_SUPPORT
            dcd_page_ptr = MMIAPIDCD_MAIN_GetNetHomePage();
            if (0 == strncmp((char *)mmiota_setting_ptr->wap_sl.sl_herf, dcd_page_ptr, strlen(dcd_page_ptr)))
            {
                //SCI_TRACE_LOW:"dcd is wap push entered"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_3528_112_2_18_2_42_44_7,(uint8*)"");
                MMIAPIDCD_WappushUpdate(); // FOR 唤醒DCD的功能....
                ParserRootDestroy(g_parsered_xml_root);
                MMISettingDestroy(mmiota_setting_ptr);
                WbxmlParserDestroy();
                return TRUE;
            }
#endif
        }
        provision_setting(mmiota_setting_ptr, sender_ptr, dual_sys);
    } 
    
    //Destroy g_parsered_xml_root
    ParserRootDestroy(g_parsered_xml_root);     

    //Destroy MMI_setting_ptr & MMI_bookmark_ptr
    MMISettingDestroy(mmiota_setting_ptr); 
    
    //Destroy WBXML Parser 
    WbxmlParserDestroy();
    
    return TRUE;
}
#endif
/* ----------------------------------------------------------------------------
* Name:         MMIPushOTA_ParseWBXML
* Function:     PUSH WBXML解析主函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN  MMIAPIOTA_ParseWBXML(const uint8  *push_data_ptr, uint32 push_data_len, MMIOTA_CBF provision_setting, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys)
{
    OTA_DATA_T  ota_data = {0};
    OTA_MIME_TYPE_E mime_type = MIME_MAX;         
    MMIOTA_SETTING_T  *mmiota_setting_ptr = PNULL;
    P_MMIOTA_SETTING_T temp_setting_ptr = PNULL;
    // P_MMIOTA_SETTING_T temp_setting_ptr1 = PNULL;
    // P_MMIOTA_BOOKMARK_T temp_bookmark_ptr = PNULL;
    MMIOTA_SETTING_TYPE_E ota_setting_type = MMIOTA_SETTING_MAX;
    uint8 setting_num = 0;
#ifdef DCD_SUPPORT
    char* dcd_page_ptr = PNULL;
#endif
    #if defined(OMA_CP_SEC_SUPPORT)
    int sec = -1;
    uint8 mac[20*2+1]={0};
    BOOLEAN isNeedShowDiag = FALSE;
    BOOLEAN securityCheckResult = FALSE;
    #endif

    
    if (push_data_ptr == PNULL || push_data_len == 0)
    {
        return FALSE;
    }
    if (provision_setting == PNULL)
    {
        return FALSE;
    }

    #ifdef OMA_CP_SEC_DEBUG
    SCI_TRACE_LOW("[OTA] push_data_len:%d",push_data_len);
    {
        int i=0;
        for(;i<push_data_len;i++)
        {
            SCI_TRACE_LOW("[OTA] push_data_ptr[%d]:%d",i,push_data_ptr[i]);
        }
    }
    #endif
    
    //init the get value index
    s_oma_napdef_index = 0;   
    s_oma_application_index = 0;
    s_oma_phisical_add_index = 0;
    //Create Stack /
    g_ota_stack = OTAStackCreate(OTA_STACK_SIZE, OTA_STACK_ALLOW_GROW); 
    
    //Create WBXML Parser
    if (FALSE == WbxmlParserCreate())
    {
        return FALSE;
    }

    //Divided Push Data 
    DividedOTADataToWSPHeaderAndWBXML(push_data_ptr,push_data_len, &ota_data);   
    
    //Get MIME 
    #if defined(OMA_CP_SEC_SUPPORT)
    mime_type = GetOTAMIMEFromWSPHeader(ota_data.wsp.ota_wsp_header_data_arr,ota_data.wsp.length,&sec,mac);
    if (MIME_OTACONNECTIVITY == mime_type)
    {
           switch(sec)
           {
               case 0:     //NETWPIN
                   //need to check networkpin
                   isNeedShowDiag = FALSE;
                   {
                   unsigned char		hashValue[20];
                   unsigned char		hashValueString[40];
                   uint8 hashValueStringLen  = 0;
                   unsigned char		hmacKey[20];
                   uint32				hmacKeyLen;
                   int i=0;
       
                   MN_IMSI_T imsi = {0};
       
                   imsi = MNSIM_GetImsiEx(dual_sys);
       	     SCI_TRACE_LOW("[OTA] imsi[%d]:%2X %2X %2X %2X %2X %2X %2X %2X",dual_sys,imsi.imsi_val[0],imsi.imsi_val[1],imsi.imsi_val[2]
			 	,imsi.imsi_val[3],imsi.imsi_val[4],imsi.imsi_val[5],imsi.imsi_val[6],imsi.imsi_val[7]);		
                   HmacSha1((unsigned char *) imsi.imsi_val, 
                               imsi.imsi_len,
                               (const unsigned char *)ota_data.body.ota_user_data_arr,
                               ota_data.body.length, 
                               hashValue, hmacKey,&hmacKeyLen); 
                   for (i=0;i<20;i++){
                           sprintf(hashValueString+hashValueStringLen,"%02X",hashValue[i]);
                           hashValueStringLen += 2;
                   }
                   SCI_TRACE_LOW("[OTA] mac:%s",mac);			
                   SCI_TRACE_LOW("[OTA] hashValueString:%s",hashValueString);
                   if (0==strncmp(hashValueString,mac,40))
                   {
                       securityCheckResult = TRUE;
                   }				
                   }	                   
                   break;
               case 1:     //USERPIN
                   isNeedShowDiag = TRUE;
		     securityCheckResult = FALSE;					   
                   SCI_TRACE_LOW("[OTA]  USERPIN");
                   break;
               case 2:     //USERNETWPIN
                   isNeedShowDiag = TRUE;
		     securityCheckResult = FALSE;					   
                   SCI_TRACE_LOW("[OTA]  USERNETWPIN");
                   break;
               case 3:     //USERPINMAC
                   isNeedShowDiag = FALSE;
		     securityCheckResult = TRUE;	//先直接接收				   
                   SCI_TRACE_LOW("[OTA]  USERPINMAC");
                   break;     
               default:     //NO AU
                   isNeedShowDiag = FALSE;
		     securityCheckResult = FALSE;		   
                   SCI_TRACE_LOW("[OTA]  NO AU return 0");
                   break;    			
           }

	    SCI_TRACE_LOW("[OTA] isNeedShowDiag=%d,securityCheckResult=%d",isNeedShowDiag,securityCheckResult);
	    if (isNeedShowDiag)
	    {
		        OTA_WIN_DATA_T* addData = PNULL;
		        addData = (OTA_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(OTA_WIN_DATA_T));
			 if (PNULL == addData)
			 {	
				SCI_TRACE_LOW("[OTA] malloc addData failed!");
				//Destroy g_ota_stack
				OTAStackDestroy(&g_ota_stack);                   
				//destroy ota data
				DestroyOTAData(&ota_data);	
				//Destroy WBXML Parser 
				WbxmlParserDestroy();	
				return FALSE;		 				
			 }
			 memset(addData,0,sizeof(OTA_WIN_DATA_T));	
			 //malloc push data
			 addData->push_data_len = push_data_len;
			 addData->push_data_ptr = (uint8*)SCI_ALLOC_APP(addData->push_data_len*sizeof(uint8));
			 if (PNULL == addData->push_data_ptr)
			 {	
				SCI_TRACE_LOW("[OTA] malloc addData->push_data_ptr failed!");
				if (PNULL != addData)
				{
					SCI_FREE(addData);
				}
				//Destroy g_ota_stack
				OTAStackDestroy(&g_ota_stack);                   
				//destroy ota data
				DestroyOTAData(&ota_data);	
				//Destroy WBXML Parser 
				WbxmlParserDestroy();	
				return FALSE;					
			 }			 
			 memset(addData->push_data_ptr,0,addData->push_data_len*sizeof(uint8));
			 memcpy(addData->push_data_ptr,push_data_ptr,addData->push_data_len*sizeof(uint8));
			 //mallock wbxml
			 addData->security_info.wbxml_length = ota_data.body.length;
			 addData->security_info.wbxml = (uint8 *)SCI_ALLOC_APP(addData->security_info.wbxml_length*sizeof(uint8));
			 if (PNULL == addData->security_info.wbxml)
			 {	
				SCI_TRACE_LOW("[OTA] malloc addData->security_info.wbxml failed!");
				if (PNULL != addData->push_data_ptr)
				{
					SCI_FREE(addData->push_data_ptr);
				}
				if (PNULL != addData)
				{
					SCI_FREE(addData);
				}	
				//Destroy g_ota_stack
				OTAStackDestroy(&g_ota_stack);                   
				//destroy ota data
				DestroyOTAData(&ota_data);	
				//Destroy WBXML Parser 
				WbxmlParserDestroy();	
				return FALSE;					
			 }				 
			 memset(addData->security_info.wbxml,0,addData->security_info.wbxml_length*sizeof(uint8));
			 memcpy(addData->security_info.wbxml,ota_data.body.ota_user_data_arr,(addData->security_info.wbxml_length*sizeof(uint8)));
			 //send num
			 memcpy(addData->send_num,sender_ptr,sizeof(addData->send_num));
			 //provision setting callback
			 addData->provision_setting = provision_setting;
			 //security info others
			 addData->security_info.dual_sys = dual_sys;
			 addData->security_info.sec = sec;
			 memcpy(addData->security_info.mac,mac,sizeof(addData->security_info.mac));
			 addData->security_info.needCheckPin = FALSE; //reserved
			 
			 SCI_TRACE_LOW("[OTA] userpin,create pin inputing win");
			 enterOTAPinInputWin((void*) addData);

			 //Destroy g_ota_stack
			 OTAStackDestroy(&g_ota_stack); 
                  
			 //destroy ota data
			 DestroyOTAData(&ota_data);	
              
			 //Destroy WBXML Parser 
			 WbxmlParserDestroy();	
			 return FALSE;		 
			
	    }	   
	    else if (!securityCheckResult){
			//need not to parse,return
                  //Destroy g_ota_stack
                  OTAStackDestroy(&g_ota_stack); 
                  
                  //destroy ota data
                  DestroyOTAData(&ota_data);	
              
                  //Destroy WBXML Parser 
                  WbxmlParserDestroy();	
			return FALSE;
	    }

    }
    #else
    mime_type = GetOTAMIMEFromWSPHeader(ota_data.wsp.ota_wsp_header_data_arr,ota_data.wsp.length);
    #endif
    switch(mime_type) 
    {
    case MIME_OTACONNECTIVITY:
    case MIME_BROWERSETTING:
    case MIME_BROWERBOOKMARK:
    case MIME_WAP_PUSH_SI:
    case MIME_WAP_PUSH_CO:
    case MIME_WAP_PUSH_SL:
    	break;

 #ifdef DCD_SUPPORT   
     case MIME_OMA_DCD:
         //SCI_TRACE_LOW:"MMIOTA_ParseWBXML DCD 2"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_3412_112_2_18_2_42_44_5,(uint8*)"");
         MMIAPIDCD_WappushUpdate();		
         return TRUE;
 #endif
#ifdef DRM_SUPPORT 
    case MIME_DRM_RIGHTS:
        return MMIDRM_InstallRoFile((char*)ota_data.body.ota_user_data_arr, ota_data.body.length, TRUE, TRUE);
#endif         
    default:
        return FALSE;
    }

    //Parse WBXML document
    if (!ParseOTAWbxml(mime_type, &ota_data))
    {
        return FALSE;
    }       

    //Destroy g_ota_stack
    OTAStackDestroy(&g_ota_stack); 
    
    //destroy ota data
    DestroyOTAData(&ota_data);

    //为了不影响其他的功能，实现兼容，对oma 的ota单独的处理如下
    if (MIME_OTACONNECTIVITY == mime_type)
    {
        OTA_APPLICATION_MAP_T *temp_app_item_ptr  = PNULL;

        //得到application结构
        MMIOTA_InitSettingMap(mime_type);
        temp_setting_ptr = (MMIOTA_SETTING_T *)CallocOTAMem(sizeof(MMIOTA_SETTING_T));
        if (PNULL == temp_setting_ptr)
        {            
            MMIOTA_FreeApplicationMap(g_ota_connectivity_application_map_ptr);
            g_ota_connectivity_application_map_ptr = PNULL;
            MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
            g_ota_connectivity_map_ptr = PNULL;
            ParserRootDestroy(g_parsered_xml_root);
            WbxmlParserDestroy();
            //SCI_TRACE_LOW:"MMIAPIOTA_ParseWBXML all memery failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_3452_112_2_18_2_42_44_6,(uint8*)"");
            return FALSE;
        }        
        temp_app_item_ptr = g_ota_connectivity_application_map_ptr;
        if(PNULL != temp_app_item_ptr)    //cr153733
        {
            SCI_MEMSET(temp_setting_ptr, 0, sizeof(MMIOTA_SETTING_T));            
            
            if (MMIOTA_GetConSettingValue(temp_app_item_ptr, temp_setting_ptr) && 
                (MMIOTA_MMS_SETTING == temp_setting_ptr->setting_type
                || MMIOTA_BRW_SETTING == temp_setting_ptr->setting_type))
            {
                provision_setting(temp_setting_ptr, sender_ptr, dual_sys);
            }
            
//only for multi application account name getting config for cr131778
            if(MMIOTA_MMS_SETTING == temp_setting_ptr->setting_type
                || MMIOTA_BRW_SETTING == temp_setting_ptr->setting_type)
            {
                //OTA_CHARACTERISTIC_ITEM_T * temp_application_ptr = temp_app_item_ptr->application_ptr;
                while (PNULL != temp_app_item_ptr->next_ptr)
                {
                    SCI_MEMSET(temp_setting_ptr, 0, sizeof(MMIOTA_SETTING_T));
                    if(MMIOTA_GetConSetting_MultAppAcc(temp_app_item_ptr->next_ptr, temp_setting_ptr) && //cr167880
                        (MMIOTA_MMS_SETTING == temp_setting_ptr->setting_type
                        || MMIOTA_BRW_SETTING == temp_setting_ptr->setting_type))
                    {
                        provision_setting(temp_setting_ptr, sender_ptr, dual_sys);
                    }
                    temp_app_item_ptr = temp_app_item_ptr->next_ptr;
                }
            }
            
            //temp_app_item_ptr = temp_app_item_ptr->next_ptr;
        }
        else //when config parameter g_ota_connectivity_application_map_ptr is null, pubwin show error. fixed 168831
        {
            MMIPUB_OpenAlertFailWin(TXT_SET_SETTING_FAIL);
        }
        MMISettingDestroy(mmiota_setting_ptr);
        MMIOTA_FreeApplicationMap(g_ota_connectivity_application_map_ptr);
        g_ota_connectivity_application_map_ptr = PNULL;
        MMIOTA_FreeSettingMap(g_ota_connectivity_map_ptr);
        g_ota_connectivity_map_ptr = PNULL;
        ParserRootDestroy(g_parsered_xml_root);
        WbxmlParserDestroy();
        SCI_FREE(temp_setting_ptr);
        temp_setting_ptr = PNULL;
        return TRUE;
    }    
    //GetOTASettingValue  取得设置的值
    ota_setting_type = GetOTAType(mime_type);
    s_oma_napdef_index = 0;
    s_oma_application_index = 0;
    s_oma_phisical_add_index = 0;

    temp_setting_ptr = (MMIOTA_SETTING_T *)CallocOTAMem(sizeof(MMIOTA_SETTING_T));
    if (temp_setting_ptr == PNULL)
    {
        ParserRootDestroy(g_parsered_xml_root);
        MMISettingDestroy(mmiota_setting_ptr);
        WbxmlParserDestroy();
        return FALSE;
    }

    while (ota_setting_type != MMIOTA_SETTING_MAX)
    {        
        SCI_MEMSET(temp_setting_ptr, 0, sizeof(MMIOTA_SETTING_T));         

        temp_setting_ptr->setting_type = ota_setting_type;
        if (!GetOTASettingValue(temp_setting_ptr, mime_type))
        {
            ParserRootDestroy(g_parsered_xml_root);
            MMISettingDestroy(mmiota_setting_ptr);
            WbxmlParserDestroy();
            return FALSE;
        }
        setting_num++;
        s_oma_napdef_index = setting_num;
        s_oma_application_index = setting_num;
        s_oma_phisical_add_index = setting_num;
        if (mime_type == MIME_OTACONNECTIVITY)//when it is oma
        {
            ota_setting_type = GetOTAType(mime_type);
        }
        else
        {
            ota_setting_type = MMIOTA_SETTING_MAX;
        }        
        mmiota_setting_ptr = temp_setting_ptr;
        //调用回调函数，驱动ota设置
        if (MIME_WAP_PUSH_SL == mime_type)
        {
#ifdef DCD_SUPPORT
            dcd_page_ptr = MMIAPIDCD_MAIN_GetNetHomePage();
            if (0 == strncmp((char *)mmiota_setting_ptr->wap_sl.sl_herf, dcd_page_ptr, strlen(dcd_page_ptr)))
            {
                //SCI_TRACE_LOW:"dcd is wap push entered"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_MAIN_3528_112_2_18_2_42_44_7,(uint8*)"");
                MMIAPIDCD_WappushUpdate(); // FOR 唤醒DCD的功能....
                ParserRootDestroy(g_parsered_xml_root);
                MMISettingDestroy(mmiota_setting_ptr);
                WbxmlParserDestroy();
                return TRUE;
            }
#endif
        }
        provision_setting(mmiota_setting_ptr, sender_ptr, dual_sys);
    } 
    
    //Destroy g_parsered_xml_root
    ParserRootDestroy(g_parsered_xml_root);     

    //Destroy MMI_setting_ptr & MMI_bookmark_ptr
    MMISettingDestroy(mmiota_setting_ptr); 
    
    //Destroy WBXML Parser 
    WbxmlParserDestroy();
    
    return TRUE;
}

#ifdef MMI_OTA_SUPPORT
/*****************************************************************************/
//  Description : get push list full path
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetOtaPushListPath(
                             wchar          *full_path_name_ptr,    //[OUT]
                             uint16         *full_path_len_ptr      //[OUT]
                             )
{
    BOOLEAN return_val = TRUE;

    return_val = MMIAPIFMM_GetSysFileFullPathForReadUdiskPrefer(
        TRUE,
        FALSE,
        MMIOTA_UCS2_DIR, 
        MMIOTA_UCS2_DIR_LEN,
        MMIOTA_PUSHLIST_NAME, 
        MMIOTA_PUSHLIST_NAME_LEN,        
        full_path_name_ptr,
        full_path_len_ptr
        );

    return return_val;   
}

/*****************************************************************************/
//  Description : concat push
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConcatOtaPush(
                         MMIOTA_PUSH_DETAIL_T* push_item_ptr,
                         MMIOTA_USER_VALID_DATA_T* mms_data
                         )
{
    uint32      i=0;
    uint8       message_num = 1;
    int32       detail_index = -1;
    int32       oldest_index=0;
    uint32      current_time = 0;
    uint8       push_index[MMIOTA_MAX_LONG_PUSH_NUM]={0};
    wchar       list_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16      list_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32      write_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    
    for(i=0;i<MMIOTA_MAX_LONG_PUSH_NUM;i++)
    {
        if((push_item_ptr->message_id == s_long_ota_push->push_detail[i].message_id) 
            &&(push_item_ptr->dual_sim == s_long_ota_push->push_detail[i].dual_sim))
        {
            if(push_item_ptr->index_id != s_long_ota_push->push_detail[i].index_id)
            {
                push_index[message_num] = i;
                message_num++;
            }
            else
            {
                //???t?Dò??-±￡′?óDí??ùμ?PUSH
                SCI_TRACE_LOW("the same PUSH,index_id=%d,time=%d", push_item_ptr->index_id, s_long_ota_push->push_detail[i].push_time);
                return FALSE;
            }
        }
    }
    SCI_TRACE_LOW("message_num=%d,total_num=%d", message_num, push_item_ptr->total_num);
    if(message_num < push_item_ptr->total_num)        //3¤?ìD??′μ???￡?±￡′???PUSH
    {
        current_time = (uint32)MMIAPICOM_GetCurTime();
        for(i=0;i<MMIOTA_MAX_LONG_PUSH_NUM;i++)
        {
            if(s_long_ota_push->push_detail[i].push_time == 0)
            {
                detail_index = i;
                break;
            }
        }
        
        //è?1?3¤PUSH??·??ó?ú2??üà′??￡??ò?éò??a?ú
        if(i == MMIOTA_MAX_LONG_PUSH_NUM)
        {
            for(i=0;i<MMIOTA_MAX_LONG_PUSH_NUM;i++)
            {
                if(((s_long_ota_push->push_detail[i].push_time + 5*MMIOTA_MINUTE_TIME < current_time)
                    ||(s_long_ota_push->push_detail[i].push_time > current_time))
                    &&(push_item_ptr->message_id != s_long_ota_push->push_detail[i].message_id))
                {
                    SCI_MEMSET(&(s_long_ota_push->push_detail[i]),0,sizeof(MMIOTA_PUSH_DETAIL_T));
                    s_long_ota_push->push_detail[i].dual_sim = MN_DUAL_SYS_MAX;
                    s_long_ota_push->push_detail[i].message_id = MMIOTA_INVAILD_INDEX;
                    s_long_ota_push->push_detail[i].is_vavid = FALSE;
                    detail_index = i;
                    break;
                }
                else if(s_long_ota_push->push_detail[i].push_time<s_long_ota_push->push_detail[oldest_index].push_time)
                {
                    oldest_index = i;
                }
            }
        }
        if(i==MMIOTA_MAX_LONG_PUSH_NUM)//??óD1y?úμ?
        {
            SCI_MEMSET(&(s_long_ota_push->push_detail[oldest_index]),0,sizeof(MMIOTA_PUSH_DETAIL_T));
            detail_index = oldest_index;
        }
        SCI_TRACE_LOW("detail_index=%d", detail_index);
        
        if(detail_index >= 0)
        {
            SCI_MEMCPY(&s_long_ota_push->push_detail[detail_index], push_item_ptr , sizeof(MMIOTA_PUSH_DETAIL_T));
            SCI_TRACE_LOW("write longpush file detail_index=%d,index_id=%d",detail_index,push_item_ptr->index_id);
        }
        else
        {
            return FALSE;
        }
    }
    else    //3¤?ìD?ò?μ???￡?o?2￠3¤?ìD?
    {
        uint8        j=1;
        SCI_TRACE_LOW("concact push,ok!");
        while(j<=message_num )
        {
            for(i=1;i<message_num;i++)
            {
                if(s_long_ota_push->push_detail[push_index[i]].index_id == j)
                {
                    SCI_MEMCPY(mms_data->user_valid_data_arr+mms_data->length,s_long_ota_push->push_detail[push_index[i]].push_data,s_long_ota_push->push_detail[push_index[i]].length);
                    mms_data->length += s_long_ota_push->push_detail[push_index[i]].length;
                    SCI_MEMSET(&(s_long_ota_push->push_detail[push_index[i]]),0,sizeof(MMIOTA_PUSH_DETAIL_T));
                    s_long_ota_push->push_detail[i].message_id = MMIOTA_INVAILD_INDEX;
                    s_long_ota_push->push_detail[i].is_vavid = FALSE;
                        break;
                }
                else if(push_item_ptr->index_id == j)
                {
                    SCI_MEMCPY(mms_data->user_valid_data_arr+mms_data->length,push_item_ptr->push_data,push_item_ptr->length);
                    mms_data->length += push_item_ptr->length;
                    break;
                }    
            }
            j++;
        }   
    }
    
    if(GetOtaPushListPath(list_path, &list_path_len))
    {
        //′ò?a???t
        file_handle = MMIAPIFMM_CreateFile(list_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        
        //?áè?head info
        if(SFS_INVALID_HANDLE != file_handle)
        {
            MMIAPIFMM_WriteFile(
                file_handle, 
                s_long_ota_push, 
                sizeof(MMIOTA_LONG_PUSH_T), 
                &write_size, 
                NULL);
            MMIAPIFMM_CloseFile(file_handle);
        }
    }
    return TRUE;
}


/*****************************************************************************/
//  Description : init ota push
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIOTA_PushInit(void)
{
    uint16          i=0;
    BOOLEAN         readlist_result = FALSE;
    MMIFILE_HANDLE  file_handle = 0;
    wchar           list_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16          list_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32          read_size = 0;
    uint32          write_size = 0;

    if(PNULL == s_long_ota_push)
    {
        s_long_ota_push = SCI_ALLOCAZ(sizeof(MMIOTA_LONG_PUSH_T));
        if (PNULL == s_long_ota_push)
        {
            SCI_TRACE_LOW("HandleMMSPreviewWinMsg focus_num_ptr == s_long_ota_push!");
            return;
        }
    }
    for(i=0;i<MMIOTA_MAX_LONG_PUSH_NUM;i++)
    {
        s_long_ota_push->push_detail[i].dual_sim = MN_DUAL_SYS_MAX;
    }
    if(GetOtaPushListPath(list_path, &list_path_len))
    {
        //′ò?a???t
        file_handle = MMIAPIFMM_CreateFile(list_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        
        //?áè?head info
        if(SFS_INVALID_HANDLE != file_handle)
        {
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(
                file_handle, 
                s_long_ota_push, 
                sizeof(MMIOTA_LONG_PUSH_T), 
                &read_size, 
                NULL
                ))
            {
                if(0 == MMIAPICOM_Stricmp((uint8 *)s_long_ota_push->version, (uint8 *)MMIOTA_LIST_VER))
                {
                    readlist_result = TRUE;
                }
                else
                {
                    SCI_MEMCPY(s_long_ota_push->version,MMIOTA_LIST_VER,MMIOTA_LIST_VER_LEN);
                    MMIAPIFMM_WriteFile(
                        file_handle, 
                        s_long_ota_push, 
                        sizeof(MMIOTA_LONG_PUSH_T), 
                        &write_size, 
                        NULL);
                }
            }
            MMIAPIFMM_CloseFile(file_handle);
        }
    }
}

/*****************************************************************************/
//  Description : Decode ota push 
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIOTA_DecodePush(
                              MN_DUAL_SYS_E dual_sys, 
                              APP_MMS_USER_DATA_T *sms_ptr,
                              uint8      *sender_ptr,
                              uint32     sender_len,
                              uint8      *sc_sender_ptr,
                              uint32     sc_sender_len
                              )
{
    //2êD???áa
    MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};
    MMIOTA_USER_VALID_DATA_T*    s_mms_data = PNULL;
    uint8               iei_num = 0;
    uint32              i = 0;
    int32               index= -1;
    uint16              message_id = 0;
    uint32              total_num = 0;
    uint32              index_id = 0;
    // uint8               return_val = 0;
    MMIPUSH_AP_E        sms_ap_id = MMIPUSH_NOT_AP;
    int32               sms_head_len = 0;
    int32               flag_userdata_head = 0;
    int32               sms_head_len_off = 0;
    MMIOTA_PUSH_DETAIL_T*    push_item_ptr = PNULL;
    MMIOTA_CBF ota_cbf_ptr = MMIAPIOTA_AddOTAPUSH;/*lint !e546*/
  
#ifndef WIN32
//    const uint32  push_data_artel_mms_setting_len = sizeof(push_data_artel_mms_setting);
//    const uint32  ota_push_artel_wap_setting_len  = sizeof(ota_push_artel_wap_setting);
//    const uint32  nokia_bookmark_setting_len  = sizeof(nokia_bookmark_setting);
//    const uint32  vodafone_ota_data_len  = sizeof(vodafone_ota_data);
//    const uint32  vodafone_nokia_ota_data_len  = sizeof(vodafone_nokia_ota_data);
//    const uint32  globe_ota_data_len  = sizeof(globe_ota);
//    const uint32  wap_push_si_len  = sizeof(wap_push_si);    
//    uint8 tempdata[] ={0x0};
//    MMIOTA_SETTING_T  MMI_setting_ptr;
//    MMIOTA_BOOKMARK_T MMI_bookmark_ptr;
//    uint8  *sender = "13595683732";
//    
//    const uint32 sub_ota_nokiaua_len = sizeof(sub_ota_nokiaua);
//    const uint32 globe_ota_nokia_len = sizeof(globe_ota_nokia);
    
    //for testing
   // MMIOTA_ParseWBXML(globe_ota_nokia,globe_ota_nokia_len, ota_cbf_ptr, sender, dual_sys);
#endif  
    SCI_TRACE_LOW("MMIOTA_DecodePush dual_sys = %d", dual_sys);

    if(!MMIAPISMS_IsMMSOrderReady())
    {
        SCI_TRACE_LOW("MMIOTA_DecodePush:not ready!");
        return ;
    }
    s_mms_data = SCI_ALLOCAZ(sizeof(MMIOTA_USER_VALID_DATA_T));
    if(PNULL == s_mms_data)
    {
        SCI_TRACE_LOW("s_mms_data MALLOC FAILED");
        return ;
    }
    
    //SCI_MEMSET(s_mms_data,0,sizeof(MMIOTA_USER_VALID_DATA_T));
    SCI_MEMSET(&user_head_ptr_t,0,sizeof(MN_SMS_USER_HEAD_T));
    MNSMS_DecodeUserDataHeadEx(dual_sys, (MN_SMS_USER_DATA_HEAD_T *)(&(sms_ptr->user_data_head_t)),&user_head_ptr_t);
    
    iei_num = user_head_ptr_t.iei_num;
    for (i = 0; i<iei_num;i++)
    {
        SCI_TRACE_LOW("MMIOTA_DecodePush iei_arr[%d].iei=%d", i,user_head_ptr_t.iei_arr[i].iei);

        if((user_head_ptr_t.iei_arr[i].iei == MN_SMS_CONCAT_8_BIT_REF_NUM && user_head_ptr_t.iei_arr[i].iei_data_t[1] > 1)
            ||(user_head_ptr_t.iei_arr[i].iei == MN_SMS_CONCAT_16_BIT_REF_NUM && user_head_ptr_t.iei_arr[i].iei_data_t[2] > 1))  //@aoke.hu add for long sms
        {
            SCI_TRACE_LOW("it's a CONCAT push!");
            index = i;
            break;
        }
        else
        {
            index = -1;
        }
    }
    if (index != -1)    //??PUSH?a3¤?ìD?
    {
        for (i = 0; i < MMIOTA_MAX_LONG_PUSH_NUM; i++)
        {
            if (s_long_ota_push->push_detail[i].push_time == 0)
            {
                s_long_ota_push->push_detail[i].is_vavid = FALSE;
                s_long_ota_push->push_detail[i].message_id = MMIOTA_INVAILD_INDEX;
            } 
        }
        if (user_head_ptr_t.iei_arr[index].iei == MN_SMS_CONCAT_8_BIT_REF_NUM)
        {
            message_id = user_head_ptr_t.iei_arr[index].iei_data_t[0];
            total_num = user_head_ptr_t.iei_arr[index].iei_data_t[1];
            index_id = user_head_ptr_t.iei_arr[index].iei_data_t[2];    
        }
        else if (user_head_ptr_t.iei_arr[index].iei == MN_SMS_CONCAT_16_BIT_REF_NUM)
        {
            message_id = user_head_ptr_t.iei_arr[index].iei_data_t[0];
            message_id = message_id << 8;
            message_id += user_head_ptr_t.iei_arr[index].iei_data_t[1];
            total_num = user_head_ptr_t.iei_arr[index].iei_data_t[2];
            index_id = user_head_ptr_t.iei_arr[index].iei_data_t[3];
        }
        SCI_TRACE_LOW("message_id=%d,total_num=%d,index_id=%d", message_id,total_num,index_id);
        push_item_ptr = (MMIOTA_PUSH_DETAIL_T *)SCI_ALLOCAZ(sizeof(MMIOTA_PUSH_DETAIL_T));
        if (PNULL == push_item_ptr || MMIOTA_MAX_LONG_PUSH_NUM < index_id)
        {
            SCI_TRACE_LOW("MMIOTA_DecodePush:alloc push_item_ptr failed");
            return;
        }
        //SCI_MEMSET((char *)push_item_ptr, 0 , sizeof(MMIOTA_PUSH_DETAIL_T));
        push_item_ptr->dual_sim = dual_sys;
        push_item_ptr->index_id = index_id;
        push_item_ptr->is_vavid = TRUE;
        push_item_ptr->length = sms_ptr->user_valid_data_t.length;
        push_item_ptr->message_id = message_id;
        SCI_MEMCPY(push_item_ptr->push_data,sms_ptr->user_valid_data_t.user_valid_data_arr,MN_SMS_MAX_USER_VALID_DATA_LENGTH);
        push_item_ptr->push_time = MMIAPICOM_GetCurTime();
        push_item_ptr->total_num = total_num;        
        ConcatOtaPush(push_item_ptr,s_mms_data);
        SCI_FREE(push_item_ptr);
        push_item_ptr = PNULL;
    }
    else
    {
        SCI_MEMCPY(s_mms_data->user_valid_data_arr,sms_ptr->user_valid_data_t.user_valid_data_arr,sms_ptr->user_valid_data_t.length);
        s_mms_data->length = sms_ptr->user_valid_data_t.length;
        SCI_TRACE_LOW("MMIOTA_DecodePush: s_mms_data->length = %d", s_mms_data->length);
    }
    //2êD???áaend    

#ifndef WIN32
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);
#endif
    if(0 != s_mms_data->length)
    {
#ifdef MMS_SUPPORT
        sms_ap_id = MMIMMS_PushDispatch(s_mms_data->user_valid_data_arr, s_mms_data->length);	//2é?òPUSH???￠??ó|μ?AP
#endif
        SCI_TRACE_LOW("MMIOTA_DecodePush: s_mms_data->length = %d, sms_ap_id = %d", s_mms_data->length, sms_ap_id);

        switch(sms_ap_id)
        {
//        case MMIPUSH_NOT_AP:
//            break;
            
#ifdef MMS_SUPPORT
        case MMIPUSH_MMS_AP:
            SCI_TRACE_LOW("MMIOTA_DecodePush it is a MMS push");
           	sms_head_len = MMIMMS_PushSMSGetUnitVarValue(&(s_mms_data->user_valid_data_arr[2]), (uint8 *)(&sms_head_len_off));
            
            if(sms_head_len==0)
            {
                flag_userdata_head = 0;
            }
            else
            {
                flag_userdata_head = 1;
            }
 //           MMIMMS_SetActiveSimForRecv(dual_sys);	
            s_push_dual_sys = dual_sys;
            //??è??ìD??DD?o???
            if (mms_sc_address_ptr != PNULL)
            {
                SCI_FREE(mms_sc_address_ptr);
                mms_sc_address_ptr = PNULL;
            }
            mms_sc_address_ptr = (uint8 *)SCI_ALLOC_APP(sc_sender_len + 1);
            SCI_MEMSET(mms_sc_address_ptr, 0, (sc_sender_len + 1));
            SCI_MEMCPY(mms_sc_address_ptr, sc_sender_ptr, sc_sender_len);

            MMS_ParsePushMsg(s_mms_data->user_valid_data_arr, s_mms_data->length, flag_userdata_head, MMIMMS_HandleMMSPushCallback);/*lint !e64*/
            break;
#endif

        default:
            //?ˉàà?÷PUSH

#ifdef PUSH_EMAIL_SUPPORT//Added by sunlu  2010.5.6 for ?D??ê?2?ê?push mail
            if(MMIMAIL_IsPushMailForPort(sms_ptr->user_data_head_t.user_header_arr, sms_ptr->user_data_head_t.length))
            {
                if(MMIMAIL_IsPushMailForContent(s_mms_data->user_valid_data_arr,s_mms_data->length))
                {
                    SCI_TRACE_LOW("# sunlu MMIOTA_DecodePush MMIMAIL_IsPushMailForContent return TRUE!!!!!");
                    break;
                }
            }
#endif /*PUSH_EMAIL_SUPPORT */

            //ota  wap push parser
            MMIAPIOTA_ParseWBXML(s_mms_data->user_valid_data_arr,s_mms_data->length, ota_cbf_ptr, sender_ptr, dual_sys);
#ifdef _ATEST_
#ifdef WIN32
            ATESTOTA_ProcessPressKeySerialByReceiveOta(); // added by Mingxuan.Zhu
#endif//WIN32
#endif//_ATEST_
            break;
        }
        
    }
    SCI_FREE(s_mms_data);
    return;
}

/*****************************************************************************/
//  Description : Decode ota push 
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIOTA_DecodePush(
                                     MN_DUAL_SYS_E dual_sys, 
                                     APP_MMS_USER_DATA_T *sms_ptr,
                                     uint8      *sender_ptr,
                                     uint32     sender_len,
                                     uint8      *sc_sender_ptr,
                                     uint32     sc_sender_len
                                     )
{
    MMIOTA_DecodePush(dual_sys, sms_ptr,sender_ptr,sender_len,sc_sender_ptr,sc_sender_len);
}
#endif//end MMIOTA_ParseWBXML
#endif
