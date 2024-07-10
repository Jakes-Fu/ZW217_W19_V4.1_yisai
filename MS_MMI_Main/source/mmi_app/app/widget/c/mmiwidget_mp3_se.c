#if defined MMIWIDGET_MP3_SE 
#include "graphics_3d_rotate.h"
#include "mmiwidget_mp3_se.h"

#ifdef USE_QVGA_TYPE
#define OFFSET_BLENDING_VS_S2D_4_IN_ONE 1
#define PLAYER_PLAYPERCENTAGE_X_OFFSET 5 //进度条图片实际进度起始点
#define PLAYER_PLAYPERCENTAGE_MAXPIXEL 100 //进度条图片实际进度总长度
#define PLAYER_SLIDER_DRAW_X_MAX 400
#define PLAYER_SLIDER_DRAW_Y_MAX 1
#define PLAYER_SLIDER_X 55
#define PLAYER_SLIDER_X_LIGHT_OFST 8
#define PLAYER_SLIDER_X_LIGHT_OFF 10
#define PLAYER_SLIDER_Y 160
#define PLAYER_ROT_MOVE_X_MAX 20
#define PLAYER_THICKNESS_Z 4
#define PLAYER_OFFSET_Y_CORRECT 40
#define PLAYER_OUTSODEU_ROTAXIS_Y 29 //上半部分播放器外框偏移rot的位置
#define PLAYER_OUTSODED_ROTAXIS_Y 0  //下半部分播放器外框偏移rot的位置
#define PLAYER_INSODE_ROTAXIS_Y 15  //播放器内部按钮部分偏移rot的位置
#define PLAYER_IN_OUT_ROTAXIS_OFF_X 20 //内部和外框图片x方向的偏移
#define GAP_IMG_2_PLAYER -4  //唱片图片和播放器之间的空隙大小
#define IMG_ROT_ANGLE_MAX 870  //唱片图的极限角度
#define IMG_SHOW_CDPIC_NUM_MAX 4 //同时显示的最大唱片图数量
#else
/*
#define PLAYER_ROT_MOVE_X_MAX 50
#define PLAYER_THICKNESS_Z 6
#define PLAYER_OFFSET_Y_CORRECT 30
#define PLAYER_OUTSODEU_ROTAXIS_Y 39
#define PLAYER_OUTSODED_ROTAXIS_Y 0
#define PLAYER_INSODE_ROTAXIS_Y 20
#define PLAYER_IN_OUT_ROTAXIS_OFF_X 28
#define GAP_IMG_2_PLAYER -4
#define IMG_ROT_ANGLE_MAX 870
#define IMG_SHOW_CDPIC_NUM_MAX 5
*/
#endif
#define DAMP_AMP 5/2

FIX16_POINT_T calc_Add_ptr[1024] = {0};

const static unsigned short usaTableSin[]=
{
	0,114,228,343,457,571,686,800,915,1029,
	1143,1258,1372,1486,1601,1715,1829,1944,2058,2172,
	2287,2401,2515,2630,2744,2858,2972,3087,3201,3315,
	3429,3544,3658,3772,3886,4000,4115,4229,4343,4457,
	4571,4685,4799,4913,5027,5141,5255,5369,5483,5597,
	5711,5825,5939,6053,6167,6281,6395,6509,6622,6736,
	6850,6964,7077,7191,7305,7418,7532,7646,7759,7873,
	7986,8100,8213,8327,8440,8554,8667,8780,8894,9007,
	9120,9234,9347,9460,9573,9686,9799,9913,10026,10139,
	10252,10365,10477,10590,10703,10816,10929,11042,11154,11267,
	11380,11492,11605,11717,11830,11942,12055,12167,12280,12392,
	12504,12617,12729,12841,12953,13065,13177,13289,13401,13513,
	13625,13737,13849,13961,14072,14184,14296,14407,14519,14630,
	14742,14853,14965,15076,15187,15299,15410,15521,15632,15743,
	15854,15965,16076,16187,16298,16408,16519,16630,16740,16851,
	16961,17072,17182,17293,17403,17513,17623,17734,17844,17954,
	18064,18174,18283,18393,18503,18613,18722,18832,18941,19051,
	19160,19270,19379,19488,19597,19707,19816,19925,20034,20142,
	20251,20360,20469,20577,20686,20794,20903,21011,21120,21228,
	21336,21444,21552,21660,21768,21876,21984,22091,22199,22307,
	22414,22522,22629,22736,22844,22951,23058,23165,23272,23379,
	23486,23592,23699,23806,23912,24019,24125,24231,24337,24444,
	24550,24656,24762,24868,24973,25079,25185,25290,25396,25501,
	25606,25712,25817,25922,26027,26132,26237,26342,26446,26551,
	26655,26760,26864,26969,27073,27177,27281,27385,27489,27593,
	27696,27800,27903,28007,28110,28213,28317,28420,28523,28626,
	28729,28831,28934,29037,29139,29242,29344,29446,29548,29650,
	29752,29854,29956,30058,30159,30261,30362,30463,30565,30666,
	30767,30868,30969,31069,31170,31271,31371,31471,31572,31672,
	31772,31872,31972,32072,32171,32271,32370,32470,32569,32668,
	32768,32867,32965,33064,33163,33262,33360,33458,33557,33655,
	33753,33851,33949,34047,34144,34242,34339,34437,34534,34631,
	34728,34825,34922,35019,35115,35212,35308,35405,35501,35597,
	35693,35789,35885,35980,36076,36171,36267,36362,36457,36552,
	36647,36742,36836,36931,37025,37119,37214,37308,37402,37496,
	37589,37683,37777,37870,37963,38056,38150,38242,38335,38428,
	38521,38613,38705,38798,38890,38982,39074,39165,39257,39349,
	39440,39531,39623,39714,39804,39895,39986,40077,40167,40257,
	40347,40438,40528,40617,40707,40797,40886,40975,41065,41154,
	41243,41331,41420,41509,41597,41686,41774,41862,41950,42038,
	42125,42213,42300,42388,42475,42562,42649,42735,42822,42909,
	42995,43081,43167,43253,43339,43425,43511,43596,43681,43767,
	43852,43937,44021,44106,44191,44275,44359,44443,44527,44611,
	44695,44779,44862,44945,45028,45112,45194,45277,45360,45442,
	45525,45607,45689,45771,45853,45934,46016,46097,46178,46259,
	46340,46421,46502,46582,46663,46743,46823,46903,46983,47063,
	47142,47222,47301,47380,47459,47538,47616,47695,47773,47851,
	47929,48007,48085,48163,48240,48318,48395,48472,48549,48626,
	48702,48779,48855,48931,49007,49083,49159,49234,49310,49385,
	49460,49535,49610,49685,49759,49833,49908,49982,50056,50129,
	50203,50276,50350,50423,50496,50569,50641,50714,50786,50858,
	50931,51002,51074,51146,51217,51289,51360,51431,51501,51572,
	51643,51713,51783,51853,51923,51993,52062,52132,52201,52270,
	52339,52408,52476,52545,52613,52681,52749,52817,52884,52952,
	53019,53086,53153,53220,53287,53353,53420,53486,53552,53618,
	53683,53749,53814,53880,53945,54009,54074,54139,54203,54267,
	54331,54395,54459,54522,54586,54649,54712,54775,54838,54900,
	54963,55025,55087,55149,55210,55272,55333,55395,55456,55516,
	55577,55638,55698,55758,55818,55878,55938,55997,56057,56116,
	56175,56234,56292,56351,56409,56467,56525,56583,56641,56698,
	56755,56812,56869,56926,56983,57039,57095,57151,57207,57263,
	57319,57374,57429,57484,57539,57594,57648,57702,57757,57811,
	57864,57918,57971,58025,58078,58131,58183,58236,58288,58340,
	58393,58444,58496,58547,58599,58650,58701,58752,58802,58853,
	58903,58953,59003,59052,59102,59151,59200,59249,59298,59347,
	59395,59444,59492,59539,59587,59635,59682,59729,59776,59823,
	59870,59916,59962,60008,60054,60100,60145,60191,60236,60281,
	60326,60370,60415,60459,60503,60547,60591,60634,60677,60720,
	60763,60806,60849,60891,60933,60975,61017,61059,61100,61142,
	61183,61224,61264,61305,61345,61385,61425,61465,61505,61544,
	61583,61622,61661,61700,61738,61776,61815,61852,61890,61928,
	61965,62002,62039,62076,62112,62149,62185,62221,62257,62292,
	62328,62363,62398,62433,62468,62502,62537,62571,62605,62638,
	62672,62705,62738,62771,62804,62837,62869,62901,62933,62965,
	62997,63028,63059,63090,63121,63152,63182,63213,63243,63273,
	63302,63332,63361,63390,63419,63448,63477,63505,63533,63561,
	63589,63616,63644,63671,63698,63725,63751,63778,63804,63830,
	63856,63881,63907,63932,63957,63982,64007,64031,64055,64080,
	64103,64127,64151,64174,64197,64220,64243,64265,64287,64309,
	64331,64353,64375,64396,64417,64438,64459,64479,64500,64520,
	64540,64560,64579,64599,64618,64637,64655,64674,64692,64711,
	64729,64746,64764,64781,64799,64816,64832,64849,64865,64882,
	64898,64914,64929,64945,64960,64975,64990,65004,65019,65033,
	65047,65061,65074,65088,65101,65114,65127,65140,65152,65164,
	65176,65188,65200,65211,65223,65234,65245,65255,65266,65276,
	65286,65296,65306,65315,65324,65333,65342,65351,65360,65368,
	65376,65384,65391,65399,65406,65413,65420,65427,65433,65440,
	65446,65452,65457,65463,65468,65473,65478,65483,65487,65491,
	65496,65499,65503,65507,65510,65513,65516,65519,65521,65523,
	65526,65527,65529,65531,65532,65533,65534,65535,65535,65535,65535
};

__inline int fastSin(int angle)
{
	if(900==angle)return 65536;
	else if(2700==angle)return -65536;
	else if((angle>=0)&&(angle<900))return usaTableSin[angle];
	else if((angle>900)&&(angle<=1800))return usaTableSin[1800-angle];
	else if((angle>1800)&&(angle<2700))return -usaTableSin[angle-1800];
	else if((angle>2700)&&(angle<=3600))return -usaTableSin[3600-angle];
	else if(angle < 0) 
	{
		return fastSin(angle+3600);
	}
	else 
	{
		return fastSin(angle%3600);
	}
}
__inline int fastCos(int angle)
{
	if(0==angle)return 65536;
	else if(1800==angle)return -65536;
	else if(3600==angle)return 65536;
	else if((angle>=0)&&(angle<900))return usaTableSin[900-angle];
	else if((angle>=900)&&(angle<1800))return -usaTableSin[angle-900];
	else if((angle>1800)&&(angle<2700))return -usaTableSin[2700-angle];
	else if((angle>=2700)&&(angle<3600))return usaTableSin[angle-2700];
	else if(angle < 0) 
	{
		return fastCos(angle+3600);
	}
	else 
	{
		return fastCos(angle%3600);
	}
}

LOCAL void ARGB888_2_PAF888(IMGREF_SIZE_T size,
					 uint32 *InBuf,
					 uint32 *OutBuf)
{
	register uint temp1 = 0;
	register uint temp2 = 0;
	register uint16 alpha = 0;
	uint32 i = 0;
	register uint32 *tempOutbuf = OutBuf;
	register uint32 *tempInbuf = InBuf;

	uint32 num = size.w*size.h;


	while(i<num)
	{
		temp1 = *tempInbuf;
		alpha = ((temp1>>24)&0xff);
		temp2 = temp1&0x00ff00ff;
		temp1 = temp1&0x0000ff00;
		temp2 = ((temp2*alpha)>>8)&0x00ff00ff;
		temp1 = ((temp1*alpha)>>8)&0x0000ff00;
		*tempOutbuf = temp1|temp2|((255-alpha)<<24);
		i++;
		tempOutbuf++;
		tempInbuf++;
	}
}

LOCAL void IMG_3D_MusicPlayer(	IMGREF_SIZE_T window,
						IMGREF_SIZE_T src_all_size,
						T_RECT src_rect,
						IMGREF_3D_POINT_T In_3D_ver[4],	//base 4
						uint32 *src_ptr,
						uint32 *window_ptr,
						uint32 camera_z)
{
	int a = 0;
	int i = 0;
	int y,x;
	FIX8 xStart,xEnd;
	FIX8 Hs = 0;
	FIX8 ySrc = 0;
	int yOut,xOut;
	register uint32 temp = 0;
	FIX8 dx = 0;
	POINT_2D_FIX8 ver[5];
	IMGREF_3D_POINT_T  eye;
	int temp_xEnd = 0;
	uint32 *In_Ptr = NULL;
	uint16 *Out_Ptr = NULL;
	POINT_2D_FIX8 sub_ver[4] = {0};
	
	eye.x = window.w/2;
	eye.y = window.h/2+60;
	eye.z = -camera_z;
	for( i = 0 ; i < 4 ; i ++)
	{
		s3d_Free_Point_change_POINT_2D_FIX8(In_3D_ver[i],eye,&ver[i]);
	}
	/*free_change_Interface(window,
						 src_all_size,
						 src_rect,
						 &ver[0],				//顶点数据表指针
						 window_ptr	,
						 src_ptr);*/
	/*free_change_Interface_Type(window,
								  src_all_size,
								  src_rect,
								  &ver[0],				//顶点数据表指针
								  window_ptr,
								  S3D_DATA_PAF888,
								  src_ptr,
								  S3D_DATA_PAF888,
								  NULL);*/
	sub_ver[0].x = FIX8_SET(0);
	sub_ver[0].y = FIX8_SET(0);

	sub_ver[1].x = FIX8_SET(src_rect.w-1);
	sub_ver[1].y = FIX8_SET(0);

	sub_ver[2].x = FIX8_SET(src_rect.w-1);
	sub_ver[2].y = FIX8_SET(src_rect.h-1);

	sub_ver[3].x = FIX8_SET(0);
	sub_ver[3].y = FIX8_SET(src_rect.h-1);
	S3D_All_Axis_Interface_Type(window,
								  src_all_size,
								  sub_ver,
								  &ver[0],
								  window_ptr,
								  S3D_DATA_PAF_RGB888,
								  src_ptr,
								  S3D_DATA_PAF_RGB888_NF_R,
								  NULL);



	/*
	for( i = 0 ; i < 4 ; i ++)
	{
		window_ptr[FIX8_INT(ver[i].y)*window.w+FIX8_INT(ver[i].x)] = 0xffff0000;
	}
	
	*/

}


LOCAL void TurnTableRotImg(
							IMGREF_SIZE_T window,
							uint32 *pColor,
							OBJECT_T* obj,
							int32 y_offset,
							IMGREF_3D_POINT_T* axis_PointL,
							IMGREF_3D_POINT_T* axis_PointR,
							IMGREF_3D_POINT_T* Rot_3d_axis,
							int32 xAxis_rot_angle,
							int32 yAxis_rot_angle)
{
	int32 calc_rds = 0,calc_angle = 0,calc_x = 0,calc_y = 0, calc_z = 0,i = 0;
	IMGREF_3D_POINT_T In_3D_ver[4] = {0};
	IMGREF_SIZE_T src_all_size = {0};
	T_RECT src_rect = {0};
	
	calc_rds = y_offset;
	calc_angle = xAxis_rot_angle;
i = 0;
	calc_y = -1*y_offset;
	calc_z = axis_PointL->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointL->x;
	//In_3D_ver[i].z = axis_PointL->z - ((calc_rds*fastSin(calc_angle))>>16);
	//In_3D_ver[i].y = axis_PointL->y-((calc_rds*fastCos(calc_angle))>>16);
	//In_3D_ver[i].x = axis_PointL->x;

	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;
i = 1;
	/*In_3D_ver[i].z = axis_PointR->z - ((calc_rds*fastSin(calc_angle))>>16);
	In_3D_ver[i].y = axis_PointR->y-((calc_rds*fastCos(calc_angle))>>16);
	In_3D_ver[i].x = axis_PointR->x;*/
	calc_y = -1*y_offset;
	calc_z = axis_PointR->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointR->x;
	
	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;

i = 3;
	/*calc_rds = obj->up_window.h-calc_rds-1;
	In_3D_ver[i].z = axis_PointL->z - ((calc_rds*fastSin(calc_angle+1800))>>16);
	In_3D_ver[i].y = axis_PointL->y-((calc_rds*fastCos(calc_angle+1800))>>16);
	In_3D_ver[i].x = axis_PointL->x;*/
	calc_y = obj->up_window.h-y_offset-1;
	calc_z = axis_PointL->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointL->x;
	
	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;

i = 2;
	/*In_3D_ver[i].z = axis_PointR->z - ((calc_rds*fastSin(calc_angle+1800))>>16);
	In_3D_ver[i].y = axis_PointR->y-((calc_rds*fastCos(calc_angle+1800))>>16);
	In_3D_ver[i].x = axis_PointR->x;*/
	calc_y = obj->up_window.h-y_offset-1;
	calc_z = axis_PointR->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointR->x;
	
	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;

	src_all_size.w = obj->up_window.w;
	src_all_size.h = obj->up_window.h;
	src_rect.w = src_all_size.w;
	src_rect.h = src_all_size.h;
	src_rect.x = 0;
	src_rect.y = 0;

	IMG_3D_MusicPlayer(window,
						src_all_size,
						src_rect,
						In_3D_ver,	//base 4
						obj->srcARGB_Map,
						pColor,
						window.h*2);	
}




LOCAL void IMG_3D_MusicPlayer_CtrlBar(	IMGREF_SIZE_T window,
						IMGREF_SIZE_T src_all_size,
						T_RECT src_rect,
						IMGREF_3D_POINT_T In_3D_ver[4],	//base 4
						uint32 *src_ptr,
						uint32 *window_ptr,
						uint32 camera_z)
{
	int a = 0;
	int i = 0;
	int y,x;
	FIX8 xStart,xEnd;
	FIX8 Hs = 0;
	FIX8 ySrc = 0;
	int yOut,xOut;
	register uint32 temp = 0;
	FIX8 dx = 0;
	POINT_2D_FIX8 ver[5];
	IMGREF_3D_POINT_T  eye;
	int temp_xEnd = 0;
	uint32 *In_Ptr = NULL;
	uint16 *Out_Ptr = NULL;
	S3D_ALLAXIS_EXP_T anty_alias_t={0};
	POINT_2D_FIX8 sub_ver[4] = {0};

	anty_alias_t.anti_type=ANTIALIAS_TYPE_TOTAL;
	eye.x = window.w/2;
	eye.y = window.h/2+60;
	eye.z = -camera_z;
	for( i = 0 ; i < 4 ; i ++)
	{
		s3d_Free_Point_change_POINT_2D_FIX8(In_3D_ver[i],eye,&ver[i]);
	}
	/*free_change_Interface(window,
						 src_all_size,
						 src_rect,
						 &ver[0],				//顶点数据表指针
						 window_ptr	,
						 src_ptr);*/
	/*free_change_Interface_Type(window,
								  src_all_size,
								  src_rect,
								  &ver[0],				//顶点数据表指针
								  window_ptr,
								  S3D_DATA_PAF888,
								  src_ptr,
								  S3D_DATA_PAF888,
								  NULL);*/
	sub_ver[0].x = FIX8_SET(0);
	sub_ver[0].y = FIX8_SET(0);

	sub_ver[1].x = FIX8_SET(src_rect.w-1);
	sub_ver[1].y = FIX8_SET(0);

	sub_ver[2].x = FIX8_SET(src_rect.w-1);
	sub_ver[2].y = FIX8_SET(src_rect.h-1);

	sub_ver[3].x = FIX8_SET(0);
	sub_ver[3].y = FIX8_SET(src_rect.h-1);
	S3D_All_Axis_Interface_Type(window,
								  src_all_size,
								  sub_ver,
								  &ver[0],
								  window_ptr,
								  S3D_DATA_PAF_RGB888,
								  src_ptr,
								  S3D_DATA_PAF_RGB888,
								  &anty_alias_t);



	/*
	for( i = 0 ; i < 4 ; i ++)
	{
		window_ptr[FIX8_INT(ver[i].y)*window.w+FIX8_INT(ver[i].x)] = 0xffff0000;
	}
	
	*/

}


LOCAL void TurnTableRotImg_CtrlBar(
							IMGREF_SIZE_T window,
							uint32 *pColor,
							OBJECT_T* obj,
							int32 y_offset,
							IMGREF_3D_POINT_T* axis_PointL,
							IMGREF_3D_POINT_T* axis_PointR,
							IMGREF_3D_POINT_T* Rot_3d_axis,
							int32 xAxis_rot_angle,
							int32 yAxis_rot_angle)
{
	int32 calc_rds = 0,calc_angle = 0,calc_x = 0,calc_y = 0, calc_z = 0,i = 0;
	IMGREF_3D_POINT_T In_3D_ver[4] = {0};
	IMGREF_SIZE_T src_all_size = {0};
	T_RECT src_rect = {0};
	
	calc_rds = y_offset;
	calc_angle = xAxis_rot_angle;
i = 0;
	calc_y = -1*y_offset;
	calc_z = axis_PointL->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointL->x;
	//In_3D_ver[i].z = axis_PointL->z - ((calc_rds*fastSin(calc_angle))>>16);
	//In_3D_ver[i].y = axis_PointL->y-((calc_rds*fastCos(calc_angle))>>16);
	//In_3D_ver[i].x = axis_PointL->x;

	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;
i = 1;
	/*In_3D_ver[i].z = axis_PointR->z - ((calc_rds*fastSin(calc_angle))>>16);
	In_3D_ver[i].y = axis_PointR->y-((calc_rds*fastCos(calc_angle))>>16);
	In_3D_ver[i].x = axis_PointR->x;*/
	calc_y = -1*y_offset;
	calc_z = axis_PointR->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointR->x;
	
	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;

i = 3;
	/*calc_rds = obj->up_window.h-calc_rds-1;
	In_3D_ver[i].z = axis_PointL->z - ((calc_rds*fastSin(calc_angle+1800))>>16);
	In_3D_ver[i].y = axis_PointL->y-((calc_rds*fastCos(calc_angle+1800))>>16);
	In_3D_ver[i].x = axis_PointL->x;*/
	calc_y = obj->up_window.h-y_offset-1;
	calc_z = axis_PointL->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointL->x;
	
	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;

i = 2;
	/*In_3D_ver[i].z = axis_PointR->z - ((calc_rds*fastSin(calc_angle+1800))>>16);
	In_3D_ver[i].y = axis_PointR->y-((calc_rds*fastCos(calc_angle+1800))>>16);
	In_3D_ver[i].x = axis_PointR->x;*/
	calc_y = obj->up_window.h-y_offset-1;
	calc_z = axis_PointR->z-Rot_3d_axis->z;
	In_3D_ver[i].z = ((calc_z*fastCos(calc_angle)+calc_y*fastSin(calc_angle))>>16)+Rot_3d_axis->z;
	In_3D_ver[i].y = (((-1)*calc_z*fastSin(calc_angle)+calc_y*fastCos(calc_angle))>>16)+Rot_3d_axis->y;
	In_3D_ver[i].x = axis_PointR->x;
	
	calc_x = In_3D_ver[i].x - Rot_3d_axis->x;
	calc_z = In_3D_ver[i].z - Rot_3d_axis->z;
	In_3D_ver[i].x = ((calc_x*fastCos(yAxis_rot_angle)+calc_z*fastSin(yAxis_rot_angle))>>16)+Rot_3d_axis->x;
	In_3D_ver[i].z = (((-1)*calc_x*fastSin(yAxis_rot_angle)+calc_z*fastCos(yAxis_rot_angle))>>16)+Rot_3d_axis->z;

	src_all_size.w = obj->up_window.w;
	src_all_size.h = obj->up_window.h;
	src_rect.w = src_all_size.w;
	src_rect.h = src_all_size.h;
	src_rect.x = 0;
	src_rect.y = 0;

	IMG_3D_MusicPlayer_CtrlBar(window,
						src_all_size,
						src_rect,
						In_3D_ver,	//base 4
						obj->srcARGB_Map,
						pColor,
						window.h*2);	
}


LOCAL int32 DampPow(int32 factor,int32 val, int32 pow)
{	
	while(pow>0)
	{
		val =  (val*factor)>>8;
		--pow;
	}
	return val;
}

LOCAL void WidgetMusicPlayer_PercentagePoint(IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res)
{
			int32 yh_st, yh_end, xh_st,xh_end;
			int32 y_st, y_end, x_st, x_end;
			FIX16_POINT_T cen_Poi = {0};
			int32 radius_inUse = 0;
			FIX16_POINT_T pos_inPic = {0};
			IMGREF_SIZE_T wh_in2D= {0};
			uint32 slider_color = 0xcf00fff0,slider_header_clr = 0, slider_body_clr = 0;
			IMGREF_SIZE_T cov_size = {1,1};
			int32 Breath_Max = 50;
			int32 Breath_Min = 0;
			int32 Breath_Sin_Min = 20;
			int32 Breath_Sin_Max = 100;
			//int32 Breath_Step = 3;
			static int32 s_Breath_Point = 0;
			static int32 s_Breath_Point_sign = 1;
			

			s_Breath_Point = s_Breath_Point+s_Breath_Point_sign;

			if(s_Breath_Point >= Breath_Max)
			{
				s_Breath_Point_sign = -1*s_Breath_Point_sign;
			}
			if(s_Breath_Point <= Breath_Min)
			{
				s_Breath_Point_sign = -1*s_Breath_Point_sign;
			}
//slider_color = 0xc0ff0000;
			
//slider_color = 0x3fc00000;
			cen_Poi.x = PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST+PLAYER_PLAYPERCENTAGE_X_OFFSET+PLAYER_PLAYPERCENTAGE_MAXPIXEL*res->PlayPercentage/100;//*res->PlayPercentage/100;
			cen_Poi.y = PLAYER_SLIDER_Y;	

/*			wh_in2D.w = PLAYER_SLIDER_DRAW_X_MAX*res->PlayPercentage/100;
			wh_in2D.h = PLAYER_SLIDER_DRAW_Y_MAX;

			pos_inPic.x = cen_Poi.x;
			pos_inPic.y = cen_Poi.y-wh_in2D.h/2;

			y_st = MAX(pos_inPic.y,0);
			y_end = MIN(pos_inPic.y+wh_in2D.h,window.h-1);
			x_st = MIN(pos_inPic.x,window.w-1);
			x_end = MAX(pos_inPic.x-wh_in2D.w,PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST+PLAYER_PLAYPERCENTAGE_X_OFFSET);
			slider_body_clr = slider_color;//(((slider_color>>24)*(s_Breath_Point-Breath_Min)/(Breath_Max-Breath_Min))<<24)+(slider_color&0x00ffffff);
			ARGB888_2_PAF888(cov_size,&slider_body_clr,&slider_body_clr);
			Linear_Body_R_PAF888(slider_body_clr, y_st, y_end, x_st, x_end,pColor, window,pos_inPic,wh_in2D);
*/
			
			radius_inUse = 10;
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,window.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,window.h-1);
			slider_header_clr = (slider_color>>24);
			slider_header_clr = (slider_header_clr*Breath_Sin_Min+((slider_header_clr*(Breath_Sin_Max-Breath_Sin_Min)*s2d_sin_new((s_Breath_Point-Breath_Min)*900/(Breath_Max-Breath_Min)))>>16))/Breath_Sin_Max;
			slider_header_clr = (slider_header_clr<<24)+(slider_color&0x00ffffff);
			ARGB888_2_PAF888(cov_size,&slider_header_clr,&slider_header_clr);
			Linear_Head_draw_PAF888(slider_header_clr,yh_st,yh_end,xh_st,xh_end,pColor,window,cen_Poi, radius_inUse);
			
			radius_inUse = 3;
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,window.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,window.h-1);
			slider_header_clr = 0xffffffff;
			ARGB888_2_PAF888(cov_size,&slider_header_clr,&slider_header_clr);
			Linear_Head_draw_PAF888(slider_header_clr,yh_st,yh_end,xh_st,xh_end,pColor,window,cen_Poi, radius_inUse);
			
		}

PUBLIC void WidgetMusicPlayer_CtrlBar(IMGREF_SIZE_T window,
										uint32 *pColor,
										TURNTABLE_RES_T* res,	
										IMGREF_3D_POINT_T Base_3daxis_PointL,
										IMGREF_3D_POINT_T Base_3daxis_PointR,
										IMGREF_3D_POINT_T Rot_3d_axis,
										int32 rot_yAxis_angle,
										int32 rot_yAxis_angle_max,
										int32 player_rot_angle)
{
#ifdef USE_QVGA_TYPE
	int32 gap_z = PLAYER_THICKNESS_Z;
	int32 PlayerInside_gapZ = 3;
	int32 PlayerInside_correctMaxX = 4;
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};
#else
	/*int32 gap_z = PLAYER_THICKNESS_Z;
	int32 PlayerInside_gapZ = 4;
	int32 PlayerInside_correctMaxX = 4;
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};*/
#endif
	
	if(player_rot_angle<900)
	{
	//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);

	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	}
	else if(player_rot_angle<1800)
	{
	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);

	}
	else if(player_rot_angle<2700)
	{
	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);

	}
	else if(player_rot_angle<=3600)
	{
		//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);

	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	}
	else
	{
		return;
	}

//*//inside player
	Player_3daxis_PointL.x = Player_3daxis_PointL.x + PLAYER_IN_OUT_ROTAXIS_OFF_X+PlayerInside_correctMaxX*rot_yAxis_angle/rot_yAxis_angle_max;
	Player_3daxis_PointR.x = Player_3daxis_PointL.x+res->PlayerInsideF_PLAYImg->up_window.w-1;	


	Player_3daxis_PointR.z = Player_3daxis_PointL.z = Rot_3d_axis.z+PlayerInside_gapZ/2;

	TurnTableRotImg_CtrlBar(window,pColor,res->PlayerInsideBImg,PLAYER_INSODE_ROTAXIS_Y,&Player_3daxis_PointL,
		&Player_3daxis_PointR,&Rot_3d_axis,0,rot_yAxis_angle);//*/

	Player_3daxis_PointR.z = Player_3daxis_PointL.z = Rot_3d_axis.z-PlayerInside_gapZ/2;
	if(res->IsPlaying)
	{
		TurnTableRotImg_CtrlBar(window,pColor,res->PlayerInsideF_PAUSEImg,PLAYER_INSODE_ROTAXIS_Y,&Player_3daxis_PointL,
			&Player_3daxis_PointR,&Rot_3d_axis,0,rot_yAxis_angle);
	}
	else
	{
		TurnTableRotImg_CtrlBar(window,pColor,res->PlayerInsideF_PLAYImg,PLAYER_INSODE_ROTAXIS_Y,&Player_3daxis_PointL,
			&Player_3daxis_PointR,&Rot_3d_axis,0,rot_yAxis_angle);
	}
//*/

	

	if(player_rot_angle<900)
	{
	//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);

	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	}
	else if(player_rot_angle<1800)
	{
	//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
				
	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	}
	else if(player_rot_angle<2700)
	{
	//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	}
	else if(player_rot_angle<=3600)
	{
	//player outside Up img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = Base_3daxis_PointL.z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = Base_3daxis_PointR.z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideFUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);

	//player outside next img
		Player_3daxis_PointL.x = Base_3daxis_PointL.x;
		Player_3daxis_PointL.y = Base_3daxis_PointL.y;
		Player_3daxis_PointL.z = gap_z;

		Player_3daxis_PointR.x = Base_3daxis_PointR.x;
		Player_3daxis_PointR.y = Base_3daxis_PointR.y;
		Player_3daxis_PointR.z = gap_z;
			
		TurnTableRotImg(window,pColor,res->PlayerOutsideBUImg,PLAYER_OUTSODEU_ROTAXIS_Y,&Player_3daxis_PointL,
				&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
			
		//TurnTableRotImg(window,pColor,res->PlayerOutsideDImg,PLAYER_OUTSODED_ROTAXIS_Y,&Player_3daxis_PointL,
				//&Player_3daxis_PointR,&Rot_3d_axis,player_rot_angle,rot_yAxis_angle);
	}
	else
	{
		return;
	}
}


PUBLIC void WidgetMusicPlayer_PauseCDChoose(int32 CurFrame, int32 totalFarme,BOOLEAN IsGoBack,IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res)
{
	
	IMGREF_3D_POINT_T CoverImg_3daxis_PointL = {0};
	IMGREF_3D_POINT_T CoverImg_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Rot_3d_axis = {0};
	IMGREF_3D_POINT_T Img_Rot_3d_axis = {0};
	
	int32 player_rot_angle = 0;
	int32 Img_rot_angle_first = 0,Img_rot_angle = 0,Img_rot_radius = 0, last_Show_angle = 0;
	int32 rot_yAxis_angle = 0,rot_yAxis_angle_max = 450;
	int32 base_z = 0,i = 0,calc_R_x = 0, calc_L_x = 0,gap_angle = 140;

	int32 TotalFrame = 1948, frame = 0, SlowDownFrame = 512, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 500,Frame_gap = 1000,rot_yAxis_frame = 500,rot_xAxis_frame = 800,idle_frame = 800;
	//int32 TotalFrame = 4096, frame = 0, SlowDownFrame = 750, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 1000,Frame_gap = 1000;
	int32 DampingAmp = 3,DampLen = 3600,Damping_x = 0,Damp_angle = 0,Damp_factor = 200;
	int32 PlayerInside_gapZ = 4,PlayerInside_correctMaxX = 4, gap_z = 6;
	int32 Img_Radius_Ctrl = 0;

	int32 showInPicNum = 0;

	if(res->CoverImgNum>IMG_SHOW_CDPIC_NUM_MAX+1)
	{
		showInPicNum = IMG_SHOW_CDPIC_NUM_MAX+1;
	}
	else if(res->CoverImgNum<0)
	{
		showInPicNum = 0;
	}
	else
	{
		showInPicNum = res->CoverImgNum;
	}

	//
	if(IsGoBack)
	{
		frame = Img_Rot_TotalFrame-Img_Rot_TotalFrame*CurFrame/totalFarme;
	}
	else
	{
		frame = Img_Rot_TotalFrame*CurFrame/totalFarme;
	}
	last_Show_angle = 1800-(IMG_SHOW_CDPIC_NUM_MAX-1)*gap_angle+1800;

	player_rot_angle = 0;
	rot_yAxis_angle = 450;
	Img_rot_angle_first = 100;
	Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;
	

	player_rot_angle = 1800;
	rot_yAxis_angle = rot_yAxis_angle_max;
	
//player_rot_angle = 1800;
//rot_yAxis_angle = 0;


//outside player
	Base_3daxis_PointL.x = (window.w-res->PlayerOutsideFUImg->up_window.w)/2-PLAYER_ROT_MOVE_X_MAX*rot_yAxis_angle/rot_yAxis_angle_max;
	Base_3daxis_PointL.y = window.h - res->PlayerOutsideFUImg->up_window.h-PLAYER_OUTSODEU_ROTAXIS_Y-PLAYER_OFFSET_Y_CORRECT;
	Base_3daxis_PointL.z = base_z;

	Base_3daxis_PointR.x = Base_3daxis_PointL.x+res->PlayerOutsideFUImg->up_window.w-1;	
	Base_3daxis_PointR.y = Base_3daxis_PointL.y;
	Base_3daxis_PointR.z = Base_3daxis_PointL.z;	
	
	Rot_3d_axis.x = Base_3daxis_PointR.x;
	Rot_3d_axis.y = Base_3daxis_PointR.y;	
	Rot_3d_axis.z = base_z+PLAYER_THICKNESS_Z/2;

	WidgetMusicPlayer_CtrlBar(window,pColor,res,Base_3daxis_PointL,Base_3daxis_PointR,Rot_3d_axis,rot_yAxis_angle,rot_yAxis_angle_max,player_rot_angle);

	//Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;
	//Img_rot_angle_first = player_rot_angle+1800;
	CoverImg_3daxis_PointL.x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
	CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y-Img_rot_radius;
	CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

	CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[0].up_window.w-1;
	CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y-Img_rot_radius;
	CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

	Img_Rot_3d_axis.x = Rot_3d_axis.x;
	Img_Rot_3d_axis.y = Rot_3d_axis.y-Img_rot_radius;
	Img_Rot_3d_axis.z = Rot_3d_axis.z;
	
	Img_rot_angle = IMG_ROT_ANGLE_MAX*frame/Img_Rot_TotalFrame;
	
	if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX&&Img_rot_angle>=last_Show_angle)))
	{
		TurnTableRotImg(window,pColor,&(res->CoverImg[0]),res->CoverImg[0].up_window.h,&CoverImg_3daxis_PointL,
			&CoverImg_3daxis_PointR,&Img_Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
	}

	calc_L_x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
	CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y;
	CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

	calc_R_x = calc_L_x+res->CoverImg[0].up_window.w-1;
	CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y;
	CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

	
		
	for(i = 1; i < showInPicNum; i++)//
	{
		CoverImg_3daxis_PointL.x = (calc_L_x+calc_R_x-res->CoverImg[i].up_window.w+1)/2;
		CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[i].up_window.w-1;
		Img_rot_angle = 1800-i*gap_angle+1800+gap_angle*frame/Img_Rot_TotalFrame;
		Img_rot_angle = (((Img_rot_angle%3600)+3600)%3600);
		if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX&&Img_rot_angle>=last_Show_angle))&&(player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle)<=1800)
		{
			TurnTableRotImg(window,pColor,&(res->CoverImg[i]),res->CoverImg[i].up_window.h+Img_rot_radius-Img_Radius_Ctrl,&CoverImg_3daxis_PointL,
				&CoverImg_3daxis_PointR,&Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
		}
	}


	


}


PUBLIC void WidgetMusicPlayer_ShowIn(int32 CurFrame, int32 totalFarme, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res)
{
	
	IMGREF_3D_POINT_T CoverImg_3daxis_PointL = {0};
	IMGREF_3D_POINT_T CoverImg_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Rot_3d_axis = {0};
	//IMGREF_3D_POINT_T Img_Rot_3d_axis = {0};
	
	int32 player_rot_angle = 0;
	int32 Img_rot_angle_first = 0,Img_rot_angle = 0,Img_rot_radius = 0;
	int32 rot_yAxis_angle = 0,rot_yAxis_angle_max = 450;
	int32 base_z = 0,i = 0,calc_R_x = 0, calc_L_x = 0,gap_angle = 140;

	int32 TotalFrame = 1948, frame = 0, SlowDownFrame = 512, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 500,Frame_gap = 1000,rot_yAxis_frame = 500,rot_xAxis_frame = 800,idle_frame = 800;
	//int32 TotalFrame = 4096, frame = 0, SlowDownFrame = 750, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 1000,Frame_gap = 1000;
	int32 DampingAmp = 3,DampLen = 3600,Damping_x = 0,Damp_angle = 0,Damp_factor = 200;
	int32 PlayerInside_gapZ = 4,PlayerInside_correctMaxX = 4, gap_z = 6;
	int32 Img_Radius_Ctrl = 0;
	int32 last_Show_angle = 0;
	int32 showInPicNum = 0;
	last_Show_angle = 1800-(IMG_SHOW_CDPIC_NUM_MAX-1)*gap_angle+1800;
	if(res->CoverImgNum>IMG_SHOW_CDPIC_NUM_MAX)
	{
		showInPicNum = IMG_SHOW_CDPIC_NUM_MAX;
	}
	else if(res->CoverImgNum<0)
	{
		showInPicNum = 0;
	}
	else
	{
		showInPicNum = res->CoverImgNum;
	}

	frame = TotalFrame*CurFrame/totalFarme;
	
//frame = TotalFrame+Frame_gap+1;
	player_rot_angle = 0;
	rot_yAxis_angle = 450;
	Img_rot_angle_first = 100;
	
	Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;
	if(frame<0)
	{
		player_rot_angle = 0;
		rot_yAxis_angle = 0;	
	}
	else if(frame<SlowDownFrame)
	{
		player_rot_angle = frame*1800/SlowDownFrame;
		rot_yAxis_angle = frame*rot_yAxis_angle_max/SlowDownFrame;	
	}
	else if(frame<=TotalFrame)
	{
		Damping_x = (frame-SlowDownFrame)*DampLen/(TotalFrame-SlowDownFrame);
		//1800-((10*(DampLen-Damping_x)*3.1415*fastSin(DampLen-Damping_x)/1800)>>16);
		Damp_angle = -1*((((DampLen-Damping_x)/10)*DAMP_AMP*fastSin(DampLen-Damping_x))>>16);
		player_rot_angle = 1800+Damp_angle;
		rot_yAxis_angle = rot_yAxis_angle_max;
	}
	else
	{
		player_rot_angle = 0;
		rot_yAxis_angle = 0;	
	}

//player_rot_angle = 1800;
//rot_yAxis_angle = 0;

//outside player
	Base_3daxis_PointL.x = (window.w-res->PlayerOutsideFUImg->up_window.w)/2-PLAYER_ROT_MOVE_X_MAX*rot_yAxis_angle/rot_yAxis_angle_max;
	Base_3daxis_PointL.y = window.h - res->PlayerOutsideFUImg->up_window.h-PLAYER_OUTSODEU_ROTAXIS_Y-PLAYER_OFFSET_Y_CORRECT;
	Base_3daxis_PointL.z = base_z;

	Base_3daxis_PointR.x = Base_3daxis_PointL.x+res->PlayerOutsideFUImg->up_window.w-1;	
	Base_3daxis_PointR.y = Base_3daxis_PointL.y;
	Base_3daxis_PointR.z = Base_3daxis_PointL.z;	
	
	Rot_3d_axis.x = Base_3daxis_PointR.x;
	Rot_3d_axis.y = Base_3daxis_PointR.y;	
	Rot_3d_axis.z = base_z+PLAYER_THICKNESS_Z/2;


	if(frame>Img_Start_Frame&&frame<=TotalFrame)//&&frame<=SlowDownFrame
	{
		//Img_rot_angle_first = player_rot_angle+1800;
		calc_L_x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
		CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y;
		CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

		calc_R_x = calc_L_x+res->CoverImg[0].up_window.w-1;
		CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y;
		CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

		for(i = 0; i < showInPicNum; i++)//
		{
			CoverImg_3daxis_PointL.x = (calc_L_x+calc_R_x-res->CoverImg[i].up_window.w+1)/2;
			CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[i].up_window.w-1;
			Img_rot_angle = player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle+1800;
			Img_rot_angle = (((Img_rot_angle%3600)+3600)%3600);
			if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX))&&(player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle)>=1800)
			{
				TurnTableRotImg(window,pColor,&(res->CoverImg[i]),res->CoverImg[i].up_window.h+Img_rot_radius-Img_Radius_Ctrl,&CoverImg_3daxis_PointL,
					&CoverImg_3daxis_PointR,&Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
			}
		}
	}

	WidgetMusicPlayer_CtrlBar(window,pColor,res,Base_3daxis_PointL,Base_3daxis_PointR,Rot_3d_axis,rot_yAxis_angle,rot_yAxis_angle_max,player_rot_angle);

	if(frame>Img_Start_Frame&&frame<=TotalFrame)//
	{
		//Img_rot_angle_first = player_rot_angle+1800;
		calc_L_x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
		CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y;
		CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

		calc_R_x = calc_L_x+res->CoverImg[0].up_window.w-1;
		CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y;
		CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

		for(i = 0; i < showInPicNum; i++)//
		{
			CoverImg_3daxis_PointL.x = (calc_L_x+calc_R_x-res->CoverImg[i].up_window.w+1)/2;
			CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[i].up_window.w-1;
			Img_rot_angle = player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle+1800;
			Img_rot_angle = (((Img_rot_angle%3600)+3600)%3600);
			if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX))&&(player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle)<1800)
			{
				TurnTableRotImg(window,pColor,&(res->CoverImg[i]),res->CoverImg[i].up_window.h+Img_rot_radius-Img_Radius_Ctrl,&CoverImg_3daxis_PointL,
					&CoverImg_3daxis_PointR,&Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
			}
		}
	}

}



PUBLIC void WidgetMusicPlayer_Pause2Play(int32 CurFrame, int32 totalFarme, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res)
{
	
	IMGREF_3D_POINT_T CoverImg_3daxis_PointL = {0};
	IMGREF_3D_POINT_T CoverImg_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Rot_3d_axis = {0};
	IMGREF_3D_POINT_T Img_Rot_3d_axis = {0};
	
	int32 player_rot_angle = 0;
	int32 Img_rot_angle_first = 0,Img_rot_angle = 0,Img_rot_radius = 0;
	int32 rot_yAxis_angle = 0,rot_yAxis_angle_max = 450;
	int32 base_z = 0,i = 0,calc_R_x = 0, calc_L_x = 0,gap_angle = 140;

	int32 TotalFrame = 1948, frame = 0, SlowDownFrame = 512, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 500,Frame_gap = 1000,rot_yAxis_frame = 500,rot_xAxis_frame = 800,idle_frame = 800;
	//int32 TotalFrame = 4096, frame = 0, SlowDownFrame = 750, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 1000,Frame_gap = 1000;
	int32 DampingAmp = 3,DampLen = 3600,Damping_x = 0,Damp_angle = 0,Damp_factor = 200;
	int32 PlayerInside_gapZ = 4,PlayerInside_correctMaxX = 4, gap_z = 6;
	int32 Img_Radius_Ctrl = 0;

	int32 last_Show_angle = 0;

	int32 show_MusicSlider_TF = 500, show_Rotate_TF = 500, show_MusicSlider_frame = 0;

	int32 Pause2PlayTF = 0;

	IMGREF_POINT_T window_mid = {0};
	IMGREF_POINT_T img_mid = {0};
	uint8 alpha_para = 0;

	int32 showInPicNum = 0;

	if(res->CoverImgNum>IMG_SHOW_CDPIC_NUM_MAX)
	{
		showInPicNum = IMG_SHOW_CDPIC_NUM_MAX;
	}
	else if(res->CoverImgNum<0)
	{
		showInPicNum = 0;
	}
	else
	{
		showInPicNum = res->CoverImgNum;
	}
	Pause2PlayTF = rot_yAxis_frame+show_MusicSlider_TF;	
	
	//rot_yAxis_frame = show_Rotate_TF*totalFarme/Pause2PlayTF;

	//show_MusicSlider_frame = show_MusicSlider_TF*totalFarme/Pause2PlayTF;

	frame = Pause2PlayTF*CurFrame/totalFarme;	

	last_Show_angle = 1800-(IMG_SHOW_CDPIC_NUM_MAX-1)*gap_angle+1800;

	player_rot_angle = 0;
	rot_yAxis_angle = 450;
	Img_rot_angle_first = 100;
	
	Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;
	
if(frame<=rot_yAxis_frame)
{
	player_rot_angle = 1800;
	rot_yAxis_angle = rot_yAxis_angle_max*(rot_yAxis_frame-frame)/rot_yAxis_frame;
}
else
{
	player_rot_angle = 1800;
	rot_yAxis_angle = 0;
}


//player_rot_angle = 1800;
//rot_yAxis_angle = 0;


//outside player
	Base_3daxis_PointL.x = (window.w-res->PlayerOutsideFUImg->up_window.w)/2-PLAYER_ROT_MOVE_X_MAX*rot_yAxis_angle/rot_yAxis_angle_max;
	Base_3daxis_PointL.y = window.h - res->PlayerOutsideFUImg->up_window.h-PLAYER_OUTSODEU_ROTAXIS_Y-PLAYER_OFFSET_Y_CORRECT;
	Base_3daxis_PointL.z = base_z;

	Base_3daxis_PointR.x = Base_3daxis_PointL.x+res->PlayerOutsideFUImg->up_window.w-1;	
	Base_3daxis_PointR.y = Base_3daxis_PointL.y;
	Base_3daxis_PointR.z = Base_3daxis_PointL.z;	
	
	Rot_3d_axis.x = Base_3daxis_PointR.x;
	Rot_3d_axis.y = Base_3daxis_PointR.y;	
	Rot_3d_axis.z = base_z+PLAYER_THICKNESS_Z/2;

	WidgetMusicPlayer_CtrlBar(window,pColor,res,Base_3daxis_PointL,Base_3daxis_PointR,Rot_3d_axis,rot_yAxis_angle,rot_yAxis_angle_max,player_rot_angle);

	calc_L_x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
	CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y;
	CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

	calc_R_x = calc_L_x+res->CoverImg[0].up_window.w-1;
	CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y;
	CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

	for(i = 0; i < showInPicNum; i++)//
	{
		CoverImg_3daxis_PointL.x = (calc_L_x+calc_R_x-res->CoverImg[i].up_window.w+1)/2;
		CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[i].up_window.w-1;
		Img_rot_angle = 1800-i*gap_angle+1800;//+gap_angle;
		Img_rot_angle = (((Img_rot_angle%3600)+3600)%3600);
		if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX&&Img_rot_angle>=last_Show_angle))&&(player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle)<=1800)
		{
			TurnTableRotImg(window,pColor,&(res->CoverImg[i]),res->CoverImg[i].up_window.h+Img_rot_radius-Img_Radius_Ctrl,&CoverImg_3daxis_PointL,
				&CoverImg_3daxis_PointR,&Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
		}
	}

	if(frame >= rot_yAxis_frame)
	{
		alpha_para = 255*(frame-rot_yAxis_frame)/show_MusicSlider_TF;
		//alpha_para = 0;
		window_mid.x = OFFSET_BLENDING_VS_S2D_4_IN_ONE+PLAYER_SLIDER_X;
		window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderBG->up_window.h/2;
		img_mid.x = 0;//res->PlayerSliderBG->up_window.w/2;
		img_mid.y = 0;//res->PlayerSliderBG->up_window.h/2;
		S2d_4_in_one_fast_PAF888(window,res->PlayerSliderBG->up_window,window_mid,img_mid,0,
			res->PlayerSliderBG->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);
		
		/*window_mid.x = OFFSET_BLENDING_VS_S2D_4_IN_ONE+PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST;
		window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderLight->up_window.h/2;
		S2d_4_in_one_fast_PAF888(window,res->PlayerSliderLight->up_window,window_mid,img_mid,0,
			res->PlayerSliderLight->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);*/

		/*		
		window_mid.x = OFFSET_BLENDING_VS_S2D_4_IN_ONE+PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST - res->PlayerSliderPoint->up_window.w/2+PLAYER_PLAYPERCENTAGE_X_OFFSET;
		window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderPoint->up_window.h/2;
		S2d_4_in_one_fast_PAF888(window,res->PlayerSliderPoint->up_window,window_mid,img_mid,0,
			res->PlayerSliderPoint->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);
		*/
		WidgetMusicPlayer_PercentagePoint(window,pColor,res);
	}
}


PUBLIC void WidgetMusicPlayer_Play(IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res)
{
	
	IMGREF_3D_POINT_T CoverImg_3daxis_PointL = {0};
	IMGREF_3D_POINT_T CoverImg_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Rot_3d_axis = {0};
	IMGREF_3D_POINT_T Img_Rot_3d_axis = {0};
	
	int32 player_rot_angle = 0;
	int32 Img_rot_angle_first = 0,Img_rot_angle = 0,Img_rot_radius = 0;
	int32 rot_yAxis_angle = 0,rot_yAxis_angle_max = 450;
	int32 base_z = 0,i = 0,calc_R_x = 0, calc_L_x = 0,gap_angle = 140;

	int32 TotalFrame = 1948, frame = 0, SlowDownFrame = 512, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 500,Frame_gap = 1000,rot_yAxis_frame = 500,rot_xAxis_frame = 800,idle_frame = 800;
	//int32 TotalFrame = 4096, frame = 0, SlowDownFrame = 750, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 1000,Frame_gap = 1000;
	int32 DampingAmp = 3,DampLen = 3600,Damping_x = 0,Damp_angle = 0,Damp_factor = 200;
	int32 PlayerInside_gapZ = 4,PlayerInside_correctMaxX = 4, gap_z = 6;
	int32 Img_Radius_Ctrl = 0;

	int32 last_Show_angle = 0;

	int32 show_MusicSlider_TF = 500, show_Rotate_TF = 500, show_MusicSlider_frame = 0;

	int32 Pause2PlayTF = 0;

	IMGREF_POINT_T window_mid = {0};
	IMGREF_POINT_T img_mid = {0};
	uint8 alpha_para = 0;

	IMGREF_RECT_T   show_box = {0};
	FIX16_POINT_T	offset = {0};

	int32 showInPicNum = 0;

	if(res->CoverImgNum>IMG_SHOW_CDPIC_NUM_MAX)
	{
		showInPicNum = IMG_SHOW_CDPIC_NUM_MAX;
	}
	else if(res->CoverImgNum<0)
	{
		showInPicNum = 0;
	}
	else
	{
		showInPicNum = res->CoverImgNum;
	}
	Pause2PlayTF = rot_yAxis_frame+show_MusicSlider_TF;	

	last_Show_angle = 1800-(IMG_SHOW_CDPIC_NUM_MAX-1)*gap_angle+1800;

	player_rot_angle = 0;
	rot_yAxis_angle = 450;
	Img_rot_angle_first = 100;
	
	Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;
	
	player_rot_angle = 1800;
	rot_yAxis_angle = 0;

//player_rot_angle = 1800;
//rot_yAxis_angle = 0;


//outside player
	Base_3daxis_PointL.x = (window.w-res->PlayerOutsideFUImg->up_window.w)/2-PLAYER_ROT_MOVE_X_MAX*rot_yAxis_angle/rot_yAxis_angle_max;
	Base_3daxis_PointL.y = window.h - res->PlayerOutsideFUImg->up_window.h-PLAYER_OUTSODEU_ROTAXIS_Y-PLAYER_OFFSET_Y_CORRECT;
	Base_3daxis_PointL.z = base_z;

	Base_3daxis_PointR.x = Base_3daxis_PointL.x+res->PlayerOutsideFUImg->up_window.w-1;	
	Base_3daxis_PointR.y = Base_3daxis_PointL.y;
	Base_3daxis_PointR.z = Base_3daxis_PointL.z;	
	
	Rot_3d_axis.x = Base_3daxis_PointR.x;
	Rot_3d_axis.y = Base_3daxis_PointR.y;	
	Rot_3d_axis.z = base_z+PLAYER_THICKNESS_Z/2;

	WidgetMusicPlayer_CtrlBar(window,pColor,res,Base_3daxis_PointL,Base_3daxis_PointR,Rot_3d_axis,rot_yAxis_angle,rot_yAxis_angle_max,player_rot_angle);

	calc_L_x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
	CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y;
	CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

	calc_R_x = calc_L_x+res->CoverImg[0].up_window.w-1;
	CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y;
	CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

	for(i = 0; i < showInPicNum; i++)//
	{
		CoverImg_3daxis_PointL.x = (calc_L_x+calc_R_x-res->CoverImg[i].up_window.w+1)/2;
		CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[i].up_window.w-1;
		Img_rot_angle = 1800-i*gap_angle+1800;//+gap_angle;
		Img_rot_angle = (((Img_rot_angle%3600)+3600)%3600);
		if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX&&Img_rot_angle>=last_Show_angle))&&(player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle)<=1800)
		{
			TurnTableRotImg(window,pColor,&(res->CoverImg[i]),res->CoverImg[i].up_window.h+Img_rot_radius-Img_Radius_Ctrl,&CoverImg_3daxis_PointL,
				&CoverImg_3daxis_PointR,&Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
		}
	}
/*
		alpha_para = 255;
		//alpha_para = 0;
		window_mid.x = PLAYER_SLIDER_X;
		window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderBG->up_window.h/2;
		img_mid.x = 0;//res->PlayerSliderBG->up_window.w/2;
		img_mid.y = 0;//res->PlayerSliderBG->up_window.h/2;
		S2d_4_in_one_fast_PAF888(window,res->PlayerSliderBG->up_window,window_mid,img_mid,0,
			res->PlayerSliderBG->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);
		
		window_mid.x = PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST - res->PlayerSliderPoint->up_window.w/2;
		window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderPoint->up_window.h/2;
		S2d_4_in_one_fast_PAF888(window,res->PlayerSliderPoint->up_window,window_mid,img_mid,0,
			res->PlayerSliderPoint->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);

		window_mid.x = PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST;
		window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderLight->up_window.h/2;
		S2d_4_in_one_fast_PAF888(window,res->PlayerSliderLight->up_window,window_mid,img_mid,0,
			res->PlayerSliderLight->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);
*/
		offset.x = PLAYER_SLIDER_X;
		offset.y = PLAYER_SLIDER_Y-res->PlayerSliderBG->up_window.h/2;
		show_box.x = offset.x;
		show_box.y = offset.y;
		show_box.w = res->PlayerSliderBG->up_window.w;
		show_box.h = res->PlayerSliderBG->up_window.h;
		S2D_Blending_PAF888ShowBox(window,pColor,res->PlayerSliderBG->up_window,show_box,
									offset,res->PlayerSliderBG->srcARGB_Map,pColor);
/*
		offset.x = PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST - res->PlayerSliderPoint->up_window.w/2+PLAYER_PLAYPERCENTAGE_X_OFFSET+PLAYER_PLAYPERCENTAGE_MAXPIXEL*res->PlayPercentage/100;;
		offset.y = PLAYER_SLIDER_Y-res->PlayerSliderPoint->up_window.h/2;
		show_box.x = offset.x;
		show_box.y = offset.y;
		show_box.w = res->PlayerSliderPoint->up_window.w;
		show_box.h = res->PlayerSliderPoint->up_window.h;
		S2D_Blending_PAF888ShowBox(window,pColor,res->PlayerSliderPoint->up_window,show_box,offset,res->PlayerSliderPoint->srcARGB_Map,pColor);
*/
		offset.x = PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST;
		offset.y = PLAYER_SLIDER_Y-res->PlayerSliderLight->up_window.h/2;
		show_box.x = offset.x;
		show_box.y = offset.y;
		show_box.w = PLAYER_PLAYPERCENTAGE_X_OFFSET+PLAYER_PLAYPERCENTAGE_MAXPIXEL*res->PlayPercentage/100;
		show_box.h = res->PlayerSliderLight->up_window.h;
		S2D_Blending_PAF888ShowBox(window,pColor,res->PlayerSliderLight->up_window,show_box,offset,res->PlayerSliderLight->srcARGB_Map,pColor);

		WidgetMusicPlayer_PercentagePoint(window,pColor,res);
		#if 0
		{
			int32 yh_st, yh_end, xh_st,xh_end;
			int32 y_st, y_end, x_st, x_end;
			FIX16_POINT_T cen_Poi = {0};
			int32 radius_inUse = 0;
			FIX16_POINT_T pos_inPic = {0};
			IMGREF_SIZE_T wh_in2D= {0};
			uint32 slider_color = 0xcf00fff0,slider_header_clr = 0, slider_body_clr = 0;
			IMGREF_SIZE_T cov_size = {1,1};
			int32 Breath_Max = 50;
			int32 Breath_Min = 0;
			int32 Breath_Sin_Min = 20;
			int32 Breath_Sin_Max = 100;
			//int32 Breath_Step = 3;
			static int32 s_Breath_Point = Breath_Min;
			static int32 s_Breath_Point_sign = 1;
			

			s_Breath_Point = s_Breath_Point+s_Breath_Point_sign;

			if(s_Breath_Point >= Breath_Max)
			{
				s_Breath_Point_sign = -1*s_Breath_Point_sign;
			}
			if(s_Breath_Point <= Breath_Min)
			{
				s_Breath_Point_sign = -1*s_Breath_Point_sign;
			}
//slider_color = 0xc0ff0000;
			
//slider_color = 0x3fc00000;
			cen_Poi.x = PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST+PLAYER_PLAYPERCENTAGE_X_OFFSET+PLAYER_PLAYPERCENTAGE_MAXPIXEL*res->PlayPercentage/100;//*res->PlayPercentage/100;
			cen_Poi.y = PLAYER_SLIDER_Y;	

/*			wh_in2D.w = PLAYER_SLIDER_DRAW_X_MAX*res->PlayPercentage/100;
			wh_in2D.h = PLAYER_SLIDER_DRAW_Y_MAX;

			pos_inPic.x = cen_Poi.x;
			pos_inPic.y = cen_Poi.y-wh_in2D.h/2;

			y_st = MAX(pos_inPic.y,0);
			y_end = MIN(pos_inPic.y+wh_in2D.h,window.h-1);
			x_st = MIN(pos_inPic.x,window.w-1);
			x_end = MAX(pos_inPic.x-wh_in2D.w,PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST+PLAYER_PLAYPERCENTAGE_X_OFFSET);
			slider_body_clr = slider_color;//(((slider_color>>24)*(s_Breath_Point-Breath_Min)/(Breath_Max-Breath_Min))<<24)+(slider_color&0x00ffffff);
			ARGB888_2_PAF888(cov_size,&slider_body_clr,&slider_body_clr);
			Linear_Body_R_PAF888(slider_body_clr, y_st, y_end, x_st, x_end,pColor, window,pos_inPic,wh_in2D);
*/
			
			radius_inUse = 10;
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,window.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,window.h-1);
			slider_header_clr = (slider_color>>24);
			slider_header_clr = (slider_header_clr*Breath_Sin_Min+((slider_header_clr*(Breath_Sin_Max-Breath_Sin_Min)*s2d_sin_new((s_Breath_Point-Breath_Min)*900/(Breath_Max-Breath_Min)))>>16))/Breath_Sin_Max;
			slider_header_clr = (slider_header_clr<<24)+(slider_color&0x00ffffff);
			ARGB888_2_PAF888(cov_size,&slider_header_clr,&slider_header_clr);
			Linear_Head_draw_PAF888(slider_header_clr,yh_st,yh_end,xh_st,xh_end,pColor,window,cen_Poi, radius_inUse);
			
			radius_inUse = 3;
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,window.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,window.h-1);
			slider_header_clr = 0xffffffff;
			ARGB888_2_PAF888(cov_size,&slider_header_clr,&slider_header_clr);
			Linear_Head_draw_PAF888(slider_header_clr,yh_st,yh_end,xh_st,xh_end,pColor,window,cen_Poi, radius_inUse);
			
		}
		#endif

}




PUBLIC void WidgetMusicPlayer_PlayCDChoose(int32 CurFrame, int32 totalFarme,BOOLEAN IsGoBack, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res)
{
	
	IMGREF_3D_POINT_T CoverImg_3daxis_PointL = {0};
	IMGREF_3D_POINT_T CoverImg_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Rot_3d_axis = {0};
	IMGREF_3D_POINT_T Img_Rot_3d_axis = {0};
	
	int32 player_rot_angle = 0;
	int32 Img_rot_angle_first = 0,Img_rot_angle = 0,Img_rot_radius = 0;
	int32 rot_yAxis_angle = 0,rot_yAxis_angle_max = 450;
	int32 base_z = 0,i = 0,calc_R_x = 0, calc_L_x = 0,gap_angle = 140;

	int32 TotalFrame = 1948, frame = 0, SlowDownFrame = 512, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 500,Frame_gap = 1000,rot_yAxis_frame = 500,rot_xAxis_frame = 800,idle_frame = 800;
	//int32 TotalFrame = 4096, frame = 0, SlowDownFrame = 750, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 1000,Frame_gap = 1000;
	int32 DampingAmp = 3,DampLen = 3600,Damping_x = 0,Damp_angle = 0,Damp_factor = 200;
	int32 PlayerInside_gapZ = 4,PlayerInside_correctMaxX = 4, gap_z = 6;
	int32 Img_Radius_Ctrl = 0;

	int32 last_Show_angle = 0;

	int32 show_MusicSlider_TF = 500, show_Rotate_TF = 500, show_MusicSlider_frame = 0,calc_frame = 0, MusicSlider_frame = 0;

	int32 PlayChooseRotTF = 0;

	IMGREF_POINT_T window_mid = {0};
	IMGREF_POINT_T img_mid = {0};
	uint8 alpha_para = 0;
	int32 showInPicNum;

	if(res->CoverImgNum>IMG_SHOW_CDPIC_NUM_MAX+1)
	{
		showInPicNum = IMG_SHOW_CDPIC_NUM_MAX+1;
	}
	else if(res->CoverImgNum<0)
	{
		showInPicNum = 0;
	}
	else
	{
		showInPicNum = res->CoverImgNum;
	}
	
	last_Show_angle = 1800-(IMG_SHOW_CDPIC_NUM_MAX-1)*gap_angle+1800;

	PlayChooseRotTF = Img_Rot_TotalFrame+show_MusicSlider_TF;	
	
	//Img_Rot_TotalFrame = show_Rotate_TF*totalFarme/PlayChooseRotTF;

	//show_MusicSlider_frame = show_MusicSlider_TF*totalFarme/PlayChooseRotTF;

	calc_frame = PlayChooseRotTF*CurFrame/totalFarme;
	if(calc_frame>=Img_Rot_TotalFrame)
	{
		if(IsGoBack)
		{
			frame = 0;
		}
		else
		{
			frame = Img_Rot_TotalFrame;
		}
	}
	else if(calc_frame<0)
	{
		if(IsGoBack)
		{
			frame = Img_Rot_TotalFrame;
		}
		else
		{
			frame = 0;
		}
	}
	else
	{
		if(IsGoBack)
		{
			frame = Img_Rot_TotalFrame-calc_frame;
		}
		else
		{
			frame = calc_frame;
		}
	}

	player_rot_angle = 0;
	rot_yAxis_angle = 450;
	Img_rot_angle_first = 100;
	Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;
	

	player_rot_angle = 1800;
	rot_yAxis_angle = 0;//rot_yAxis_angle_max;
	
//player_rot_angle = 1800;
//rot_yAxis_angle = 0;


//outside player
	Base_3daxis_PointL.x = (window.w-res->PlayerOutsideFUImg->up_window.w)/2-PLAYER_ROT_MOVE_X_MAX*rot_yAxis_angle/rot_yAxis_angle_max;
	Base_3daxis_PointL.y = window.h - res->PlayerOutsideFUImg->up_window.h-PLAYER_OUTSODEU_ROTAXIS_Y-PLAYER_OFFSET_Y_CORRECT;
	Base_3daxis_PointL.z = base_z;

	Base_3daxis_PointR.x = Base_3daxis_PointL.x+res->PlayerOutsideFUImg->up_window.w-1;	
	Base_3daxis_PointR.y = Base_3daxis_PointL.y;
	Base_3daxis_PointR.z = Base_3daxis_PointL.z;	
	
	Rot_3d_axis.x = Base_3daxis_PointR.x;
	Rot_3d_axis.y = Base_3daxis_PointR.y;	
	Rot_3d_axis.z = base_z+PLAYER_THICKNESS_Z/2;


	WidgetMusicPlayer_CtrlBar(window,pColor,res,Base_3daxis_PointL,Base_3daxis_PointR,Rot_3d_axis,rot_yAxis_angle,rot_yAxis_angle_max,player_rot_angle);


	//Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;
	//Img_rot_angle_first = player_rot_angle+1800;
	CoverImg_3daxis_PointL.x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
	CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y-Img_rot_radius;
	CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

	CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[0].up_window.w-1;
	CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y-Img_rot_radius;
	CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

	Img_Rot_3d_axis.x = Rot_3d_axis.x;
	Img_Rot_3d_axis.y = Rot_3d_axis.y-Img_rot_radius;
	Img_Rot_3d_axis.z = Rot_3d_axis.z;
	
	Img_rot_angle = IMG_ROT_ANGLE_MAX*frame/Img_Rot_TotalFrame;
	
	if((Img_rot_angle<IMG_ROT_ANGLE_MAX||Img_rot_angle>3600-IMG_ROT_ANGLE_MAX))
	{
		TurnTableRotImg(window,pColor,&(res->CoverImg[0]),res->CoverImg[0].up_window.h,&CoverImg_3daxis_PointL,
			&CoverImg_3daxis_PointR,&Img_Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
	}

	calc_L_x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
	CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y;
	CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

	calc_R_x = calc_L_x+res->CoverImg[0].up_window.w-1;
	CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y;
	CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

	//last_Show_angle = 1800-(IMG_SHOW_CDPIC_NUM_MAX-1)*gap_angle+1800;
		
	for(i = 1; i < showInPicNum; i++)//
	{
		CoverImg_3daxis_PointL.x = (calc_L_x+calc_R_x-res->CoverImg[i].up_window.w+1)/2;
		CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[i].up_window.w-1;
		Img_rot_angle = 1800-i*gap_angle+1800+gap_angle*frame/Img_Rot_TotalFrame;
		Img_rot_angle = (((Img_rot_angle%3600)+3600)%3600);
		if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX&&Img_rot_angle>=last_Show_angle))&&(player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle)<=1800)
		{
			TurnTableRotImg(window,pColor,&(res->CoverImg[i]),res->CoverImg[i].up_window.h+Img_rot_radius-Img_Radius_Ctrl,&CoverImg_3daxis_PointL,
				&CoverImg_3daxis_PointR,&Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
		}
	}

if(calc_frame>=Img_Rot_TotalFrame)
{
	alpha_para = 255*(calc_frame-Img_Rot_TotalFrame)/show_MusicSlider_TF;
	//alpha_para = 255;
	window_mid.x = OFFSET_BLENDING_VS_S2D_4_IN_ONE+PLAYER_SLIDER_X;
	window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderBG->up_window.h/2;
	img_mid.x = 0;//res->PlayerSliderBG->up_window.w/2;
	img_mid.y = 0;//res->PlayerSliderBG->up_window.h/2;
	S2d_4_in_one_fast_PAF888(window,res->PlayerSliderBG->up_window,window_mid,img_mid,0,
		res->PlayerSliderBG->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);
	/*
	window_mid.x = OFFSET_BLENDING_VS_S2D_4_IN_ONE+PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST;
	window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderLight->up_window.h/2;
	S2d_4_in_one_fast_PAF888(window,res->PlayerSliderLight->up_window,window_mid,img_mid,0,
		res->PlayerSliderLight->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);
	*/
	/*		
	window_mid.x = OFFSET_BLENDING_VS_S2D_4_IN_ONE+PLAYER_SLIDER_X+PLAYER_SLIDER_X_LIGHT_OFST - res->PlayerSliderPoint->up_window.w/2+PLAYER_PLAYPERCENTAGE_X_OFFSET;
	window_mid.y = PLAYER_SLIDER_Y-res->PlayerSliderPoint->up_window.h/2;
	S2d_4_in_one_fast_PAF888(window,res->PlayerSliderPoint->up_window,window_mid,img_mid,0,
		res->PlayerSliderPoint->srcARGB_Map,pColor,4096,alpha_para,calc_Add_ptr);
	*/
	WidgetMusicPlayer_PercentagePoint(window,pColor,res);
	
}
	


}



PUBLIC void WidgetMusicPlayer_Close(int32 CurFrame, int32 totalFarme, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res)
{
	
	IMGREF_3D_POINT_T CoverImg_3daxis_PointL = {0};
	IMGREF_3D_POINT_T CoverImg_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Player_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointL = {0};
	IMGREF_3D_POINT_T Base_3daxis_PointR = {0};
	IMGREF_3D_POINT_T Rot_3d_axis = {0};
	IMGREF_3D_POINT_T Img_Rot_3d_axis = {0};
	
	int32 player_rot_angle = 0;
	int32 Img_rot_angle_first = 0,Img_rot_angle = 0,Img_rot_radius = 0;
	int32 rot_yAxis_angle = 0,rot_yAxis_angle_max = 450;
	int32 base_z = 0,i = 0,calc_R_x = 0, calc_L_x = 0,gap_angle = 140;

	int32 TotalFrame = 1948, frame = 0, SlowDownFrame = 512, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 500,Frame_gap = 1000,rot_yAxis_frame = 500,rot_xAxis_frame = 800,idle_frame = 800;
	//int32 TotalFrame = 4096, frame = 0, SlowDownFrame = 750, Img_Start_Frame=SlowDownFrame/2,Img_Rot_TotalFrame = 1000,Frame_gap = 1000;
	int32 DampingAmp = 3,DampLen = 3600,Damping_x = 0,Damp_angle = 0,Damp_factor = 200;
	int32 PlayerInside_gapZ = 4,PlayerInside_correctMaxX = 4, gap_z = 6;
	int32 Img_Radius_Ctrl = 0;

	int32 last_Show_angle = 0;
	int32 showInPicNum = 0;

	if(res->CoverImgNum>IMG_SHOW_CDPIC_NUM_MAX)
	{
		showInPicNum = IMG_SHOW_CDPIC_NUM_MAX;
	}
	else if(res->CoverImgNum<0)
	{
		showInPicNum = 0;
	}
	else
	{
		showInPicNum = res->CoverImgNum;
	}
	last_Show_angle = 1800-(IMG_SHOW_CDPIC_NUM_MAX-1)*gap_angle+1800;

	frame = rot_xAxis_frame*CurFrame/totalFarme;
	
//frame = TotalFrame+Frame_gap+1;
	player_rot_angle = 0;
	rot_yAxis_angle = 450;
	Img_rot_angle_first = 100;

	Img_rot_radius = res->PlayerOutsideFDImg->up_window.h-PLAYER_OUTSODED_ROTAXIS_Y-GAP_IMG_2_PLAYER;

	player_rot_angle = 1800*(rot_xAxis_frame-frame)/rot_xAxis_frame;
	rot_yAxis_angle = 0;


//player_rot_angle = 1800;
//rot_yAxis_angle = 0;


//outside player
	Base_3daxis_PointL.x = (window.w-res->PlayerOutsideFUImg->up_window.w)/2-PLAYER_ROT_MOVE_X_MAX*rot_yAxis_angle/rot_yAxis_angle_max;
	Base_3daxis_PointL.y = window.h - res->PlayerOutsideFUImg->up_window.h-PLAYER_OUTSODEU_ROTAXIS_Y-PLAYER_OFFSET_Y_CORRECT;
	Base_3daxis_PointL.z = base_z;

	Base_3daxis_PointR.x = Base_3daxis_PointL.x+res->PlayerOutsideFUImg->up_window.w-1;	
	Base_3daxis_PointR.y = Base_3daxis_PointL.y;
	Base_3daxis_PointR.z = Base_3daxis_PointL.z;	
	
	Rot_3d_axis.x = Base_3daxis_PointR.x;
	Rot_3d_axis.y = Base_3daxis_PointR.y;	
	Rot_3d_axis.z = base_z+PLAYER_THICKNESS_Z/2;

	WidgetMusicPlayer_CtrlBar(window,pColor,res,Base_3daxis_PointL,Base_3daxis_PointR,Rot_3d_axis,rot_yAxis_angle,rot_yAxis_angle_max,player_rot_angle);

	//Img_rot_angle_first = player_rot_angle+1800;
	calc_L_x = (Base_3daxis_PointL.x+Base_3daxis_PointR.x-res->CoverImg[0].up_window.w)/2;
	CoverImg_3daxis_PointL.y = Base_3daxis_PointL.y;
	CoverImg_3daxis_PointL.z = Base_3daxis_PointL.z;

	calc_R_x = calc_L_x+res->CoverImg[0].up_window.w-1;
	CoverImg_3daxis_PointR.y = Base_3daxis_PointR.y;
	CoverImg_3daxis_PointR.z = Base_3daxis_PointR.z;

	for(i = 0; i < showInPicNum; i++)//
	{
		CoverImg_3daxis_PointL.x = (calc_L_x+calc_R_x-res->CoverImg[i].up_window.w+1)/2;
		CoverImg_3daxis_PointR.x = CoverImg_3daxis_PointL.x+res->CoverImg[i].up_window.w-1;
		Img_rot_angle = player_rot_angle -i*gap_angle+1800;//+gap_angle;
		Img_rot_angle = (((Img_rot_angle%3600)+3600)%3600);
		if((Img_rot_angle<IMG_ROT_ANGLE_MAX||(Img_rot_angle>3600-IMG_ROT_ANGLE_MAX))&&(player_rot_angle-Damp_angle + DampPow(Damp_factor,Damp_angle,i) -i*gap_angle)<=1800)
		{
			TurnTableRotImg(window,pColor,&(res->CoverImg[i]),res->CoverImg[i].up_window.h+Img_rot_radius-Img_Radius_Ctrl,&CoverImg_3daxis_PointL,
				&CoverImg_3daxis_PointR,&Rot_3d_axis,Img_rot_angle,rot_yAxis_angle);
		}
	}

}

PUBLIC void WidgetMusicPlayer_PicLoadingAnimat(TURNTABLE_LOAD_RES_T* pLoading)
{
	IMGREF_POINT_T window_mid = {0};
	IMGREF_POINT_T img_mid = {0};
	static int32 s_ang = 0;
	int32 angle = 0;
	IMGREF_RECT_T   show_box = {0};
	FIX16_POINT_T	offset = {0};

	s_ang += 450;

	s_ang = s_ang%3600;

	angle = s_ang;
	
	if(pLoading->LoadingBG != pLoading->loading_pOutBuf)
	{
		memcpy(pLoading->loading_pOutBuf->srcARGB_Map, pLoading->LoadingBG->srcARGB_Map, (pLoading->loading_pOutBuf->up_window.w*pLoading->loading_pOutBuf->up_window.h)*sizeof(COLOR32));
	}

	offset.x = (pLoading->loading_pOutBuf->up_window.w-pLoading->music_note->up_window.w)/2+1;
	offset.y = (pLoading->loading_pOutBuf->up_window.h-pLoading->music_note->up_window.h)/2;
	show_box.x = offset.x;
	show_box.y = offset.y;
	show_box.w = pLoading->music_note->up_window.w;
	show_box.h = pLoading->music_note->up_window.h;
	S2D_Blending_PAF888ShowBox(pLoading->loading_pOutBuf->up_window,
							 pLoading->loading_pOutBuf->srcARGB_Map,
							 pLoading->music_note->up_window,
							 show_box,
							 offset,
							 pLoading->music_note->srcARGB_Map,
							 pLoading->loading_pOutBuf->srcARGB_Map);
	
	//
	window_mid.x = pLoading->loading_pOutBuf->up_window.w/2+2;//+2 for the Pic rot point offset
	window_mid.y = pLoading->loading_pOutBuf->up_window.h/2;
	img_mid.x = pLoading->loading_Rot->up_window.w/2;
	img_mid.y = pLoading->loading_Rot->up_window.h/2;
	S2d_4_in_one_fast_PAF888(pLoading->loading_pOutBuf->up_window,
						pLoading->loading_Rot->up_window,
						window_mid,
						img_mid,
						angle,
						pLoading->loading_Rot->srcARGB_Map,
						pLoading->loading_pOutBuf->srcARGB_Map,
						4096,
						255,
						calc_Add_ptr);
	
	
	
}

PUBLIC void TurnTableShow(IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res, MMIWIDGET_MP3_SE_ANIM_TYPE_E type, BOOLEAN is_goback)
{
	int32 s_t_ini = -800;
	static int32 s_t = -800;
	int32 TotalFrame = 1948, Img_Rot_TotalFrame = 500,rot_yAxis_frame = 500,rot_xAxis_frame = 800, show_MusicSlider_TF = 500;
	int32 totalF = 0, curF = 0;
	
	switch(type)
	{
	case MMIWIDGET_MP3_SE_ANIM_PLAY:
		res->PlayPercentage++;
		if(res->PlayPercentage >= 100)
		{
			res->PlayPercentage = 0;
		}
		WidgetMusicPlayer_Play(window,pColor,res);
		break;    
		
	case MMIWIDGET_MP3_SE_ANIM_SHOW_IN:
		totalF = TotalFrame;
		s_t += 30;
		//s_t = 0;//totalF;
		if(s_t>=totalF+1000)
			s_t = s_t_ini;
		if(s_t>totalF)
		{
			curF = totalF;
		}
		else if(s_t < 0 )
		{
			curF = 0;
		}
		else
		{
			curF = s_t;
		}
		WidgetMusicPlayer_ShowIn(curF,totalF,window,pColor,res);
		break;
		
	case MMIWIDGET_MP3_SE_ANIM_PAUSE2PLAY:
		totalF = rot_yAxis_frame+show_MusicSlider_TF;
		s_t += 30;
		if(s_t>=totalF+1000)
			s_t = s_t_ini;
		//s_t = totalF;
		if(s_t>totalF)
		{
			curF = totalF;
			WidgetMusicPlayer_Play(window,pColor,res);
		}
		else if(s_t < 0 )
		{
			curF = 0;
			WidgetMusicPlayer_Pause2Play(curF,totalF,window,pColor,res);
		}
		else
		{
			curF = s_t;
			WidgetMusicPlayer_Pause2Play(curF,totalF,window,pColor,res);
		}
		break;    
		
	case MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PAUSE:
		totalF = Img_Rot_TotalFrame;
		s_t += 30;
		if(s_t>=totalF+1000)
			s_t = s_t_ini;
		//s_t = totalF;
		if(s_t>totalF)
		{
			curF = totalF;
		}
		else if(s_t < 0 )
		{
			curF = 0;
		}
		else
		{
			curF = s_t;
		}
		WidgetMusicPlayer_PauseCDChoose(curF,totalF,is_goback,window,pColor,res);
		break;
		
	case MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PLAY:
		totalF = Img_Rot_TotalFrame;//+show_MusicSlider_TF;
		s_t += 30;
		if(s_t>=totalF+1000)
			s_t = s_t_ini;
		//s_t = 0;
		if(s_t>totalF)
		{
			curF = totalF;
		}
		else if(s_t < 0 )
		{
			curF = 0;
		}
		else
		{
			curF = s_t;
		}
		WidgetMusicPlayer_PlayCDChoose(curF,totalF,is_goback,window,pColor,res);
		break;   
	default:
	    break;
	}
}
#endif
