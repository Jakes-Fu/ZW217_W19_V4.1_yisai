#ifndef AMUSE_AS_VALUE_API_H
#define AMUSE_AS_VALUE_API_H

#include "amuse_ani.h"

//������������
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


// ��ȡһ��Value��ֵ
int as_value_get_value(avalue *value, void *ret, int type);

//����һ��null
int as_value_set_null(avalue *res);

//����һ��bool
int as_value_set_boolean(avalue* res, unsigned char bool_val);

//����һ������Ϊtype��  ֵΪ*number  ����
int as_value_set_number(avalue* res, int type, void* number);

//����һ��string
int as_value_set_string(avalue* res, char* string);

//�趨����ֵ������һ��Array
int as_value_set_array(avalue* res);

//�趨����ֵ������һ��Object
int as_value_set_object(avalue* res);

//���һ������Ϊtype��  ֵΪ*data �ĳ�Ա��Array
int as_value_push_member_array(avalue* res, int type, void* data);

//����һ������Ϊtype�� ����Ϊindex��  ֵΪ*data �ĳ�Ա
int as_value_get_member_array(avalue* res, int index, void * data, int type);

//���һ������Ϊtype������Ϊ member_name�� ֵΪ*data �ĳ�Ա��Object
int as_value_push_member_object(avalue* res, int type, char* member_name, void* data);

// ��һ��object�����ȡһ������Ϊname����Ϊtype�ĳ�Ա����
int as_value_get_member_object(avalue *obj, char *name, void *ret, int type);



/*ʾ������


//����bool
unsigned char bool_val = 0;
....................................................................
...������ʵ�־��幦�ܵĴ���...
....................................................................
as_value_set_boolean(res, bool_val);



//����number
int num = 12345;
....................................................................
...������ʵ�־��幦�ܵĴ���...
....................................................................
as_value_set_number(res, AS_VALUE_TYPE_NUMBER_INT, &num);



//����string
char* string = "for test only";
....................................................................
...������ʵ�־��幦�ܵĴ���...
....................................................................
as_value_set_string(res, string);
ע�����string  ����malloc  �õ��ģ������ﻹҪfree  ��



//����array
char* str_array[3] = {"string1", "string2", "string3"};
....................................................................
...������ʵ�־��幦�ܵĴ���...
....................................................................
as_value_set_array(res);
as_value_push_member_array(res, AS_VALUE_TYPE_STRING, &str_array[0]);
as_value_push_member_array(res, AS_VALUE_TYPE_STRING, &str_array[1]);
as_value_push_member_array(res, AS_VALUE_TYPE_STRING, &str_array[2]);	
ע�����str_array  ����malloc  �õ��ģ������ﻹҪfree  ��



//����object
unsigned char bool_val = 0;
int num = 12345;
char* string = "for test only";
char* names[3] = {"bool", "number", "string"};
....................................................................
...������ʵ�־��幦�ܵĴ���...
....................................................................
as_value_set_object(res);
as_value_push_member_object(res, AS_VALUE_TYPE_BOOLEAN, names[0], &bool_val);
as_value_push_member_object(res, AS_VALUE_TYPE_NUMBER_INT, names[1], &num );
as_value_push_member_object(res, AS_VALUE_TYPE_STRING, names[2], &string);	
ע�����string  ��names  ����malloc  �õ��ģ������ﻹҪfree  ��

*/
#endif

