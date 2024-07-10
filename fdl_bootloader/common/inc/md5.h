#ifndef _MD5AMR_H_
#define _MD5AMR_H_
#define u32 unsigned int
#define u64 unsigned long long
#define u8 unsigned char

int  md5_init();
/*the unit of inputLen is 8bit*/
int md5_update(const u8 *data, unsigned int len);
int md5_final(u8 *out);

#endif
