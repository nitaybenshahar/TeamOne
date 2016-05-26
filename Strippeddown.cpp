# include <stdio.h>
# include <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);

int main (){
  init(0);
    while(1==1){
        take_picture();
        int c = 0;
        int count = 0;
        int error = 0;
        int numberofwhite;
            
        for(int i =0; i<240; i++){
          c = get_pixel(200,i,3);
            if(c>127){
              numberofwhite = 1;
            }
            else{
              numberofwhite = 0;
            }
            erorr = error+(i-120)*numberofwhite;
            count = count + numberofwhite;
          }
          if(numberofwhite>0){
            error = error/numberofwhite;
            int r = 70 + 0.5*error;
            int l = -70 - 0.5*error;
            set_motor(1, r);
            set_motor(2,l);
          }
          else{
            set_motor(1, -30); //reverse
            set_motor(2, -30);
          }
          Sleep(0,500000);
          }
          return 0;
}
