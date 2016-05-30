# include <time.h>
# include <stdio.h>
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);
extern "C" int Sleep(int sec, int usec);
extern "C" int read_analog(int ch_adc);
int main(){
        int leftSensor = 0;
        int rightSensor = 0;

        init(1);
        while(true){
        Sleep(0,500000);
        leftSensor = read_analog(0); 
        rightSensor = read_analog(1);

        printf("Sensor 0: %d\n\n", leftSensor);
        printf("Sensor 1: %d\n\n", rightSensor);
        }  
set_motor(1,0);
set_motor(2,0);
return 0;
}
