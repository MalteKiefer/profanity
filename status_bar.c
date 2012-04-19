/* 
 * status_bar.c
 *
 * Copyright (C) 2012 James Booth <boothj5@gmail.com>
 * 
 * This file is part of Profanity.
 *
 * Profanity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Profanity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Profanity.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string.h>

#include <ncurses.h>
#include "windows.h"
#include "util.h"

static WINDOW *status_bar;
static char _active[29] = "[ ][ ][ ][ ][ ][ ][ ][ ][  ]";

static int dirty;
static char curr_time[80];

static void _status_bar_update_time(void);

void create_status_bar(void)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    status_bar = newwin(1, cols, rows-2, 0);
    wbkgd(status_bar, COLOR_PAIR(3));
    wattron(status_bar, COLOR_PAIR(4));
    mvwprintw(status_bar, 0, cols - 29, _active);
    wattroff(status_bar, COLOR_PAIR(4));

    get_time(curr_time);
    dirty = TRUE;
}

void status_bar_refresh(void)
{
    char new_time[80];
    get_time(new_time);

    if (strcmp(new_time, curr_time) != 0) {
        dirty = TRUE;
        strcpy(curr_time, new_time);
    }

    if (dirty) {
        _status_bar_update_time();
        wrefresh(status_bar);
        inp_put_back();
        dirty = FALSE;
    }
}

void status_bar_inactive(const int win)
{
    int active_pos = 1 + ((win -1) * 3);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
 
    mvwaddch(status_bar, 0, cols - 29 + active_pos, ' ');
    if (win == 9)
        mvwaddch(status_bar, 0, cols - 29 + active_pos + 1, ' ');

    dirty = TRUE;
}

void status_bar_active(const int win)
{
    int active_pos = 1 + ((win -1) * 3);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
 
    if (win < 9)
        mvwprintw(status_bar, 0, cols - 29 + active_pos, "%d", win+1);
    else
        mvwprintw(status_bar, 0, cols - 29 + active_pos, "10");

    dirty = TRUE;
}

void status_bar_new(const int win)
{
    int active_pos = 1 + ((win -1) * 3);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
 
    wattron(status_bar, A_BLINK);
    if (win < 9)
        mvwprintw(status_bar, 0, cols - 29 + active_pos, "%d", win+1);
    else
        mvwprintw(status_bar, 0, cols - 29 + active_pos, "10");
    wattroff(status_bar, A_BLINK);

    dirty = TRUE;
}

void status_bar_get_password(void)
{
    mvwprintw(status_bar, 0, 9, "Enter password:");

    dirty = TRUE;
}

void status_bar_print_message(const char * const msg)
{
    mvwprintw(status_bar, 0, 9, msg);

    dirty = TRUE;
}

void status_bar_clear(void)
{
    wclear(status_bar);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    wattron(status_bar, COLOR_PAIR(4));
    mvwprintw(status_bar, 0, cols - 29, _active);
    wattroff(status_bar, COLOR_PAIR(4));

    dirty = TRUE;
}

static void _status_bar_update_time(void)
{
    char bar_time[6];
    char tstmp[80];
    get_time(tstmp);
    sprintf(bar_time, "%s", tstmp);

    wattron(status_bar, COLOR_PAIR(4));
    mvwaddch(status_bar, 0, 1, '[');
    wattroff(status_bar, COLOR_PAIR(4));
    mvwprintw(status_bar, 0, 2, bar_time);
    wattron(status_bar, COLOR_PAIR(4));
    mvwaddch(status_bar, 0, 7, ']');
    wattroff(status_bar, COLOR_PAIR(4));

    dirty = TRUE;
}

