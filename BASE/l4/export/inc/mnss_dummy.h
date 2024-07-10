#ifndef _MNSS_DUMMY_H
#define _MNSS_DUMMY_H

#define MN_MAX_SS_TEST_TEXT_LEN  110

typedef struct{
    SIGNAL_VARS
    MN_DUAL_SYS_E    dual_sys;
    uint8            text_len;
    uint8            text_ptr[MN_MAX_SS_TEST_TEXT_LEN];
    BOOLEAN          is_need_rsp;
}   APP_MN_SS_TEST_TEXT_IND_T;

typedef struct{
    MN_DUAL_SYS_E    dual_sys;
    uint8            text_len;
    uint8            text_ptr[MN_MAX_SS_TEST_TEXT_LEN];
    BOOLEAN          is_need_rsp;
}   APP_MN_SS_TEST_TEXT_INDS_T;


typedef enum 
{
    MIN_TEST_CASE_INDEX,
    TEST_CASE_31_1_1_1,
    TEST_CASE_31_1_1_2_1,
    TEST_CASE_31_1_1_2_2,  // CLIP Interrogation rejected
    TEST_CASE_31_1_2_1,
    TEST_CASE_31_1_2_2,
    TEST_CASE_31_1_2_3_1,
    TEST_CASE_31_1_2_3_2, // CLIR Interrogation rejected
    TEST_CASE_31_1_3_1,
    TEST_CASE_31_1_3_2_1,
    TEST_CASE_31_1_3_2_2, // COLP Interrogation rejected
    TEST_CASE_31_1_4_1_1,
    TEST_CASE_31_1_4_1_2, // COLR Interrogation rejected
    
    TEST_CASE_31_2_1_1_1, // call forward
    TEST_CASE_31_2_1_1_2, // Registration rejected
    TEST_CASE_31_2_1_2_1,
    TEST_CASE_31_2_1_2_2, // Erasure rejected
    TEST_CASE_31_2_1_3,
    TEST_CASE_31_2_1_4,   
    TEST_CASE_31_2_1_6_1,
    TEST_CASE_31_2_1_6_2,  // Interrogation rejected
    TEST_CASE_31_2_1_7_1_1,
    TEST_CASE_31_2_1_7_1_2,
    TEST_CASE_31_2_1_7_2,

    TEST_CASE_31_3_1_1,   // call wait
    TEST_CASE_31_3_1_2_1, // Waiting call accepted; existing call released
    TEST_CASE_31_3_1_2_2, // Waiting call accepted; existing call on hold
    TEST_CASE_31_3_1_2_3, // Existing call released by user A; waiting call accepted
    TEST_CASE_31_3_1_3_1, // Waiting call released by subscriber B
    TEST_CASE_31_3_1_3_2, // Waiting call released by calling user C
    TEST_CASE_31_3_1_4,   // Activation
    TEST_CASE_31_3_1_5,   // Dectivation
    TEST_CASE_31_3_1_6_1, // Interrogation accepted
    TEST_CASE_31_3_1_6_2, // Interrogation rejected
    TEST_CASE_31_3_2_1,   // call hold
    TEST_CASE_31_3_2_2,   // Retrieve
    TEST_CASE_31_3_2_3,   // Alternate from one call to the other

    TEST_CASE_31_4_1_1,    //build mpty, successful
    TEST_CASE_31_4_1_2,    //build mpty, fail
    TEST_CASE_31_4_1_3,    //build mpty, expire
    TEST_CASE_31_4_2_1_1_1, //hold mpty, successful
    TEST_CASE_31_4_2_1_1_2, //hold mpty, fail
    TEST_CASE_31_4_2_1_1_3, //hold mpty, expire
    TEST_CASE_31_4_2_1_2_1, //split mpty
    TEST_CASE_31_4_2_1_2_2, //split mpty, fail
    TEST_CASE_31_4_2_1_2_3, //split mpty, expire
    TEST_CASE_31_4_2_1_3,   //disconnect the entire mpty
    TEST_CASE_31_4_2_1_4,   //disconnect one remote party call
    TEST_CASE_31_4_2_2_1,   //release from the MultiParty call
    TEST_CASE_31_4_3_1_1,   //retrieve mpty, successful
    TEST_CASE_31_4_3_1_2,   //retrieve mpty, fail
    TEST_CASE_31_4_3_1_3,   //retrieve mpty, expire
    TEST_CASE_31_4_3_2,     // initiate a new call
    TEST_CASE_31_4_3_3,     // process a call waiting request
    TEST_CASE_31_4_3_4,     // terminate the hold mpty
    TEST_CASE_31_4_4_1_1,   // disconnect the single call
    TEST_CASE_31_4_4_1_2_3, // Clear all parties of held mpty call
    TEST_CASE_31_4_4_1_2_4, // Clear all parties of active mpty call
    TEST_CASE_31_4_4_2,     // disconnect all calls
    TEST_CASE_31_4_4_3_1,   // add the single call to the mpty, successful
    TEST_CASE_31_4_4_3_2,   // add the single call to the mpty, fail
    TEST_CASE_31_4_4_4,     // alternate between the MPTY call and the single call
    TEST_CASE_31_4_5,       // adding extra remote parties

    TEST_CASE_31_6_1_1,
    TEST_CASE_31_6_1_2,
    TEST_CASE_31_6_1_5,
    TEST_CASE_31_6_1_6,
    TEST_CASE_31_6_1_7,
    TEST_CASE_31_6_1_8,
    TEST_CASE_31_6_2_4,
    TEST_CASE_31_6_2_5,

    TEST_CASE_31_8_1_1,     // call restriction, register successful
    TEST_CASE_31_8_1_2_1,   // Rejection after invoke of the RegisterPassword operation
    TEST_CASE_31_8_1_2_2,   // Rejection after password check with negative result
    TEST_CASE_31_8_1_2_3,   // Rejection after new password mismatch
    TEST_CASE_31_8_3_1,     // Activation accepted
    TEST_CASE_31_8_3_2_1,   // Rejection after invoke of ActivateSS operation
    TEST_CASE_31_8_3_2_2,   // Rejection after use of password procedure
    TEST_CASE_31_8_4_1,     // Deactivation accepted
    TEST_CASE_31_8_4_2_1,   // Rejection after invoke of DeactivateSS operation
    TEST_CASE_31_8_4_2_2,   // Rejection after use of password procedure
    TEST_CASE_31_8_6_1,     // Interrogation accepted
    TEST_CASE_31_8_6_2,     // Interrogation rejected
    TEST_CASE_31_8_7,       // Normal operation
    TEST_CASE_31_9_1_1,     // ProcessUnstructuredSS-request/accepted
    TEST_CASE_31_9_1_2, 
    TEST_CASE_31_9_2_1, 
    TEST_CASE_31_9_2_2, 
    TEST_CASE_31_9_2_3, 
    TEST_CASE_31_9_2_4,
    TEST_CASE_31_10,
    MAX_TEST_CASE_INDEX
}SS_TEST_CASE_INDEX_E;

#endif
