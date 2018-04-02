#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

int main()
{
  int _W, _H, i, j, c, pos = 0;
  char *title = "M I N E S W E E P E R";
  char *difficulty[] = {
    "E A S Y ( 9 X 9, 1 6 M I N E S )",
    "M E D I U M ( 1 6 X 1 6, 4 0 M I N E S )",
    "H A R D ( 3 0 X 1 6, 9 9 M I N E S )",
    "C U S T O M"
  };  

  /* Start curse mode */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  /* Set cursor invisible */
  curs_set(0);

  /*
  printw("Upper left corner           "); addch(ACS_ULCORNER); printw("\n");
  printw("Lower left corner           "); addch(ACS_LLCORNER); printw("\n");
  printw("Upper right corner          "); addch(ACS_URCORNER); printw("\n");
  printw("Lower right corner          "); addch(ACS_LRCORNER); printw("\n");
  */

  /* Get current screen's width and height */
  getmaxyx(stdscr, _H, _W);

  /* Print title */
  for(i = 0; title[i]; i++){
    mvprintw(_H/2 - 5, _W/2 - 10 + i, "%c", title[i]);
    usleep(40000);
    refresh();
  }

  /* Blink title, unnecessary but it looks cool tho `,:) */
  for(i = 0; i < 3; i++){
    usleep(200000);
    clear();
    refresh();
    usleep(200000);
    mvprintw(_H/2 - 5, _W/2 - 10, "%s", title);
    refresh();
  }


  /* Print difficulties */
  for(i = 0; i < 4; i++){
    usleep(200000);
    mvprintw(_H/2 - 3 + i*2, _W/2 - 10, "%s", difficulty[i]);
    refresh();
  }
  mvprintw(_H/2 - 3 + pos*2, _W/2 - 12, ">");
  refresh();

  /* Get key-input selection */
  while((c = getch()) != '\n'){
    if(c == KEY_UP || c == KEY_DOWN){
      mvprintw(_H/2 - 3 + pos*2, _W/2 - 12, " ");
      pos = pos + (c == KEY_UP ? -1 : 1) & 3;
    }
    
    mvprintw(_H/2 - 3 + pos*2, _W/2 - 12, ">");
    refresh();
  }

  endwin();
  return 0;
}
