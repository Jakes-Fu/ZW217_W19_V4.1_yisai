/****************************************************************************
 *
 * indic_mt.c
 *
 */
#if 0
#include "utli/IIMEstd.h"
#include "improvider/provider_com.h"
#include "inc/kmxime.h"
#include "core/ldbapi4/kmxime_latin.h"

#include "improvider/IIMEmtprovider.h"
#include "improvider/IIMEpvdlatinfull.h"
#else
#include "IIMEstd.h"
#include "provider_com.h"
#include "kmxime.h"
#include "kmxime_latin.h"

#include "IIMEmtprovider.h"
#include "IIMEpvdlatinfull.h"
#endif
#include "indic_mt.h"


static int find_char_in_multitap_strings(const LDB_CHAR multi_string[MAX_MULTITAPS][32], LDB_CHAR c);

#ifdef KMX_LDB_TELUGU

int GetMultitype_Telugu(
        IN const void*          pdbMisc,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR *pBuffer = 0;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;

    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)pdbMisc;
    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (previous_char)
    {
        if (previous_char >= TELUGU_C_KA && previous_char <= TELUGU_C_HA)
        {
            *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];//default_telugumatra_multitap_strings[index];
        }
        else
        {
            switch (previous_char)
            {
                case (TELUGU_IV_A):
                case (TELUGU_IV_AA):
                case (TELUGU_IV_I):
                case (TELUGU_IV_II):
                case (TELUGU_IV_U):
                case (TELUGU_IV_UU):
                case (TELUGU_IV_VR):
                case (TELUGU_IV_VL):
                case (TELUGU_IV_E):
                case (TELUGU_IV_EE):
                case (TELUGU_IV_AI):
                case (TELUGU_IV_O):
                case (TELUGU_IV_OO):
                case (TELUGU_IV_AU):
                case (TELUGU_DV_AA):
                case (TELUGU_DV_I):
                case (TELUGU_DV_II):
                case (TELUGU_DV_U):
                case (TELUGU_DV_UU):
                case (TELUGU_DV_VR):
                case (TELUGU_DV_VRR):
                case (TELUGU_DV_E):
                case (TELUGU_DV_EE):
                case (TELUGU_DV_AI):
                case (TELUGU_DV_O):
                case (TELUGU_DV_OO):
                case (TELUGU_DV_AU):
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
                    break;
                }
                default:
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
                }
            }
        }

    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
    }

    i = 0;
    pBuffer = *char_list_ptr;
    while (*pBuffer++)
        i++;

    *count_ptr = i;

    return 1;
}

int kmx_is_valid_telugu_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c < 0x0C00 || c > 0x0C7F)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_telugu_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    
    if (c1 < 0x0C00 || c1 > 0x0C7F)
        return kmx_is_valid_telugu_word_start_char(database, c2);
    else if (c2 < 0x0C00 || c2 > 0x0C7F)
        return 1;

    if (c1 >= TELUGU_C_KA && c1 <= TELUGU_C_HA)
    {
        multi_string = pdb->multitap_string_2;//default_telugumatra_multitap_strings;
    }
    else
    {
        switch (c1)
        {
            case (TELUGU_IV_A):
            case (TELUGU_IV_AA):
            case (TELUGU_IV_I):
            case (TELUGU_IV_II):
            case (TELUGU_IV_U):
            case (TELUGU_IV_UU):
            case (TELUGU_IV_VR):
            case (TELUGU_IV_VL):
            case (TELUGU_IV_E):
            case (TELUGU_IV_EE):
            case (TELUGU_IV_AI):
            case (TELUGU_IV_O):
            case (TELUGU_IV_OO):
            case (TELUGU_IV_AU):
            case (TELUGU_DV_AA):
            case (TELUGU_DV_I):
            case (TELUGU_DV_II):
            case (TELUGU_DV_U):
            case (TELUGU_DV_UU):
            case (TELUGU_DV_VR):
            case (TELUGU_DV_VRR):
            case (TELUGU_DV_E):
            case (TELUGU_DV_EE):
            case (TELUGU_DV_AI):
            case (TELUGU_DV_O):
            case (TELUGU_DV_OO):
            case (TELUGU_DV_AU):
            {
                multi_string = pdb->multitap_string_3;
                break;
            }
            default:
            {
                multi_string = pdb->multitap_string_1;
            }
        }
    }

    return find_char_in_multitap_strings(multi_string, c2);
}

#endif

#ifdef KMX_LDB_MARATHI

int GetMultitype_Marathi(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR *pBuffer;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
	
    if (index >= MAX_MULTITAPS || index < 0)
        return 0;
    if (previous_char)
    {
       if ((previous_char >= MARATHI_C_KA && previous_char <= MARATHI_C_HA) || 
           (previous_char >= MARATHI_AC_QA && previous_char <= MARATHI_AC_YYA))
       {
            switch(previous_char)
            {
                case MARATHI_C_KA:
                case MARATHI_C_KHA:
                case MARATHI_C_GA:
                case MARATHI_C_JA:
                case MARATHI_C_DDA:
                case MARATHI_C_DDHA:
                case MARATHI_C_NA:
                case MARATHI_C_PHA:
                case MARATHI_C_YA:
                case MARATHI_C_RA:
                case MARATHI_C_LLA:
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
                    break;
                default:
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_5[index];
            
            }
        }
        else
        {
            switch (previous_char)
            {
                case (MARATHI_DV_AA):
                case (MARATHI_DV_U):
                case (MARATHI_DV_UU):
                case (MARATHI_DV_R):
                case (MARATHI_IV_A):
                case (MARATHI_IV_AA):
                case (MARATHI_IV_I):
                case (MARATHI_IV_U):
                case (MARATHI_IV_UU):
                case (MARATHI_IV_R):
                case (MARATHI_IV_E):
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
                    break;
                }
                case (MARATHI_DV_I):
                case (MARATHI_DV_II):
                case (MARATHI_DV_CE):
                case (MARATHI_DV_E):
                case (MARATHI_DV_AI):
                case (MARATHI_NDV_CO):
                case (MARATHI_DV_O):
                case (MARATHI_DV_AU):
                case (MARATHI_IV_II):
                case (MARATHI_IV_AI):
                case (MARATHI_NIV_CO):
                case (MARATHI_IV_O):
                case (MARATHI_IV_AU):
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_4[index];
                    break;
                }
                default:
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
            }
        }
       
    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
    }
    
    i = 0;
    pBuffer = *char_list_ptr;
    while (*pBuffer++)
        i++;

    *count_ptr = i;

    return 1;
}

int kmx_is_valid_marathi_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c < 0x0900 || c > 0x097F)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_marathi_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c1 < 0x0900 || c1 > 0x097F)
        return kmx_is_valid_marathi_word_start_char(database, c2);
    else if (c2 < 0x0900 || c2 > 0x097F)
        return 1;

    if ((c1 >= MARATHI_C_KA && c1 <= MARATHI_C_HA) || 
       (c1 >= MARATHI_AC_QA && c1 <= MARATHI_AC_YYA))
    {
        switch(c1)
        {
            case MARATHI_C_KA:
            case MARATHI_C_KHA:
            case MARATHI_C_GA:
            case MARATHI_C_JA:
            case MARATHI_C_DDA:
            case MARATHI_C_DDHA:
            case MARATHI_C_NA:
            case MARATHI_C_PHA:
            case MARATHI_C_YA:
            case MARATHI_C_RA:
            case MARATHI_C_LLA:
                multi_string = pdb->multitap_string_2;
                break;
            default:
                multi_string = pdb->multitap_string_5;
        
        }
    }
    else
    {
        switch (c1)
        {
            case (MARATHI_DV_AA):
            case (MARATHI_DV_U):
            case (MARATHI_DV_UU):
            case (MARATHI_DV_R):
            case (MARATHI_IV_A):
            case (MARATHI_IV_AA):
            case (MARATHI_IV_I):
            case (MARATHI_IV_U):
            case (MARATHI_IV_UU):
            case (MARATHI_IV_R):
            case (MARATHI_IV_E):
            {
                multi_string = pdb->multitap_string_3;
                break;
            }
            case (MARATHI_DV_I):
            case (MARATHI_DV_II):
            case (MARATHI_DV_CE):
            case (MARATHI_DV_E):
            case (MARATHI_DV_AI):
            case (MARATHI_NDV_CO):
            case (MARATHI_DV_O):
            case (MARATHI_DV_AU):
            case (MARATHI_IV_II):
            case (MARATHI_IV_AI):
            case (MARATHI_NIV_CO):
            case (MARATHI_IV_O):
            case (MARATHI_IV_AU):
            {
                multi_string = pdb->multitap_string_4;
                break;
            }
            default:
                multi_string = pdb->multitap_string_1;
        }
    }

    return find_char_in_multitap_strings(multi_string, c2);
}

#endif

#ifdef KMX_LDB_TAMIL

int GetMultitype_Tamil(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR* pBuffer;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (previous_char)
    {
        if (previous_char >= TAMIL_C_KA && previous_char <= TAMIL_C_HA)
        {
            *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
        }
        else
        {
            switch (previous_char)
            {
                case (TAMIL_DV_AA):
                case (TAMIL_DV_I):
                case (TAMIL_DV_II):
                case (TAMIL_DV_U):
                case (TAMIL_DV_UU):
                case (TAMIL_DV_E):
                case (TAMIL_DV_EE):
                case (TAMIL_DV_AI):
                case (TAMIL_DV_O):
                case (TAMIL_DV_OO):
                case (TAMIL_DV_AU):
                case (TAMIL_IV_A):
                case (TAMIL_IV_AA):
                case (TAMIL_IV_I):
                case (TAMIL_IV_II):
                case (TAMIL_IV_U):
                case (TAMIL_IV_UU):
                case (TAMIL_IV_E):
                case (TAMIL_IV_EE):
                case (TAMIL_IV_O):
                case (TAMIL_IV_OO):
                case (TAMIL_IV_AU):
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
                    break;
                }
                default:
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
            }
        }
    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
    }

    i = 0;
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;
}

int kmx_is_valid_tamil_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    if (c < 0x0B80 || c > 0x0BFF)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_tamil_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c1 < 0x0B80 || c1 > 0x0BFF)
        return kmx_is_valid_tamil_word_start_char(database, c2);
    else if (c2 < 0x0B80 || c2 > 0x0BFF)
        return 1;

    if (c1 >= TAMIL_C_KA && c1 <= TAMIL_C_HA)
    {
        multi_string = pdb->multitap_string_2;
    }
    else
    {
        switch (c1)
        {
            case (TAMIL_DV_AA):
            case (TAMIL_DV_I):
            case (TAMIL_DV_II):
            case (TAMIL_DV_U):
            case (TAMIL_DV_UU):
            case (TAMIL_DV_E):
            case (TAMIL_DV_EE):
            case (TAMIL_DV_AI):
            case (TAMIL_DV_O):
            case (TAMIL_DV_OO):
            case (TAMIL_DV_AU):
            case (TAMIL_IV_A):
            case (TAMIL_IV_AA):
            case (TAMIL_IV_I):
            case (TAMIL_IV_II):
            case (TAMIL_IV_U):
            case (TAMIL_IV_UU):
            case (TAMIL_IV_E):
            case (TAMIL_IV_EE):
            case (TAMIL_IV_O):
            case (TAMIL_IV_OO):
            case (TAMIL_IV_AU):
            {
                multi_string = pdb->multitap_string_3;
                break;
            }
            default:
                multi_string = pdb->multitap_string_1;
        }
    }

    return find_char_in_multitap_strings(multi_string, c2);
}

#endif /* KMX_LDB_TAMIL */

#ifdef KMX_LDB_PUNJABI

int GetMultitype_Punjabi(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR* pBuffer;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (previous_char)
    {
        if (previous_char >= PUNJABI_C_KA && previous_char <= PUNJABI_C_HA || previous_char >= PUNJABI_AC_KHHA &&
            previous_char <= PUNJABI_AC_FA)
        {
            *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
        }
        else
        {
            switch (previous_char)
            {
                case (PUNJABI_DV_AA):
                case (PUNJABI_DV_II):
                case (PUNJABI_DV_EE):
                case (PUNJABI_DV_AI):
                case (PUNJABI_DV_OO):
                case (PUNJABI_DV_AU):
                case (PUNJABI_IV_AA):
                case (PUNJABI_IV_EE):
                case (PUNJABI_IV_II):
                case (PUNJABI_IV_AI):
                case (PUNJABI_IV_AU):
                case (PUNJABI_IV_U):
                case (PUNJABI_IV_UU):
                case (PUNJABI_IV_OO):
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
                    break;
                }
                case (PUNJABI_DV_I):
                case (PUNJABI_DV_U):
                case (PUNJABI_DV_UU):
                case (PUNJABI_IV_A):
                case (PUNJABI_IV_I):
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_4[index];
                    break;
                }
                default:
                {
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
                }
            }
        }

    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
    }

    i = 0; 
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;
}

int kmx_is_valid_punjabi_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    if (c < 0x0A00 || c > 0x0A7F)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_punjabi_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c1 < 0x0A00 || c1 > 0x0A7F)
        return kmx_is_valid_punjabi_word_start_char(database, c2);
    else if (c2 < 0x0A00 || c2 > 0x0A7F)
        return 1;

    if (c1 >= PUNJABI_C_KA && c1 <= PUNJABI_C_HA || c1 >= PUNJABI_AC_KHHA && c1 <= PUNJABI_AC_FA)
    {
        multi_string = pdb->multitap_string_2;
    }
    else
    {
        switch (c1)
        {
            case (PUNJABI_DV_AA):
            case (PUNJABI_DV_II):
            case (PUNJABI_DV_EE):
            case (PUNJABI_DV_AI):
            case (PUNJABI_DV_OO):
            case (PUNJABI_DV_AU):
            case (PUNJABI_IV_AA):
            case (PUNJABI_IV_EE):
            case (PUNJABI_IV_II):
            case (PUNJABI_IV_AI):
            case (PUNJABI_IV_AU):
            case (PUNJABI_IV_U):
            case (PUNJABI_IV_UU):
            case (PUNJABI_IV_OO):
            {
                multi_string = pdb->multitap_string_3;
                break;
            }
            case (PUNJABI_DV_I):
            case (PUNJABI_DV_U):
            case (PUNJABI_DV_UU):
            case (PUNJABI_IV_A):
            case (PUNJABI_IV_I):
            {
                multi_string = pdb->multitap_string_4;
                break;
            }
            default:
            {
                multi_string = pdb->multitap_string_1;
            }
        }
    }

    return find_char_in_multitap_strings(multi_string, c2);
}
#endif /* KMX_LDB_PUNJABI */

#if defined(KMX_LDB_GUJARATI)

int GetMultitype_Gujarati(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR* pBuffer;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (previous_char)
    {
        if ((previous_char >= GUJARATI_C_KA && previous_char <= GUJARATI_C_HA))
        {
            *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
        }
        else
        {
            switch (previous_char)
            {
                case (GUJARATI_IV_A):
                case (GUJARATI_IV_AA):
                case (GUJARATI_IV_VR):
                case (GUJARATI_IV_VL):
                case (GUJARATI_DV_AA):
                
                case (GUJARATI_DV_U):
                case (GUJARATI_DV_UU):
                case (GUJARATI_DV_VR):
                case (GUJARATI_DV_VRR):
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
                    break;

                case (GUJARATI_DV_I):
                case (GUJARATI_DV_II):
                case (GUJARATI_DV_CE):
                case (GUJARATI_DV_E):
                case (GUJARATI_DV_AI):
                case (GUJARATI_DV_CO):
                case (GUJARATI_DV_O):
                case (GUJARATI_DV_AU):
                	
                case (GUJARATI_IV_I):
                case (GUJARATI_IV_II):
                case (GUJARATI_IV_U):
                case (GUJARATI_IV_UU):
                	
                case (GUJARATI_IV_CE):
                case (GUJARATI_IV_E):
                case (GUJARATI_IV_AI):
                case (GUJARATI_IV_CO):
                case (GUJARATI_IV_O):
                case (GUJARATI_IV_AU):
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_4[index];
                    break;
                case (GUJARATI_S_NUKTA):
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
                    break;
                default:
                    /* return ((UI_character_type*) default_gujaratimatra_multitap_strings[i]); */
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
                    break;
            }
        }

    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
        /* return ((UI_character_type*) default_gujarati_multitap_strings[i]); */
    }

    i = 0; 
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;
}

int kmx_is_valid_gujarati_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    if (c < 0x0A80 || c > 0x0AFF)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_gujarati_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c1 < 0x0A80 || c1 > 0x0AFF)
        return kmx_is_valid_gujarati_word_start_char(database, c2);
    else if (c2 < 0x0A80 || c2 > 0x0AFF)
        return 1;

    if ((c1 >= GUJARATI_C_KA && c1 <= GUJARATI_C_HA))
    {
        multi_string = pdb->multitap_string_2;
    }
    else
    {
        switch (c1)
        {
            case (GUJARATI_IV_A):
            case (GUJARATI_IV_AA):
            case (GUJARATI_IV_VR):
            case (GUJARATI_IV_VL):
            case (GUJARATI_DV_AA):
            
            case (GUJARATI_DV_U):
            case (GUJARATI_DV_UU):
            case (GUJARATI_DV_VR):
            case (GUJARATI_DV_VRR):
                multi_string  = pdb->multitap_string_3;
                break;

            case (GUJARATI_DV_I):
            case (GUJARATI_DV_II):
            case (GUJARATI_DV_CE):
            case (GUJARATI_DV_E):
            case (GUJARATI_DV_AI):
            case (GUJARATI_DV_CO):
            case (GUJARATI_DV_O):
            case (GUJARATI_DV_AU):
            	
            case (GUJARATI_IV_I):
            case (GUJARATI_IV_II):
            case (GUJARATI_IV_U):
            case (GUJARATI_IV_UU):
            	
            case (GUJARATI_IV_CE):
            case (GUJARATI_IV_E):
            case (GUJARATI_IV_AI):
            case (GUJARATI_IV_CO):
            case (GUJARATI_IV_O):
            case (GUJARATI_IV_AU):
                multi_string  = pdb->multitap_string_4;
                break;
            case (GUJARATI_S_NUKTA):
                multi_string  = pdb->multitap_string_2;
                break;
            default:
                /* return ((UI_character_type*) default_gujaratimatra_multitap_strings[i]); */
                multi_string = pdb->multitap_string_1;
                break;
        }
    }

    return find_char_in_multitap_strings(multi_string, c2);
}

#endif /* KMX_LDB_GUJARATI */

#ifdef KMX_LDB_ORIYA/* KMX_LDB_ORIYA */

int GetMultitype_Oriya(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR* pBuffer;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (previous_char)
    {
        if ((previous_char >= ORIYA_C_KA && previous_char <= ORIYA_C_HA) ||
            (previous_char >= ORIYA_AC_RRA && previous_char <= ORIYA_AC_YYA) || (previous_char >= ORIYA_S_ISSHAR &&
                                                                                 previous_char <= ORIYA_S_WA))
        {
            *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
        }
        else
        {
            switch (previous_char)
            {
                case (ORIYA_IV_A):
                case (ORIYA_IV_AA):
                case (ORIYA_IV_I):
                case (ORIYA_IV_II):
                case (ORIYA_IV_U):
                case (ORIYA_IV_UU):
                case (ORIYA_IV_R):
                case (ORIYA_IV_L):
                case (ORIYA_IV_E):     
                case (ORIYA_IV_O):	
                case (ORIYA_DV_AA):	           	                	
                case (ORIYA_DV_U):		
                case (ORIYA_DV_UU):
                case (ORIYA_DV_R):
                case (ORIYA_DV_RR):   
                case (ORIYA_DV_E):     
                case (ORIYA_DV_O):   
                case (ORIYA_NG_L):     
                case (ORIYA_NG_VLL):
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
                    break;

                case (ORIYA_IV_AI):
                case (ORIYA_IV_AU):
                case (ORIYA_DV_I):
                case (ORIYA_DV_II):
                case (ORIYA_DV_AI):
                case (ORIYA_DV_AU):
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_4[index];
                    break;
                default:
                    /* return ((UI_character_type*) default_ORIYAmatra_multitap_strings[i]); */
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
                    break;
            }
        }
    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
    }

    i = 0; 
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;

}

int kmx_is_valid_oriya_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    if (c < 0x0B00 || c > 0x0B7F)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_oriya_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    
    if (c1 < 0x0B00 || c1 > 0x0B7F)
        return kmx_is_valid_oriya_word_start_char(database, c2);
    else if (c2 < 0x0B00 || c2 > 0x0B7F)
        return 1;

    if ((c1 >= ORIYA_C_KA && c1 <= ORIYA_C_HA) ||
        (c1 >= ORIYA_AC_RRA && c1 <= ORIYA_AC_YYA) || 
        (c1 >= ORIYA_S_ISSHAR && c1 <= ORIYA_S_WA))
    {
        multi_string = pdb->multitap_string_2;
    }
    else
    {
        switch (c1)
        {
            case (ORIYA_IV_A):
            case (ORIYA_IV_AA):
            case (ORIYA_IV_I):
            case (ORIYA_IV_II):
            case (ORIYA_IV_U):
            case (ORIYA_IV_UU):
            case (ORIYA_IV_R):
            case (ORIYA_IV_L):
            case (ORIYA_IV_E):     
            case (ORIYA_IV_O):	
            case (ORIYA_DV_AA):	           	                	
            case (ORIYA_DV_U):		
            case (ORIYA_DV_UU):
            case (ORIYA_DV_R):
            case (ORIYA_DV_RR):   
            case (ORIYA_DV_E):     
            case (ORIYA_DV_O):   
            case (ORIYA_NG_L):     
            case (ORIYA_NG_VLL):
                multi_string = pdb->multitap_string_3;
                break;
            case (ORIYA_IV_AI):
            case (ORIYA_IV_AU):
            case (ORIYA_DV_I):
            case (ORIYA_DV_II):
            case (ORIYA_DV_AI):
            case (ORIYA_DV_AU):
                multi_string = pdb->multitap_string_4;
                break;
            default:
                /* return ((UI_character_type*) default_ORIYAmatra_multitap_strings[i]); */
                multi_string = pdb->multitap_string_1;
                break;
        }
    }

    return find_char_in_multitap_strings(multi_string, c2);

}
#endif /* KMX_LDB_ORIYA */

#ifdef KMX_LDB_KANNADA

int GetMultitype_Kannada(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR* pBuffer;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (previous_char)
    {
        if (previous_char >= KANNADA_C_KA && previous_char <= KANNADA_C_HA)
        {
            *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
        }
        else
        {
            switch (previous_char)
            {
                case (KANNADA_IV_A):
                case (KANNADA_IV_AA):
                case (KANNADA_IV_I):
                case (KANNADA_IV_II):
                case (KANNADA_IV_U):
                case (KANNADA_IV_UU):
                case (KANNADA_IV_VR):
                case (KANNADA_IV_VL):
                case (KANNADA_IV_E):
                case (KANNADA_IV_EE):
                case (KANNADA_IV_AI):
                case (KANNADA_IV_O):
                case (KANNADA_IV_OO):
                case (KANNADA_IV_AU):
                case (KANNADA_DV_AA):
                case (KANNADA_DV_I):
                case (KANNADA_DV_II):
                case (KANNADA_DV_U):
                case (KANNADA_DV_UU):
                case (KANNADA_DV_VR):
                case (KANNADA_DV_VRR):
                case (KANNADA_DV_E):
                case (KANNADA_DV_EE):
                case (KANNADA_DV_AI):
                case (KANNADA_DV_O):
                case (KANNADA_DV_OO):
                case (KANNADA_DV_AU):
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
    				break;
                default:
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
                    break;
            }
        }
    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
    }

    i = 0; 
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;
}

int kmx_is_valid_kannada_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    if (c < 0x0C80 || c > 0x0CFF)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_kannada_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c1 < 0x0C80 || c1 > 0x0CFF)
        return kmx_is_valid_kannada_word_start_char(database, c2);
    else if (c2 < 0x0C80 || c2 > 0x0CFF)
        return 1;

    if (c1 >= KANNADA_C_KA && c1 <= KANNADA_C_HA)
    {
        multi_string = pdb->multitap_string_2;
    }
    else
    {
        switch (c1)
        {
            case (KANNADA_IV_A):
            case (KANNADA_IV_AA):
            case (KANNADA_IV_I):
            case (KANNADA_IV_II):
            case (KANNADA_IV_U):
            case (KANNADA_IV_UU):
            case (KANNADA_IV_VR):
            case (KANNADA_IV_VL):
            case (KANNADA_IV_E):
            case (KANNADA_IV_EE):
            case (KANNADA_IV_AI):
            case (KANNADA_IV_O):
            case (KANNADA_IV_OO):
            case (KANNADA_IV_AU):
            case (KANNADA_DV_AA):
            case (KANNADA_DV_I):
            case (KANNADA_DV_II):
            case (KANNADA_DV_U):
            case (KANNADA_DV_UU):
            case (KANNADA_DV_VR):
            case (KANNADA_DV_VRR):
            case (KANNADA_DV_E):
            case (KANNADA_DV_EE):
            case (KANNADA_DV_AI):
            case (KANNADA_DV_O):
            case (KANNADA_DV_OO):
            case (KANNADA_DV_AU):
                multi_string = pdb->multitap_string_3;
				break;
            default:
                multi_string = pdb->multitap_string_1;
                break;
        }
    }    

    return find_char_in_multitap_strings(multi_string, c2);
}

#endif /* KMX_LDB_KANNADA */

#ifdef KMX_LDB_MALAYALAM

int GetMultitype_Malayalam(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i;
    const LDB_CHAR* pBuffer;
    int index = (iMsg & 0xFFFF) - IIME_VK_0;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (previous_char)
    {
        if ((previous_char >= MALAYALAM_C_KA) && (previous_char <= MALAYALAM_C_HA))
        {
            *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_2[index];
        }
        else
        {
            switch (previous_char)
            {
                case (MALAYALAM_IV_A):
                case (MALAYALAM_IV_AA):
                case (MALAYALAM_IV_I):
                case (MALAYALAM_IV_II):
                case (MALAYALAM_IV_U):
                case (MALAYALAM_IV_UU):
                case (MALAYALAM_IV_VR):
                case (MALAYALAM_IV_VL):
                case (MALAYALAM_IV_E):
                case (MALAYALAM_IV_EE):
                case (MALAYALAM_IV_AI):
                case (MALAYALAM_IV_O):
                case (MALAYALAM_IV_OO):
                case (MALAYALAM_IV_AU):
                case (MALAYALAM_DV_AA):
                case (MALAYALAM_DV_I):
                case (MALAYALAM_DV_II):
                case (MALAYALAM_DV_U):
                case (MALAYALAM_DV_UU):
                case (MALAYALAM_DV_VR):
                case (MALAYALAM_DV_VRR):
                case (MALAYALAM_DV_E):
                case (MALAYALAM_DV_EE):
                case (MALAYALAM_DV_AI):
                case (MALAYALAM_DV_O):
                case (MALAYALAM_DV_OO):
                case (MALAYALAM_DV_AU):
                case (MALAYALAM_IV_VRR):
                case (MALAYALAM_IV_VLL):
				case (MALAYALAM_DV_VL):
                case (MALAYALAM_DV_VLL):                	
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_3[index];
                    break;
                default:
                    *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
                    break;
            }
        }

    }
    else
    {
        *char_list_ptr = (LDB_CHAR*)pdb->multitap_string_1[index];
    }

    i = 0; 
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;
}
int kmx_is_valid_malayalam_word_start_char(const void* database, LDB_CHAR c)
{
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    if (c < 0x0D00 || c > 0x0D7F)
        return 1;

    return find_char_in_multitap_strings(pdb->multitap_string_1, c);
}

int kmx_is_valid_malayalam_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    const LDB_CHAR (*multi_string)[32];
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    if (c1 < 0x0D00 || c1 > 0x0D7F)
        return kmx_is_valid_malayalam_word_start_char(database, c2);
    else if (c2 < 0x0D00 || c2 > 0x0D7F)
        return 1;

    if ((c1 >= MALAYALAM_C_KA) && (c1 <= MALAYALAM_C_HA))
    {
        multi_string = pdb->multitap_string_2;
    }
    else
    {
        switch (c1)
        {
            case (MALAYALAM_IV_A):
            case (MALAYALAM_IV_AA):
            case (MALAYALAM_IV_I):
            case (MALAYALAM_IV_II):
            case (MALAYALAM_IV_U):
            case (MALAYALAM_IV_UU):
            case (MALAYALAM_IV_VR):
            case (MALAYALAM_IV_VL):
            case (MALAYALAM_IV_E):
            case (MALAYALAM_IV_EE):
            case (MALAYALAM_IV_AI):
            case (MALAYALAM_IV_O):
            case (MALAYALAM_IV_OO):
            case (MALAYALAM_IV_AU):
            case (MALAYALAM_DV_AA):
            case (MALAYALAM_DV_I):
            case (MALAYALAM_DV_II):
            case (MALAYALAM_DV_U):
            case (MALAYALAM_DV_UU):
            case (MALAYALAM_DV_VR):
            case (MALAYALAM_DV_VRR):
            case (MALAYALAM_DV_E):
            case (MALAYALAM_DV_EE):
            case (MALAYALAM_DV_AI):
            case (MALAYALAM_DV_O):
            case (MALAYALAM_DV_OO):
            case (MALAYALAM_DV_AU):
            case (MALAYALAM_IV_VRR):
            case (MALAYALAM_IV_VLL):
			case (MALAYALAM_DV_VL):
            case (MALAYALAM_DV_VLL):                	
                multi_string = pdb->multitap_string_3;
                break;
            default:
                multi_string = pdb->multitap_string_1;
                break;
        }
    }

    return find_char_in_multitap_strings(multi_string, c2);

}

#endif /* KMX_LDB_MALAYALAM */

#ifdef KMX_LDB_KHMER
/*
 *  Khmer rulers:
 *              1. the begin must be Consonants(C) or Independent vowel(IV)
 *              2. Independent Vowel (IV) don't following a IV or Dependent Vowel(DV)
 *              3. DV DONT'T following a DV.
 */

typedef enum 
{
    khm_xx,     // Reserved and Obsolete
    khm_cc,     // Consonants
    khm_iv,     // Independent Vowel
    khm_dv,     // Dependent Vowel
    khm_vs     // Various Signs
} KHMER_CHAR_CLASS;

static const KHMER_CHAR_CLASS KHMER_CHAR_CLASS_TABLE[] =
{
  khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, /* 1780 - 178F */
  khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_cc, khm_xx, khm_xx, khm_cc, /* 1790 - 179F */
  khm_cc, khm_cc, khm_cc, khm_xx, khm_xx, khm_iv, khm_iv, khm_iv, khm_xx, khm_iv, khm_iv, khm_iv, khm_iv, khm_iv, khm_iv, khm_iv, /* 17A0 - 17AF */
  khm_iv, khm_iv, khm_iv, khm_iv, khm_xx, khm_xx, khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, /* 17B0 - 17BF */
  khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, khm_dv, khm_vs, khm_vs, khm_vs, khm_vs, khm_vs, khm_vs, khm_vs, khm_vs, khm_vs, khm_vs, /* 17C0 - 17CF */
  khm_vs, khm_xx, khm_vs, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, khm_xx, /* 17D0 - 17DF */
};

static KHMER_CHAR_CLASS GetCharClass(LDB_CHAR c)
{
    int i = 0;
    if (c < 0x1780 || c > 0x17DF)
        return khm_xx;
    else
    {
        i = c - 0x1780;
        return KHMER_CHAR_CLASS_TABLE[i];
    }
}

int GetMultitype_Khmer(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i, index;
    const LDB_CHAR* pBuffer;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    KHMER_CHAR_CLASS cls = GetCharClass(previous_char);
    
    index = (iMsg & 0xFFFF) - IIME_VK_0;
    if (index >= MAX_MULTITAPS || index < 0)
        return 0;

    if (cls == khm_iv)
    {
        *char_list_ptr = pdb->multitap_string_2[index];
    }
    else if (cls == khm_dv)
    {
        *char_list_ptr = pdb->multitap_string_3[index];
    }
    else if (cls == khm_cc || cls == khm_vs)
    {
        *char_list_ptr = pdb->multitap_string_4[index];
    }
    else
    {
        *char_list_ptr = pdb->multitap_string_1[index];
    }

    i = 0; 
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;
}

int kmx_is_valid_khmer_word_start_char(const void* database, LDB_CHAR c)
{
    int ret = 0;
    KHMER_CHAR_CLASS cls = GetCharClass(c);

    switch(cls)
    {
    case khm_xx:
    case khm_cc:
    case khm_iv:
        ret = 1;
    }

    return ret;
}

int kmx_is_valid_khmer_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    int ret = 1;
    KHMER_CHAR_CLASS cls1, cls2;

    cls1 = GetCharClass(c1);
    cls2 = GetCharClass(c2);

    if (cls1 == khm_xx)
        ret = kmx_is_valid_khmer_word_start_char(database, c2);
    if (cls1 == khm_iv && (cls2 == khm_iv || cls2 == khm_dv))
        ret = 0;
    if (cls1 == khm_dv && cls2 == khm_dv)
        ret = 0;

    return ret;
}

#endif /* KMX_LDB_KHMER */
 
#if defined(KMX_LDB_LAO)
/* 
 *  the start must consonants or vowels [0x0EC0, 0EC4]
 */

//static const LDB_CHAR khmer_default_multitap_strings[MAX_MULTITAPS][32] = 
//{
    /* 0x0EAF, 0x0EB1, 0x0EB4, 0x0EB5, 0x0EB6, 0x0EB7, 0x0EB8, 0x0EB9, 0x0EC6, 0x0EC8, 0x0EC9, 0x0ECA, 0x0ECB, 0x0ECC, 0x0ECD, */
    /* 0x0E81 - 0x0E84 */
    /* 0x0E87 - 0x0E88 */
    /* 0x0E8A - 0x0E8D */
    /* 0x0E94 - 0x0E97 */
    /* 0x0E99 - 0x0E9E */
    /* 0x0EB0, 0x0EB2, 0x0EB3, 0x0EBB, 0x0EBC, 0x0EC0, 0x0EC1, 0x0EC2, 0x0EC3, 0x0EC4 */
//}

int GetMultitype_Lao(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i, index;
    const LDB_CHAR *pBuffer;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;

    index = (iMsg & 0xFFFF) - IIME_VK_0;
#if defined(WATERWORLD_LAO_SPECIAL)
    if (index >= MAX_MULTITAPS + 1 || index < 0)
#else
    if (index >= MAX_MULTITAPS || index < 0)
#endif
        return 0;

    if (previous_char >= 0x0E80 && previous_char <= 0x0EDD)
    {
        *char_list_ptr = pdb->multitap_string_2[index];
    }
    else
    {
        *char_list_ptr = pdb->multitap_string_1[index];
    }

    i = 0; 
    pBuffer = *char_list_ptr;
    while(*pBuffer++)
        i++;

    *count_ptr = i;
    return 1;
}

int kmx_is_valid_lao_word_start_char(const void* database, LDB_CHAR c)
{
    if ((c >= 0x0E81 && c <= 0x0EAE) || (c >= 0x0EC0 && c <= 0x0EC4))
        return 1;
    else
        return 0;
}

int kmx_is_valid_lao_follow_char(const void* database, LDB_CHAR c1, LDB_CHAR c2)
{
    if (c1 >= 0x0E81 && c1 <= 0x0ECD)
        return 1;
    else
    {
        if ((c2 >= 0x0E81 && c2 <= 0x0EAE) || (c2 >= 0x0EC0 && c2 <= 0x0EC4))
            return 1;
        else
            return 0;
    }
}

#endif  /* KMX_LDB_LAO */

#if defined(KMX_LDB_MYANMAR)

extern int ldbapi4_is_valid_myanmar_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2);

int kmx_is_valid_myanmar_word_start_char(const void* database, 
    unsigned short              c)
{
    return ldbapi4_is_valid_myanmar_follow_char(0, c);
}

int kmx_is_valid_myanmar_follow_char(const void* database, 
    unsigned short              c1,
    unsigned short              c2)
{
    return ldbapi4_is_valid_myanmar_follow_char(c1, c2);
}

#endif

/* bengali dummy ldb */
#ifdef KMX_LDB_BENGALI

#define B_OOV       0       /* Not a Bengali letter */
#define B_CON       1       /* Consonants           */
#define B_INV       2       /* Independent vowels   */
#define B_DEV       3       /* Dependent vowels     */
#define B_VIR       4       /* Virama sign          */
#define B_SIG       5       /* Other signs          */
#define B_RAF       6       /* RAFALA (09CD + 09B0) or JAPHALA (09CD + 09AF) */
#define B_REP       7       /* REPH (09B0 + 09CD) */
#define B_NUK       8       /* NUKTA (0x09BC) */

unsigned char dummy_get_bengali_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char BENGALI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0980 - 0x098F */ B_OOV ,B_SIG ,B_SIG ,B_SIG ,B_OOV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_OOV ,B_OOV ,B_INV,
    /* 0x0990 - 0x099F */ B_INV ,B_OOV ,B_OOV ,B_INV ,B_INV ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON,
    /* 0x09A0 - 0x09AF */ B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_OOV ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON,
    /* 0x09B0 - 0x09BF */ B_CON ,B_OOV ,B_CON ,B_OOV ,B_OOV ,B_OOV ,B_CON ,B_CON ,B_CON ,B_CON ,B_OOV ,B_OOV ,B_NUK ,B_SIG ,B_DEV ,B_DEV,
    /* 0x09C0 - 0x09CF */ B_DEV ,B_DEV ,B_DEV ,B_DEV ,B_OOV ,B_OOV ,B_OOV ,B_DEV ,B_DEV ,B_OOV ,B_OOV ,B_DEV ,B_DEV ,B_VIR ,B_OOV ,B_OOV,
    /* 0x09D0 - 0x09DF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_CON ,B_CON ,B_OOV ,B_CON,
    /* 0x09E0 - 0x09EF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV,
    /* 0x09F0 - 0x09FF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_RAF ,B_RAF ,B_REP ,B_OOV
    };

    if (c >= 0x980 && c <= 0x9FF)
        return BENGALI_CHAR_TYPE_TABLE[c - 0x980];
    else
        return B_OOV;
}

int dummy_is_valid_bengali_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char BENGALI_CHAR_START_TABLE[9] =
    {
        /* OOV CON INV DEV VIR SIG RAF REP NUK*/
            1,  1,  1,  0,  0,  0,  0,  0,  0
    };

    return BENGALI_CHAR_START_TABLE[dummy_get_bengali_char_type(c)];
}

int dummy_is_valid_bengali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(SPRD_BENGALI_SPECIAL)
    static const unsigned char BENGALI_CHAR_FOLLOW_TABLE[81] =
    {
              /* OOV CON INV DEV VIR SIG RAF REP NUK*/
        /* OOV */ 1,  1,  1,  0,  0,  0,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,  1,  1,  1, 
        /* INV */ 1,  1,  1,  0,  0,  1,  1,  1,  0,
        /* DEV */ 1,  1,  1,  0,  0,  1,  0,  0,  0,
        /* VIR */ 1,  1,  0,  0,  0,  0,  0,  0,  0, 
        /* SIG */ 1,  1,  1,  0,  0,  0,  0,  0,  0,    /* INV can not follow SIG in Hindi, the Only difference. */
        /* RAF */ 1,  1,  1,  1,  1,  1,  0,  0,  0,
        /* REP */ 1,  1,  1,  1,  1,  1,  0,  0,  0,
        /* NUK */ 1,  1,  1,  1,  1,  1,  1,  1,  0
    };
#else
    static const unsigned char BENGALI_CHAR_FOLLOW_TABLE[81] =
    {
              /* OOV CON INV DEV VIR SIG RAF REP NUK*/
        /* OOV */ 1,  1,  1,  0,  0,  0,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,  1,  1,  1, 
        /* INV */ 1,  1,  1,  0,  0,  1,  1,  1,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,  0,  0,  0,
        /* VIR */ 1,  1,  0,  0,  0,  0,  0,  0,  0, 
        /* SIG */ 1,  1,  1,  0,  0,  0,  0,  0,  0,    /* INV can not follow SIG in Hindi, the Only difference. */
        /* RAF */ 1,  1,  1,  1,  1,  1,  0,  0,  0,
        /* REP */ 1,  1,  1,  1,  1,  1,  0,  0,  0,
        /* NUK */ 1,  1,  1,  1,  1,  1,  1,  1,  0
    };
#endif
    unsigned char t1;
    unsigned char t2;
    
    /* For special case 0x985 0x9cd 0x9af */
    if ((c1 == 0x985 || c1 == 0x987  || c1 == 0x989 || c1 == 0x98A || c1 == 0x98F || c1 == 0x993 || c1 == 0x994 )
        && (c2 == 0x9CD || c2 == 0x09FC || c2 == 0x09FD || c2 == 0x09FE))
        return 1;
    
    t1 = dummy_get_bengali_char_type(c1);
    t2 = dummy_get_bengali_char_type(c2);

    return BENGALI_CHAR_FOLLOW_TABLE[t1 * 9 + t2];
}

int GetMultitype_Bengali(
        IN const void*          database,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
    int i, index;
    LDB_CHAR *pBuffer;
    const LDB_CHAR* pList;
    const INDIC_DUMMY_DB *pdb = (const INDIC_DUMMY_DB *)database;
    static LDB_CHAR pOutBuffer[32];

    iMsg = (iMsg & 0xFFFF);
    if (iMsg >= IIME_VK_0 && iMsg <= IIME_VK_9)
        index = iMsg - IIME_VK_0;
    else if (iMsg == IIME_VK_STAR)
        index = 10;
    else if (iMsg == IIME_VK_POUND)
        index = 11;
    else
        return 0;

    pList = pdb->multitap_string_1[index];

    i = 0;
    memset(pOutBuffer, 0, sizeof(LDB_CHAR) * 32);
    pBuffer = pOutBuffer;
    while (*pList)
    {
        if (dummy_is_valid_bengali_follow_char(previous_char, *pList))
        {
            *pBuffer = *pList;
            pBuffer++;
            i++;
        }
        pList++;
    }

    if (i == 0)
        return 0;

    *char_list_ptr = pOutBuffer;
    *count_ptr = i;

    return 1;
}

#endif

static int find_char_in_multitap_strings(const LDB_CHAR multi_string[MAX_MULTITAPS][32], LDB_CHAR c)
{
    int i;
    for (i = 0; i < MAX_MULTITAPS; i++)
    {
        const LDB_CHAR *pString = multi_string[i];
        while (*pString)
        {
            if (c == *pString)
                return 1;
            pString++;
        }
    }

    return 0;
}
