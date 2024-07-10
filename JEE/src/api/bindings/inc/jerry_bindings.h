#ifndef __JERRY_BINDINGS_H__
#define __JERRY_BINDINGS_H__

#include "jerryscript.h"
#include "jerry_util.h"
#include "jerry_module.h"
//#include "jerry_port.h"

#define JERRY_STANDALONE_EXIT_CODE_OK   (0)
#define JERRY_STANDALONE_EXIT_CODE_FAIL (1)

#ifndef JERRY_MAX_READ_BUFFER_SIZE
#if defined(__NUTTX__) || defined(__TIZENRT__)
#define JERRY_MAX_READ_BUFFER_SIZE 1023
#define JERRY_MAX_PATH_SIZE 120
#else /* !__NUTTX__ && !__TIZENRT__ */
#define JERRY_MAX_READ_BUFFER_SIZE 65535
#define JERRY_MAX_PATH_SIZE 255
#endif /* __NUTTX__ || TIZENRT */
#endif /* JERRY_MAX_READ_BUFFER_SIZE */

/* for idl_binding selftest */
//#define IDL_BINDING_TEST_WITH_STUB

#define JERRY_CHECK(predicate)                           \
  if (!(predicate)) {                                 \
    return JERRY_CREATE_ERROR(COMMON, "Internal error"); \
  }

#define __JERRY_CHECK_TYPE(index, type) jerry_value_is_##type(args_p[index])

#define JERRY_CHECK_ARG(index, type) JERRY_CHECK(__JERRY_CHECK_TYPE(index, type))

// check with message
#define JERRY_CHECK_MSG(predicate, msg)     \
  if (!(predicate)) {                       \
    return JERRY_CREATE_ERROR(COMMON, msg); \
  }

#define __JERRY_CHECK_X_TYPE(x, type) jerry_value_is_##type(x)
#define JERRY_CHECK_TYPE_MSG(x, type, msg) JERRY_CHECK_MSG(__JERRY_CHECK_X_TYPE(x, type), msg)

#define JERRY_GET_ARG(index, type) jerry_jval_as_##type(args_p[index])

#define JERRY_GET_ARG_IF_EXIST(index, type)                  \
  ((args_cnt > index) && jerry_value_is_##type(args_p[index]) \
       ? args_p[index]                                     \
       : jerry_create_null())

#define JERRY_CHECK_ARG_IF_EXIST(index, type) \
  if (args_cnt > index) {                     \
    JERRY_CHECK(__JERRY_CHECK_TYPE(index, type)) \
  }

#define JERRY_CHECK_ARGS_0()

#define JERRY_CHECK_ARGS_1(type0) \
  JERRY_CHECK_ARGS_0()            \
  __JERRY_CHECK_TYPE(0, type0)

#define JERRY_CHECK_ARGS_2(type0, type1) \
  JERRY_CHECK_ARGS_1(type0)              \
  &&__JERRY_CHECK_TYPE(1, type1)

#define JERRY_CHECK_ARGS_3(type0, type1, type2) \
  JERRY_CHECK_ARGS_2(type0, type1)              \
  &&__JERRY_CHECK_TYPE(2, type2)

#define JERRY_CHECK_ARGS_4(type0, type1, type2, type3) \
  JERRY_CHECK_ARGS_3(type0, type1, type2)              \
  &&__JERRY_CHECK_TYPE(3, type3)

#define JERRY_CHECK_ARGS_5(type0, type1, type2, type3, type4) \
  JERRY_CHECK_ARGS_4(type0, type1, type2, type3)              \
  &&__JERRY_CHECK_TYPE(4, type4)

#define JERRY_CHECK_ARGS_6(type0, type1, type2, type3, type4, type5) \
  JERRY_CHECK_ARGS_5(type0, type1, type2, type3,type4)              \
  &&__JERRY_CHECK_TYPE(5, type5)

#define JERRY_CHECK_ARGS_7(type0, type1, type2, type3, type4, type5, type6) \
  JERRY_CHECK_ARGS_6(type0, type1, type2, type3,type4, type5)              \
  &&__JERRY_CHECK_TYPE(6, type6)

#define JERRY_CHECK_ARGS_8(type0, type1, type2, type3, type4, type5, type6, type7) \
  JERRY_CHECK_ARGS_7(type0, type1, type2, type3,type4, type5, type6)              \
  &&__JERRY_CHECK_TYPE(7, type7)

#define JERRY_CHECK_ARGS_9(type0, type1, type2, type3, type4, type5, type6, type7, type8) \
  JERRY_CHECK_ARGS_8(type0, type1, type2, type3,type4, type5, type6, type7)              \
  &&__JERRY_CHECK_TYPE(8, type8)

#define JERRY_CHECK_ARGS(argc, ...) \
  JERRY_CHECK(args_cnt >= argc && JERRY_CHECK_ARGS_##argc(__VA_ARGS__))

#define JERRY_DECLARE_PTR(JOBJ, TYPE, NAME)                              \
  TYPE* NAME = NULL;                                                  \
  do {                                                                \
    if (!jerry_get_object_native_pointer(JOBJ, (void**)&NAME,         \
                                         &this_module_native_info)) { \
      return JERRY_CREATE_ERROR(COMMON, "Internal");                     \
    }                                                                 \
  } while (0)

#define JERRY_DECLARE_THIS_PTR(type, name) \
  JERRY_DECLARE_PTR(jthis, jerry_##type##_t, name)

#define JERRY_DECLARE_OBJECT_PTR(index, type, name) \
  JERRY_DECLARE_PTR(args_p[index], jerry_##type##_t, name)

#define DECLARE_HANDLER(NAME) \
static jerry_value_t \
NAME ## _handler (const jerry_value_t  function_obj_val , \
                  const jerry_value_t  this_val , \
                  const jerry_value_t  args_p[], \
                  const jerry_length_t  args_cnt)

#define DECLARE_PUBLIC_HANDLER(NAME) \
jerry_value_t \
NAME ## _handler (const jerry_value_t  function_obj_val , \
                  const jerry_value_t  this_val , \
                  const jerry_value_t  args_p[], \
                  const jerry_length_t  args_cnt)

#define REGISTER_HANDLER(NAME) \
  register_native_function ( # NAME, NAME ## _handler)

#define REGISTER_GLOBAL_HANDLER(NAME)      \
    jerryx_handler_register_global ( (jerry_char_t *)# NAME, NAME ## _handler)


#define REGISTER_METHOD(OBJ, NAME)  \
    js_add_function (OBJ, # NAME, NAME ## _handler)

#ifdef __cplusplus
extern "C" {
#endif

	bool js_bindings_init(void);
  bool register_native_function(const char* name,
        jerry_external_handler_t handler);

#ifdef __cplusplus
}
#endif

#endif // __JERRY_BINDINGS_H__
