// SciPort.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ComPort.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	UNREFERENCED_PARAMETER(lpReserved);
	UNREFERENCED_PARAMETER(ul_reason_for_call);
	UNREFERENCED_PARAMETER(hModule);
    return TRUE;
}

IPORT_API BOOL CreatePort( IPort ** pPort, PORT_TYPE PortType )
{
    switch( PortType )
    {
    case PORT_TYPE_COM:
        *pPort = new CCOMPort;
        break;
    default:
//        _ASSERTE( 0 );
		break;
    }

    return ( *pPort != NULL );
}