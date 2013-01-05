/*
 * =====================================================================================
 *
 *       Filename:  crypt.h
 *
 *    Description:  Cryptography library using gmp
 *
 *        Version:  1.0
 *        Created:  06/12/2012 13:19:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

//TODO: 
// * Random number management                       DONE
// * Echange de clé de diffie hellman avec gmp
// * Interface utilisateur (ncurse??)
// * Signatures

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifndef CRYPT_H
#define CRYPT_H 0

#define NB_TURN 10
#define EG_KEY_SIZE 14
#define DH_KEY_SIZE 14
#define RSA_KEY_SIZE 256

// Number of supported characters
#define NB_CHAR 256

/*-----------------------------------------------------------------------------
 *  RSA
 *-----------------------------------------------------------------------------*/

typedef struct rsaPrivateKey {
    mpz_t p, q, n, phi, pvk;
    // It's the greatest integer k such that NB_CHAR^k <= n
    unsigned long long int k;
} rsapvk_t;

typedef struct rsaPublicKey {
    mpz_t n, pbk;
    // It's the greatest integer k such that NB_CHAR^k <= n
    unsigned long long int k;
} rsapbk_t;

typedef struct rsaEncryptedMessage {
    mpz_t *mess;
    int size;
} rsaem_t;

typedef struct rsaDecryptedMessage {
    char *mess;
    int size;
} rsadm_t;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaInitKey
 *  Description:  
 * =====================================================================================
 */
void rsaInitKey(rsapvk_t *pvk, rsapbk_t *pbk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaCleanKey
 *  Description:  
 * =====================================================================================
 */
void rsaCleanKey(rsapvk_t *pvk, rsapbk_t *pbk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaKeyGen
 *  Description:  
 * =====================================================================================
 */
void rsaKeyGen(rsapvk_t *pvk, rsapbk_t *pbk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaEncrypt
 *  Description:  
 * =====================================================================================
 */
void rsaEncrypt(rsapbk_t k, rsadm_t m, rsaem_t *c);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaDecrypt
 *  Description:  
 * =====================================================================================
 */
void rsaDecrypt(rsapvk_t k, rsaem_t c, rsadm_t *m);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaPrintKey
 *  Description:  
 * =====================================================================================
 */
void rsaPrintKey(rsapbk_t k, char **str);

/*-----------------------------------------------------------------------------
 *  El Gamal
 *-----------------------------------------------------------------------------*/

typedef struct egPrivateKey {
    mpz_t a, p;
    unsigned long int k;
} egpvk_t;

typedef struct egPublicKey {
    mpz_t alpha, beta, p;
    unsigned long int k;
} egpbk_t;

typedef struct egEncryptedMessage {
    mpz_t y1, *y2;
    int size;
} egem_t;

typedef struct egDecryptedMessage {
    char *m;
    int size;
} egdm_t;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initEgKey
 *  Description:  
 * =====================================================================================
 */
void egInitKey(egpvk_t *pvk, egpbk_t *pbk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clearEgKey
 *  Description:  
 * =====================================================================================
 */
void egClearKey(egpvk_t *pvk, egpbk_t *pbk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  egKeyGen
 *  Description:  
 * =====================================================================================
 */
void egKeyGen(egpvk_t *pvk, egpbk_t *pbk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  egEncrypt
 *  Description:  
 * =====================================================================================
 */
void egEncrypt(egpbk_t k, egdm_t m, egem_t *c);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  egDecrypt
 *  Description:  
 * =====================================================================================
 */
void egDecrypt(egpvk_t k, egem_t c, egdm_t *m);

/*-----------------------------------------------------------------------------
 *  Diffie Hellman
 *-----------------------------------------------------------------------------*/

typedef struct dhPrivateKey {
    mpz_t a, key, p, g;
} dhpvk_t;

typedef struct dhPublicKey {
    mpz_t p, g, pow;
} dhpbk_t;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhInitKey
 *  Description:  
 * =====================================================================================
 */
void dhInitPrivKey(dhpvk_t *k);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhInitPubKey
 *  Description:  
 * =====================================================================================
 */
void dhInitPubKey(dhpbk_t *k);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhClearPrivKey
 *  Description:  
 * =====================================================================================
 */
void dhClearPrivKey(dhpvk_t *k);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhClearPubKey
 *  Description:  
 * =====================================================================================
 */
void dhClearPubKey(dhpbk_t *k);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhGenPubKey
 *  Description:  
 * =====================================================================================
 */
void dhGenPubKey(dhpvk_t *pvk, dhpbk_t *pbk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhGenPubKeyFromAnother
 *  Description:  
 * =====================================================================================
 */
void dhGenPubKeyFromAnother(dhpbk_t *pbk, dhpvk_t *pvk);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhGenPrivKey
 *  Description:  
 * =====================================================================================
 */
void dhGenPrivKey(dhpvk_t *pvk, dhpbk_t pbk);

/*-----------------------------------------------------------------------------
 *  Random Number Management
 *-----------------------------------------------------------------------------*/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rnInit
 *  Description:  
 * =====================================================================================
 */
void rnInit();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rnClear
 *  Description:  
 * =====================================================================================
 */
void rnClear();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  convertToInt
 *  Description:  
 * =====================================================================================
 */
mpz_t *convertToInt(char *str, size_t str_size, int block_size, size_t *ret_size);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  convertToChar
 *  Description:  
 * =====================================================================================
 */
char *convertToChar(mpz_t *toconv, size_t arr_size, int block_size, size_t *ret_size);

#endif

