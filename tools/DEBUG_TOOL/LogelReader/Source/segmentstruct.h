//磁盘首结构，占用1个sect大小，即512字节

//用于表示磁盘当前被当作log记录工具使用，放在第一个扇区，该扇区在磁盘格式化时会被删除

//little endian


typedef struct	//该段只在格式化时写入固定信息，之后不会改变
{
	BYTE	OEMname[16];		//eg."SPREADTRUM" ，该标志也用于判断磁盘当前是否被用作存储器
	DWORD	segmentStartPos;	//第一个段的首sect位置，该位置之前的扇区保留，之后的区域被划分成一个个的段区域
	DWORD	segmentSize;		//磁盘存储的段大小，该大小必须足够存下整个assert信息，同时又不能过大，建议64M
	DWORD	segmentCount;		//有效段的总个数
	BYTE	reserved[512-28];	//保留字段
}DISKINFO_T;

typedef struct
{
//sect 1	//该sector只在该段启用时写入一次信息
	DWORD	groupFlag;		//0xFFFFFFFF表示无效。磁盘的所有group号必须连续，顺序累加，另类视为无效
	DWORD	SN;			//当前段在整个group中的序号，如果整个盘的group是一样的，那么该序号可以用来判断log首位置
	DWORD	dataStartPos;		//该段中log数据起始sector位置
	BYTE	reserved1[512-12];	//保留字段;
//sect 2	//该sector用于记录实际数据流长度，可能被多次写入
	DWORD	dataSize;		//该段有效数据流长度；如果当前段是assert段类型 ，那么该长度是所有assert小段长度之和
	BYTE	reserved2[512-4];	//保留字段
}SEGMENTINFO_T;

//--------------------------------