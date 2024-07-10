
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint16 short

//number of MMI_ARGON_MNC_T 
#define ARGON_MNC_NUM  (6)

typedef struct
{
    uint16 mcc;
    uint16 mnc;
    char ims_call_icon;
    char ims_icon_status;
    char ims_toggle_status;
    char reserved;
}MMI_ARGON_MNC_T;

 
typedef struct _MMI_IMS_TOGGLE_ST {
         uint16   flag;  // 0xface valid
         uint16   len;
         uint16   reserved[6];
} MMI_IMS_TOGGLE_ST ;


char *pInput = "ims_toggle.txt";
char *pOutput = "tog.bin";

int isBigEndian(){
     uint16 a = 0x1122;
	 char b = *(char*)&a;
	 if(b == 0x11){
	     return 1;
	 } else{
	     return 0;
	 }
}

/*
* write head to output file
*/
void writeHead(int len) {

	FILE* fp = NULL;

    MMI_IMS_TOGGLE_ST head ;
	memset(&head,0,sizeof(MMI_IMS_TOGGLE_ST));
	// flag is fixed and set to 0xface
	head.flag = 0xface;
	head.len = len;
 


	fp = fopen(pOutput, "wb");
	if(fp == NULL){
	    	perror("\nopen file ims_toggle.txt error");
	}
    fwrite(&head, sizeof(MMI_IMS_TOGGLE_ST), 1, fp);
 
	fclose(fp);

	printf("\nwriteHead complete");
	return;
}

/*
* get the data length
*/
int getDataLen() {

	char num[10] = { 0 };
	FILE* fp = NULL;
    MMI_ARGON_MNC_T data;


	int c;
	int n = 0;
	int i = 0;


	int totalNum = 0;
	int len = 0;
    short tempNum = 0;

	fp = fopen(pInput, "r");



	if (fp == NULL)
	{
		perror("\n open file ims_toggle.txt error");
		return(-1);
	}

	do
	{
		c = fgetc(fp);

		if (feof(fp))
		{
			//printf("read file complete");
			break;
		}
        
		if((c>='0') && (c <= '9')){
			if((n == 0) && (c == '0' )){
                //printf("ignore first 0\n");
			}else {
				//printf("%c\n", c);
			    num[n] = c;
			    n++;
			}
		}
	    else if(c == ','){

			totalNum ++;

			for(i =0 ;i <n; i++)
			{
			    num[i] = 0;
			}
			n =0 ;
		}
		else {
		    
		}
		//printf("%c", c);
	} while (1);
    len = sizeof(MMI_ARGON_MNC_T) * (totalNum/ARGON_MNC_NUM);
	//printf("\n data len %d ", len );
 
	fclose(fp);
	return len;
}

/*
* write MMI_ARGON_MNC_T data to output file
*/
int writeData() {
 
	char num[10] = { 0 };
	FILE* fp = NULL;
	FILE* wfp = NULL;
    MMI_ARGON_MNC_T data;


	int c;
	int n = 0;
	int i = 0;
	int j = 0;

	int totalNum = 0;
	int len = 0;
    short tempNum = 0;



	fp = fopen(pInput, "r");

	wfp = fopen(pOutput, "ab+");
    memset(&data,0,sizeof(MMI_ARGON_MNC_T));
	if (fp == NULL)
	{
		perror("\n open file error");
		return  -1;
	}

	do
	{
		c = fgetc(fp);

		if (feof(fp))
		{
			//printf("read file complete");
			break;
		}
        
		if((c>='0') && (c <= '9')){
			if((n == 0) && (c == '0' )){
                //printf("ignore first 0\n");
			}else {
				//printf("%c\n", c);
			    num[n] = c;
			    n++;
			}
		}

	    else if(c == ','){	
            //printf("new num %d \n", atoi(num));
            tempNum =  (uint16)atoi(num);           
            
            switch (totalNum % ARGON_MNC_NUM){
			case 0:
                data.mcc = tempNum;
				break;
			case 1:
				data.mnc = tempNum;
				break;
			case 2:
	            data.ims_call_icon = (char)tempNum;
				break;
			case 3:
			    data.ims_icon_status = (char)tempNum;
				break;
			case 4:
				data.ims_toggle_status = (char)tempNum;
				break;
			case 5:
				data.reserved = (char)tempNum;
				break;
			default:
				break;
			}
			totalNum ++;
			if((totalNum % ARGON_MNC_NUM) == 0){
				 //printf("\n mnc %d ims_call_icon %c\n", data.mnc,data.ims_call_icon);
                 fwrite(&data, sizeof(MMI_ARGON_MNC_T), 1, wfp);
			}
			for(i =0 ;i <n; i++)
			{
			    num[i] = 0;
			}
			n =0 ;
		}
		else {
		    
		}
		
	} while (1);

	fclose(wfp);
	fclose(fp);
	printf("\nwrite data complete");
	return 1;
}


/*
* write bin file to output file
*/
void writeBin(){

	//detal_nv.bin
	FILE* fp = NULL;
	FILE* wfp = NULL;

	char c;

	fp = fopen("detal_nv.bin", "rb");

	wfp = fopen(pOutput, "ab+");

	if (fp == NULL)
	{
		perror("\n open file error");
		return ;
	}

	if (wfp == NULL)
	{
		perror("\nopen file error");
		return ;
	}

	do
	{
		c = fgetc(fp);

		if (feof(fp))
		{
			//printf("read file complete");
			break;
		}
        
		fputc(c,wfp);

	} while (1);

	fclose(wfp);
	fclose(fp);

	printf("\nwrite bin complete");

}

int main(int argc , char* argv[])
{
    /*
	int i;
	for(i = 0;i<argc; i++){
	    printf("%s\n", argv[i]);
	}
	*/
	if(argc == 1){
	    printf("Use defalut input file: %s and default output file: %s ", pInput, pOutput);
	}
	else if(argc == 2) {		
	    pInput = argv[1];
		printf("Use defalut output %s ",   pOutput);
	}
	else if(argc == 3)
	{
	    pInput = argv[1];
		pOutput = argv[2];
	} else {
	
	    printf("Please input right file name, for example:\n ImsToggleConverter.exe ims_toggle.txt result  ");
		return -1;
	}

	writeHead ( getDataLen() );

    writeData();

    //writeBin();

	return 1;
}

