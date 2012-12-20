/*
 * =====================================================================================
 *
 *       Filename:  tp.c
 *
 *    Description:  Cryptography
 *
 *        Version:  1.0
 *        Created:  21/11/2012 23:51:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  mfreeze (DUVILLIE Guillerme)
 *   Organization:  Université Montpellier II
 *
 * =====================================================================================
 */

#include "crypt.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  euclide
 *  Description:  euclide algorithm, it computes the gcd of a and b
 * =====================================================================================
 */
long long int euclide(long long int a, long long int b) {
    if (b > a)
        return euclide(b, a);
    if (!b)
        return a;
	return euclide(b, a%b);
}		/* -----  end of function euclide  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  extendedEuclide
 *  Description:  Extended version of Euclide algorithm, it computes the gcd of a and b
 *                and couple of coefficients u and v of bezout theorem
 * =====================================================================================
 */
long long int extendedEuclide(
        long long int a,
        long long int b,
        long long int *u,
        long long int *v) {
    if (b > a)
        return extendedEuclide(b, a, v, u);
    long long int r = a%b, q = a / b, t0, t1;
    if (!r) {
        *u = 0;
        *v = 1;
        return b;
    }
    t0 = extendedEuclide(b, r, &t1, u);
    *v = t1 - q * *u; 
	return t0;
}		/* -----  end of function extendedEuclide  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  extEuclideInv
 *  Description:  Computes the inverse of a by using extended euclide algorithm
 * =====================================================================================
 */
long long int extEuclideInv(
        long long int b,
        long long int n,
        long long int *u) {
    long long int t0 = 0, n0 = n, b0 = b, t = 1;
    long long int q = n0 / b0;
    long long int r = n0 - q *b0;
    while (r > 0) {
        long long int tmp = t0 - q * t;
        if (tmp >= 0)
            tmp = tmp % n;
        else 
            tmp = n - ((-tmp) % n);
        t0 = t;
        t = tmp;
        n0 = b0;
        b0 = r;
        q = n0 / b0;
        r = n0 - q*b0;
    }
    *u = t;
	return t0;
}		/* -----  end of function extEuclideInv  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dhKeyGen
 *  Description:  Generates Diffie Hellman Key from a and b (a^b)
 * =====================================================================================
 */
long long int dhKeyGen(long long int a,
		long long int b) {
	return powl(a, b);
}		/* -----  end of function dhKeyGen  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  eratosthene
 *  Description:  
 * =====================================================================================
 */
int eratosthene(long long int a) {
    if (!(a%2))
        return 0;
    long long int i = 0;
    for (i=3; i<floor(a); i+=2)
        if (!(a%i))
            return 0;
    return 1;
}		/* -----  end of function eratosthene  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isGenerator
 *  Description:  
 * =====================================================================================
 */
int isGenerator(long long int ord, long long int a) {
    int i;
    for (i=2; i<ord+1; i++)
        if (fastExp(a, i, ord + 1) == a)
            return 0;
	return 1;
}		/* -----  end of function isGenerator  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  findPrime
 *  Description:  
 * =====================================================================================
 */
long long int findPrime(long long int max) {
    long long int a = (rand() % (max/2)) * 2 + 1;
    while(!eratosthene(a))
        a = (rand() % (max/2)) * 2 + 1;
	return a;
}		/* -----  end of function findPrime  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaKeyGen
 *  Description:  
 * =====================================================================================
 */
void rsaKeyGen(long long int n,
        long long int p,
        long long int q,
		long long int phi,
		rsapvk_t *key,
		rsapbk_t *pubkey) {
    do {
        pubkey->pubkey = findPrime(phi);
    } while (euclide(phi, pubkey->pubkey) != 1);
    pubkey->n = n;

    long long int a;
    extEuclideInv(pubkey->pubkey, phi, &a);
    key->privkey = a % phi;
    key->p = p;
    key->q  = q;
}		/* -----  end of function rsaKeyGen  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaEncrypt
 *  Description:  
 * =====================================================================================
 */
long long int rsaEncrypt(rsapbk_t pubkey, long long int m) {
	return fastExp(m, pubkey.pubkey, pubkey.n);
}		/* -----  end of function rsaEncrypt  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rsaDecrypt
 *  Description:  
 * =====================================================================================
 */
long long int rsaDecrypt(rsapvk_t k, long long int m) {
	return fastExp(m, k.privkey, k.p * k.q);
}		/* -----  end of function rsaDecrypt  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  fastExp
 *  Description:  Computes a^b % n
 * =====================================================================================
 */
long long int fastExp(long long int a, long long int b, long long int n) {
    if (b == 1)
        return a%n;
    if (b == 0) 
        return 1;
    return (b%2 ? (a * fastExp((a*a)%n, (b-1)/2, n))%n : (fastExp((a*a)%n, b/2, n))%n);
}		/* -----  end of function fastExp  ----- */


/*-----------------------------------------------------------------------------
 *  El Gamal functions
 *-----------------------------------------------------------------------------*/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  elGamalPrivKeyGen
 *  Description:  
 * =====================================================================================
 */
void elGamalPrivKeyGen(egpvk_t *key, long long int ord) {
    key->privkey = 1 + rand() % ord;
    key->ord = ord;
}		/* -----  end of function elGamalKeyGen  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  elGamalPubKeyGen
 *  Description:  
 * =====================================================================================
 */
void elGamalPubKeyGen(egpbk_t *k, long long int ord, long long int gen,
        egpvk_t privkey) {
    k->h = fastExp(gen, privkey.privkey, ord + 1);
    k->g = gen;
    k->ord = ord +1;
}		/* -----  end of function elGamalPubKeyGen  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  elGamalEncrypt
 *  Description:  
 * =====================================================================================
 */
void elGamalEncrypt(egcm_t *mess, egpbk_t pubkey, long long int m) {
    long long int ephemerialkey;
    do {
        ephemerialkey = 1 + (rand() % (pubkey.ord - 1));
    } while (euclide(ephemerialkey, pubkey.ord) != 1);
    mess->y1 = fastExp(pubkey.g, ephemerialkey, pubkey.ord + 1);
    mess->y2 = fastExp(pubkey.h, ephemerialkey, pubkey.ord + 1) * m % (pubkey.ord +1);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  elGamalDecrypt
 *  Description:  
 * =====================================================================================
 */
long long int elGamalDecrypt(egcm_t mess, egpvk_t privkey) {
    long long int inter = fastExp(mess.y1, privkey.privkey, privkey.ord + 1);
    long long int inve;
    extEuclideInv(inter, privkey.ord + 1, &inve);
    return ((mess.y2 * inve) % (privkey.ord + 1));
}
