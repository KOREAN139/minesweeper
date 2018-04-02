#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>

int _W, _H;

int getScreenSize()
{
  int resized = 0;
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  /* If window were resized, get current window size. */
  if(_W != w.ws_col || _H != w.ws_row) {
    _W = w.ws_col;
    _H = w.ws_row;
    resized = 1;
  }

  return resized;
}

int drawStartScreen (int *width, int *height)
{
  int _width, _height, i;
  char in[4], *p;
  
  system("clear");

  for(i = 0; i < _H/2 - 3; i++) puts("");

  for(i = 0; i < _W/2 - 11; i++) printf(" ");
  puts("Set your board size\n");
  
  /* Get board's width. */
  for(i = 0; i < _W/2 - 6; i++) printf(" ");
  printf("width: ");
  scanf("%s", in);
  puts("");

  /* Check whether input is correct or not. */
  for(p = in; *p; _width = _width*10 + *p++ - '0')
    if(*p < '0' || *p > '9')
      goto err;

  /* Get board's width. */
  for(i = 0; i < _W/2 - 7; i++) printf(" ");
  printf("height: ");
  scanf("%s", in);
  puts("");

  /* Check whether input is correct or not. */
  for(p = in; *p; _height = _height*10 + *p++ - '0')
    if(*p < '0' || *p > '9')
      goto err;

  *width = _width;
  *height = _height;

  return 0;

err:

  /* When improper input was given. */
  for(i = 0; i < _W/2 - 8; i++) printf(" ");
  puts("Improper input");
  for(i = 0; i < _W/2 - 13; i++) printf(" ");
  puts("Press any key to restart");

  getchar();
  getchar();

  return 1;
}

int main(){
  int _W, _H;
  int width, height, i;

  while(1){
    // start screen
    getScreenSize();
    if(drawStartScreen(&width, &height))
      continue;

    // draw board
    scanf("%d", &_W);
  }
  return 0;
}
