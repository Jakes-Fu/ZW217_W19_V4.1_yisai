#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cputable.h"

typedef struct cpucaps_s {
	int cpuType;
	int cpuStepping;
	int hasMMX;
	int hasMMX2;
	int has3DNow;
	int has3DNowExt;
	int hasSSE;
	int hasSSE2;
	int isX86;
	unsigned cl_size; /* size of cache line */
	int hasAltiVec;
	int hasTSC;
	int freq;
	char vendor[20];
	char* cpuFullName;
} CpuCaps;

#include "cpuinfo.h"

#define rdtsc(var) _asm rdtsc _asm shrd eax,edx,10 _asm mov var,eax

static void do_cpuid(unsigned int level, unsigned int *p)
{
	__asm{
		mov	eax,level
			cpuid
			mov esi,p
			mov [esi+4],ebx
			mov [esi+12],edx
			mov [esi+8],ecx
			mov [esi+0],eax
	};
}

#define CPUID_EXTFAMILY	((regs2[0] >> 20)&0xFF) /* 27..20 */
#define CPUID_EXTMODEL	((regs2[0] >> 16)&0x0F) /* 19..16 */
#define CPUID_TYPE		((regs2[0] >> 12)&0x04) /* 13..12 */
#define CPUID_FAMILY	((regs2[0] >>  8)&0x0F) /* 11..08 */
#define CPUID_MODEL		((regs2[0] >>  4)&0x0F) /* 07..04 */
#define CPUID_STEPPING	((regs2[0] >>  0)&0x0F) /* 03..00 */

unsigned long MeasureCPUSpeed(int fStartMeasure)
{
	static unsigned long tscstart, start = 0;
	unsigned long tscstop, stop;

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	stop = GetTickCount();
	rdtsc(tscstop);
	if (fStartMeasure || start == 0 || tscstop < tscstart) {
		// re-measuring
		start = GetTickCount();
		rdtsc(tscstart);
		if (!fStartMeasure) stop = start;
	}
	if (stop == start) {
		Sleep(100);
		stop = GetTickCount();
		rdtsc(tscstop);
	}
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	return (tscstop-tscstart)/(stop-start);
}

char *GetCpuFriendlyName(CpuCaps *caps, unsigned int regs[], unsigned int regs2[]){
	char *retname;
	int i=0;

	if (!(retname=(char*)malloc(256))) {
		return NULL;
	}

	sprintf(caps->vendor,"%.4s%.4s%.4s",(char*)(regs+1),(char*)(regs+3),(char*)(regs+2));

	for(i=0; i<MAX_VENDORS; i++){
		if(!strcmp(cpuvendors[i].string,caps->vendor)){
			if(cpuname[i][CPUID_FAMILY][CPUID_MODEL]){
				sprintf(retname,"%s %s",cpuvendors[i].name,cpuname[i][CPUID_FAMILY][CPUID_MODEL]);
			}
		}
	}

	return retname;
}

void GetCpuCaps( CpuCaps *caps)
{
	unsigned int regs[4];
	unsigned int regs2[4];
	char buf[512], *p = buf;

	memset(caps, 0, sizeof(CpuCaps));
	caps->isX86=1;
	caps->cl_size=32; /* default */

	do_cpuid(0x00000000, regs); // get _max_ cpuid level and vendor name
	//p += sprintf(p, "%.4s%.4s%.4s\r\n", (char*) (regs+1),(char*) (regs+3),(char*) (regs+2));
	if (regs[0]>=0x00000001)
	{
		char *tmpstr;
		unsigned cl_size;

		do_cpuid(0x00000001, regs2);

		caps->cpuType=(regs2[0] >> 8)&0xf;
		if(caps->cpuType==0xf){
			// use extended family (P4, IA64)
			caps->cpuType=8+((regs2[0]>>20)&255);
		}
		caps->cpuStepping=regs2[0] & 0xf;

		// general feature flags:
		caps->hasTSC  = (regs2[3] & (1 << 8  )) >>  8; // 0x0000010
		caps->hasMMX  = (regs2[3] & (1 << 23 )) >> 23; // 0x0800000
		caps->hasSSE  = (regs2[3] & (1 << 25 )) >> 25; // 0x2000000
		caps->hasSSE2 = (regs2[3] & (1 << 26 )) >> 26; // 0x4000000
		caps->hasMMX2 = caps->hasSSE; // SSE cpus supports mmxext too
		cl_size = ((regs2[1] >> 8) & 0xFF)*8;
		if(cl_size) caps->cl_size = cl_size;

		tmpstr=GetCpuFriendlyName(caps, regs, regs2);
		p += sprintf(p,"%s ",tmpstr);
		free(tmpstr);
		p += sprintf(p,"(Family: %d, Stepping: %d)",
			caps->cpuType, caps->cpuStepping);

	}
	do_cpuid(0x80000000, regs);
	p += sprintf(p, " / Internal Cache: %u KB",caps->cl_size);
	caps->cpuFullName = strdup(buf);
}
