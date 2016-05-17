#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);

int main(){
    init(0);

    while(true){
       take_picture();
       display_picture(2,0);
       Sleep(0,500000);
       }
return 0;}
