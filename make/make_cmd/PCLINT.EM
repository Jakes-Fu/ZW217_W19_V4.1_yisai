macro Pclint_Issue_Go()
{
    hwnd = GetCurrentWnd()
    if (hwnd == 0)
      stop
      
    hBuf = GetWndBuf(hwnd)
    szpathName = GetBufName(hBuf)
    str_index = strstr(szpathName,"\\MS_Code\\",)
    path_start = strmid(szpathName,0,str_index+9)

    szLineNO = GetWndSelLnFirst (hwnd)
    hfilebuf = GetCurrentBuf ()
    if (hfilebuf == 0)
          stop
    szLinecontent = GetBufLine (hfilebuf, szLineNO)

    str_index = strstr(szLinecontent,"line = \"")
    if(str_index>2)
    {
    	path_end = strmid(szLinecontent,14,str_index-2)
    	FullPath=cat(path_start, path_end)

    	line_start = strstr(szLinecontent,"line = \"")
    	line_end  = strstr(szLinecontent,"\" number = \"")
    	line_number = strmid(szLinecontent,line_start+8,line_end)
    	hfile = OpenMiscFile(FullPath)
    	if(hfile == 0)
              stop   	
    	hwnd = GetCurrentWnd()
    	if (hwnd == 0)
             stop
       ScrollWndToLine (hwnd, line_number-1)
    }
}

macro strstr(srcStr,desStr)
{
    destLength = strlen(desStr)
    srcLength = strlen(srcStr)
    uIndex = 0;
    loop = 0
    while (loop < destLength)
    {
	if( desStr[loop] == srcStr[uIndex] )
 	{
     		loop = loop + 1
     		uIndex = uIndex + 1;
   	}
   	else
   	{
  		uIndex = uIndex + 1;   
  		loop = 0;
  		if( ( uIndex + destLength) > srcLength )
  		{
   			return invalid
  		}
   	}
    }

    uIndex = uIndex - destLength;
 
    return uIndex
}
