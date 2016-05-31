#include <stdio.h>
#include <time.h>

extern "C" char get_pixel(int row,int col,int colour);
extern "C" int init(int d_lev);
extern "C" int take_picture();

int main(){
  char c;
  init(0);
  take_picture();
  for(int i = 320;i>0; i--){
    for(int j = 0; j<240; j++){
      c = get_pixel(i,j,3);
      printf(" %d",c);
    }
    printf("\n");
  }
  return 0;
}
