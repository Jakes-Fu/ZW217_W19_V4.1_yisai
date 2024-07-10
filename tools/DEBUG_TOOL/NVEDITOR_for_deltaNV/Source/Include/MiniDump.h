
// ------------------------------------------------------------------------------------------------
// Summary: 
//	 Windowsת���ļ�dmp�����ɺ�ʹ�á�������Ա�������������б��������ж�λ����ѭ�������ж�λ��
// Remarks:
//	 ��Ӧ�ó�����ڵ㴦����˺�������, ����:
// 	 Sample:
// 		<code>
// 			WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) ��
// 			int main(int argc, char **argv)
// 			{
// 				::SetUnhandledExceptionFilter(MyUnhandledFilter);
// 
// 				int *p;
// 				*p = 100;
// 				return 0;
// 			}
// 		</code>
//
// ------------------------------------------------------------------------------------------------

#include <stdio.h>		// ��̨�����C����ʹ��
#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")
void CreateDumpFile(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	TCHAR szDmpFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
	SYSTEMTIME st;
	::GetLocalTime(&st);
	
	// ע�͵�ԭ����д��, (ֻ���ļ�����,û��·��,�㲻����浽����ȥ��)
	// wsprintf(szFileName, TEXT("%04d-%02d-%02d-%02d-%02d-%02d-%02d-%02d.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, rand()%100);
	
	// ��Ϊ����Ŀ¼
	_stprintf(szFileName, _T("%04d-%02d-%02d-%02d-%02d-%02d-%02d-%02d.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, rand()%100);
	
	::GetModuleFileName( NULL, szDmpFile, _MAX_PATH );
    LPTSTR pExt = _tcsrchr(szDmpFile, _T('.'));
    _tcscpy(pExt + 1, szFileName);
	
	
	// ��ʵ���������ﷴ��������Ϣ,���籣���û������װλ�û������Զ����͵ȴ���
	
	HANDLE hFile = ::CreateFile(szDmpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;
		
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = lpExceptionInfo;
		ExInfo.ClientPointers = false;
		
		// write the dump
		
		BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
		
		if (bOK)
		{
			printf("Create Dump File Success!\n");
		}
		else
		{
			printf("MiniDumpWriteDump Failed: %d\n", GetLastError());
		}
		
		::CloseHandle(hFile);
	}
	else
	{
		printf("Create File %s Failed %d\n", szFileName, GetLastError());
	}
}
LONG WINAPI MyUnhandledFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	LONG ret = EXCEPTION_EXECUTE_HANDLER;

	CreateDumpFile(lpExceptionInfo);
	//AfxGetMainWnd()->SendMessage(WM_CLOSE,0,0);
	return ret;
}
LONG WINAPI FriendlyHandleException(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	LONG ret = EXCEPTION_EXECUTE_HANDLER;
	
	CreateDumpFile(lpExceptionInfo);
	AfxGetMainWnd()->SendMessage(WM_CLOSE,0,0);
	return ret;
}



// ------------------------------------------------------------------------------------------------
// Summary: ʹ��˵��
//	 
// ��������:
//		Debug	-	���԰�ֱ�ӱ��롣
//		Release	-	���а��޸������ط���
//			1��C/C++ - ���� - ������Ϣ��ʽ:ʹ�ó������ݿ�(/Zi)
//			2��C/C++ - ���� - �Ż� ����
//			3��C/C++ - ���� - ���ɵ�����Ϣ:��(/DEBUG)
//	
// ���������ж�λ:
//	 �����������C�̸�Ŀ¼������dmp�ļ�(��Ĭ������´��ļ������%systemroot%��)��vs�ô���Ŀ���������ķ�ʽ��dmp�ļ���F5���ɶ�λ������������
//
// Note:
//	 �õ��û���dmp�ļ���ͨ�������漸������Ҫע��:
// 	 1��Dump�ļ��������
// 			Dump�ļ����÷�Ҫ��������������λ�ã�����ȫ���Խ���һ���µ��ļ������������������Ǵ���ڱ��������λ�ã���Ҫ���������ɵ�PDB�ļ�������Dump�ļ�Ŀ¼����������VS2005��Dump�ļ�������PDB�ļ�·����
//		������Tool-Options-Debugging-Symbols	�Ҳ�"Cache Symbols from symbol servers to this directory"�༭��ָ��PDB�ļ�����λ��,����Ҫϵͳ��pdb���http://msdl.microsoft.com/download/symbols��ѡ���ϼ���
// 
// 	 2����λָ���ʱ���ֳ���
// 			������Ҫ�ʣ���ô���ܣ����dump�ļ������û������ҵģ��Ҳ�����ȥ�û�������԰ɣ�����ָ��ֳ��ɲ���ָ�ķ�Ҫ����̨������ȥ������Ҫ�Ѳ���dump�ļ���Ӧ�Ķ������ļ��õ���
// 		���ǻָ��ֳ���Ҫ���еĶ������ļ���Ҫ��Ӧ����һ��Ҫ�е����û���������ЩExe��DLL����Ȼ���㷢���ĳ���Exe�ļ���Ȼ����С���������ֻ����DLL�����ˡ�
// 		Dump�ļ��м�¼������DLL�ļ��İ汾�ź�ʱ�����������һ������ͬ��ĳ��;���õ�����������ܴ��û������õ���ã���������㣬�û��������õ�������ƽ�������õĲ���ϵͳ�������Ҹ��ж�Ӧϵͳ�Ļ���һ�㶼���С����Ǽ�ס�������ļ�����Ҫһ�£���Ҫ�˶԰汾��ʱ����������ͬһ��û�а취�á�
// 			����ͻ�����ĳ������Ĳ�����ô�죿
// 		��ʵ�������Ҳ�ܺý����ֻҪ�����谭�Ķ���ջ���Ͳ��ù���������Dump�����г���һ����ȱ��һ����DLLû���κ����⡣
// 
// 	 3����������Ҫ��ô�죿
// 			�����ļ�������Ҫ��ָPDB�ļ���
// 		���û�з����ļ�����ô���Ե�ʱ����ܵ��¶�ջ����
// 		����㶪ʧ����������汾��������������Щexe��DLL��PDB����ô�����ʧ�����صģ����±�������İ汾�ǲ���ʹ�õġ�
// 		���Լ���DLL�����ˣ�����ȱ����ϵͳ��DLL�Ķ�ӦPDB�ļ���ô�죿ͼ1.4���Ѿ������˷�����΢�������ķ������ݿ���Ϊ�����ṩ�����е�PDB�ļ������в��ַǹؼ�DLL�����úú�����Զ�������Ҫ��PDB��DLL�ļ���
// 
// 	 4���õ���Ҫ���ļ��ˣ���Щ�ļ�Ӧ�÷������
// 			�������ݿ��е��ļ����ö�����������exe��DLL��PDB�ļ�����dump�ļ�Ŀ¼������ˡ�
// 
// 	 5�����õ���VS2005��������Դ���룬Ϊʲô��ʾ���ˣ�
// 			�����dump���Ե���ͷʹ���⣬��������Ѿ��Ĺ��ˣ���ʹ���SVN�õ���ʱ�İ汾��ʱ���Ҳ�Ǵ�ģ�VS2005���ǲ�������ʾ���롣��ʵֻҪ��
// 		Tools\Options��Debugging\General��ȥ�� Require source files to exactly match the original version �ĸ�ѡ������
//	
//	 6��autoCAD����ʱ��ת���ļ����ĸ�?
//			autoCAD�ڽ�����ת���ļ����ƣ����Զ�����ת���ļ���ֻҪ���ú��ʵĹ������ü��ɡ�����ָ��Ƕ����ļ�
//			acminidump.dmp		-	%Temp%������������ʾ���ļ�λ�ã�һ����C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp
//			acadminidump.dmp	-	C:\\acadminidump.dmp
// ------------------------------------------------------------------------------------------------
