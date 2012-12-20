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

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifndef CRYPT_H
#define CRYPT_H

#define NB_TURN 10
#define EG_KEY_SIZE 28
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
    int *mess;
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
    int *m;
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
void edClearKey(egpvk_t *pvk, egpbk_t *pbk);

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
#endif
