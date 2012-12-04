/*
 * =====================================================================================
 *
 *       Filename:  crypt.h
 *
 *    Description:  Header file for cryptography practical work
 *
 *        Version:  1.0
 *        Created:  02/12/2012 13:16:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef CRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CRYPT_H

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  euclide
 *  Description:  Computes the Greatest Common Divisor of a and b
 * =====================================================================================
 */
long long int euclide(long long int a,
		long long int b);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  extendedEuclide
 *  Description:  Computes u, v  in Z* such that au + bv = 1
 * =====================================================================================
 */
long long int extendedEuclide(
        long long int a, 
        long long int b, 
        long long int *u, 
        long long int *v);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  extEuclideInv
 *  Description:  Computes the inverse of a by using extended euclide algorithm
 * =====================================================================================
 */
long long int extEuclideInv(
        long long int a,
        long long int b,
        long long int *u);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaKeyGen
 *  Description:  Generates the RSA Key
 * =====================================================================================
 */
void rsaKeyGen(long long int n,
		long long int phi,
		long long int *key,
		long long int *pubkey);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaEncrypt
 *  Description:  Encrypt a number with RSA
 * =====================================================================================
 */
long long int rsaEncrypt(long long int k, long long int n, long long int a);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaDecrypt
 *  Description:  Decrypt a number with RSA
 * =====================================================================================
 */
long long int rsaDecrypt(long long int k, long long int n, long long int a);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  eratosthene
 *  Description:  Check if a is prime
 * =====================================================================================
 */
int eratosthene(long long int a);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isGenerator
 *  Description:  Check if a is a generator for Z/ordZ
 * =====================================================================================
 */
int isGenerator(long long int ord, long long int a);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  findPrime
 *  Description:  Find a random prime number between 2 ans max_bound
 * =====================================================================================
 */
long long int findPrime(long long int max_bound);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  fastExp
 *  Description:  Use to compute a^b (mod n)
 * =====================================================================================
 */
long long int fastExp(long long int a, long long int b, long long int n);

#endif
