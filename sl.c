/*========================================
 *    sl.c: SL version 5.0
 *	Copyright 1993,1998,2013
 *                Toyoda Masashi 
 *		  (mtoyoda@acm.org)
 *	Last Modified: 2013/ 5/ 5
 *========================================
 */
/* sl version 5.00 : add -c option
/*                                              by Toyoda Masashi 2013/ 5/ 5 */
/* sl version 4.00 : add C51, usleep(40000)                                  */
/*                                              by Toyoda Masashi 2002/12/31 */
/* sl version 3.03 : add usleep(20000)                                       */
/*                                              by Toyoda Masashi 1998/ 7/22 */
/* sl version 3.02 : D51 flies! Change options.                              */
/*                                              by Toyoda Masashi 1993/ 1/19 */
/* sl version 3.01 : Wheel turns smoother                                    */
/*                                              by Toyoda Masashi 1992/12/25 */
/* sl version 3.00 : Add d(D51) option                                       */
/*                                              by Toyoda Masashi 1992/12/24 */
/* sl version 2.02 : Bug fixed.(dust remains in screen)                      */
/*                                              by Toyoda Masashi 1992/12/17 */
/* sl version 2.01 : Smoke run and disappear.                                */
/*                   Change '-a' to accident option.			     */
/*                                              by Toyoda Masashi 1992/12/16 */
/* sl version 2.00 : Add a(all),l(long),F(Fly!) options.                     */
/* 						by Toyoda Masashi 1992/12/15 */
/* sl version 1.02 : Add turning wheel.                                      */
/*					        by Toyoda Masashi 1992/12/14 */
/* sl version 1.01 : Add more complex smoke.                                 */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.00 : SL runs vomitting out smoke.                            */
/*						by Toyoda Masashi 1992/12/11 */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include "sl.h"

int LONGER = 0;
#define EXTRACARGONUM 10

static int phase = 0;
static void add_punpun(int, int);
static void add_baka(int, int);
static void add_golua(int, int);
static void add_cargo(int, int);

int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
	if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
	if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

void option(char *str)
{
    while (*str != '\0') {
	switch (*str++) {
	    case 'l': LONGER  = 1;  break;
	    default:                break;
	}
    }
}

int main(int argc, char *argv[])
{
    int x, i;

    for (i = 1; i < argc; ++i) {
	if (*argv[i] == '-') {
	    option(argv[i] + 1);
	}
    }
    initscr();
    signal(SIGINT, SIG_IGN);
    noecho();
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);

    for (x = COLS - 1; ; --x) {
	if (add_sl(x) == ERR) break;
	phase++;
	refresh();
	usleep(40000);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
}


int add_sl(int x)
{
    static char *sl[QUITHEIGHT]
    = {QUITSTR01, QUITSTR02, QUITSTR03, QUITSTR04, QUITSTR05, QUITSTR06,
       QUITSTR07, QUITSTR08, QUITSTR09, QUITSTR10, QUITSTR11, QUITSTR12,
       QUITSTR13, QUITSTR14, QUITSTR15, QUITSTR16, QUITSTR17};

    int y, i;

    if (x < -QUITLENGTH - LONGER * EXTRACARGONUM * EXTRACARGOLENGTH) return ERR;
    y = (LINES - QUITHEIGHT)/2;

    for (i = 0; i < QUITHEIGHT; ++i) {
	    my_mvaddstr(y + i, x, sl[i]);
    }
    add_punpun(y + 6, x);
    add_baka(y + 6, x + 38);
    add_golua(y + 6, x + 55);
    if (!LONGER) {
	add_smoke(y + 15, x + QUITLENGTH);
    } else {
	for(i = 0; i < EXTRACARGONUM; i ++) {
	    add_cargo(y + 6, x + QUITLENGTH + i * EXTRACARGOLENGTH);
	    if(i % 2 == 0) {
		add_baka(y + 6, x + 7 + QUITLENGTH + i * EXTRACARGOLENGTH);
	    } else {
		add_golua(y + 6, x + 5 + QUITLENGTH + i * EXTRACARGOLENGTH);
	    }
	}
   	 add_smoke(y + 15, x + QUITLENGTH + EXTRACARGONUM * EXTRACARGOLENGTH);
    }

    return OK;
}

int add_smoke(int y, int x)
{
    static char *Smoke[SMOKEPTNS] = { SMOKE01, SMOKE02, SMOKE03 };

    my_mvaddstr(y, x, Smoke[x % SMOKEPTNS]);
}

void add_cargo(int y, int x)
{
    static char *cargo[EXTRACARGOHEIGHT]
    = { EXTRACARGO0, EXTRACARGO0, EXTRACARGO0, EXTRACARGO1, EXTRACARGO2,
	EXTRACARGO3, EXTRACARGO4, EXTRACARGO5, EXTRACARGO6, EXTRACARGO7,
	EXTRACARGO8 };

    int i;
    for (i = 0; i < EXTRACARGOHEIGHT; ++i) {
	    my_mvaddstr(y + i, x, cargo[i]);
    }
}

void add_punpun(int y, int x)
{
    int i;
    static char *pun[PUNHEIGHT]
	= {PUNSTR01, PUNSTR02, PUNSTR03};

    for(i = 0; i < PUNHEIGHT; i++) {
	my_mvaddstr(y + i, x + (((unsigned int)x & 4)<<1), pun[i]);
    }
}

void add_baka(int y, int x)
{
    int i;
    static char *baka[BAKAHEIGHT]
	= {BAKASTR01, BAKASTR02, BAKASTR03};

    if ((phase & 15) < 8)
	for(i = 0; i < BAKAHEIGHT; i++) {
	    my_mvaddstr(y + i, x, baka[i]);
	}
}

void add_golua(int y, int x)
{
    int i;
    static char *golua[GOLUAHEIGHT]
	= {GOLUASTR01, GOLUASTR02, GOLUASTR03};

    if ((phase & 15) >= 8)
	for(i = 0; i < GOLUAHEIGHT; i++) {
	    my_mvaddstr(y + i, x, golua[i]);
	}
}

