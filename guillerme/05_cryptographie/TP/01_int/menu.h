/*
 * =====================================================================================
 *
 *       Filename:  menu.h
 *
 *    Description:  Header file containing all usefull functions for printing menu
 *
 *        Version:  1.0
 *        Created:  02/12/2012 14:03:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

#define MENU_H

#define defineBorder(w) wborder((w).win, (w).ls, (w).rs, (w).us, (w).ds, (w).c1, (w).c2, (w).c3, (w).c4)


typedef struct win_v2 {
    char ls, rs, us, ds, c1, c2, c3, c4;
    int height, width;
    int startx, starty;
    WINDOW *win;
} WIN;

void printMenu(int *choice);

void initWin(WIN *win, int h, int w, int sx, int sy, char ls, char rs, char us, char ds, char c1, char c2, char c3, char c4);
void createWindow(WIN *windows);	
void destroy_win(WIN local_win);	

void requestWindow(WIN ask, char *quest, char *buffer);
int yesOrNo (WIN win, char *qu);

void printOptions(WIN win, char *opt[]);
int printWindow(WIN win, int *cpt, char *s);

#endif
