
#include "mmk_type.h"
#include "cafcontrol.h"

#ifndef _TYPE_MANAGER_H_
#define _TYPE_MANAGER_H_


typedef uint32 TYPE_TAG_T;

// base object type
typedef struct _object_type_tag
{
    CAF_VTBL_T(IGUICTRL_T)  *vtbl_ptr;  // 虚表指针，这样定义，使object成为caf对象，只是为了兼容以前的caf模型
    uint32                  ref_num;    // 引用计数，只为兼容
    CAF_GUID_T              guid;       // guid，只为兼容
    TYPE_TAG_T              type;       // 类型信息，这里指向类型信息节点的指针
} OBJECT_TYPE_T, *OBJECT_TYPE_PTR;

// base object class type
typedef struct _object_vtbl_tag
{
    CAF_VTBL_T(IGUICTRL_T)  *vtbl_ptr;  // 虚表指针，这样定义，使object成为caf对象，只是为了兼容以前的caf模型
    uint32                  ref_num;    // 引用计数，只为兼容
    CAF_GUID_T              guid;       // guid，只为兼容
    TYPE_TAG_T              type;       // 类型信息，这里指向类型信息节点的指针
} OBJECT_VTBL_T, *OBJECT_VTBL_PTR;

// object construct function
typedef BOOLEAN  (*OBJECT_CONSTRUCTOR_FUNC)(OBJECT_TYPE_PTR obj_ptr, void* param_ptr);

// object destruct function
typedef BOOLEAN  (*OBJECT_DESTRUCTOR_FUNC)(OBJECT_TYPE_PTR obj_ptr);

// class init function
typedef BOOLEAN  (*VTBL_INIT_FUNC)(OBJECT_VTBL_PTR vtbl_ptr);

// param packing function
typedef void  (*PARAM_PACKING_FUNC)(void* src_param_ptr, void* dst_param_ptr);


/*****************************************************************************/
//  Description : create an object
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC OBJECT_TYPE_PTR TYPEMNG_Create(
                                      TYPE_TAG_T  type
                                      );

/*****************************************************************************/
//  Description : destroy an object
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TYPEMNG_Destroy(
                               OBJECT_TYPE_PTR obj_ptr
                               );

/*****************************************************************************/
//  Description : construct an object
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TYPEMNG_Init(
    OBJECT_TYPE_PTR     object_ptr,
    void                *param_ptr
    );

/*****************************************************************************/
//  Description : register a type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TYPEMNG_Register(
                                  TYPE_TAG_T                parent_type,
                                  char                      *type_name,
                                  uint32                    obj_size,
                                  OBJECT_CONSTRUCTOR_FUNC   obj_constructor_func,
                                  OBJECT_DESTRUCTOR_FUNC    obj_destructor_func,
                                  uint32                    param_size,
                                  PARAM_PACKING_FUNC        param_packing_func,
                                  uint32                    vtbl_ptr,
                                  VTBL_INIT_FUNC            vtbl_init_func
                                  );

/*****************************************************************************/
//  Description : check whether object is the specified type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TYPEMNG_IsTypeOf(
                                OBJECT_TYPE_PTR     object_ptr,
                                TYPE_TAG_T          type
                                );

/*****************************************************************************/
//  Description : get class ptr
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC OBJECT_VTBL_PTR TYPEMNG_GetVtbl(
                                       TYPE_TAG_T           type,
                                       OBJECT_TYPE_PTR      object_ptr
                                       );

/*****************************************************************************/
//  Description : get parent class ptr
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC OBJECT_VTBL_PTR TYPEMNG_GetParentVtbl(
                                             TYPE_TAG_T           type,
                                             OBJECT_TYPE_PTR      object_ptr
                                             );

#endif

