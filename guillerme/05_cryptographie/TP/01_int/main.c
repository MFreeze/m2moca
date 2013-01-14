/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file
 *
 *        Version:  1.0
 *        Created:  02/12/2012 14:18:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

// TODO :
// * RSA key generation
// * RSA encryption
// * RSA decryption

#include "menu.h"
#include "crypt.h"
#include <ncurses.h>
#include <time.h>

#define MARGE_Y 1
#define MARGE_X 3
#define SEP_Y 1
#define SEP_X 2

#define CTRL(c) ((c) & 037)

#define TITLE "Asymetric Cryptography"
#define BODY "Main Window"
#define PFUNC "Local Actions"
#define GFUNC "Global Actions"

#define REFRESH \
    wrefresh(header.win);\
    wrefresh(body.win);\
    wrefresh(pers_func.win);\
    wrefresh(glob_func.win);

#define DRAW_BORDER \
    defineBorder(header);\
    defineBorder(body);\
    defineBorder(pers_func);\
    defineBorder(glob_func);

#define DESTROY \
    destroy_win(pers_func); \
    destroy_win(glob_func); \
    destroy_win(body);      \
    destroy_win(header);    \

#define BEG 0
#define DH  1
#define RSA 2
#define ELGAMAL 3

#define BUFF_SIZE 100
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char **argv) {
    //{{{ Declaration
    srand(time(NULL));
    WIN glob_func, pers_func, header, body, ask;
	int ch, cur_window = BEG;
    char buffer[BUFF_SIZE];
    int body_position = 1, i;

    long long int dhgen = 0, dhkey = 0, dhord = 0, dhrand = 0, dhalrand = 0, dhalkey = 0, dhalice = 0, dhbob = 0;
    long long int rsaaprime = 0, rsabprime = 0, rsan = 0, rsaphi = 0;
    rsapbk_t rsapbkey;
    rsapvk_t rsaprivkey;

    memset(&rsapbkey, 0, sizeof(rsapbkey));
    memset(&rsaprivkey, 0, sizeof(rsaprivkey));

    char *title[] = {"Asymetric Cryptography", "Diffie Hellman", "Rivest Shamir Adlemar", "El Gamal"};
    char *goptions[] = {GFUNC, "F1  : Diffie Hellman", "F2  : RSA", "F3  : El Gamal", "F12 : Quit", NULL};

    char *dhoptions[] = {PFUNC, "g : Generator definition", "o : Order definition", "r : Pick random number",
        "k : Key generation", "p : Print Informations", NULL};
    char *rsaoptions[] = {PFUNC, "k : Key generation", "e : Encrypt", "d : Decrypt",
        "n : Prime numbers definition", "p : print", NULL};
    char *egamaloptions[] = {PFUNC, "g : Generator definition", "o : Order definition", "k : Private Key generation",
        "K : Public Key generation", "d : Decryption", "e : Encrytion", "p : Print Informations", NULL};
    //}}}

    //{{{ Launching ncurse
    initscr();
    // Disable line buffering
    cbreak();
    // Enable function keys (<F1>,<F2>,<F3>,...)
    keypad(stdscr, TRUE);
    // Disable echoing
    noecho();
    refresh();
    //}}}

    // {{{ Windows initialization
    initWin(&header, (int)(LINES - 2*MARGE_Y) * 0.1, COLS - 2*MARGE_X,
            MARGE_X, MARGE_Y, '|', '|', '=', '=', '*', '*', '*', '*');
    initWin(&body, LINES - SEP_Y - header.height - 2*MARGE_Y, (int)(COLS - 2*MARGE_X)*0.7,
            MARGE_X, MARGE_Y + SEP_Y + header.height, '|', '|', '=', '=', '*', '*', '*', '*');
    initWin(&pers_func, (body.height)*0.4, COLS - 2*MARGE_X - SEP_X - body.width, MARGE_X + SEP_X + body.width,
            MARGE_Y + SEP_Y + header.height, '|', '|', '=', '=', '*', '*', '*', '*');
    initWin(&glob_func, body.height - SEP_Y - pers_func.height, pers_func.width, pers_func.startx,
            pers_func.starty + SEP_Y + pers_func.height, '|', '|', '=', '=', '*', '*', '*', '*');

    initWin(&ask, 3, 80, (int)((COLS - 80)/2), (int)((LINES - 3)/2),
            '|', '|', '=', '=', '*', '*', '*', '*');

    createWindow(&header);
    createWindow(&body);
    createWindow(&pers_func);
    createWindow(&glob_func);

    mvwprintw(header.win, ceil((header.height)/2), ceil((header.width - sizeof(title[cur_window]))/2), title[cur_window]);
    mvwprintw(body.win, 0, ceil((body.width - sizeof(BODY))/2), BODY);
    mvwprintw(pers_func.win, 0, 3, PFUNC);
    mvwprintw(glob_func.win, 0, 3, GFUNC);

    printOptions(glob_func, goptions);

    REFRESH;
    // }}}

    // {{{ Key Management
    while((ch = getch()) != KEY_F(12)) {
        long long int tmpord;
        switch(ch) {
            //{{{ F1
            case KEY_F(1):
                // Diffie Hellman
                cur_window = DH;
                printOptions(pers_func, dhoptions);
                werase(header.win);                
                defineBorder(header);
                mvwprintw(header.win, ceil((header.height)/2), 
                        ceil((header.width - sizeof(title[cur_window]))/2), title[cur_window]);
                wrefresh(header.win);
                wrefresh(pers_func.win);
                break;
                //}}}
                //{{{ F2
            case KEY_F(2):
                // RSA
                cur_window = RSA;
                printOptions(pers_func, rsaoptions);
                werase(header.win);                
                defineBorder(header);
                mvwprintw(header.win, ceil((header.height)/2), 
                        ceil((header.width - sizeof(title[cur_window]))/2), title[cur_window]);
                wrefresh(header.win);
                wrefresh(pers_func.win);
                break;
                //}}}
                //{{{ F3
            case KEY_F(3):
                // ElGamal
                cur_window = ELGAMAL;
                printOptions(pers_func, egamaloptions);
                werase(header.win);                
                defineBorder(header);
                mvwprintw(header.win, ceil((header.height)/2), 
                        ceil((header.width - sizeof(title[cur_window]))/2), title[cur_window]);
                wrefresh(header.win);
                wrefresh(pers_func.win);
                break;
                //}}}
                // {{{ d
            case 'd':
                // {{{ RSA
                if (cur_window == RSA) {
                    if (!rsaprivkey.privkey) {
                        printWindow(body, &body_position, "[Error] You have to generate a couple of key before.");
                        wrefresh(body.win);
                    }
                    else {
                        createWindow(&ask);
                        echo();
                        long long int rsadecrypt, initmess;
                        do {
                            wrefresh(body.win);
                            requestWindow(ask, "Enter the integer to decrypt : ", buffer);
                            rsadecrypt = atoll(buffer); 
                        } while ((rsadecrypt > rsaphi && printWindow(body, &body_position,
                                    "[Error] Invalid encrypted int, it must be lesser or equal to Euler totient function")) ||
                                (rsadecrypt <= 1 && printWindow(body, &body_position,
                                    "[Error] Invalid int : must be greater than 1")));
                        initmess = rsaDecrypt(rsaprivkey, rsadecrypt);
                        destroy_win(ask);
                        sprintf(buffer, "[RSA] Initial Message : %lld", initmess);
                        printWindow(body, &body_position, buffer);
                        DRAW_BORDER;
                        REFRESH;
                        noecho();
                    }
                }
                // }}}
                break;
                // }}}
                // {{{ e
            case 'e':
                // {{{ RSA
                if (cur_window == RSA) {
                    if (!rsapbkey.pubkey) {
                        printWindow(body, &body_position, "[Error] You have to generate keys before.");
                        wrefresh(body.win);
                        continue;
                    }
                    long long int encrypt, initmess;
                    createWindow(&ask);
                    echo();
                    do {
                        wrefresh(body.win);
                        requestWindow(ask, "Please enter the integer to encrypt : ", buffer);
                        initmess = atoll(buffer);
                    } while ((initmess > rsan && printWindow(body, &body_position, 
                                    "[Error] The integer must be lesser than n.")) ||
                            (initmess < 1 && printWindow(body, &body_position, 
                                    "[Error] The integer must be greater than 0.")));
                    destroy_win(ask);
                    noecho();
                    DRAW_BORDER;
                    encrypt = rsaEncrypt(rsapbkey, initmess);
                    sprintf(buffer, "[RSA] Encryption of %lld : %lld.", initmess, encrypt);
                    printWindow(body, &body_position, buffer);
                    REFRESH;
                }
                // }}}
                break;
                // }}}
                //{{{g
            case 'g':
                // {{{ DH
                // If diffie-hellman screen then ask for a generator
                if (cur_window == DH) {
                    // dhord have to be filled previously
                    if (dhord) {
                        createWindow(&ask);
                        echo();
                        do {
                            // Create a request window
                            wrefresh(body.win);
                            requestWindow(ask, "Please enter a generator : ", buffer);
                            dhgen = atoll(buffer);
                        } while ((dhgen < 2 && 
                                    // If dhgen < 2 then this part is run
                                    // This part must always be true (a && (b || 1)) = a && 1 = a
                                printWindow(body, &body_position,"[Error] Integer must be greater than 1.")) ||
                                (dhgen > dhord &&
                                    // If dhgen greate than dhord then this part is run
                                    // This part must always be true (a && (b || 1)) = a && 1 = a
                                printWindow(body, &body_position, "[Error] Generator must be lesser or equal to order.")) ||
                                ((!isGenerator(dhord, dhgen)) &&
                                 printWindow(body, &body_position, "[Error] Number entered isn't a generator.")));
                        sprintf(buffer, "[DH] Group generator saved : %lld", dhgen);
                        printWindow(body, &body_position, buffer);
                        wrefresh(body.win);
                        noecho();
                        destroy_win(ask);
                        DRAW_BORDER;
                        REFRESH;
                    }
                    else {
                        printWindow(body, &body_position, "[Error] You must inform group order before.");
                        wrefresh(body.win);
                    }
                }
                //}}}
                break;
                //}}}
                // {{{ k
            case 'k':
                // {{{ Diffie Hellman
                if (cur_window == DH) {
                // Every other field have to be informed
                    if (dhord && dhrand && dhgen) {
                        createWindow(&ask);
                        wrefresh(ask.win);
                        if (yesOrNo(ask, "Do you want to manually enter a value for Alice key")) {
                            echo();
                            // Ask for the number computed by alice
                            do {
                                wrefresh(body.win);
                                requestWindow(ask, "Please enter number sent by Alice: ", buffer);
                                dhalice = atoll(buffer);
                            } while ((dhalice < 1 && 
                                    printWindow(body, &body_position, "[Error] Integer must be greater than 0.")) ||
                                    (dhalice > dhord &&
                                    printWindow(body, &body_position, 
                                        "[Error] Integer must be lesser or equal to group order")));
                            sprintf(buffer, "[DH] Alice number entered : %lld", dhalice);
                            printWindow(body, &body_position, buffer);
                            DRAW_BORDER;
                            REFRESH;
                            noecho();
                        }
                        else {
                            dhalrand = rand() % dhord + 1;
                            // TODO Fast exponential
                            dhalice = fastExp(dhgen, dhalrand, dhord + 1);
                            sprintf(buffer, "[DH] Alice sent you this number : %lld", dhalice);
                            printWindow(body, &body_position, buffer);
                            wrefresh(body.win);
                        }
                        destroy_win(ask);
                        dhbob = fastExp(dhgen, dhrand, dhord + 1);
                        sprintf(buffer, "[DH] You sent to Alice this number : %lld", dhbob);
                        printWindow(body, &body_position, buffer);
                        dhkey = fastExp(dhalice, dhrand, dhord + 1);
                        sprintf(buffer, "[DH] Key computed : %lld", dhkey);
                        printWindow(body, &body_position, buffer);
                        dhalkey = fastExp(dhbob, dhalrand, dhord + 1);
                        sprintf(buffer, "[DH] Alice key computed : %lld", dhalkey);
                        printWindow(body, &body_position, buffer);
                        wrefresh(body.win); 
                    }
                    if (!dhord) {
                        printWindow(body, &body_position, "[Error] You must inform group order before.");
                        wrefresh(body.win);
                    }
                    if (!dhrand) {
                        printWindow(body, &body_position, "[Error] You have to generate random number before.");
                        wrefresh(body.win);
                    }
                    if (!dhgen) {
                        printWindow(body, &body_position, "[Error] You must inform group generator before.");
                        wrefresh(body.win);
                    }
                    //}}}
                    // {{{ RSA
                } else if (cur_window == RSA) {
                    if (rsaphi) {
                        rsaKeyGen(rsan, rsaaprime, rsabprime, rsaphi, &rsaprivkey, &rsapbkey);
                        sprintf(buffer, "[RSA] Key computed : %lld", rsaprivkey.privkey);
                        printWindow(body, &body_position, buffer);
                        sprintf(buffer, "[RSA] Public key computed : %lld", rsapbkey.pubkey);
                        printWindow(body, &body_position, buffer);
                        sprintf(buffer, "[RSA] Key product : %lld", (rsapbkey.pubkey * rsaprivkey.privkey) % rsaphi);
                        printWindow(body, &body_position, buffer);
                        wrefresh(body.win);
                    }
                    else {
                        printWindow(body, &body_position, "[Error] You jave to define prime numbers before.");
                        wrefresh(body.win);
                    }
                }
                    // }}}
                break;
                // }}}
                //{{{ o
            case 'o':
                // {{{ DH
                if(cur_window == DH) {
                    createWindow(&ask);
                    echo();
                    do {
                        wrefresh(body.win);
                        requestWindow(ask, "Please enter a prime number: ", buffer);
                        tmpord = atoll(buffer);
                    } while ((tmpord < 2 && 
                                printWindow(body, &body_position, "[Error] Integer must be greater than 1.")) ||
                            (!(eratosthene(tmpord)) &&
                             printWindow(body, &body_position, "[Error] Number has to be prime.")));
                    tmpord--;
                    sprintf(buffer, "[DH] Group order computed : %lld", tmpord);
                    printWindow(body, &body_position, buffer);
                    wrefresh(body.win);
                    noecho();
                    destroy_win(ask);
                    DRAW_BORDER;
                    REFRESH;
                    dhord = tmpord;
                }
                // }}}
                break;
                //}}}
                // {{{ p
            case 'p':
                // {{{ Diffie Hellman print
                if (cur_window == DH) {
                    dhord ? sprintf(buffer, "[DH] Order : %lld.", dhord) : buffer == strcpy(buffer,
                            "[DH] Order not yet defined.");
                    printWindow(body, &body_position, buffer);
                    dhkey ? sprintf(buffer, "[DH] Key : %lld.", dhkey) : buffer == strcpy(buffer,
                            "[DH] Key not yet defined.");
                    printWindow(body, &body_position, buffer);
                    dhrand ? sprintf(buffer, "[DH] random number : %lld.", dhrand) : buffer == strcpy(buffer,
                            "[DH] random number not yet defined.");
                    printWindow(body, &body_position, buffer);
                    dhgen ? sprintf(buffer, "[DH] generator : %lld.", dhgen) : buffer == strcpy(buffer,
                            "[DH] generator not yet defined.");
                    printWindow(body, &body_position, buffer);
                }
                // }}}
                // {{{ RSA print
                else if (cur_window == RSA) {
                    rsaphi ? sprintf(buffer, "[RSA] Euler Totient Function : %lld.", rsaphi) : buffer == strcpy(buffer,
                            "[RSA] Euler Totient Function not yet defined.");
                    printWindow(body, &body_position, buffer);
                    rsaaprime ? sprintf(buffer, "[RSA] first prime number : %lld.", rsaaprime) : buffer == strcpy(buffer,
                            "[RSA] first prime number not yet defined.");
                    printWindow(body, &body_position, buffer);
                    rsabprime ? sprintf(buffer, "[RSA] second prime number : %lld.", rsabprime) : buffer == strcpy(buffer,
                            "[RSA] second prime number not yet defined.");
                    printWindow(body, &body_position, buffer);
                    rsan ? sprintf(buffer, "[RSA] n : %lld.", rsan) : buffer == strcpy(buffer,
                            "[RSA] n not yet computed.");
                    printWindow(body, &body_position, buffer);
                    rsaphi ? sprintf(buffer, "[RSA] phi(n) : %lld.", rsaphi) : buffer == strcpy(buffer,
                            "[RSA] phi(n) not yet computed.");
                    printWindow(body, &body_position, buffer);
                    rsapbkey.pubkey ? sprintf(buffer, "[RSA] Public Key (n, k) : (%lld, %lld).", rsapbkey.n,
                            rsapbkey.pubkey) : buffer == strcpy(buffer, "[RSA] Public Key not yet defined.");
                    printWindow(body, &body_position, buffer);
                    rsaprivkey.privkey ? sprintf(buffer, "[RSA] Key (p, q, k): (%lld, %lld, %lld).", rsaprivkey.p,
                            rsaprivkey.q, rsaprivkey.privkey) : buffer == strcpy(buffer, "[RSA] Key not yet defined.");
                    printWindow(body, &body_position, buffer);
                }
                //}}}
                wrefresh(body.win);
                break;                
                // }}}
                //{{{ r
            case 'r':
                // {{{ DH
                if (cur_window == DH) {
                    // Order has to be filled previously
                    if (dhord) {
                        dhrand = rand()%dhord + 1;
                        sprintf(buffer, "[DH] Random integer computed : %lld", dhrand);
                        printWindow(body, &body_position, buffer);
                        wrefresh(body.win);
                    }
                    else {
                        printWindow(body, &body_position, "[Error] You must inform group order before.");
                        wrefresh(body.win);
                    }
                }
                    //}}}
                break;
                // }}}
                // {{{ n
            case 'n':
                // {{{ RSA
                if (cur_window == RSA) {
                    long long int primes[2] = {0};
                    char tmpchar[100];
                    createWindow(&ask);
                    echo();
                    for (i=0; i<2; i++) {
                        sprintf(tmpchar, "Please enter a prime number (%d/2) : ", i+1);
                        do {
                            wrefresh(ask.win);
                            requestWindow(ask, tmpchar, buffer);
                            primes[i] = atoll(buffer);
                        } while ((primes[i] < 2 && printWindow(body, &body_position,
                                        "[Error] The integer must be greater than 1.")) ||
                                (!eratosthene(primes[i]) && printWindow(body, &body_position, 
                                        "[Error] Given integer isn't prime.")));
                    }
                    noecho();
                    rsaaprime = primes[0];
                    rsabprime = primes[1];
                    sprintf(buffer, "[RSA] You entered this two prime numbers : %lld and %lld.", rsaaprime, rsabprime);
                    printWindow(body, &body_position, buffer);
                    // n
                    rsan = (rsaaprime * rsabprime);
                    sprintf(buffer, "[RSA] n computed : %lld", rsan);
                    printWindow(body, &body_position, buffer);
                    // phi
                    rsaphi = (rsaaprime - 1) * (rsabprime -1);
                    sprintf(buffer, "[RSA] phi computed : %lld", rsaphi);
                    printWindow(body, &body_position, buffer);

                    destroy_win(ask);
                    DRAW_BORDER;
                    REFRESH;
                }
                // }}}
                break;
                // }}}
                // {{{ CTRL + l
            case CTRL('l'):
                DESTROY;
                clear();
                refresh();
                initWin(&header, (int)(LINES - 2*MARGE_Y) * 0.1, COLS - 2*MARGE_X,
                        MARGE_X, MARGE_Y, '|', '|', '=', '=', '*', '*', '*', '*');
                initWin(&body, LINES - SEP_Y - header.height - 2*MARGE_Y, (int)(COLS - 2*MARGE_X)*0.7,
                        MARGE_X, MARGE_Y + SEP_Y + header.height, '|', '|', '=', '=', '*', '*', '*', '*');
                initWin(&pers_func, (body.height)*0.4, COLS - 2*MARGE_X - SEP_X - body.width, MARGE_X + SEP_X + body.width,
                        MARGE_Y + SEP_Y + header.height, '|', '|', '=', '=', '*', '*', '*', '*');
                initWin(&glob_func, body.height - SEP_Y - pers_func.height, pers_func.width, pers_func.startx,
                        pers_func.starty + SEP_Y + pers_func.height, '|', '|', '=', '=', '*', '*', '*', '*');
                initWin(&ask, 3, 80, (int)((COLS - 80)/2), (int)((LINES - 3)/2),
                        '|', '|', '=', '=', '*', '*', '*', '*');

                createWindow(&header);
                createWindow(&body);
                createWindow(&pers_func);
                createWindow(&glob_func);

                mvwprintw(header.win, ceil((header.height)/2), ceil((header.width - sizeof(title[cur_window]))/2),
                        title[cur_window]);
                mvwprintw(body.win, 0, ceil((body.width - sizeof(BODY))/2), BODY);
                mvwprintw(pers_func.win, 0, 3, PFUNC);
                mvwprintw(glob_func.win, 0, 3, GFUNC);

                printOptions(glob_func, goptions);
                if (cur_window == DH)
                    printOptions(pers_func, dhoptions);
                else if(cur_window == RSA)
                    printOptions(pers_func, rsaoptions);

                refresh();
                DRAW_BORDER;
                REFRESH;
                break;
                // }}}
        }
    }
    // }}}

    // {{{ cleaning
    DESTROY;
	endwin();			/* End curses mode		  */
    //}}}
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
