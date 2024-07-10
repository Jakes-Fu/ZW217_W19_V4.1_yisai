/*
* amuse_ani_type.h
* Author: Tim <tim@Microrapid.com>
* 
* This file contains basic type definitions of 
* Amuse ANI(ActionScript Native Interface).
* 
*/

#ifndef _AMUSE_ANI_TYPE_H_
#define _AMUSE_ANI_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if __WINSCW__ || __GCCE__
#include <_ansi.h>
#else
#define IMPORT_C
#define EXPORT_C
#endif

typedef char achar;
typedef char aboolean;
typedef int aint;

#if ADI
typedef long double anumber;
#else
typedef double anumber;
#endif

/**
 * You must make sure astring contains UTF-8 string data when you
 * pass astring to AS, because AS only accept UTF-8 string.
 */
struct astring{
	aint length;
	achar* data;
};
typedef struct astring astring;

struct aobject;
typedef struct aobject aobject;

#define ani_true  1
#define ani_false 0

enum atype{
	ANI_UNDEFINED,
	ANI_NULL,
	ANI_BOOLEAN,
	ANI_NUMBER,
	ANI_STRING,
	ANI_OBJECT
};

struct avalue{
	enum atype type;
	union{
		aboolean b;
		anumber n;
		astring* s;
		aobject* o;
	}u;
};

typedef struct avalue avalue;

/* Obtain the value's type */
#define ANI_VALUE_GET_TYPE(v)	((v)->type)

/* This macro is not part of the public API and may change */
#define _ANI_VALUE_SET_TYPE(v, t)		\
	(v)->type = t

/* Fill a value storage with the undefined value */
#define ANI_SET_UNDEFINED(v)			\
	_ANI_VALUE_SET_TYPE(v, ANI_UNDEFINED)

/* Fill a value storage with the null value */
#define ANI_SET_NULL(v)				\
	_ANI_VALUE_SET_TYPE(v, ANI_NULL)

/* Fill a value storage with a boolean value */
#define ANI_SET_BOOLEAN(v, bb) 			\
	do {					\
	_ANI_VALUE_SET_TYPE(v, ANI_BOOLEAN);	\
	(v)->u.b = (bb);			\
	} while (0)

/* Fill a value storage with a number(double) value */
#define ANI_SET_NUMBER(v, nn) 			\
	do {					\
	_ANI_VALUE_SET_TYPE(v, ANI_NUMBER);	\
	(v)->u.n = (nn);			\
	} while (0)

/* Fill a value storage with a string value */
#define ANI_SET_STRING(v, ss) 			\
	do {					\
	_ANI_VALUE_SET_TYPE(v, ANI_STRING);	\
	(v)->u.s = (ss);			\
	} while (0)

/* Fill a value storage with a object value */
#define ANI_SET_OBJECT(v, oo) 			\
	do {					\
	_ANI_VALUE_SET_TYPE(v, ANI_OBJECT);	\
	(v)->u.o = (oo);			\
	} while (0)

typedef void (*ani_native_function_t)(aint argc, avalue** argv, avalue* res);

#ifdef __cplusplus
}
#endif

#endif
