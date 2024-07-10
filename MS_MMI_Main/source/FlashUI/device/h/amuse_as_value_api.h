#ifndef AMUSE_AS_VALUE_API_H
#define AMUSE_AS_VALUE_API_H

#include "amuse_ani.h"

//可用数据类型
#define AS_VALUE_TYPE_BOOLEAN							0x0
#define AS_VALUE_TYPE_NUMBER_DOUBLE					0x10
#define AS_VALUE_TYPE_NUMBER_FLOAT					0x11
#define AS_VALUE_TYPE_NUMBER_CHAR					0x12
#define AS_VALUE_TYPE_NUMBER_UNSIGNED_CHAR			0x13
#define AS_VALUE_TYPE_NUMBER_SHORT					0x14
#define AS_VALUE_TYPE_NUMBER_UNSIGNED_SHORT		0x15
#define AS_VALUE_TYPE_NUMBER_INT						0x16
#define AS_VALUE_TYPE_NUMBER_UNSIGNED_INT			0x17
#define AS_VALUE_TYPE_NUMBER_LONG					0x18
#define AS_VALUE_TYPE_NUMBER_UNSIGNED_LONG			0x19
#define AS_VALUE_TYPE_STRING							0x2
#define AS_VALUE_TYPE_OBJECT								0x3


// 获取一个Value的值
int as_value_get_value(avalue *value, void *ret, int type);

//返回一个null
int as_value_set_null(avalue *res);

//返回一个bool
int as_value_set_boolean(avalue* res, unsigned char bool_val);

//返回一个类型为type，  值为*number  的数
int as_value_set_number(avalue* res, int type, void* number);

//返回一个string
int as_value_set_string(avalue* res, char* string);

//设定返回值类型是一个Array
int as_value_set_array(avalue* res);

//设定返回值类型是一个Object
int as_value_set_object(avalue* res);

//添加一个类型为type，  值为*data 的成员到Array
int as_value_push_member_array(avalue* res, int type, void* data);

//返回一个类型为type， 索引为index，  值为*data 的成员
int as_value_get_member_array(avalue* res, int index, void * data, int type);

//添加一个类型为type，名字为 member_name， 值为*data 的成员到Object
int as_value_push_member_object(avalue* res, int type, char* member_name, void* data);

// 从一个object里面获取一个名字为name类型为type的成员变量
int as_value_get_member_object(avalue *obj, char *name, void *ret, int type);



/*示例代码


//返回bool
unsigned char bool_val = 0;
....................................................................
...这里是实现具体功能的代码...
....................................................................
as_value_set_boolean(res, bool_val);



//返回number
int num = 12345;
....................................................................
...这里是实现具体功能的代码...
....................................................................
as_value_set_number(res, AS_VALUE_TYPE_NUMBER_INT, &num);



//返回string
char* string = "for test only";
....................................................................
...这里是实现具体功能的代码...
....................................................................
as_value_set_string(res, string);
注：如果string  是用malloc  得到的，在这里还要free  掉



//返回array
char* str_array[3] = {"string1", "string2", "string3"};
....................................................................
...这里是实现具体功能的代码...
....................................................................
as_value_set_array(res);
as_value_push_member_array(res, AS_VALUE_TYPE_STRING, &str_array[0]);
as_value_push_member_array(res, AS_VALUE_TYPE_STRING, &str_array[1]);
as_value_push_member_array(res, AS_VALUE_TYPE_STRING, &str_array[2]);	
注：如果str_array  是用malloc  得到的，在这里还要free  掉



//返回object
unsigned char bool_val = 0;
int num = 12345;
char* string = "for test only";
char* names[3] = {"bool", "number", "string"};
....................................................................
...这里是实现具体功能的代码...
....................................................................
as_value_set_object(res);
as_value_push_member_object(res, AS_VALUE_TYPE_BOOLEAN, names[0], &bool_val);
as_value_push_member_object(res, AS_VALUE_TYPE_NUMBER_INT, names[1], &num );
as_value_push_member_object(res, AS_VALUE_TYPE_STRING, names[2], &string);	
注：如果string  和names  是用malloc  得到的，在这里还要free  掉

*/
#endif

