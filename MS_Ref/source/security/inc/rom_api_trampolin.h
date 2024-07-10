
#ifndef __RDA_ROM_API_H__
#define __RDA_ROM_API_H__

#define inline 
struct chip_id {
	// ASIC information
	uint32  chip;     // CHIP(31:15) NA(15:15) BOND(14:12) METAL(11:0)
	uint32  res1;
	// Production information
	uint16  date;     // YYYY:MMMM:DDDDD
	uint16  wafer;
	uint16  xy;
	uint16  res2;
};

struct chip_security_context {
	// Security context
	uint8   rda_key_index; // 0-5 : 0 default key
	uint8   vendor_id;     // 0-50
	uint16  flags;
#define RDA_SE_CFG_UNLOCK_ALLOWED       (1<<0)
#define RDA_SE_CFG_SECURITY_ENABLE_BIT  (1<<4)
#define RDA_SE_CFG_INDIRECT_SIGN_BIT    (1<<5)
#define RDA_SE_CFG_RDCERT_DISABLE_BIT   (1<<6)
#define RDA_SE_CFG_TRACE_DISABLE_BIT    (1<<7)
};

struct chip_unique_id {
	uint8	  id[32];
};

/* security */
#define RDASIGN     "RDAS"
#define PKALG       "Ed"
#define KDFALG      "BK"

#define SIGBYTES    64
#define PUBLICBYTES 32
#define SECRETBYTES 64
#define FPLEN        8

#define UNIQUE_ID_ANTI_CLONE 0
#define UNIQUE_ID_RD_CERT 1

struct pubkey {
	uint8 rdasign[4];//RDAS
	uint8 pkalg[2];  //Ed
	uint8 dummy[2];
	uint8 name[16];
	uint8 fingerprint[FPLEN];
	uint8 pubkey[PUBLICBYTES];
};

// RDASEdPo
struct sig {
	uint8 rdasign[4];//RDAS
	uint8 pkalg[2];  //Ed
	uint8 hashalg[2];//Po/B2/SH
	uint8 name[16];
	uint8 fingerprint[FPLEN];
	uint8 sig[SIGBYTES];
};


/* ROM Cryto API */

struct ROM_crypto_api {
	char     magic[8];  // "RDA API"
	unsigned version;   // 100

	// signature
	int (*signature_open) (
		const uint8 *message, unsigned length,
		const struct sig    *sig,
		const struct pubkey *pubkey);
/* Return values */
#define ROM_API_SIGNATURE_OK      0
#define ROM_API_SIGNATURE_FAIL   -1
// positive values is for invalid arguments

	// hash
	unsigned sz_hash_context;
	int (*hash_init) ( unsigned *S, uint8 outlen );
	int (*hash_update) ( unsigned *S, const uint8 *in, unsigned inlen );
	int (*hash_final) ( unsigned *S, uint8 *out, uint8 outlen );

	// info API
	void (*get_chip_id) (struct chip_id *id);
	void (*get_chip_unique) (struct chip_unique_id *out, int kind);
	int (*get_chip_security_context) (struct chip_security_context *context,
                                          struct pubkey *pubkey);
/* Return values */
#define ROM_API_SECURITY_ENABLED      0
#define ROM_API_SECURITY_DISABLED     1
#define ROM_API_INVALID_KEYINDEX      2
#define ROM_API_INVALID_VENDOR_ID     3
#define ROM_API_SECURITY_UNAVAILABLE  4

	// RND
	void (*get_chip_true_random) (uint8 *out, uint8 outlen);

	// NEW in v101:
	int (*signature_open_w_hash)(uint8 message_hash[64],
                              const struct sig    *signature,
                              const struct pubkey *pubkey);
	void (*decrypt_aes_image)(uint8 *buf, int len);
/* Return values */
// same as signature_open() plus ROM_API_UNAVAILABLE if method is unavailable.
#define ROM_API_UNAVAILABLE 100

	// Future extension
	unsigned dummy[15];
};

#ifndef BOOTROM

// The ROMAPI is allocated either at 0x3f00 or 0xff00.
#define ROMAPI_BASE        0x3f00
#define ROMAPI_BASE_LIST   ROMAPI_BASE, 0xff00, 0

struct ROM_crypto_api *romapi;

/* Detect the ROMAPI base address */


// info API

static void inline get_chip_id(struct chip_id *id)
{
	romapi->get_chip_id(id);
}

/* Replaced with a enhanced version in arch/arm/rda/rda_romapi.c */
int get_chip_security_context(struct chip_security_context *context, struct pubkey *pubkey);
/*
static inline int get_chip_security_context(struct chip_security_context *context, struct pubkey *pubkey)
{
	return romapi->get_chip_security_context(context, pubkey);
}*/


//
// RND
//
static inline void get_chip_true_random(uint8 *out, uint8 outlen)
{
	romapi->get_chip_true_random(out, outlen);
}

//
// SIGNATURE CHECK
//
static inline int signature_check(
		const uint8 *message, unsigned length,
		const struct sig    *sig,
		const struct pubkey *pubkey)
{
	return romapi->signature_open(message, length, sig, pubkey);
}

// NEW in v101:
//
// SIGNATURE CHECK variant that can be used to check larger than RAM messages
//
static inline int signature_check_w_hash(
		uint8 message_hash[64],
		const struct sig    *sig,
		const struct pubkey *pubkey)
{
	if (romapi->signature_open_w_hash)
		return romapi->signature_open_w_hash(message_hash, sig, pubkey);
	else
		return ROM_API_UNAVAILABLE;
}

static inline void decrypt_aes_image(uint8 *buf, int len)
{
        romapi->decrypt_aes_image(buf, len);
}
#endif
#endif
