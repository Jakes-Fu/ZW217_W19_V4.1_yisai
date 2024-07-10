typedef struct
{
	uint8 log_id;
	uint8 enable;
	uint32 time_len;
}LOG_INFO_T;

typedef struct
{
uint32	msg_id;
uint16	length;
uint8	log_num;
LOG_INFO_T	log_info;
}EM_MASK_LOG_REQ;

typedef struct
{
	uint16 ta;
	uint16 cell_id[32];
	uint16 uarfcn[32];
	uint8 cell_param_id[32];
	uint16 rscp[32];
	uint16 iscp[32];
	uint16 rssi[32];
}EM_L1_MEAS_T;

typedef struct
{
uint16	sir[6];
uint16	sir_target[6];
uint16	tx_power[6];
}EM_L1_MEAS_OTHER_T;

typedef struct
{
uint16	tx_power;
uint16	ul_power;
uint16	fpach_rscp;
uint16	rach_power;
}EM_L1_POWER_T;

typedef struct
{
uint16	fpach_pos;
uint16	fpach_delta;
uint16	pathloss;
}EM_L1_DELTA_T;

typedef struct
{
uint16	ts_flag;
uint16	mak;
uint16	mak_mode;
uint16	ma_shiftnum;
uint16	ma_shift_index[16];
}EM_L1_MA_T;

typedef struct
{
uint16	bler[16];
uint16	trch_id[16];
uint16	bad_crc[16];
uint16	total_crc[16];
uint8	amr_type[16];
}EM_L1_BLER_T;

typedef struct
{
uint16	mcc;
uint16	mnc;
uint8	att;
uint16	lac;
uint8	cellIdentifier[4];
uint8	drxParameter[2];
uint16	uarfcn;
uint16	t3212Len;
}EM_MM_PARAM_T;

typedef struct
{
uint16	mcc;
uint16	rac;
uint16	nmo;
uint16	t3302Len;
uint16	t3312Len;
}EM_GMM_PARAM_T;

typedef struct
{
uint32	send_pack_num;
uint32	rcv_pack_num;
uint32	send_byte_num;
uint32	rcv_byte_num;
uint32	crc_error;
}EM_MAC_STAT_INFO_T;

typedef struct
{
uint8	rb_count;
uint32	rb_id;
uint32	send_pack_num;
uint32	rcv_pack_num;
uint32	send_byte_num;
uint32	rcv_byte_num;
uint32	retrans_num;
uint32	retrans_req_num;
}EM_RLC_STAT_INFO_T;

typedef struct
{
uint32	send_pack_num;
uint32	rcv_pack_num;
uint32	send_byte_num;
uint32	rcv_byte_num;
}EM_PDCP_STAT_INFO_T;

typedef struct 
{ 
    uint32    TMSI; 
    uint32    PTMSI; 
    uint8     c_IMSI; // 用于提示有效长度。有效值为15      当其为0xff时无效 
    uint8     IMSI[15]; 
    uint8     c_IMEI; // 用于提示有效长度。有效值为15或17   当其为0xff时无效 
    uint8     IMEI[17]; 
    uint8     c_IMEISV; // 用于提示有效长度。有效值为17    当其为0xff时无效 
    uint8     IMEISV[17]; 
}EM_MOBILE_IDENTITY_T;

typedef struct 
{ 
uint32	freq;
uint8	cell_num;
uint16	cell_id[3];
}FREQ_INFO_T;
typedef struct 
{ 
uint32	msg_id;
uint16	length;
uint8	freq_num;
FREQ_INFO_T	freq_info[4];
}EM_LOCK_FREQ_REQ;

typedef enum
{
IDLE = 1,
CELL_FACH,
CELL_DCH,
CELL_PCH,
URA_PCH,
RRC_STATE_MAX
}EM_RRC_STATE_E;

typedef struct
{
uint16	uarfcn;
uint8	param_id;
}EM_CELL_IDENTIFIER_T;

typedef struct
{
EM_CELL_IDENTIFIER_T	scell_id;
uint8	scell_Srxlev;
uint8	scell_Qmeas;
uint8	scell_Qrxlevmin;
uint8	scell_Sintrasearch;
uint8	scell_Sintersearch;
uint8	scell_Cell_Barred;
uint8	HCS_is_supported;
uint8	hs;
uint16	scell_HCS_PRIO;
uint8	scell_Qhcs;
uint8	rs;
uint8	scell_Qhysts;
}EM_SCELL_RESEL_PARAM_T;

typedef struct
{
EM_CELL_IDENTIFIER_T	ncell_id;
uint8	ncell_Qmeas;
uint8	HCS_is_supported;
uint8	hn;
uint16	ncell_HCS_PRIO;
uint8	ncell_Qhcs;
uint8	rn;
uint8	s_n_offset;
}EM_NCELL_RESEL_PARAM_T;

typedef struct
{
uint32	resel_num;
}EM_RESEL_STATISTIC_INFO_T;