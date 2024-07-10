#ifndef JERRY_PORT_H
#define JERRY_PORT_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* typedefs in ual_type.h
 * ToDo: remove this part if ual_type has fix the undef build error.
 */
 /*
#ifndef uint32
typedef unsigned long int uint32;
#endif
#ifndef int32
typedef signed long int int32;
#endif
*/
double jerry_port_get_current_time(void);
void jerry_port_sleep (uint32_t sleep_time);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* JERRY_PORT_H */