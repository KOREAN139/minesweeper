#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

/* Current screen's width & height */
int _W, _H;

int startScreen(){
  int i, c, pos = 0;
  char *title = "M I N E S W E E P E R";
  char *difficulty[] = {
    "E A S Y ( 9 X 9, 1 6 M I N E S )",
    "M E D I U M ( 1 6 X 1 6, 4 0 M I N E S )",
    "H A R D ( 3 0 X 1 6, 9 9 M I N E S )",
    "C U S T O M"
  };  

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
  
  return pos;
}

void drawBoard(int width, int height){
  int i, j;
  int baseW, baseH;

  /* Set base point at left upper corner of board */
  baseW = _W/2 - width;
  baseH = _H/2 - height;

  /* Draw upper half */
  usleep(100000);
  move(baseH, baseW);
  addch(ACS_ULCORNER);
  refresh();

  for(i = 1; i < 2*width + 1; i++){
    for(j = 0; j <= i && j < 2*height + 1; j++){
      move(baseH + j, baseW + i - j);
      if(i & 1) addch(j & 1 ? ACS_VLINE : ACS_HLINE);
      else{
        if(j & 1) continue;
        else if(j < 2*height) addch(j ? i > j ? ACS_PLUS : ACS_LTEE : ACS_TTEE);
        else addch(ACS_BTEE);
        if(i == j && j == 2*height) mvaddch(baseH + j, baseW, ACS_LLCORNER);
      }
    }
    usleep(20000);
    refresh();
  }
  
  /* Draw lower half */
  move(baseH, baseW + width*2);
  addch(ACS_URCORNER);

  for(i = 1; i < 2*height + 1; i++){
    for(j = 0; j + i < height*2 + 1 && j < 2*width + 1; j++){
      move(baseH + i + j, baseW + 2*width - j);
      if(i & 1) addch(j & 1 ? ACS_HLINE : ACS_VLINE);
      else{
        if(j & 1) continue;
        else if(j < 2*width) addch(j ? i + j < 2*height ? ACS_PLUS : ACS_BTEE : ACS_RTEE);
        else addch(ACS_LTEE);
        if(i + j == 2*height && j == 2*width) mvaddch(baseH + i + j, baseW, ACS_LLCORNER);
        if(i == 2*height) mvaddch(baseH + i, baseW + 2*width, ACS_LRCORNER);
      }
    }
    usleep(20000);
    refresh();
  }

  getch();
}

int main(){
  int op;
  int width, height, mine;

  /* Start curse mode */
  initscr();

  /* Make characters typed immediately available to the program */
  cbreak();

  /* Do not echo characters typed */
  noecho();
  
  /* Let getch() can get function key */
  keypad(stdscr, TRUE);

  /* Set cursor invisible */
  curs_set(0);

  /* Get current screen's width and height */
  getmaxyx(stdscr, _H, _W);

  /* Draw start screen, gets difficulty */
  op = startScreen();

  /* Set width, height with given option(difficulty) */
  switch(op){
    case 0: 
      width = 9; 
      height = 9; 
      mine = 16;
      break;
    case 1: 
      width = 16; 
      height = 16;
      mine = 40;
      break;
    case 2: 
      width = 30; 
      height = 16;
      mine = 99;
      break;
    case 3:
      clear();
      echo();
      curs_set(1);
      do{
        clear();
        mvprintw(_H/2 - 5, _W/2 - 10, "Set your custom game :)");
        mvprintw(_H/2 - 3, _W/2 - 10, "WIDTH (MAX %d) : ", _W/2 - 1);
        refresh();
        scanw("%d", &width);
      } while(width > _W/2 - 1);
      do{
        clear();
        mvprintw(_H/2 - 5, _W/2 - 10, "Set your custom game :)");
        mvprintw(_H/2 - 3, _W/2 - 10, "WIDTH (MAX %d) : %d", _W/2 - 1, width);
        mvprintw(_H/2 - 1, _W/2 - 10, "HEIGHT (MAX %d) : ", _H/2 - 1);
        refresh();
        scanw("%d", &height);
      } while(height > _H/2 - 1);
      do{
        clear();
        mvprintw(_H/2 - 5, _W/2 - 10, "Set your custom game :)");
        mvprintw(_H/2 - 3, _W/2 - 10, "WIDTH (MAX %d) : %d", _W/2 - 1, width);
        mvprintw(_H/2 - 1, _W/2 - 10, "HEIGHT (MAX %d) : %d", _H/2 - 1, height);
        mvprintw(_H/2 + 1, _W/2 - 10, "MINES (MAX %d) : ", width*height - 1);
        refresh();
        scanw("%d", &mine);
      } while(mine > width*height - 1);
      noecho();
      curs_set(0);
  }
  clear();
  refresh();

  /* Draw board */
  drawBoard(width, height);

  endwin();
  return 0;
}
