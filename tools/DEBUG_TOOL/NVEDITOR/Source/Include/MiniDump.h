
// ------------------------------------------------------------------------------------------------
// Summary: 
//	 Windows转储文件dmp的生成和使用。开发人员可以拿来来进行崩溃代码行定位或死循环代码行定位。
// Remarks:
//	 在应用程序入口点处加入此函数调用, 例如:
// 	 Sample:
// 		<code>
// 			WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) 或
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

#include <stdio.h>		// 控台程序或C程序使用
#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")
void CreateDumpFile(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	TCHAR szDmpFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
	SYSTEMTIME st;
	::GetLocalTime(&st);
	
	// 注释掉原来的写法, (只有文件名称,没有路径,搞不清楚存到哪里去了)
	// wsprintf(szFileName, TEXT("%04d-%02d-%02d-%02d-%02d-%02d-%02d-%02d.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, rand()%100);
	
	// 改为保存目录
	_stprintf(szFileName, _T("%04d-%02d-%02d-%02d-%02d-%02d-%02d-%02d.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, rand()%100);
	
	::GetModuleFileName( NULL, szDmpFile, _MAX_PATH );
    LPTSTR pExt = _tcsrchr(szDmpFile, _T('.'));
    _tcscpy(pExt + 1, szFileName);
	
	
	// 其实可以在这里反馈更多信息,比如保存用户软件安装位置或网络自动发送等代码
	
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
// Summary: 使用说明
//	 
// 工程设置:
//		Debug	-	调试版直接编译。
//		Release	-	发行版修改三个地方。
//			1、C/C++ - 常规 - 调试信息格式:使用程序数据库(/Zi)
//			2、C/C++ - 常规 - 优化 禁用
//			3、C/C++ - 调试 - 生成调试信息:是(/DEBUG)
//	
// 崩溃代码行定位:
//	 程序崩溃后，在C盘根目录下生成dmp文件(在默认情况下此文件存放在%systemroot%下)，vs用打开项目或解决方案的方式打开dmp文件，F5即可定位到崩溃代码行
//
// Note:
//	 拿到用户的dmp文件后，通常有下面几个问题要注意:
// 	 1、Dump文件放在哪里？
// 			Dump文件不用非要放在你编译出来的位置，你完全可以建立一个新的文件夹来放它。但若不是存放在编译出来的位置，需要将编译生成的PDB文件拷贝到Dump文件目录，或是利用VS2005打开Dump文件后，设置PDB文件路径。
//		方法：Tool-Options-Debugging-Symbols	右侧"Cache Symbols from symbol servers to this directory"编辑框指定PDB文件所在位置,若需要系统的pdb则把http://msdl.microsoft.com/download/symbols复选框勾上即可
// 
// 	 2、如何恢复当时的现场？
// 			可能你要问，怎么可能，这个dump文件可是用户发给我的，我不可能去用户家里调试吧？这个恢复现场可不是指的非要到那台机器上去，而是要把产生dump文件对应的二进制文件拿到。
// 		但是恢复现场需要所有的二进制文件都要对应，你一定要有导致用户崩溃的那些Exe和DLL。既然是你发布的程序，Exe文件当然你会有。所以这里只考虑DLL就行了。
// 		Dump文件中记录了所有DLL文件的版本号和时间戳，所以你一定可以同过某种途径拿到它。如果你能从用户那里拿到最好，如果不方便，用户不可能用的是我们平常不常用的操作系统，所以找个有对应系统的机器一般都会有。但是记住不仅是文件名称要一致，还要核对版本和时间戳，如果不同一样没有办法用。
// 			如果客户用了某个特殊的补丁怎么办？
// 		其实这个问题也很好解决，只要它不阻碍阅读堆栈，就不用管它，调试Dump和运行程序不一样，缺少一两个DLL没有任何问题。
// 
// 	 3、如果真的需要怎么办？
// 			符号文件现在主要是指PDB文件。
// 		如果没有符号文件，那么调试的时候可能导致堆栈错误。
// 		如果你丢失了这个发布版本中你编译出来的那些exe和DLL的PDB，那么这个损失是严重的，重新编译出来的版本是不能使用的。
// 		我自己的DLL都有了，可是缺的是系统的DLL的对应PDB文件怎么办？图1.4中已经介绍了方法。微软在它的符号数据库上为我们提供了所有的PDB文件，还有部分非关键DLL。设置好后程序将自动下载需要的PDB及DLL文件。
// 
// 	 4、拿到需要的文件了，这些文件应该放在哪里？
// 			符号数据库中的文件不用动，把其它的exe和DLL、PDB文件放在dump文件目录里就行了。
// 
// 	 5、我用的是VS2005，明明有源代码，为什么显示不了？
// 			这个是dump调试的最头痛问题，代码可能已经改过了，即使你从SVN拿到当时的版本，时间戳也是错的，VS2005就是不让你显示代码。其实只要在
// 		Tools\Options，Debugging\General中去掉 Require source files to exactly match the original version 的复选就行了
//	
//	 6、autoCAD崩溃时的转储文件是哪个?
//			autoCAD内建的有转储文件机制，会自动生成转储文件，只要设置合适的工程设置即可。不用指定嵌入此文件
//			acminidump.dmp		-	%Temp%环境变量所表示的文件位置，一般在C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp
//			acadminidump.dmp	-	C:\\acadminidump.dmp
// ------------------------------------------------------------------------------------------------
