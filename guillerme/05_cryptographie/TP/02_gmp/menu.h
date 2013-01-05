/*
 * =====================================================================================
 *
 *       Filename:  menu.h
 *
 *    Description:  Header file for MMI 
 *
 *        Version:  1.0
 *        Created:  04/01/2013 23:52:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initMMI
 *  Description:  Initialization of usefull variables
 * =====================================================================================
 */
void initMMI(int *menu, int *submenu);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clearMMI
 *  Description:  Memory cleaning
 * =====================================================================================
 */
void clearMMI();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printAppropMenu
 *  Description:  Print Appropriate menu according to given parameters
 * =====================================================================================
 */
void printAppropMenu(int menu);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  askUser
 *  Description:  
 * =====================================================================================
 */
int askUser(int *menu, int *submenu);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getFile
 *  Description:  
 * =====================================================================================
 */
FILE *getFile(const char *mod);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  readFile
 *  Description:  
 * =====================================================================================
 */
char *readFile(FILE *f, size_t *s);

#endif
