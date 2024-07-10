class CFileName 
{
public:
	CString FileName;
	void GetName(CString& result);
	void GetDir(CString& result);
	void GetExtName(CString& result);
	void SetExtName(TCHAR* pszExt);
	void RemovePathName();
	void RemoveExtName();
	void RemoveDriveLetter();
	void operator=( CString& str );
	void operator=( TCHAR* pszData );
};
