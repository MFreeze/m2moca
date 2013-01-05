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

    // Menu
    int menu, submenu, loop = 1, au, i, j;

    // Others
    FILE *f, *w, *wp;

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
                            case 5:
                                loop --;
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

                                printf("Please choose file name to encrypt : ");
                                f = getFile("r");

                                if (f) {
                                    printf("Please choose name of destination file : ");
                                    w = getFile("w");
                                    if (w) {
                                        m.mess = readFile(f, &(m.size));
                                        rsaEncrypt(pbk, m, &c);
                                        for (i=0; i<c.size; i++)
                                            gmp_fprintf(w, "%Zd ", c.mess[i]);
                                        fclose(w);

                                        for (i=0; i<c.size; i++)
                                            mpz_clear(c.mess[i]);
                                        free(c.mess);
                                        free(m.mess);
                                    }
                                    fclose(f);
                                }
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
//                                rsaDecrypt(pvk, c, &m);
/*                                 for (i=0; i<m.size; i++)
 *                                     gmp_printf("%d ", m.mess[i]);
 *                                 printf("\n");
 */
                                printf("Please choose file name to decrypt : ");
                                f = getFile("r");

                                if (f) {
                                    printf("Please choose name of destination file : ");
                                    w = getFile("w");
                                    if (w) {
                                        char *tmp_str = readFile(f, &i);
                                        c.mess = convertToCryptInt(tmp_str, i, &(c.size));
                                        for (i=0; i<c.size; i++)
                                            gmp_printf("%Zd \n", c.mess[i]);
                                        rsaDecrypt(pvk, c, &m);
                                        fprintf(w, "%s", m.mess);
                                        free(m.mess);
                                        fclose(w);

                                        for (i=0; i<c.size; i++)
                                            mpz_clear(c.mess[i]);
                                        free(c.mess);
                                        free(tmp_str);
                                    }
                                    fclose(f);
                                }
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

                                printf("Please choose file name to encrypt : ");
                                f = getFile("r");

                                if (f) {
                                    printf("Please choose name of destination file (part 1) : ");
                                    w = getFile("w");
                                    printf("Please choose name of destination file (part 2) : ");
                                    wp = getFile("w");
                                    if (w && wp) {
                                        m1.m = readFile(f, &(m1.size));
                                        egEncrypt(egpb, m1, &c1);
                                        for (i=0; i<c1.size; i++)
                                            gmp_fprintf(w, "%Zd ", c1.y2[i]);
                                        fclose(w);

                                        gmp_fprintf(wp, "%Zd ", c1.y1);
                                        fclose(wp);

                                        for (i=0; i<c1.size; i++)
                                            mpz_clear(c1.y2[i]);

                                        mpz_clear(c1.y1);
                                        free(c1.y2);
                                        free(m1.m);
                                    }
                                    fclose(f);
                                }
/*                                 egEncrypt(egpb, m1, &c1);
 *                                 printf("Taille : %d\n", c1.size);
 */
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

                                printf("Please choose file name to decrypt (part 1) : ");
                                f = getFile("r");
                                printf("Please choose file name to decrypt (part 2) : ");
                                wp = getFile("r");

                                if (f && wp) {
                                    printf("Please choose name of destination file : ");
                                    w = getFile("w");
                                    if (w) {
                                        mpz_t *a;
                                        char *tmp_str = readFile(f, &i);
                                        c1.y2 = convertToCryptInt(tmp_str, i, &(c.size));
                                        free(tmp_str);
                                        tmp_str = readFile(wp, &i);
                                        a = convertToCryptInt(tmp_str, i, &i);
                                        mpz_set(c1.y1, a[0]);
                                        mpz_clear(a[0]);
                                        free(a);
                                        free(tmp_str);
                                        for (i=0; i<c.size; i++)
                                            gmp_printf("%Zd \n", c1.y2[i]);
                                        egDecrypt(egpv, c1, &m1);
                                        fprintf(w, "%s", m1.m);
                                        free(m1.m);
                                        fclose(w);

                                        for (i=0; i<c.size; i++)
                                            mpz_clear(c1.y2[i]);
                                        mpz_clear(c1.y1);
                                        free(c1.y2);
                                    }
                                    fclose(f);
                                    fclose(wp);
                                }
                                //for (i=0; i<csize; i++)
                                //   gmp_printf("c[%d] = %Zd\n", i, c[i]);

/*                                 egDecrypt(egpv, c1, &m1);
 *                                 for (i=0; i<m1.size; i++)
 *                                     gmp_printf("%d ", m1.m[i]);
 */
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
