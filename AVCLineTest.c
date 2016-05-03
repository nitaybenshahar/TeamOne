#include <stdio.h>
#include <stdlib.h>


extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int init(int d_lev);
extern "C" int set_motor(int motor , int speed );

int main()
{
    while(1==1){
    char c;

        take_picture();
        c = get_pixel(160,120,3);


    set_motor(1,255);//sets motor connected to pin 1 to roate forward at max speed
    Sleep(0,5000000);
    set_motor(2,255); // sets motor connected to pin 2 to roatate forward at max speed
    Sleep(0,5000000);}
    return 0;





}
