class CMediaTool : CEncoder
{
public:
	CMediaTool();
	~CMediaTool();
	virtual int Run (int action, T_ITEM* item) { return 0; };
	CString result;
protected:
	void* handle;
};

class CMediaInfo : public CMediaTool
{
public:
	int Run(int action, T_ITEM* item);
	TCHAR* GetName() { return "MediaInfo"; }
};

class CShibatch : public CMediaTool
{
	int Run(int action, T_ITEM* item);
	TCHAR* GetName() { return "Shibatch Sample Rate Converter"; }
};

class CMPParser : public CMediaTool
{
public:
	int Run(int action, T_ITEM* item);
	TCHAR* GetName() { return "MPlayer Parser"; }
};
