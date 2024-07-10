/****************************************************************************
** File Name:      soa_api.h                                                *
** Author:         bruce.chi                                                *
** Date:           2008.4                                                   *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the OFFER/ANSWER api       *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2008.5        bruce.chi         Modify                                   *
****************************************************************************/
#ifndef __SOA_API_H__
#define  __SOA_API_H__

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "in_message.h"
#include "sdp_api.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SOA_OK  0   //SOA functions return vaule

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef uint32  SOA_SESS_HANDLE_T;      //soa session handle
typedef uint32  SOA_STREAM_HANDLE_T;    //soa stream handle
typedef int32   SOA_ERR_T;              //soa function retuned error code

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
/*@****************************************************************************
 * SOA_SESS_STATE_E (Offer-Answer Session)
 * ----------------------------------------------------------------------------
 * Represents the state of the SDP Session, while negotiating
 * media capabilities and addresses. The application can retrieve this state using
 * RvOaSessionGetState().
 ***************************************************************************@*/
typedef enum
{
    SOA_SESSION_STATE_UNDEFINED = -1,
        /* Sessions that are not allocated currently stay in UNDEFINED
           state. Calling RvOaCreateSession() moves the session from this
           state to the IDLE state. Calling RvOaSessionTerminate() moves the
           session to the UNDEFINED state. */

    SOA_SESSION_STATE_IDLE,        /*0*/
        /* The newly allocated session resides in the IDLE state.
           Calling RvOaSessionGenerateOffer() or RvOaSessionSetRcvdMsg()
           moves the session out of this state. Calling
           RvOaSessionGenerateOffer() moves the object to the
           OFFER_READY state. Calling RvOaSessionSetRcvdMsg() moves the
           object to the ANSWER_READY state. */

    SOA_SESSION_STATE_OFFER_READY, /*1*/
        /* The session in the OFFER_READY state has an OFFER message that is
           ready to be sent to the remote side. This message can be gotten from
           the session using RvOaSessionGetMsgToBeSent(). Calling to
           RvOaSessionSetRcvdMsg() moves the session from
           this state to the ANSWER_RCVD state.
           Note that RvOaSessionGenerateOffer() can be called only for
           sessions that reside in the IDLE state. */

    SOA_SESSION_STATE_ANSWER_RCVD, /*2*/
        /* The session in the ANSWER_RCVD state handled the received ANSWER
           message. The negotiation is done. The application can retrieve
           addresses and media parameters that are acceptable by both the local
           and remote sides.
           The application can provide the received ANSWER message
           to the session using RvOaSessionSetRcvdMsg().
           Calling RvOaSessionGenerateOffer() or RvOaSessionSetRcvdMsg()
           moves the session out of this state. If these functions
           are called for an object in the ANSWER_RCVD state, this means that
           the Session modification process began.
           Calling to RvOaSessionGenerateOffer() moves the object to
           the OFFER_READY state. Calling RvOaSessionSetRcvdMsg() moves the object
           to the ANSWER_READY state.
           Note that RvOaSessionSetRcvdMsg() can be called only for
           sessions that reside in the OFFER_READY or IDLE states. */

    SOA_SESSION_STATE_ANSWER_READY /*3*/
        /* The session in the ANSWER_READY state has an ANSWER message
           that the negotiation is done. The application can retrieve
           addresses and media parameters that are acceptable by both the local
           and remote sides.
           The application can provide the received OFFER message
           to the session using RvOaSessionSetRcvdMsg().
           Calling RvOaSessionGenerateOffer() or RvOaSessionSetRcvdMsg()
           moves the session out of this state. If these functions
           are called for an object in the ANSWER_READY state, this means
           that the session modification process began. Calling
           RvOaSessionGenerateOffer() moves the object to the
           OFFER_READY state. Calling  RvOaSessionSetRcvdMsg() moves the
           object to the ANSWER_READY state.
           Note that RvOaSessionSetRcvdMsg() can be called only for
           sessions that reside in the OFFER_READY or IDLE states. */

} SOA_SESS_STATE_E;

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         PUBLIC FUNCTION DECLARE                          *
**--------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : soa init
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN SOA_Init(void);

/*@****************************************************************************
 * SOA_SetCapabilities (Offer-Answer Manager API)
 * ----------------------------------------------------------------------------
 * General:
 *  Loads default media capabilities into the Offer-Answer Module.
 *  The capabilities should be provided in the form of a proper SDP message.
 *  If the local capabilities were not set using RvOaSessionSetCapabilities(),
 *  the default capabilities are used for:
 *      1. Generating an OFFER message
 *      2. Identifying the set of codecs supported by both offerer and answerer
 *
 *  Note: The OFFER message is generated by copy-construct of the SDP message
 *        that contains the capabilities. For this reason, the capabilities
 *        should be provided in the form of a proper SDP message.
 *
 * Arguments:
 * Input:  sdp_str_ptr - The default capabilities as a NULL-terminated string.
 *                     Can be NULL.
 *         sdp_msg_handle   - The default capabilities in the form of a 
 *                     SDP Stack message. Can be NULL.
 *                     This parameter has a lower priority than sdp_str_ptr.
 * Output: none.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
//call set session capabilities
/*PUBLIC SOA_ERR_T SOA_SetCapabilities(
                     char               *sdp_str_ptr,       //[IN]
                     SDP_MSG_HANDLE_T   sdp_msg_handle      //[IN]
                     );*/

/*---------------------------------------------------------------------------*/
/*                                SESSION API                                */
/*---------------------------------------------------------------------------*/
/*@****************************************************************************
 * SOA_CreateSession (Offer-Answer Manager API)
 * ----------------------------------------------------------------------------
 * General:
 *  Creates a session object (session). The application should use the session object for
 *  the negotiation of media capabilities. The application should provide the
 *  OFFER and ANSWER messages (that were received as bodies of SIP messages)
 *  to the session. As a result, the session will generate
 *  stream objects (stream) for each negotiated media description. Using these
 *  objects, the application can find various parameters of the Offer-Answer
 *  stream to be transmitted, the including remote
 *  address and media format to be used, the direction of the transmission,
 *  and so on.
 *  The session can generate either OFFER to be sent to answerer, based on
 *  the local or default capabilities, or ANSWER to be sent to the offerer, based
 *  on the received OFFER and local or default capabilities. The generated message
 *  can be gotten anytime during life cycle of the session, using
 *  RvOaSessionGetMsgToBeSent().
 *  The created session stays in the IDLE state until
 *  RvOaSessionGenerateOffer() or RvOaSessionSetRcvdMsg() is called.
 *
 * Arguments:
 * Input:
 * Output: phSession   - The handle to the created session.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_CreateSession(
                                SOA_SESS_HANDLE_T*      sess_handle_ptr   //[OUT]
                                );

/*@****************************************************************************
 * RvOaSessionSetCapabilities (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Loads the local media capabilities into the session.
 *  The local capabilities are used for:
 *      1. Generating an OFFER message
 *      2. Identifying the set of codecs supported by both offerer and answerer
 *
 *  Note: The OFFER message is generated by copy-construct of the SDP message
 *        that contains the capabilities. For this reason, the capabilities
 *        should be provided in the form of a proper SDP message.
 *
 *  The capabilities can be set into IDLE sessions only.
 *
 * Arguments:
 * Input:  hsession  - The handle to the session.
 *         sdp_str_ptr - The local capabilities as a NULL-terminated string.
 *                     Can be NULL.
 *         sdp_msg_handle   - The local capabilities in form of RADVISION SDP Stack
 *                     message. Can be NULL.This parameter has a lower priority
 *                     than strSdpMsg.
 * Output: None.
 *
 * Return Values: Returns RvStatus.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T SOA_SessionSetCapabilities(
                        SOA_SESS_HANDLE_T   sess_handle,        //[IN]
                        char                *sdp_str_ptr,       //[IN]
                        SDP_MSG_HANDLE_T    sdp_msg_handle      //[IN]
                        );

/*@****************************************************************************
 * SOA_SessionGenerateOffer (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Causes a session to build an OFFER message.
 *  After calling this function, the application can get the OFFER message in the
 *  format of a NULL-terminated string and send it with the SIP message.
 *  This can be done using RvOaSessionGetMsgToBeSent() any time during
 *  the life cycle of the session.
 *  RvOaSessionGenerateOffer() performs the following actions:
 *  1. Builds the OFFER message in the RADVISION SDP Stack message format.
 *     The OFFER message is built by the copy-construct operation, when the local
 *     or default capabilities are used as a source.
 *	   The local capabilities can be set into the session using
 *     RvOaSessionSetCapabilities().
 *	   The default capabilities can be set into the session using
 *     RvOaSetCapabilities().
 *     The local capabilities have a higher priority than the default ones.
 *     If local or default capabilities were not set, the empty message
 *     will be built.
 *  2. Adds the missing mandatory lines to the built message, using the default values.
 *  3. Generates a stream for each media descriptor found in the built
 *     message, and inserts it into the list that the session manages.
 *
 *  Any time during the life cycle of a session, the application can iterate
 *  through the list of streams that belong to the session.
 *  The application can inspect and modify its attributes and parameters for
 *  each stream. This can be done using the Offer-Answer Module API for the stream,
 *  or using the RADVISION SDP Stack API for the message object.
 *  Note: The SDP Stack API requires the handle to the message structure that
 *  contains the session description. This handle can be gotten from the session
 *  object using RvOaSessionGetSdpMsg().
 *  RvOaSessionGenerateOffer() moves the session from the IDLE and
 *  ANSWER_READY states to the OFFER_READY state. The session stays
 *  in the OFFER_READY state until RvOaSessionSetRcvdMsg() is called.
 *  RvOaSessionGenerateOffer() can be called for sessions in the IDLE,
 *  ANSWER_RCVD, or ANSWER_READY states only.
 *
 * Arguments:
 * Input:  sess_handle - The the handle to the session.
 * Output: None.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_SessionGenerateOffer(
                                    SOA_SESS_HANDLE_T sess_handle);


/*@****************************************************************************
 * SOA_SessionTerminate (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Terminates a session. This function frees the resources that were
 *  consumed by the session. After calling this function the session
 *  object can no longer be referenced.
 *
 * Arguments:
 * Input:  sess_handle - The handle to the session.
 * Output: None.
 *
 * Return Values: Returns SOA_ERR_T.
 *                This function should not normally fail. A failure should be
 *                treated as an exception.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_SessionTerminate(
                                     SOA_SESS_HANDLE_T sess_handle);


/*@****************************************************************************
 * SOA_SessionSetRcvdMsg (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Loads an incoming SDP message into a session. The incoming SDP
 *  message can be OFFER or ANSWER, which is determined by the state of the
 *  session. If RvOaSessionGenerateOffer() was previously called for the
 *  session , the message is ANSWER. Otherwise, it is OFFER.
 *
 *  For the offerer, RvOaSessionSetRcvdMsg() moves the session from the
 *  OFFER_READY state to the ANSWER_RCVD state.
 *  For the answerer, RvOaSessionSetRcvdMsg() moves the session from the
 *  IDLE state to the ANSWER_READY state.
 *
 * Arguments:
 * Input:  hSession  - The handle to the session.
 *         strSdpMsg - The SDP message in the form of a NULL-terminated string.
 * Output: None.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_SessionSetRcvdMsg(
                                    SOA_SESS_HANDLE_T   sess_handle,    //[IN]
                                    char*               sdp_str_ptr     //[IN]
                                    );

/*@****************************************************************************
 * RvOaSessionGetMsgToBeSent (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Gets the outgoing SDP message from the session.
 *  The outgoing SDP message can be OFFER or ANSWER. If the SDP message was
 *  generated as a result of calling RvOaSessionGenerateOffer(), the message
 *  is OFFER. If the SDP message was generated as a result of calling
 *  RvOaSessionSetRcvdMsg(), the message is ANSWER.
 *
 * Arguments:
 * Input:  hSession  - The handle to the session.
 *         buffSize  - The size in bytes of the memory buffer, pointed by strSdpMsg.
 * Output: strSdpMsg - The SDP message in the form of a NULL-terminated string.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_SessionGetMsgToBeSent(
                                    SOA_SESS_HANDLE_T   sess_handle,    //[IN]
                                    uint32              buff_max_size,  //[IN]
                                    char*               sdp_str_ptr     //[OUT]
                                    );


/*@****************************************************************************
 * SOA_SessionGetStream (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Gets the handle of the streams that belong to the session.
 *  Using this handle and Stream API, the application can retrieve capabilities
 *  and addresses of the remote side, or inspect and update local capabilities
 *  and local addresses.
 *  The remote capabilities and addresses are stored in the remote SDP message.
 *  The local capabilities and addresses are stored in the local SDP message.
 *  Pointers to the local and/or remote SDP messages be retrieved using
 *  RvOaSessionGetSdpMsg().
 *  The local and remote SDP messages can be referenced as OFFER or ANSWER.
 *  If the application is an offerer, the local message represents OFFER and
 *  the remote message represents ANSWER.
 *  If the application is an answerer, the local message represents ANSWER and
 *  the remote message represents OFFER.
 *
 *  The application can retrieve and update the session description using
 *  a pointer to the SDP message and the RADVISION SDP Stack API functions.
 *
 * Arguments:
 * Input:  hSession    - The handle to the session.
 *         hPrevStream - The handle to the stream that appears before
 *                       phStrem in the session's stream list.
 *                       Can be NULL. In this case, the handle of the first stream
 *                       will be supplied by the function.
 * Output: phStream    - The handle to the requested stream.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_SessionGetStream(
                                    SOA_SESS_HANDLE_T       session_handle,     //[IN]
                                    SOA_STREAM_HANDLE_T     prev_stream_handle, //[IN]
                                    SOA_STREAM_HANDLE_T*    stream_handle_ptr   //[OUT]
                                    );

/*@****************************************************************************
 * RvOaSessionResetStream (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Makes the specified stream to be not usable for media delivery.
 *  To be compliant to the RFC 3264, the Stream object is not removed
 *  physically from the Session. Instead, it's port is set to zero and all
 *  media description parameters are removed.
 *
 * Arguments:
 * Input:  hSession - The handle to the session.
 *         hStream  - The handle to the stream.
 * Output: none.
 *
 * Return Values: Returns SOA_ERR_T.
 *****************************************************************************/
PUBLIC SOA_ERR_T  SOA_SessionResetStream(
                                    SOA_SESS_HANDLE_T   sess_handle,    //[IN]
                                    SOA_STREAM_HANDLE_T stream_handle   //[IN]
                                    );

/*@****************************************************************************
 * RvOaSessionGetSdpMsg (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Gets the pointer to the local or remote message of the session.
 *  Using this pointer the application can inspect and modify the local message,
 *  or inspect the remote message.
 *  The local message can represent an OFFER or ANSWER message. This depends
 *  the on role of the application. The local message is OFFER if the application
 *  an is offerer. Or it is ANSWER  if the application is an answerer.
 *  The offerer builds the local message based on local or default capabilities.
 *  The answerer builds the local message based on local or default capabilities
 *  and a received OFFER message. The local capabilities can be set into the
 *  session using RvOaSessionSetCapabilities(). The default capabilities can be
 *  set into the session using RvOaSetCapabilities.
 *  The remote message can represent an OFFER or ANSWER message. This depends
 *  on the role of the application. The remote message is ANSWER if the
 *  application is an offerer. Or it is OFFER if the application is an answerer.
 *  The remote message is built by the session while parsing the string,
 *  provided to the session by the application using RvOaSessionSetRcvdMsg().
 *
 * Arguments:
 * Input:  hSession       - The handle to the session.
 * Output: ppSdpMsgLocal  - The pointer to the RADVISION SDP Stack Message
 *                          object, used as a local message. Can be NULL.
 * Output: ppSdpMsgRemote - The pointer to the RADVISION SDP Stack Message
 *                          object, used as a local message. Can be NULL.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_SessionGetSdpMsg(
                                    SOA_SESS_HANDLE_T   sess_handle,        //[IN]
                                    SDP_MSG_HANDLE_T    *local_sdp_msg_ptr, //[OUT]
                                    SDP_MSG_HANDLE_T    *remote_sdp_msg_ptr //[OUT]
                                    );


/*@****************************************************************************
 * RvOaSessionGetState (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Gets current state of the session.
 *  The state can be OFFER_READY and ANSWER_RCVD for an offerer, ANSWER_READY for
 *  an answerer and IDLE or TERMINATED for both sides.
 *
 * Arguments:
 * Input:  hSession - The handle to the session.
 * Output: peState  - The state.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_SessionGetState(
                                    SOA_SESS_HANDLE_T   sess_handle,   //[IN]
                                    SOA_SESS_STATE_E*   state_ptr      //[OUT]
                                    );
                                    

/*@****************************************************************************
 * RvOaSessionHold (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Changes the media description parameters in order to put the remote side
 *  on hold, for each stream in the session.
 *  For more details, see the "Put-on-hold" procedure described in RFC 3264.
 *  To get the SDP message with the changed parameters, use
 *  RvOaSessionGetMsgToBeSent().
 *
 * Arguments:
 * Input:  hStream  - The handle to the stream.
 * Output: None.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
/*PUBLIC SOA_ERR_T  RvOaSessionHold(IN  SOA_SESS_HANDLE_T   hSession);*/



/*@****************************************************************************
 * RvOaSessionResume (Offer-Answer Session API)
 * ----------------------------------------------------------------------------
 * General:
 *  Changes the media description parameters in order to resume the remote side
 *  from hold, for each stream in the session.
 *  For more details, see the "Put-on-hold" procedure described in RFC 3264.
 *  To get the SDP message with the changed parameters, use
 *  RvOaSessionGetMsgToBeSent().
 *
 * Arguments:
 * Input:  hStream  - The handle to the stream.
 * Output: None.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
/*PUBLIC SOA_ERR_T  RvOaSessionResume(IN  SOA_SESS_HANDLE_T   hSession);*/

/*---------------------------------------------------------------------------*/
/*                                STREAM API                                 */
/*---------------------------------------------------------------------------*/
/*@****************************************************************************
 * RvOaStreamGetMediaDescriptor (Offer-Answer Stream API)
 * ----------------------------------------------------------------------------
 * General:
 *  Gets the pointer to the media-descriptor object in the local message.
 *  Using this pointer, the application can inspect and modify the stream
 *  parameters that are contained in the corresponding media-descriptor sub-tree.
 *  For more details, see the documentation for RvOaSessionGetSdpMsg().
 *
 * Arguments:
 * Input:  hstream      - The handle to the stream.
 *         hSession     - The handle to the session.
 * Output: ppMediaDescrLocal  - The pointer to the media-descriptor object from
 *                              the locally generated SDP message. Can be NULL.
 *         ppMediaDescrRemote - The pointer to the media-descriptor object from
 *                              the SDP message received from remote side.
 *                              Can be NULL.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
PUBLIC SOA_ERR_T  SOA_StreamGetMediaDescriptor(
                                    SOA_STREAM_HANDLE_T         stream_handle,                  //[IN]
                                    SOA_SESS_HANDLE_T           sess_handle,                    //[IN]
                                    SDP_MEDIA_DESCR_HANDLE_T    *local_media_descr_handle_ptr,  //[OUT]
                                    SDP_MEDIA_DESCR_HANDLE_T    *remote_media_descr_handle_ptr  //[OUT]
                                    );


/*@****************************************************************************
 * RvOaStreamGetStatus (Offer-Answer Stream API)
 * ----------------------------------------------------------------------------
 * General:
 * Gets the status of stream. The status can be affected by an incoming OFFER
 * message. As a result of incoming OFFER processing, the stream can be held,
 * resumed, modified, and so on.
 *
 * Arguments:
 * Input:  hStream  - The handle to the stream.
 *         hSession - The handle to the session.
 * Output: pStatus  - The status.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
/*PUBLIC SOA_ERR_T  RvOaStreamGetStatus(
                                    IN  SOA_STREAM_HANDLE_T hStream,
                                    IN  SOA_SESS_HANDLE_T   hSession,
                                    OUT RvOaStreamStatus*   pStatus);*/

/*@****************************************************************************
 * RvOaStreamHold (Offer-Answer Stream API)
 * ----------------------------------------------------------------------------
 * General:
 *  Changes the parameters of stream that put the remote side on hold.
 *  For more details, see the "Put-on-hold" procedure described in RFC 3264.
 *  To get the SDP message with the changed parameters, use
 *  RvOaSessionGetMsgToBeSent().
 *
 * Arguments:
 * Input:  hSession - The handle to the session.
 *         hStream  - The handle to the stream.
 * Output: None.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
/*PUBLIC SOA_ERR_T  RvOaStreamHold(
                                    IN  SOA_SESS_HANDLE_T hSession,
                                    IN  SOA_STREAM_HANDLE_T  hStream);*/

/*@****************************************************************************
 * RvOaStreamResume (Offer-Answer Stream API)
 * ----------------------------------------------------------------------------
 * General:
 *  Changes the parameters of the stream that resume the other side from hold.
 *  For more details, see the "Put-on-hold" procedure described in RFC 3264.
 *  To get the SDP message with the changed parameters, use
 *  RvOaSessionGetMsgToBeSent().
 *
 * Arguments:
 * Input:  hSession - The handle to the session.
 *         hStream  - The handle to the stream.
 * Output: None.
 *
 * Return Values: Returns SOA_ERR_T.
 ***************************************************************************@*/
/*PUBLIC SOA_ERR_T  RvOaStreamResume(
                                    IN  SOA_SESS_HANDLE_T hsession,
                                    IN  SOA_STREAM_HANDLE_T  hStream);*/


#endif