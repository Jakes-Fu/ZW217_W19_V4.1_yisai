/*
 * FileName: mal_phone_context.h
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.11.3
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

#ifndef MAL_PHONE_CONTEXT_H
#define MAL_PHONE_CONTEXT_H


typedef struct
{
	unsigned int		m_call_ready;
	unsigned int		m_sim_ready;
	unsigned int		m_signal_level;
	unsigned int 		m_bat_level;
	unsigned int 		m_rssi_level;
	unsigned int 		m_start_type; 
	unsigned int 		m_sim_status;
	unsigned char		m_charge_status;
	unsigned int		m_network_supported;
	unsigned int		m_phone_locked;
	
}MAL_PHONE_CONTEXT;


int MAL_Phone_Context_Init(void);


int MAL_Phone_Context_Destroy(void);


#endif // MAL_PHONE_CONTEXT_H

