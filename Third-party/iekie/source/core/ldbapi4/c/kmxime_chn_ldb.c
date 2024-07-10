
#include "IIMEk2api.h"
#include "kmxime_latin.h"

#ifdef IEKIE_IM_LDB_TEST
#include "ldb_all_define.h"
#endif



#include "ldb_english_107.h"










#include "IIMEstd.h"
#include "provider_com.h"
#include "IIMEmtprovider.h"
//#include "ldbapi4.h"


#include "../utli/IIMEstring.h"


//µ¥×ÖÆ´Òô
const IIME_IME_INFO IIME_ldb_dzpy_gb2312_20k={IM_HINDI, &LDB_107};

const IIME_IME_INFO IIME_ldb_stroke_gb2312_mini={IM_HINDI, &LDB_107};


Provider_Com* WrappingProvider_Create(PIIME_IMM_PARAMS  pImmParams,
									  PIIME_USER_PARAMS pUserParam1,
									  PIIME_USER_PARAMS pUserParam2,
									  const void* pdbMisc,
									  const void* pdbMO,
                                      const IIME_MULTI_STRING_TABLE* pMulti_string_table,
									  IIME_INT idMethod);

void WrappingProvider_Delete(Provider_Com* pThis);




