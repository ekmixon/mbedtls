/**
 * \file psa/crypto.h
 * \brief Platform Security Architecture cryptography module
 */

#ifndef PSA_CRYPTO_H
#define PSA_CRYPTO_H

#include "crypto_platform.h"

#include <stddef.h>

#ifdef __DOXYGEN_ONLY__
/** \defgroup platform Implementation-specific definitions
 * @{
 */

/** \brief Key slot number.
 *
 * This type represents key slots. It must be an unsigned integral
 * type.* The choice of type is implementation-dependent.
 * 0 is not a valid key slot number. The meaning of other values is
 * implementation dependent.
 *
 * At any given point in time, each key slot either contains a
 * cryptographic object, or is empty. Key slots are persistent:
 * once set, the cryptographic object remains in the key slot until
 * explicitly destroyed.
 */
typedef _unsigned_integral_type_ psa_key_slot_t;

/**@}*/
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup basic Basic definitions
 * @{
 */

/**
 * \brief Function return status.
 *
 * Zero indicates success, anything else indicates an error.
 */
typedef enum {
    /** The action was completed successfully. */
    PSA_SUCCESS = 0,
    /** The requested operation or a parameter is not supported
        by this implementation. */
    PSA_ERROR_NOT_SUPPORTED,
    /** The requested action is denied by a policy. */
    PSA_ERROR_NOT_PERMITTED,
    /** An output buffer is too small. */
    PSA_ERROR_BUFFER_TOO_SMALL,
    /** A slot is occupied, but must be empty to carry out the
        requested action. */
    PSA_ERROR_OCCUPIED_SLOT,
    /** A slot is empty, but must be occupied to carry out the
        requested action. */
    PSA_ERROR_EMPTY_SLOT,
    /** The requested action cannot be performed in the current state. */
    PSA_ERROR_BAD_STATE,
    /** The parameters passed to the function are invalid. */
    PSA_ERROR_INVALID_ARGUMENT,
    /** There is not enough runtime memory. */
    PSA_ERROR_INSUFFICIENT_MEMORY,
    /** There is not enough persistent storage. */
    PSA_ERROR_INSUFFICIENT_STORAGE,
    /** There was a communication failure inside the implementation. */
    PSA_ERROR_COMMUNICATION_FAILURE,
    /** A hardware failure was detected. */
    PSA_ERROR_HARDWARE_FAILURE,
    /** A tampering attempt was detected. */
    PSA_ERROR_TAMPERING_DETECTED,
    /** There is not enough entropy to generate random data needed
        for the requested action. */
    PSA_ERROR_INSUFFICIENT_ENTROPY,
    /** The signature or MAC is incorrect. */
    PSA_ERROR_INVALID_SIGNATURE,
    /** An error occurred that does not correspond to any defined
        failure cause. */
    PSA_ERROR_UNKNOWN_ERROR,
} psa_status_t;

/**
 * \brief Library initialization.
 *
 * Applications must call this function before calling any other
 * function in this module.
 *
 * Applications may call this function more than once. Once a call
 * succeeds, subsequent calls are guaranteed to succeed.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_INSUFFICIENT_MEMORY
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 *         * \c PSA_ERROR_INSUFFICIENT_ENTROPY
 */
psa_status_t psa_crypto_init(void);

#define BITS_TO_BYTES(bits) (((bits) + 7) / 8)
#define BYTES_TO_BITS(bytes) ((bytes) * 8)

/**@}*/

/** \defgroup crypto_types Key and algorithm types
 * @{
 */

typedef uint32_t psa_key_type_t;

#define PSA_KEY_TYPE_NONE                       0x00000000
#define PSA_KEY_TYPE_RAW_DATA                   0x00000001
#define PSA_KEY_TYPE_RSA_PUBLIC_KEY             0x40000001
#define PSA_KEY_TYPE_RSA_KEYPAIR                0x60000001
#define PSA_KEY_TYPE_ECC_BASE                   0x40010000

#define PSA_KEY_TYPE_VENDOR_FLAG                0x80000000
#define PSA_KEY_TYPE_ASYMMETRIC_FLAG            0x40000000
#define PSA_KEY_TYPE_ASYMMETRIC_MASK            0x60000000
#define PSA_KEY_TYPE_ASYMMETRIC_MASK_PUBLIC     0x40000000
#define PSA_KEY_TYPE_ASYMMETRIC_MASK_KEYPAIR    0x60000000
#define PSA_KEY_TYPE_ASYMMETRIC_TEST_MASK       0x5fff0000
#define PSA_KEY_TYPE_RSA_TEST_VALUE             0x40000000
#define PSA_KEY_TYPE_ECC_TEST_VALUE             0x40010000

#define PSA_KEY_TYPE_IS_VENDOR(type) \
    (((type) & PSA_KEY_TYPE_VENDOR_FLAG) != 0)
#define PSA_KEY_TYPE_IS_ASYMMETRIC(type) \
    (((type) & PSA_KEY_TYPE_ASYMMETRIC_FLAG) != 0)
#define PSA_KEY_TYPE_IS_PUBLIC_KEY(type) \
    (((type) & PSA_KEY_TYPE_ASYMMETRIC_MASK) == PSA_KEY_TYPE_ASYMMETRIC_MASK_PUBLIC)
#define PSA_KEY_TYPE_IS_KEYPAIR(type) \
    (((type) & PSA_KEY_TYPE_ASYMMETRIC_MASK) == PSA_KEY_TYPE_ASYMMETRIC_MASK_KEYPAIR)
#define PSA_KEY_TYPE_IS_RSA(type)                                       \
    (((type) & PSA_KEY_TYPE_ASYMMETRIC_TEST_MASK) == PSA_KEY_TYPE_RSA_TEST_VALUE)
#define PSA_KEY_TYPE_IS_ECC(type)                                       \
    (((type) & PSA_KEY_TYPE_ASYMMETRIC_TEST_MASK) == PSA_KEY_TYPE_ECC_TEST_VALUE)

typedef uint32_t psa_algorithm_t;

#define PSA_ALG_HASH_BITS                       0x01000000
#define PSA_ALG_RSA_HASH_MASK                   0x000000ff
#define PSA_ALG_MD2                             0x01000001
#define PSA_ALG_MD4                             0x01000002
#define PSA_ALG_MD5                             0x01000003
#define PSA_ALG_SHA_256_128                     0x01000004
#define PSA_ALG_RIPEMD160                       0x01000005
#define PSA_ALG_SHA_1                           0x01000006
#define PSA_ALG_SHA_256_160                     0x01000007
#define PSA_ALG_SHA_224                         0x01000008
#define PSA_ALG_SHA_256                         0x01000009
#define PSA_ALG_SHA_384                         0x0100000a
#define PSA_ALG_SHA_512                         0x0100000b
#define PSA_ALG_SHA_512_224                     0x0100000c
#define PSA_ALG_SHA_512_256                     0x0100000d
#define PSA_ALG_SHA3_224                        0x01000010
#define PSA_ALG_SHA3_256                        0x01000011
#define PSA_ALG_SHA3_384                        0x01000012
#define PSA_ALG_SHA3_512                        0x01000013

#define PSA_ALG_RSA_PKCS1V15_RAW                0x40000100
#define PSA_ALG_RSA_PSS_MGF1                    0x40000200
#define PSA_ALG_RSA_OAEP                        0x40000300
#define PSA_ALG_RSA_PKCS1V15(hash_alg)          \
    (PSA_ALG_RSA_PKCS1V15_RAW | ((hash_alg) & PSA_ALG_RSA_HASH_MASK))
#define PSA_ALG_IS_RSA_PKCS1V15(alg)            \
    (((alg) & 0x7fffff00) == PSA_ALG_RSA_PKCS1V15_RAW)
#define PSA_ALG_RSA_GET_HASH(alg)               \
    (((alg) & PSA_ALG_RSA_HASH_MASK) | PSA_ALG_HASH_BITS)

#define PSA_ALG_VENDOR_FLAG                     0x80000000

/**@}*/

/** \defgroup key_management Key management
 * @{
 */

/**
 * \brief Import a key in binary format.
 *
 * This function supports any output from psa_export_key().
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_NOT_SUPPORTED
 *         * \c PSA_ERROR_INVALID_ARGUMENT
 *         * \c PSA_ERROR_INSUFFICIENT_MEMORY
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_import_key(psa_key_slot_t key,
                            psa_key_type_t type,
                            const uint8_t *data,
                            size_t data_length);

/**
 * \brief Destroy a key.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_EMPTY_SLOT
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_destroy_key(psa_key_slot_t key);

/**
 * \brief Get basic metadata about a key.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_EMPTY_SLOT
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_get_key_information(psa_key_slot_t key,
                                     psa_key_type_t *type,
                                     size_t *bits);

/**
 * \brief Export a key in binary format.
 *
 * The output of this function can be passed to psa_import_key() to
 * create an equivalent object.
 *
 * If a key is created with psa_import_key() and then exported with
 * this function, it is not guaranteed that the resulting data is
 * identical: the implementation may choose a different representation
 * of the same key.
 *
 * \return * \c PSA_SUCCESS: success.
 *         * \c PSA_ERROR_EMPTY_SLOT
 *         * \c PSA_ERROR_COMMUNICATION_FAILURE
 *         * \c PSA_ERROR_HARDWARE_FAILURE
 *         * \c PSA_ERROR_TAMPERING_DETECTED
 */
psa_status_t psa_export_key(psa_key_slot_t key,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length);


/**@}*/

/** \defgroup asymmetric Asymmetric cryptography
 * @{
 */

/**
 * \brief Maximum ECDSA signature size for a given curve bit size
 *
 * \param curve_bits    Curve size in bits
 * \return              Maximum signature size in bytes
 *
 * \note This macro returns a compile-time constant if its argument is one.
 *
 * \warning This macro may evaluate its argument multiple times.
 */
/*
 * RFC 4492 page 20:
 *
 *     Ecdsa-Sig-Value ::= SEQUENCE {
 *         r       INTEGER,
 *         s       INTEGER
 *     }
 *
 * Size is at most
 *    1 (tag) + 1 (len) + 1 (initial 0) + curve_bytes for each of r and s,
 *    twice that + 1 (tag) + 2 (len) for the sequence
 * (assuming curve_bytes is less than 126 for r and s,
 * and less than 124 (total len <= 255) for the sequence)
 */
#define PSA_ECDSA_SIGNATURE_SIZE(curve_bits)                          \
    ( /*T,L of SEQUENCE*/ ((curve_bits) >= 61 * 8 ? 3 : 2) +          \
      /*T,L of r,s*/       2 * (((curve_bits) >= 127 * 8 ? 3 : 2) +   \
      /*V of r,s*/               ((curve_bits) + 8) / 8))


#define PSA_ASYMMETRIC_SIGN_OUTPUT_SIZE(key_type, key_bits, alg)        \
    (PSA_KEY_TYPE_IS_RSA(key_type) ? ((void)alg, BITS_TO_BYTES(key_bits)) : \
     PSA_KEY_TYPE_IS_ECC(key_type) ? PSA_ECDSA_SIGNATURE_SIZE(key_bits) : \
     0)

/**
 * \brief Sign a hash or short message with a private key.
 *
 */
psa_status_t psa_asymmetric_sign(psa_key_slot_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *hash,
                                 size_t hash_length,
                                 const uint8_t *salt,
                                 size_t salt_length,
                                 uint8_t *signature,
                                 size_t signature_size,
                                 size_t *signature_length);

/**
 * \brief Verify the signature a hash or short message using a public key.
 *
 */
psa_status_t psa_asymmetric_verify(psa_key_slot_t key,
                                   psa_algorithm_t alg,
                                   const uint8_t *hash,
                                   size_t hash_length,
                                   const uint8_t *salt,
                                   size_t salt_length,
                                   uint8_t *signature,
                                   size_t signature_size);

/**@}*/

#ifdef __cplusplus
}
#endif

#include "crypto_extra.h"

#endif /* PSA_CRYPTO_H */
