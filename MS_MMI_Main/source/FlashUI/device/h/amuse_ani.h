/*
 * amuse_ani.h
 * Author: Tim <tim@Microrapid.com>
 * 
 * This file contains basic API of Amuse ANI(ActionScript 
 * Native Interface).
 */

#ifndef _AMUSE_ANI_H_
#define _AMUSE_ANI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "amuse_ani_type.h"

#if __WINSCW__ || __GCCE__
#include <_ansi.h>
#else
#define IMPORT_C
#define EXPORT_C
#endif

enum ANI_ERROR_ENUM
{
		NO_INIT_AMUSE=1,
		BAD_PARAMS=2,
};

#define ANI_ASSERT_ARGC(n);	 \
	if (argc != n){amuse_printf("argc error! %d is supposed to be %d, %s:Line %d\n", argc, n, __FILE__, __LINE__); ANI_SET_UNDEFINED(res); return;}
#define ANI_ASSERT_ARGV(n, type); \
	if (ANI_VALUE_GET_TYPE(argv[n]) != type)\
	{\
		amuse_printf("argv type error! %d is supposed to be %d, %s:Line %d\n", ANI_VALUE_GET_TYPE(argv[n]), type, __FILE__, __LINE__);\
		ANI_SET_UNDEFINED(res); \
		return;\
	}
/**
 * This function creates a new string, which should be
 * destroyed by ani_string_delete.
 * Make sure that string data is UTF-8 format.
 * 
 * Parameters:
 * space: data space to create
 * data: string source to copy, return empty string if pass 0
 * length: string length, return empty string if pass 0
 *
 * Returns: the new created string
 */
EXPORT_C astring* ani_string_new(aint space, void* data, aint length);

/**
 * This function destroys a string created by ani_string_new.
 *
 * Parameters:
 * string: the string to be destroyed
 * 
 * Returns: None
 */
EXPORT_C void ani_string_delete(astring* string);

/**
 * This function creates a new object, which will be
 * collected by GC automatically.
 * 
 * Parameters: None
 *
 * Returns: the new created object
 */
EXPORT_C aobject* ani_object_new(void);

/**
 * This function gets a property from an object.
 * 
 * Parameters:
 * obj: object from whom property is obtained
 * prop: property name
 * res: value of the property
 *
 * Returns: None
 */
EXPORT_C void ani_object_get(aobject* obj, astring* prop, avalue* res);

/**
 * This function puts a property to an object.
 * 
 * Parameters:
 * obj: object to whom property is put
 * prop: property name
 * val: value of the property to put
 *
 * Returns: None
 */
EXPORT_C void ani_object_put(aobject* obj, astring* prop, avalue* val);

/**
 * This function creates a new array object, which will be
 * collected by GC automatically.
 * 
 * Parameters: None
 *
 * Returns: the new created array object
 */
EXPORT_C aobject* ani_array_new(void);

/**
 * This function adds an element to the array object.
 * 
 * Parameters:
 * arrayObj: array object to whom element is added
 * val: value of the new element
 *
 * Returns: None
 */
EXPORT_C void ani_array_push(aobject* arrayObj, avalue* val);

/**
 * This function gets an element from the array object.
 * 
 * Parameters:
 * arrayObj: array object from whom element is obtained
 * index: index of the element
 * res: value of the element
 *
 * Returns: None
 */
EXPORT_C void ani_array_get(aobject* arrayObj, aint index, avalue* res);

/*
 * This function gets the length of array object.
 *
 * Parameters: 
 * arrayObj: array object
 *
 * Returns: length of array
 */
EXPORT_C aint ani_array_length(aobject* arrayObj);

/**
 * This function initializes a value by setting it undefined.
 *
 * Parameters:
 * v: value to be initialized
 *
 * Returns: None
 */
EXPORT_C void ani_value_init(avalue* v);

/**
 * This function destroys a value.
 *
 * Parameters:
 * v: value to be destroyed
 *
 * Returns: None
 */
EXPORT_C void ani_value_destroy(avalue* v);

/**
 * This function can call an ActionScript function registered by
 * AS method ExternalInterface.addCallback.
 *
 * Parameters:
 * funcName: registered AS function name
 * argc: number of arguments to pass to the as function
 * argv: arguments to pass to the as function
 * res: return value of the as function
 * 
 * Returns: ani_true, success; ani_false, errors.
 */
EXPORT_C aboolean ani_callASFunc(achar* funcName, aint argc, avalue** argv, avalue* res);

/** 
 * This function registers a native C function, which
 * can be called in ActionScript if registered successfully.
 *
 * Parameters:
 * funcName: registered name to be used in AS
 * nativeFunc: native function to be registered
 * 
 * Returns: ani_true, success; ani_false, errors.
 */
EXPORT_C aboolean ani_registerNative(achar* funcName, ani_native_function_t nativeFunc);
/** 
* This function return the error reason
*
* Parameters:
*   NA
* Returns: ani_true, amuse working now; ani_false, amuse not init or destory.
*/
EXPORT_C int ani_get_last_error(void);

#ifdef __cplusplus
}
#endif

#endif
