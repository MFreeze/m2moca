/*
 * =====================================================================================
 *
 *       Filename:  menu.c
 *
 *    Description:  Implementation of menu.h 's functions
 *
 *        Version:  1.0
 *        Created:  02/12/2012 14:05:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "menu.h"

void cleanBuffer(char *selString) {
    char *p = strchr(selString, '\n'), c;
    if (!p)
        while((c = getchar()) != '\n' && c != EOF) {}
}

void printMenu(int *choice) {
    char buffer[50];
    do {
        printf("\n*=====================================*\n"
               "|       Please choose an action       |\n"
               "*=====================================*\n"
               "\n"
               "1. [Diffie Hellman] Key computation \n"
               "2. [RSA] Key computation \n"
               "3. [RSA] Encryption \n"
               "4. [RSA] Decryption \n"
               "\n"
               "Your choice : ");
        fgets(buffer,sizeof(buffer),stdin);
        cleanBuffer(buffer);
        *choice = atoi(buffer);
        printf("%d\n", *choice);
    } while (*choice <= 0 || *choice > 4);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  yesOrNo
 *  Description:  aimed at grabbing a yes or no answer to a question
 * =====================================================================================
 */
int yesOrNo(WIN ask, char *qu) {
    char ans;
    char buffer[150];
    sprintf(buffer, "%s [yn]", qu);
    do {
        mvwprintw(ask.win, 1, 2, buffer);
        wclrtoeol(ask.win);
        defineBorder(ask);
        wrefresh(ask.win);
        ans = tolower(wgetch(ask.win));
    } while (ans != 'y' && ans!='n');
    return (ans == 'y');
}		/* -----  end of function yesOrNo  ----- */

void createWindow(WIN *w) {	
	w->win = newwin(w->height, w->width, w->starty, w->startx);
	box(w->win, 0 , 0);	
    defineBorder(*w);
	wrefresh(w->win);	
}

void destroy_win(WIN w) {	
    wclear(w.win);
	wrefresh(w.win);
	delwin(w.win);
}

void initWin(WIN *win, int h, int w, int sx, int sy, char ls, char rs, char us, char ds, char c1, char c2, char c3, char c4) {
    win->height = h; 
    win->width = w; 
    win->ls = ls; 
    win->rs = rs; 
    win->us = us; 
    win->ds = ds;
    win->c1 = c1;
    win->c2 = c2;
    win->c3 = c3;
    win->c4 = c4; 
    win->startx = sx;
    win->starty = sy;
}

void printOptions(WIN win, char *opt[]) {
    int cursor = 0;
    werase(win.win);
    defineBorder(win);
    mvwprintw(win.win, cursor, 2, opt[cursor]);
    cursor++;
    while(opt[cursor]) {
        mvwprintw(win.win, cursor + 1, 2, opt[cursor]);
        cursor++;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  requestWindow
 *  Description:  
 * =====================================================================================
 */
void requestWindow(WIN ask, char *request, char *buffer) {
    mvwprintw(ask.win, 1, 2, request);
    wclrtoeol(ask.win);
    defineBorder(ask);
    wrefresh(ask.win);
    wgetstr(ask.win, buffer);
}		/* -----  end of function requestWindow  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printWindow
 *  Description:  
 * =====================================================================================
 */
int printWindow(WIN win, int *cpt, char *s) {
    if (*cpt >= win.height - 3) {
        wmove(win.win, 1, 2);
        wdeleteln(win.win);
        wmove(win.win, win.height -2, 2);
        wdeleteln(win.win);
        defineBorder(win);
        mvwprintw(win.win, *cpt, 2, s);
    }
    else {
        mvwprintw(win.win, *cpt, 2, s);
        (*cpt)++;
    }
    return 1;
}		/* -----  end of function printWindow  ----- */
