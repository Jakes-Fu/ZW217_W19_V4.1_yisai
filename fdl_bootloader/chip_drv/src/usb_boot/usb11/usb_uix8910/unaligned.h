#ifndef __UNALIGNED_H__
#define __UNALIGNED_H__

#include "sci_types.h"

#if 0
/* define __force to nothing in U-Boot */
#define __force

/*
 * Cause a link-time error if we try an unaligned access other than
 * 1,2,4 or 8 bytes long
 */
extern void __bad_unaligned_access_size(void);

#define __get_unaligned_le(ptr) ((__force typeof(*(ptr)))({			\
	__builtin_choose_expr(sizeof(*(ptr)) == 1, *(ptr),			\
	__builtin_choose_expr(sizeof(*(ptr)) == 2, get_unaligned_le16((ptr)),	\
	__builtin_choose_expr(sizeof(*(ptr)) == 4, get_unaligned_le32((ptr)),	\
	__builtin_choose_expr(sizeof(*(ptr)) == 8, get_unaligned_le64((ptr)),	\
	__bad_unaligned_access_size()))));					\
	}))

#else
#define __get_unaligned_le(ptr)		__get_unaligned_le16((uint8 *)(ptr))
#define __put_unaligned_le(val, ptr)	__put_unaligned_le16((val), (uint8 *)(ptr))
#endif

#define get_unaligned	__get_unaligned_le
#define put_unaligned	__put_unaligned_le

#if 0
#define __put_unaligned_le(val, ptr) ({					\
	void *__gu_p = (ptr);						\
	switch (sizeof(*(ptr))) {					\
	case 1:								\
		*(uint8 *)__gu_p = (__force uint8)(val);			\
		break;							\
	case 2:								\
		put_unaligned_le16((__force uint16)(val), __gu_p);		\
		break;							\
	case 4:								\
		put_unaligned_le32((__force uint32)(val), __gu_p);		\
		break;							\
	case 8:								\
		put_unaligned_le64((__force uint64)(val), __gu_p);		\
		break;							\
	default:							\
		__bad_unaligned_access_size();				\
		break;							\
	}								\
	(void)0; })
#endif

static uint16 __get_unaligned_le16(const uint8 *p)
{
	return p[0] | p[1] << 8;
}

static void __put_unaligned_le16(uint16 val, uint8 *p)
{
	*p++ = val;
	*p++ = val >> 8;
}

#if 0
static uint32 __get_unaligned_le32(const uint8 *p)
{
	return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}

static uint64 __get_unaligned_le64(const uint8 *p)
{
	return (uint64)__get_unaligned_le32(p + 4) << 32 |
	       __get_unaligned_le32(p);
}

static void __put_unaligned_le32(uint32 val, uint8 *p)
{
	__put_unaligned_le16(val >> 16, p + 2);
	__put_unaligned_le16(val, p);
}

static void __put_unaligned_le64(uint64 val, uint8 *p)
{
	__put_unaligned_le32(val >> 32, p + 4);
	__put_unaligned_le32(val, p);
}

static uint16 get_unaligned_le16(const void *p)
{
	return __get_unaligned_le16((const uint8 *)p);
}

static uint32 get_unaligned_le32(const void *p)
{
	return __get_unaligned_le32((const uint8 *)p);
}

static uint64 get_unaligned_le64(const void *p)
{
	return __get_unaligned_le64((const uint8 *)p);
}

static void put_unaligned_le16(uint16 val, void *p)
{
	__put_unaligned_le16(val, p);
}

static void put_unaligned_le32(uint32 val, void *p)
{
	__put_unaligned_le32(val, p);
}

static void put_unaligned_le64(uint64 val, void *p)
{
	__put_unaligned_le64(val, p);
}
#endif

#endif
