/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0 *        Created:  09/12/2012 19:56:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "menu.h"
#include "crypt.h"

void print_help();

int main(int argc, char **argv) {
    // {{{ Declaration block
    // RSA
    rsapvk_t pvk;
    rsapbk_t pbk;
    rsaem_t c;
    rsadm_t m;
    int rsa_key_generated = 0;

    // El Gamal
    egpvk_t egpv;
    egpbk_t egpb;
    egem_t c1;
    egdm_t m1;
    int eg_key_generated = 0;

    // Diffie Hellman
    dhpbk_t dhpb;
    dhpvk_t dhpv_alice, dhpv_bob;

    // Message 
    int ma[] = {3, 3, 1, 3, 3, 3, 3, 3, 3, 3}, i, *mess;
    int mb[] = {2, 2, 2, 2, 25, 227, 9, 8, 12, 26};
    m.mess = ma;
    m1.m = mb;
    m.size = sizeof(ma)/sizeof(ma[0]);
    m1.size = sizeof(mb)/sizeof(mb[0]);

    // Menu
    int menu, submenu, loop = 1, au;

    // Others
    FILE *f;

    // }}}

    rsaInitKey(&pvk, &pbk);
    egInitKey(&egpv, &egpb);

    initMMI(&menu, &submenu);

    while(loop) {
        au = askUser(&menu, &submenu);
        switch(au) {
            case -1:
                fprintf(stderr, "Error in menu selection\n");
                loop --;
                break;
            case 0:
                break;
            case 1:
                switch(menu) {
                    // {{{ MENU = 0 (Main Menu)
                    case 0:
                        switch (submenu) {
                            // {{{ SUB = 3 (Diffie Hellman)
                            case 3:
                                printf("\nKey exchange Simulation.\n");

                                dhInitPubKey(&dhpb);
                                dhInitPrivKey(&dhpv_alice);
                                dhInitPrivKey(&dhpv_bob);

                                printf("Alice Computation :\n\n");

                                dhGenPubKey(&dhpv_alice, &dhpb);

                                gmp_printf("Finite Field Z/pZ with p : %Zd\n"
                                        "Alice random number a : %Zd\n"
                                        "Field generator g : %Zd\n"
                                        "g^a pow : %Zd\n", dhpb.p, dhpv_alice.a, dhpb.g, dhpb.pow);

                                printf("\nBob Computation :\n\n");

                                dhGenPubKeyFromAnother(&dhpb, &dhpv_bob);

                                gmp_printf("Finite Field Z/pZ with p : %Zd\n"
                                        "Bob random number b : %Zd\n"
                                        "Field generator g : %Zd\n"
                                        "g^b : %Zd\n", dhpb.p, dhpv_bob.a, dhpb.g, dhpb.pow);

                                printf("\nKeys\n\n");

                                dhGenPrivKey(&dhpv_alice, dhpb);

                                gmp_printf("Alice : %Zd\n"
                                        "Bob : %Zd\n", dhpv_alice.key, dhpv_bob.key);

                                dhClearPubKey(&dhpb);
                                dhClearPrivKey(&dhpv_alice);
                                dhClearPrivKey(&dhpv_bob);
                                break;
                                // }}}
                                // {{{ SUB = 4 (Help)
                            case 4:
                                print_help();
                                break;
                                // }}}
                                // {{{ SUB = 5 (Quit)
                            case 6:
                                loop --;
                                break;
                                // }}}
                                // {{{ TEST
                            case 5:
                                f = getFile("r");
                                if (f) {
                                    size_t s, r; 
                                    int i;
                                    mpz_t *tab;
                                    char *tmp = readFile(f, &s);
                                    tab = convertToInt(tmp, s, 2, &r);
                                    fclose(f);
                                    free(tmp);
                                    convertToChar(tab, r, 2, &s);
                                    for (i=0; i<r; i++)
                                        mpz_clear(tab[i]);
                                    free(tab);
                                }
                                break;
                                // }}}
                        }
                        break;
                        // }}}
                        // {{{ MENU = 1 (RSA)
                    case 1:
                        switch(submenu) {
                            // {{{ SUB = 1 (Key Generation)
                            case 1:
                                rsa_key_generated = 1;
                                rsaKeyGen(&pvk, &pbk);

                                gmp_printf("p : %Zd\n"
                                        "q : %Zd\n"
                                        "n : %Zd\n"
                                        "k : %lld\n"
                                        "pvk : %Zd\n"
                                        "pbk : %Zd\n", pvk.p, pvk.q, pvk.n, pvk.k, pvk.pvk, pbk.pbk);
                                break;
                                // }}}
                                // {{{ SUB = 2 (Encryption)
                            case 2:
                                if (!rsa_key_generated) {
                                    printf("Key not yet generated. Generating one ...\n");
                                    rsa_key_generated = 1;
                                    rsaKeyGen(&pvk, &pbk);
                                    gmp_printf("p : %Zd\n"
                                            "q : %Zd\n"
                                            "n : %Zd\n"
                                            "k : %lld\n"
                                            "pvk : %Zd\n"
                                            "pbk : %Zd\n", pvk.p, pvk.q, pvk.n, pvk.k, pvk.pvk, pbk.pbk);
                                }
                                rsaEncrypt(pbk, m, &c);
                                printf("Taille : %d\n", c.size);
                                break;

                                //for (i=0; i<csize; i++)
                                //   gmp_printf("c[%d] = %Zd\n", i, c[i]);
                                // }}}
                                // {{{ SUB = 3 (Decryption)
                            case 3:
                                if (!rsa_key_generated) {
                                    printf("Key not yet generated. Generating one ...\n");
                                    rsa_key_generated = 1;
                                    rsaKeyGen(&pvk, &pbk);
                                    gmp_printf("p : %Zd\n"
                                            "q : %Zd\n"
                                            "n : %Zd\n"
                                            "k : %lld\n"
                                            "pvk : %Zd\n"
                                            "pbk : %Zd\n", pvk.p, pvk.q, pvk.n, pvk.k, pvk.pvk, pbk.pbk);
                                }
                                rsaDecrypt(pvk, c, &m);
                                for (i=0; i<m.size; i++)
                                    gmp_printf("%d ", m.mess[i]);
                                printf("\n");
                                break;
                                // }}}
                                // {{{ SUB = 4 (Signature)
                            case 4:
                                printf("Not yet implemented\n");
                                break;
                                // }}}
                        }
                        break;
                        // }}}
                        // {{{ MENU = 2 (EL Gamal)
                    case 2:
                        switch(submenu) {
                            // {{{ SUB = 1 (Key Generation)
                            case 1:
                                eg_key_generated = 1;
                                egKeyGen(&egpv, &egpb);
                                gmp_printf("p : %Zd\n"
                                        "a : %Zd\n"
                                        "g : %Zd\n",egpv.p, egpv.a, egpb.alpha);
                                break;
                                // }}}
                                // {{{ SUB = 2 (Encryption)
                            case 2:
                                if (!eg_key_generated) {
                                    printf("Key not yet generated. Generating one ...\n");
                                    eg_key_generated = 1;
                                    egKeyGen(&egpv, &egpb);
                                    gmp_printf("p : %Zd\n"
                                            "a : %Zd\n"
                                            "g : %Zd\n",egpv.p, egpv.a, egpb.alpha);
                                }
                                egEncrypt(egpb, m1, &c1);
                                printf("Taille : %d\n", c1.size);
                                break;
                                // }}}
                                // {{{ SUB = 3 (Decryption)
                            case 3:
                                if (!eg_key_generated) {
                                    printf("Key not yet generated. Generating one ...\n");
                                    eg_key_generated = 1;
                                    egKeyGen(&egpv, &egpb);
                                    gmp_printf("p : %Zd\n"
                                            "a : %Zd\n"
                                            "g : %Zd\n",egpv.p, egpv.a, egpb.alpha);
                                }
                                //for (i=0; i<csize; i++)
                                //   gmp_printf("c[%d] = %Zd\n", i, c[i]);

                                egDecrypt(egpv, c1, &m1);
                                for (i=0; i<m1.size; i++)
                                    gmp_printf("%d ", m1.m[i]);
                                break;
                                // }}}
                                // {{{ SUB = 4 (Signature)
                            case 4:
                                printf("Not yet implemented\n");
                                break;
                                // }}}

                        }
                        break;
                        // }}}
                }
                break;
        }
    }
    rsaCleanKey(&pvk, &pbk);
    egClearKey(&egpv, &egpb);
    clearMMI();
}

void print_help() {
    printf("Not yet implemented.\n");
}
