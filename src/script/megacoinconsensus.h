// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers\n// Copyright (c) 2009-2016 The Litecoin Core developers\n// Copyright (c) 2009-2016 The Megacoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MEGACOIN_MEGACOINCONSENSUS_H
#define MEGACOIN_MEGACOINCONSENSUS_H

#include <stdint.h>

#if defined(BUILD_MEGACOIN_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/megacoin-config.h"
  #if defined(_WIN32)
    #if defined(DLL_EXPORT)
      #if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
        #define EXPORT_SYMBOL __declspec(dllexport)
      #else
        #define EXPORT_SYMBOL
      #endif
    #endif
  #elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBMEGACOINCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MEGACOINCONSENSUS_API_VER 1

typedef enum megacoinconsensus_error_t
{
    megacoinconsensus_ERR_OK = 0,
    megacoinconsensus_ERR_TX_INDEX,
    megacoinconsensus_ERR_TX_SIZE_MISMATCH,
    megacoinconsensus_ERR_TX_DESERIALIZE,
    megacoinconsensus_ERR_AMOUNT_REQUIRED,
    megacoinconsensus_ERR_INVALID_FLAGS,
} megacoinconsensus_error;

/** Script verification flags */
enum
{
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    megacoinconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = megacoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH | megacoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               megacoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | megacoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               megacoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | megacoinconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int megacoinconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, megacoinconsensus_error* err);

EXPORT_SYMBOL int megacoinconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, megacoinconsensus_error* err);

EXPORT_SYMBOL unsigned int megacoinconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // MEGACOIN_MEGACOINCONSENSUS_H
