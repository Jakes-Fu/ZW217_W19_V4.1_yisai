// AUTO GENERATED

#define TAG_verify_ApSide 0x5b85f17a
struct IPAR_verify_ApSide
{
    ap_pkt_t verifycode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_verify_ApSide));

struct OPAR_verify_ApSide
{
    uint8 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_verify_ApSide));

#define TAG_verify_CpSide 0xa1cc34ff
struct OPAR_verify_CpSide
{
    uint8 result;
    ap_pkt_t verifycode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_verify_CpSide));

#define TAG_verify_Cp_result 0x90f70866
struct IPAR_verify_Cp_result
{
    uint8 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_verify_Cp_result));

struct OPAR_verify_Cp_result
{
    uint8 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_verify_Cp_result));


// AUTO GENERATED END
