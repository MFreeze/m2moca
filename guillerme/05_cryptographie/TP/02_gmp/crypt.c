/*
 * ===================================================================================== *
 *       Filename:  crypt.c
 *
 *    Description:  Implementation file for crypt.h
 *
 *        Version:  1.0
 *        Created:  08/12/2012 13:22:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "crypt.h"

gmp_randstate_t seed;
int seed_initialized = 0;

// {{{ MISC FUNC
/*-----------------------------------------------------------------------------
 *  Miscellaneous functions
 *-----------------------------------------------------------------------------*/
int isGenerator(mpz_t p, mpz_t g) {
    if (!mpz_cmp_ui(g, 0))
        return 0;
    mpz_t temp, count;
    mpz_init_set(temp, g);
    mpz_init_set_ui(count, 2);
    while(mpz_cmp(count, p) < 0) {
        mpz_mul(temp, temp, g);
        mpz_mod(temp, temp, p);
        if (!mpz_cmp(temp, g))
            return 0;
        mpz_add_ui(count, count, 1);
    }
    return 1;
}
// }}}

// {{{ RSA
/*-----------------------------------------------------------------------------
 *  RSA
 *-----------------------------------------------------------------------------*/

void rsaInitKey(rsapvk_t *pvk, rsapbk_t *pbk) {
    mpz_init(pvk->p);
    mpz_init(pvk->q);
    mpz_init(pvk->n);
    mpz_init(pvk->phi);
    mpz_init(pvk->pvk);
    mpz_init(pbk->n);
    mpz_init(pbk->pbk);
}

void rsaCleanKey(rsapvk_t *pvk, rsapbk_t *pbk) {
    mpz_clear(pvk->p);
    mpz_clear(pvk->q);
    mpz_clear(pvk->n);
    mpz_clear(pvk->phi);
    mpz_clear(pvk->pvk);
    mpz_clear(pbk->n);
    mpz_clear(pbk->pbk);
}

void rsaKeyGen(rsapvk_t *pvk, rsapbk_t *pbk) {
    // Temporary variable
    mpz_t tmp;
    mpz_init(tmp);

    // Integer used for random numbers
    if (!seed_initialized)
        rnInit();

    do {
        mpz_urandomb(pvk->p, seed, RSA_KEY_SIZE);        
    } while (!mpz_probab_prime_p(pvk->p, NB_TURN));

    do {
        mpz_urandomb(pvk->q, seed, RSA_KEY_SIZE);        
    } while (!mpz_probab_prime_p(pvk->q, NB_TURN));

    // Initialize keys
    mpz_mul(pbk->n, pvk->p, pvk->q);
    mpz_set(pvk->n, pbk->n);

    // Compute phi = (p-1)(q-1) = q(p-1) - (p-1)
    // I don't compute p-1 and q-1 to avoid many initializations
    mpz_sub_ui(tmp, pvk->p, 1);
    mpz_mul(pvk->phi, pvk->q, tmp);
    mpz_sub(pvk->phi, pvk->phi, tmp);

    // Find a random number prime with phi
    do {
        mpz_urandomb(pbk->pbk, seed, RSA_KEY_SIZE);        
        mpz_gcd(tmp, pbk->pbk, pvk->phi);
    } while (mpz_cmp_ui(tmp, 1));

    // Compute pvk
    mpz_invert(pvk->pvk, pbk->pbk, pvk->phi);

    // Compute k
    mpz_set_ui(tmp, NB_CHAR);
    pvk->k = 0;
    while (mpz_cmp(tmp, pbk->n) < 0) {
        mpz_mul_ui(tmp, tmp, NB_CHAR);
        pvk->k ++;
    }

    pvk->k --;

    pbk->k = pvk->k;

    // Clearing variables
    mpz_clear(tmp);
}

void rsaEncrypt(rsapbk_t k, rsadm_t m, rsaem_t *c) {
    // Counters and size of encrypted message
    int i = 0, j = 0, l = 0, tmp;

    // Memory initialization
    c->mess = convertToInt(m.mess, m.size, k.k, &tmp);
    c->size = tmp;

    while(i < c->size) {
        // Encryption
        mpz_powm(c->mess[i], c->mess[i], k.pbk, k.n);
        i++;
    }
}

void rsaDecrypt(rsapvk_t k, rsaem_t c, rsadm_t *m) {
    int i;

    // Decryption
    for (i=0; i<c.size; i++)
        mpz_powm(c.mess[i], c.mess[i], k.pvk, k.n);

    // String transformation
    m->mess = convertToChar(c.mess, c.size, k.k, &i);
    m->size = i;
}

void rsaPrintKey(rsapbk_t k, char **str){
}
// }}}

// {{{ El Gamal
/*-----------------------------------------------------------------------------
 *  El Gamal
 *-----------------------------------------------------------------------------*/
void egInitKey(egpvk_t *pvk, egpbk_t *pbk){
    mpz_init(pvk->a);
    mpz_init(pvk->p);
    mpz_init(pbk->alpha);
    mpz_init(pbk->beta);
    mpz_init(pbk->p);
}

void egClearKey(egpvk_t *pvk, egpbk_t *pbk){
    mpz_clear(pvk->a);
    mpz_clear(pvk->p);
    mpz_clear(pbk->alpha);
    mpz_clear(pbk->beta);
    mpz_clear(pbk->p);
}

void egKeyGen(egpvk_t *pvk, egpbk_t *pbk) {
    // Useful for random generation
    if (!seed_initialized)
		rnInit();

    // Temporary variables
    mpz_t tmp;
    mpz_init(tmp);
    
    // p definition
    do {
        mpz_urandomb(pvk->p, seed, EG_KEY_SIZE);        
    } while (!mpz_probab_prime_p(pvk->p, NB_TURN));

    mpz_set(pbk->p, pvk->p);

    mpz_set_ui(pbk->alpha, 2);
    // Search alpha so that alpha is a generator
    while(!isGenerator(pbk->p, pbk->alpha))
        mpz_add_ui(pbk->alpha, pbk->alpha, 1);

    // Compute private key
    mpz_urandomb(pvk->a, seed, EG_KEY_SIZE);

    // Compute last part of public key
    mpz_powm(pbk->beta, pbk->alpha, pvk->a, pvk->p);

    // Compute k
    mpz_set_ui(tmp, NB_CHAR);
    pvk->k = 0;
    while (mpz_cmp(tmp, pbk->p) < 0) {
        mpz_mul_ui(tmp, tmp, NB_CHAR);
        pvk->k ++;
    }

    pbk->k = pvk->k;

    mpz_clear(tmp);
}

void egEncrypt(egpbk_t k, egdm_t m, egem_t *c) {
    // Useful for random generation
    if (!seed_initialized)
		rnInit();

    // Temporary variables
    mpz_t tmp;
    mpz_init(tmp);

    mpz_urandomb(tmp, seed, EG_KEY_SIZE);

    // Counters and size of encrypted message
    int i = 0, j = 0, l = 0;

    // Memory initialization
    mpz_init(c->y1);
    c->y2 = convertToInt(m.m, m.size, k.k, &l);
    c->size = l;

    mpz_powm(c->y1, k.alpha, tmp, k.p);
    mpz_powm(tmp, k.beta, tmp, k.p);

    while(i < c->size) {
        // Encryption
        mpz_mul(c->y2[i], c->y2[i], tmp);
        mpz_mod(c->y2[i], c->y2[i], k.p);
        i++;
    }
    mpz_clear(tmp);
}

void egDecrypt(egpvk_t k, egem_t c, egdm_t *m) {
    int i;
    mpz_t tmp, inv;
    mpz_init(tmp);
    mpz_init(inv);

    mpz_powm(tmp, c.y1, k.a, k.p);
    mpz_invert(inv, tmp, k.p);

    for (i=0; i<c.size; i++) {
        // Decryption
        mpz_mul(c.y2[i], c.y2[i], inv);
        mpz_mod(c.y2[i], c.y2[i], k.p);
    }

    m->m = convertToChar(c.y2, c.size, k.k, &i);
    m->size = i;

    mpz_clear(tmp);
    mpz_clear(inv);
}
// }}}

// {{{ Diffie Hellman
/*-----------------------------------------------------------------------------
 *  Diffie Hellman
 *-----------------------------------------------------------------------------*/
void dhInitPrivKey(dhpvk_t *k) {
    mpz_init(k->a);
    mpz_init(k->p);
    mpz_init(k->g);
    mpz_init(k->key);
}

void dhInitPubKey(dhpbk_t *k) {
    mpz_init(k->p);
    mpz_init(k->g);
    mpz_init(k->pow);
}

void dhClearPrivKey(dhpvk_t *k) {
    mpz_clear(k->a);
    mpz_clear(k->p);
    mpz_clear(k->g);
    mpz_clear(k->key);
}

void dhClearPubKey(dhpbk_t *k) {
    mpz_clear(k->p);
    mpz_clear(k->g);
    mpz_clear(k->pow);
}

void dhGenPubKey(dhpvk_t *pvk, dhpbk_t *pbk) {
    // Integer used for random numbers
    if (!seed_initialized)
        rnInit();

    do {
        mpz_urandomb(pbk->p, seed, DH_KEY_SIZE);        
    } while (!mpz_probab_prime_p(pbk->p, NB_TURN));
    mpz_set(pvk->p, pbk->p);

    mpz_set_ui(pbk->g, 2);
    // Search alpha so that alpha is a generator
    while(!isGenerator(pbk->p, pbk->g))
        mpz_add_ui(pbk->g, pbk->g, 1);

    mpz_set(pvk->g, pbk->g);

    mpz_urandomm(pvk->a, seed, pvk->p);

    mpz_powm(pbk->pow, pbk->g, pvk->a, pbk->p);
}

void dhGenPubKeyFromAnother(dhpbk_t *pbk, dhpvk_t *pvk) {
    if (!seed_initialized)
		rnInit();

    mpz_set(pvk->p, pbk->p);
    mpz_set(pvk->g, pbk->g);

    mpz_urandomm(pvk->a, seed, pvk->p);

    mpz_powm(pvk->key, pbk->pow, pvk->a, pvk->p);

    mpz_powm(pbk->pow, pbk->g, pvk->a, pbk->p);
}

void dhGenPrivKey(dhpvk_t *pvk, dhpbk_t pbk) {
    mpz_powm(pvk->key, pbk.pow, pvk->a, pvk->p);
}

// }}}

// {{{ Random Number Management
void rnInit() {
    gmp_randinit_mt(seed);
    seed_initialized = 1;
}

void rnClear() {
    gmp_randclear(seed);
    seed_initialized = 0;
}
// }}}

// {{{ Conversions

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  convertToInt
 *  Description:  
 * =====================================================================================
 */
mpz_t *convertToInt(char *str, int str_size, int block_size, int *ret_size) {
    mpz_t *result, tmp;
    int res_size = str_size / block_size + 1, pts = 0, pta = -1, i;
    unsigned long int pow = block_size;
    
    result = (mpz_t *) malloc (res_size * sizeof(mpz_t));
    for (i=0; i<res_size; i++)
        mpz_init(result[i]);

    mpz_init(tmp);

    while (str[pts] != '\0') {
        if (!(pts % block_size)) {
            pow = block_size - 1;
            pta ++;
        }
        mpz_ui_pow_ui(tmp, NB_CHAR, pow);
        mpz_mul_ui(tmp, tmp, (unsigned int)str[pts]);
        mpz_add(result[pta], result[pta], tmp);
        pts++;
        pow --;
    }

    *ret_size = res_size;

    return result;
}		/* -----  end of function convertToInt  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  convertToChar
 *  Description:  
 * =====================================================================================
 */
char *convertToChar(mpz_t *arr, int arr_size, int block_size, int *ret_size) {
    char *result;
    int res_size = arr_size * block_size + 1, i, j;

    result = (char *) malloc (sizeof(char) * res_size);
    
    mpz_t tmp;
    mpz_init(tmp);

    for (i=0; i<arr_size; i++) {
        mpz_set(tmp, arr[i]);
        for (j=0; j<block_size; j++) {
            result[block_size - 1 - j + i *block_size] = (char) mpz_fdiv_ui(tmp, NB_CHAR);
            mpz_fdiv_q_ui(tmp, tmp, NB_CHAR);
        }
    }

    *ret_size = res_size;
    result[res_size - 1] = '\0';
    return result;
}		/* -----  end of function convertToChar  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  convertToCryptInt
 *  Description:  
 * =====================================================================================
 */
mpz_t *convertToCryptInt(char *str, int str_size, int *ret_size) {
    mpz_t *result;

    int count = 0, i = 0, j = 0;
    char *tmp = 0;

    while (str[i] != '\0') {
        if (str[i] == ' ')
            count ++;
        i++;
    }

    if (str[strlen(str) - 1] != ' ')
        count ++;

    result = (mpz_t *) malloc (sizeof(mpz_t) * count);

    i = 0;

    while (str[i] != '\0') {
        int nsize = 32, csize = 1;
        tmp = (char *) malloc (sizeof(char) * nsize);
        while(str[i] != ' ' && str[i] != '\0') {
            if (csize == nsize) {
                nsize = nsize << 2;
                tmp = (char *) realloc (tmp, nsize * sizeof(char));
            }
            tmp[csize - 1] = str[i];
            csize ++; i ++;
        }
        tmp[csize - 1] = '\0';
        i ++; 
        mpz_init(result[j]);
        mpz_set_str(result[j], tmp, 10);
        j++;

        free(tmp);
    }

    *ret_size = j;
    return result;
} /* -----  end of function convertToCryptInt  ----- */
// }}}
