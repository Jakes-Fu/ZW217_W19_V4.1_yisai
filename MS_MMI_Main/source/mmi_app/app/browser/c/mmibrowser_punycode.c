/*****************************************************************************
** File Name:      mmibrowser_punycode.c                                     *
** Author:         jiaoyou.wu                                                *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmibrowser_punycode.h"
#include "mmk_type.h"
#include "mmi_common.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef MMI_PUNYCODE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

LOCAL const uint32 s_maxint = -1;

enum {
    MMI_PUNYCODE_BASE = 36,
    MMI_PUNYCODE_TMIN = 1,
    MMI_PUNYCODE_TMAX = 26,
    MMI_PUNYCODE_SKEW = 38,
    MMI_PUNYCODE_DAMP = 700,
    MMI_PUNYCODE_INITIAL_BIAL = 72,
    MMI_PUNYCODE_INITIAL_N = 0x80,
    MMI_PUNYCODE_DELIMITER = 0x2D
};

#define MMI_PUNYCODE_BASIC(cp) ((uint32)(cp) < 0x80)
#define MMI_PUNYCODE_DELIM(cp) ((cp) == MMI_PUNYCODE_DELIMITER)
#define MMI_PUNYCODE_INDICATOR "xn--"
#define MMI_PUNYCODE_FLAGGED(bcp) ((uint32)(bcp) - 65 < 26)

LOCAL uint32 decode_digit(uint32 cp)
{
    return  cp - 48 < 10 ? cp - 22 :  cp - 65 < 26 ? cp - 65 :
          cp - 97 < 26 ? cp - 97 :  MMI_PUNYCODE_BASE;
}

LOCAL char encode_digit(uint32 d, int flag)
{
    return d + 22 + 75 * (d < 26) - ((flag != 0) << 5);
}

LOCAL char encode_basic(uint32 bcp, int flag)
{
    bcp -= (bcp - 97 < 26) << 5;
    return bcp + ((!flag && (bcp - 65 < 26)) << 5);
}

LOCAL uint32 adapt(
  uint32 delta, uint32 numpoints, int firsttime )
{
    uint32 k;

    delta = firsttime ? delta / MMI_PUNYCODE_DAMP : delta >> 1;
    delta += delta / numpoints;

    for (k = 0;  delta > ((MMI_PUNYCODE_BASE - MMI_PUNYCODE_TMIN) * MMI_PUNYCODE_TMAX) / 2;  k += MMI_PUNYCODE_BASE) {
    delta /= MMI_PUNYCODE_BASE - MMI_PUNYCODE_TMIN;
    }

    return k + (MMI_PUNYCODE_BASE - MMI_PUNYCODE_TMIN + 1) * delta / (delta + MMI_PUNYCODE_SKEW);
}

LOCAL MMI_PUNYCODE_E punycode_encode(
    uint32 input_length,
    const uint32 input[],
    const unsigned char case_flags[],
    uint32 *output_length,
    char output[] )
{
    uint32 n, delta, h, b, out, max_out, bias, j, m, q, k, t;

    n = MMI_PUNYCODE_INITIAL_N;
    delta = out = 0;
    max_out = *output_length;
    bias = MMI_PUNYCODE_INITIAL_BIAL;

    for (j = 0;  j < input_length;  ++j) {
    if (MMI_PUNYCODE_BASIC(input[j])) {
      if (max_out - out < 2) return MMI_PUNYCODE_BIG_OUTPUT;
      output[out++] =
        case_flags ?  encode_basic(input[j], case_flags[j]) : input[j];
    }
    }

    h = b = out;

    if (b > 0) output[out++] = MMI_PUNYCODE_DELIMITER;

    while (h < input_length) {
    for (m = s_maxint, j = 0;  j < input_length;  ++j) {
      if (input[j] >= n && input[j] < m) m = input[j];
    }

    if (m - n > (s_maxint - delta) / (h + 1)) return MMI_PUNYCODE_OVERFLOW;
    delta += (m - n) * (h + 1);
    n = m;

    for (j = 0;  j < input_length;  ++j) {
      if (input[j] < n) {
        if (++delta == 0) return MMI_PUNYCODE_OVERFLOW;
      }

      if (input[j] == n) {

        for (q = delta, k = MMI_PUNYCODE_BASE;  ;  k += MMI_PUNYCODE_BASE) {
          if (out >= max_out) return MMI_PUNYCODE_BIG_OUTPUT;

          t = k <= bias ? MMI_PUNYCODE_TMIN :
              k >= bias + MMI_PUNYCODE_TMAX ? MMI_PUNYCODE_TMAX : k - bias;
          if (q < t) break;
          output[out++] = encode_digit(t + (q - t) % (MMI_PUNYCODE_BASE - t), 0);
          q = (q - t) / (MMI_PUNYCODE_BASE - t);
        }

        output[out++] = encode_digit(q, case_flags && case_flags[j]);
        bias = adapt(delta, h + 1, h == b);
        delta = 0;
        ++h;
      }
    }

    ++delta, ++n;
    }

    *output_length = out;
    return MMI_PUNYCODE_SUCCESS;
}

LOCAL MMI_PUNYCODE_E punycode_decode(
    uint32 input_length,
    const char input[],
    uint32 *output_length,
    uint32 output[],
    unsigned char case_flags[] )
{
    uint32 n, out, i, max_out, bias,
                 b, j, in, oldi, w, k, digit, t;

    n = MMI_PUNYCODE_INITIAL_N;
    out = i = 0;
    max_out = *output_length;
    bias = MMI_PUNYCODE_INITIAL_BIAL;

    for (b = j = 0;  j < input_length;  ++j) if (MMI_PUNYCODE_DELIM(input[j])) b = j;
    if (b > max_out) return MMI_PUNYCODE_BIG_OUTPUT;

    for (j = 0;  j < b;  ++j) {

    if (case_flags) case_flags[out] = MMI_PUNYCODE_FLAGGED(input[j]);
    if (!MMI_PUNYCODE_BASIC(input[j])) return MMI_PUNYCODE_BAD_INPUT;
    output[out++] = input[j];
    }

    for (in = b > 0 ? b + 1 : 0;  in < input_length;  ++out) {

    for (oldi = i, w = 1, k = MMI_PUNYCODE_BASE;  ;  k += MMI_PUNYCODE_BASE) {
      if (in >= input_length) return MMI_PUNYCODE_BAD_INPUT;
      digit = decode_digit(input[in++]);
      if (digit >= MMI_PUNYCODE_BASE) return MMI_PUNYCODE_BAD_INPUT;
      if (digit > (s_maxint - i) / w) return MMI_PUNYCODE_OVERFLOW;
      i += digit * w;
      t = k <= bias ? MMI_PUNYCODE_TMIN :
          k >= bias + MMI_PUNYCODE_TMAX ? MMI_PUNYCODE_TMAX : k - bias;
      if (digit < t) break;
      if (w > s_maxint / (MMI_PUNYCODE_BASE - t)) return MMI_PUNYCODE_OVERFLOW;
      w *= (MMI_PUNYCODE_BASE - t);
    }

    bias = adapt(i - oldi, out + 1, oldi == 0);


    if (i / (out + 1) > s_maxint - n) return MMI_PUNYCODE_OVERFLOW;
    n += i / (out + 1);
    i %= (out + 1);

    if (out >= max_out) return MMI_PUNYCODE_BIG_OUTPUT;

    if (case_flags) {
      memmove(case_flags + i + 1, case_flags + i, out - i);

      case_flags[i] = MMI_PUNYCODE_FLAGGED(input[in - 1]);
    }

    memmove(output + i + 1, output + i, (out - i) * sizeof *output);
    output[i++] = n;
    }

    *output_length = out;
    return MMI_PUNYCODE_SUCCESS;
}

PUBLIC MMIBROWSER_Unicode_punycode_encode(uint16 *input, uint16 input_len, int32 *output_len, char *output)
{
    char                *encode = PNULL;
    char                *temp = PNULL;
    uint32       *puny_array = PNULL;
    uint16              cnt = 0;
    int32               i = 0;
    uint16              array_cnt = 0;
    uint16              temp_cnt = 0;
    uint32       out_len = 0;
    BOOLEAN             has_dot = FALSE;
    BOOLEAN             is_all_ascii = TRUE;
    BOOLEAN             ret = TRUE;
    BOOLEAN             is_valid = FALSE;

    if (input_len >= CHAR_MAX)
    {
        return FALSE;
    }

    do {
        encode = SCI_ALLOCAZ(CHAR_MAX + 1);
        temp = SCI_ALLOCAZ(CHAR_MAX + 1);
        puny_array = SCI_ALLOCAZ((CHAR_MAX + 1) * sizeof(uint32));

        if (PNULL == encode || PNULL == encode || PNULL == puny_array)
        {
            ret = FALSE;
            break;
        }

        for(i =0; i < input_len; i++)
        {
            if ('.' == input[i])
            {
                if ((i + 1) == input_len)//'.' is the last char
                {
                    ret = FALSE;
                    break;
                }
                has_dot = TRUE;
                is_valid = TRUE;
            }
            else
            {
                if (is_all_ascii)
                {
                    if (MMI_PUNYCODE_BASIC(input[i]))
                    {
                        temp[temp_cnt++] = input[i];
                    }
                    else
                    {
                        is_all_ascii = FALSE;
                        SCI_MEMSET(temp, 0, sizeof(temp));
                        temp_cnt = 0;
                    }
                }
                puny_array[array_cnt++] = input[i];

                if ((i + 1) != input_len)// Not the last char
                {
                    continue;
                }
            }

            if (is_all_ascii)
            {
                if (temp_cnt != 0)
                {
                    SCI_MEMCPY(encode + cnt, temp, temp_cnt);
                    cnt += temp_cnt;
                    if (has_dot)
                    {
                        encode[cnt++] = '.';
                    }
                }
                temp_cnt = 0;
                has_dot = FALSE;
                is_all_ascii = TRUE;
                continue;
            }
            out_len = CHAR_MAX;
            if (MMI_PUNYCODE_SUCCESS == punycode_encode(array_cnt, puny_array, PNULL, &out_len, output))
            {
                uint16 len = 0;

                if (out_len + cnt >= CHAR_MAX)
                {
                    ret = FALSE;
                    break;
                }

                len = strlen(MMI_PUNYCODE_INDICATOR);

                SCI_MEMCPY(encode + cnt, MMI_PUNYCODE_INDICATOR, len);
                cnt += len;

                SCI_MEMCPY(encode + cnt, output, out_len);
                cnt += out_len;
                if (has_dot)
                {
                    encode[cnt++] = '.';
                }

                SCI_MEMSET(puny_array, 0, sizeof(puny_array));
                SCI_MEMSET(output, 0, sizeof(output));
                array_cnt = 0;
                temp_cnt = 0;
                has_dot = FALSE;
                is_all_ascii = TRUE;
            }
            else
            {
                ret = FALSE;
                break;
            }
        }
    } while(0);



    if (!is_valid || cnt > *output_len)
    {
        ret = FALSE;
    }

    if (ret && PNULL != encode)
    {
        SCI_MEMCPY(output, encode, cnt);
        *output_len = cnt;
    }

    if(PNULL != encode)
    {
        SCI_FREE(encode);
    }

    if(PNULL != temp)
    {
        SCI_FREE(temp);
    }

    if(PNULL != puny_array)
    {
        SCI_FREE(puny_array);
    }

    SCI_TRACE_LOW("[BRW]brw_Unicode_punycode_encode ret = %d cnt %d", ret, cnt);

    return ret;
}
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
