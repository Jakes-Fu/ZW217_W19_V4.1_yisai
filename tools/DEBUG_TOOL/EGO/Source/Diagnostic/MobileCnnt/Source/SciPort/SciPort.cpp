// SciPort.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Uart.h"

#pragma warning( disable : 4127)

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(ul_reason_for_call);
	UNREFERENCED_PARAMETER(lpReserved);
    return TRUE;
}

IPORT_API BOOL CreatePort( IPort ** pPort, PORT_TYPE PortType )
{
    switch( PortType )
    {
    case PORT_TYPE_COM:
		*pPort = new CUart;
        break;
    default:
        _ASSERTE( 0 );
    }

    return ( *pPort != NULL );
}