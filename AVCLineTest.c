#include <stdio.h>
#include <time.h>

extern "C" int InitHardWare();
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int SetMotor(int motor, int dir, int speed);

int main()
{
    InitHardWare();
    while(1==1){
    char c;

        take_picture();
        c = get_pixel(160,120,3);


    SetMotor(1,1,255);//sets motor connected to pin 1 to roate forward at max speed
    Sleep(0,5000000);
    SetMotor(2,1,255); // sets motor connected to pin 2 to roatate forward at max speed
    Sleep(0,5000000);}
    return 0; // didn't the tutor say you delete the return 0 incase it causes an issue?
    
    // probably want to add a [set motor speed to 0] function at the end of this so you don't have the pi just running the motor
    // endlessly without a way to stop it when it comes time to test. Think arthur was insistent on that when we were playing with
    // this stuff last time.



}
