
/*****************************************************************************
** File Name:      srvsample_api.c                                         *
** Author:                                                                  *
** Date:                                                                     *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
                                *
******************************************************************************/



//Read one
PUBLIC BOOLEAN SAMPLESRV_ReadOnebyIndex(uint16 index, SRVSAMPLE_PERSON_ITEM_DATA_T *oneitem_ptr)
{

    SRVSAMPLE_PERSON_DATA_T srv_data = {0};

    if (PNULL == oneitem_ptr)
    {
        //Raise Error
        return FALSE;
    }

    if ( index >= SRVSAMPLE_MAX_RECORD_NUM )
    {
        //Raise Error
        return FALSE;
    }
    if (SAMPLESRV_ReadNV(&srv_data))
    {
        SCI_MEMCPY(oneitem_ptr, &(srv_data.person_item[index]), sizeof(SRVSAMPLE_PERSON_ITEM_DATA_T));
    }

    return TRUE;
}

//EDIT one
PUBLIC BOOLEAN SAMPLESRV_EditOnebyIndex(uint16 index, SRVSAMPLE_PERSON_ITEM_DATA_T *oneitem_ptr)
{
    SRVSAMPLE_PERSON_DATA_T srv_data = {0};

    if (PNULL == oneitem_ptr)
    {
        //Raise Error
        return FALSE;
    }

    if ( index >= SRVSAMPLE_MAX_RECORD_NUM )
    {
        //Raise Error
        return FALSE;
    }

    if (SAMPLESRV_ReadNV(&srv_data))
    {
        SCI_MEMCPY(&(srv_data.person_item[index]), oneitem_ptr, sizeof(SRVSAMPLE_PERSON_ITEM_DATA_T));
        SAMPLESRV_WriteNV(&srv_data);
        return TRUE;
    }

    return FALSE;
}

//Edit one age
PUBLIC BOOLEAN SAMPLESRV_EditOneAgebyIndex(uint16 index, uint16 age)
{
    SRVSAMPLE_PERSON_DATA_T srv_data = {0};

    if ( index >= SRVSAMPLE_MAX_RECORD_NUM )
    {
        //Raise Error
        return FALSE;
    }

    if (SAMPLESRV_ReadNV(&srv_data))
    {
        srv_data.person_item[index].age = age;
        SAMPLESRV_WriteNV(&srv_data);
        return TRUE;
    }

    return FALSE;
}

//Delete  one
PUBLIC BOOLEAN SAMPLESRV_DeleteOnebyIndex(uint16 index)
{
    SRVSAMPLE_PERSON_DATA_T srv_data = {0};

    if ( index >= SRVSAMPLE_MAX_RECORD_NUM )
    {
        //Raise Error
        return FALSE;
    }

    if (SAMPLESRV_ReadNV(&srv_data))
    {
        SCI_MEMSET(&(srv_data.person_item[index]), 0, sizeof(SRVSAMPLE_PERSON_ITEM_DATA_T));
        SAMPLESRV_WriteNV(&srv_data);
        return TRUE;
    }

    return FALSE;
}

//Read Age by input index
PUBLIC BOOLEAN SAMPLESRV_ReadAgebyIndex(uint16 index, uint16 *age)
{
    SRVSAMPLE_PERSON_DATA_T srv_data = {0};

    if ( index >= SRVSAMPLE_MAX_RECORD_NUM )
    {
        //Raise Error
        return FALSE;
    }
    if (SAMPLESRV_ReadNV(&srv_data))
    {
        *age = srv_data.person_item[index].age;
        return TRUE;
    }

    return FALSE;
}

//Read by Filter
PUBLIC uint16 SAMPLESRV_ReadAll(uint16 *output_array, uint16 array_index)
{
    uint16  i = 0;
    SRVSAMPLE_PERSON_DATA_T srv_data = {0};


    if ( output_array == PNULL )
    {
        //Raise Error
        return 0;
    }

    if (array_index > SRVSAMPLE_MAX_RECORD_NUM)
    {
        //Raise Error
        array_index = SRVSAMPLE_MAX_RECORD_NUM;
    }

    if (!SAMPLESRV_ReadNV(&srv_data))
    {
        return 0;
    }

    for (i = 0; i < array_index; i++)
    {
        *output_array = i;      //Get index and set to output array
        output_array++;
    }

    return array_index;
}

//MultiWrite
PUBLIC void SAMPLESRV_byFilter(uint16 *array, uint16 array_len)
{}

//Backup

//Restore


//Asyn

//Timeout


//Req Delete One and wait result
PUBLIC BOOLEAN SAMPLESRV_AysnReadList(uint16 list_idx,uint16 list_len)
{
}







