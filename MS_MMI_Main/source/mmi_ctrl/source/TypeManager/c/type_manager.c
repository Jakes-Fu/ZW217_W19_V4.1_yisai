
#include "type_manager.h"

//////////////////////////////////////////////////////////////
typedef struct _type_info_tag
{
    char*                           type_name;

    uint32                          obj_size;
    OBJECT_CONSTRUCTOR_FUNC         obj_constructor_func;
    OBJECT_DESTRUCTOR_FUNC          obj_destructor_func;

    uint32                          vtbl_size;
    VTBL_INIT_FUNC                  vtbl_init_func;
    uint32*                         vtbl_tbl_ptr;

    int32                           param_size;
    PARAM_PACKING_FUNC              param_packing_func;
    void*                           param_ptr;

    // this is a tree
    struct _type_info_tag           *parent_ptr;
    struct _type_info_tag           *child_ptr;
    struct _type_info_tag           *next_ptr;
} TYPE_INFO_T;
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
static TYPE_INFO_T    s_type_info = {0};

//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
/*****************************************************************************/
//  Description : find type by name
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL TYPE_TAG_T FindTypeByName (
    TYPE_INFO_T     *cur_type_info_ptr,
    char*           name
);

/*****************************************************************************/
//  Description : Construct an object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConstructObject (
    TYPE_INFO_T       *type_info_ptr,
    OBJECT_TYPE_PTR   obj_ptr,
    void              *param_ptr
);

/*****************************************************************************/
//  Description : Destruct an object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestructObject (
    TYPE_INFO_T        *type_info_ptr,
    OBJECT_TYPE_PTR    obj_ptr
);

//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
/*****************************************************************************/
//  Description : create an object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC OBJECT_TYPE_PTR TYPEMNG_Create (
    TYPE_TAG_T  type
)
{
    OBJECT_TYPE_PTR     obj_ptr = PNULL;
    TYPE_INFO_T         *type_info_ptr = (TYPE_INFO_T*) type;

    if (PNULL != type_info_ptr)
    {
        obj_ptr = (OBJECT_TYPE_PTR) SCI_ALLOC_APP (type_info_ptr->obj_size);
        SCI_MEMSET (obj_ptr, 0, type_info_ptr->obj_size);

        obj_ptr->type = (TYPE_TAG_T) type_info_ptr;
    }

    return obj_ptr;
}

/*****************************************************************************/
//  Description : destroy an object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TYPEMNG_Destroy (
    OBJECT_TYPE_PTR obj_ptr
)
{
    if (PNULL != obj_ptr)
    {
        DestructObject ( (TYPE_INFO_T*) (obj_ptr->type), obj_ptr);

        SCI_FREE (obj_ptr);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : construct an object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TYPEMNG_Init (
    OBJECT_TYPE_PTR     object_ptr,
    void                *param_ptr
)
{
    BOOLEAN     result = FALSE;
    TYPE_INFO_T *type_info_ptr = PNULL;

    if (PNULL != object_ptr && PNULL != object_ptr->type)
    {
        type_info_ptr = (TYPE_INFO_T*) object_ptr->type;
		if (PNULL != type_info_ptr->param_ptr
            && PNULL != type_info_ptr->param_packing_func)
        {
            SCI_MEMSET(type_info_ptr->param_ptr, 0, type_info_ptr->param_size);

            type_info_ptr->param_packing_func(param_ptr, type_info_ptr->param_ptr);

            result = ConstructObject (
                         type_info_ptr,
                         object_ptr, type_info_ptr->param_ptr);
        }
        else
        {
            SCI_TRACE_LOW("== TYPEMNG_Init == type no param size or func");
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : register a type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TYPEMNG_Register (
    TYPE_TAG_T                  parent_type,
    char                        *type_name,
    uint32                      obj_size,
    OBJECT_CONSTRUCTOR_FUNC     obj_constructor_func,
    OBJECT_DESTRUCTOR_FUNC      obj_destructor_func,
    uint32                      param_size,
    PARAM_PACKING_FUNC          param_packing_func,
    uint32                      vtbl_size,
    VTBL_INIT_FUNC              vtbl_init_func
)
{
    uint32              str_len = 0;
    TYPE_INFO_T         *cur_type_info_ptr = PNULL;
    TYPE_INFO_T         *parent_type_info_ptr = (TYPE_INFO_T*) parent_type;
    OBJECT_TYPE_PTR     vtbl_obj_ptr = PNULL;

    if (PNULL == type_name
            || FindTypeByName (&s_type_info, type_name))
    {
        return 0;
    }

    cur_type_info_ptr = (TYPE_INFO_T*) SCI_ALLOC_APP (sizeof (TYPE_INFO_T));
    SCI_MEMSET (cur_type_info_ptr, 0, sizeof (TYPE_INFO_T));

    // size and init func for obj and class
    cur_type_info_ptr->obj_size = obj_size;
    cur_type_info_ptr->obj_constructor_func = obj_constructor_func;
    cur_type_info_ptr->obj_destructor_func = obj_destructor_func;
    cur_type_info_ptr->vtbl_size = vtbl_size;
    cur_type_info_ptr->vtbl_init_func = vtbl_init_func;
    cur_type_info_ptr->param_size = param_size;
    cur_type_info_ptr->param_packing_func = param_packing_func;

    // name
    str_len = strlen (type_name);
    cur_type_info_ptr->type_name = (char*) SCI_ALLOC_APP (sizeof (char) * (str_len + 1));
    SCI_MEMSET (cur_type_info_ptr->type_name, 0, sizeof (char) * (str_len + 1));
    SCI_STRCPY (cur_type_info_ptr->type_name, type_name);

    // class public methods
    cur_type_info_ptr->vtbl_tbl_ptr = (uint32 *) SCI_ALLOC_APP (vtbl_size);
    SCI_MEMSET (cur_type_info_ptr->vtbl_tbl_ptr, 0, vtbl_size);

    // param
    if (0 != param_size && PNULL != param_packing_func)
    {
        cur_type_info_ptr->param_ptr = (uint32 *) SCI_ALLOC_APP (param_size);
        SCI_MEMSET (cur_type_info_ptr->param_ptr, 0, param_size);
    }

    // set tree list
    if (0 == parent_type_info_ptr)
    {
        cur_type_info_ptr->parent_ptr = &s_type_info;
    }
    else
    {
        cur_type_info_ptr->parent_ptr = parent_type_info_ptr;

        SCI_MEMCPY (
            cur_type_info_ptr->vtbl_tbl_ptr,
            parent_type_info_ptr->vtbl_tbl_ptr,
            parent_type_info_ptr->vtbl_size);
    }

    vtbl_obj_ptr = (OBJECT_TYPE_PTR) cur_type_info_ptr->vtbl_tbl_ptr;
    vtbl_obj_ptr->type = (TYPE_TAG_T) cur_type_info_ptr;

    if (PNULL != cur_type_info_ptr->parent_ptr->child_ptr)
    {
        cur_type_info_ptr->next_ptr = cur_type_info_ptr->parent_ptr->child_ptr;
    }

    cur_type_info_ptr->parent_ptr->child_ptr = cur_type_info_ptr;

    // init class func
    if (PNULL != vtbl_init_func)
    {
        vtbl_init_func ((OBJECT_VTBL_PTR)cur_type_info_ptr->vtbl_tbl_ptr);
    }

    return (TYPE_TAG_T) cur_type_info_ptr;
}

/*****************************************************************************/
//  Description : check whether object is the specified type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TYPEMNG_IsTypeOf(
                                OBJECT_TYPE_PTR     object_ptr,
                                TYPE_TAG_T          type
                                )
{
    BOOLEAN         result = FALSE;
    TYPE_INFO_T     *type_info_ptr = (TYPE_INFO_T*) type;
    TYPE_INFO_T     *cur_type_info_ptr = PNULL;

    if (PNULL != object_ptr)
    {
        cur_type_info_ptr = (TYPE_INFO_T*)object_ptr->type;

        while (PNULL != cur_type_info_ptr)
        {
            if (cur_type_info_ptr == type_info_ptr)
            {
                result = TRUE;
                break;
            }

            cur_type_info_ptr = cur_type_info_ptr->parent_ptr;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : get vtbl ptr
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC OBJECT_VTBL_PTR TYPEMNG_GetVtbl(
    TYPE_TAG_T           type,          // [in] current ctrl type
    OBJECT_TYPE_PTR      object_ptr     // [in] current ctrl object pointer
)
{
    TYPE_INFO_T *type_info_ptr = (TYPE_INFO_T*) object_ptr->type;

    return (OBJECT_VTBL_PTR)type_info_ptr->vtbl_tbl_ptr;
}

/*****************************************************************************/
//  Description : get parent vtbl ptr
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC OBJECT_VTBL_PTR TYPEMNG_GetParentVtbl (
    TYPE_TAG_T           type,          // [in] current ctrl type
    OBJECT_TYPE_PTR      object_ptr     // [in] current ctrl object pointer
)
{
    OBJECT_VTBL_PTR     parent_vtbl_ptr = PNULL;
    TYPE_INFO_T         *type_info_ptr = (TYPE_INFO_T*) type;

    if (PNULL != type_info_ptr && PNULL != type_info_ptr->parent_ptr)
    {
        parent_vtbl_ptr = (OBJECT_VTBL_PTR)type_info_ptr->parent_ptr->vtbl_tbl_ptr;
    }

    return parent_vtbl_ptr;
}

//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
/*****************************************************************************/
//  Description : get base control type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL TYPE_TAG_T FindTypeByName (
    TYPE_INFO_T     *cur_type_info_ptr,
    char*           name
)
{
    TYPE_TAG_T  type = 0;

    if (PNULL != cur_type_info_ptr)
    {
        // check itself
        if (PNULL != cur_type_info_ptr->type_name
                && 0 == strcmp (cur_type_info_ptr->type_name, name))
        {
            type = (TYPE_TAG_T) cur_type_info_ptr;
        }

        if (0 == type)
        {
            // check neighbour
            type = FindTypeByName (
                       cur_type_info_ptr->next_ptr,
                       name);

            if (0 == type)
            {
                // check child
                type = FindTypeByName (
                           cur_type_info_ptr->child_ptr,
                           name);
            }
        }
    }

    return type;
}

/*****************************************************************************/
//  Description : Construct an object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConstructObject (
    TYPE_INFO_T       *type_info_ptr,
    OBJECT_TYPE_PTR   obj_ptr,
    void              *param_ptr
)
{
    BOOLEAN     result = TRUE;

    if (PNULL != type_info_ptr)
    {
        result = ConstructObject (type_info_ptr->parent_ptr, obj_ptr, param_ptr);

        if (result && type_info_ptr->obj_constructor_func)
        {
            result = type_info_ptr->obj_constructor_func (obj_ptr, param_ptr);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Destruct an object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestructObject (
    TYPE_INFO_T        *type_info_ptr,
    OBJECT_TYPE_PTR    obj_ptr
)
{
    BOOLEAN     result = TRUE;

    if (PNULL != type_info_ptr)
    {
        if (type_info_ptr->obj_destructor_func)
        {
            result = type_info_ptr->obj_destructor_func (obj_ptr);
        }

        result = DestructObject (type_info_ptr->parent_ptr, obj_ptr);
    }

    return result;
}

//////////////////////////////////////////////////////////////

