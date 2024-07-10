#include "fdl_stdio.h"
#include "fdl_crc.h"
//#include "cmd_def.h"
#include "sci_types.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/*********************************************************************/

unsigned int crc_16_l_calc (char *buf_ptr,unsigned int len)
{
    unsigned int i;
    unsigned short crc = 0;

    while (len--!=0)
    {
        for (i = CRC_16_L_SEED; i !=0 ; i = i>>1)
        {
            if ( (crc & CRC_16_L_POLYNOMIAL) !=0)
            {
                crc = crc << 1 ;
                crc = crc ^ CRC_16_POLYNOMIAL;
            }
            else
            {
                crc = crc << 1 ;
            }

            if ( (*buf_ptr & i) != 0) /*lint !e737*/
            {
                crc = crc ^ CRC_16_POLYNOMIAL;
            }
        }

        buf_ptr++;
    }

    return (crc);
}

unsigned short frm_chk (const unsigned short *src, int len)
{
    unsigned int sum = 0;

    while (len > 3)
    {
        sum += *src++;
        sum += *src++;
        len -= 4;
    }

    switch (len&0x03)
    {
        case 2:
            sum += *src++;
            break;
        case 3:
            sum += *src++;
            sum += * ( (unsigned char *) src);
            break;
        case 1:
            sum += * ( (unsigned char *) src);
            break;
        default:
            break;
    }

    sum = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);
    return (~sum);
}
//This param src must be 4 byte aligned.
unsigned long Get_CheckSum (const unsigned char *src, int len)
{
    unsigned long sum =0;

    while (len > 3)
    {
        //sum += *((unsigned long *)src)++;
        sum += *src++;
        sum += *src++;
        sum += *src++;
        sum += *src++;

        len-=4;
    }

    while (len)
    {
        sum +=*src++;
        len--;
    }

    return sum;
}
/**
 * crc16 - compute the CRC-16 for the data buffer
 * @crc:        previous CRC value
 * @buffer:     data pointer
 * @len:        number of bytes in the buffer
 *
 * Returns the updated CRC value.
 */
/*lint -e765 ,"crc16" refered by fdl_update_fixnv.c */
PUBLIC uint16 crc16 (uint16 crc, uint8 const *buffer, uint32 len)
{
    uint16 crc2 = 0;
    uint16 c    = 0;
    uint16 j    = 0;

    while (len--)
    {
        crc2 = 0;
        c = (crc^ (*buffer++)) &0xff;

        for (j=0; j<8; j++)
        {

            if ( (crc2 ^ c) & 0x0001)
            {
                crc2 = (crc2 >> 1) ^ 0xA001;
            }
            else
            {
                crc2 =   crc2 >> 1;
            }

            c = c >> 1;
        }

        crc = (uint16) ( (crc >> 8) ^crc2);
    }

    return crc;
}
/**---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
