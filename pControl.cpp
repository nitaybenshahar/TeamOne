#include <stdio.h>
#include <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );

int main(){
    //This sets up the RPi hardware and ensures
    //everything is working correctly
    init(0);
    //Define variables
    char c; //stores whiteness of pixel
    float kp = 1; //constant of proportionality control
    float ki = 0;//constant of integral control, not used at the moment
    float kd = 0;//constant of derivative control, not used at the moment

    double errorSignal;//the total distance away from the line
    double prop;//scaled error signal of proportionality, scaled for motor to handle

    while(true){
        //initialise values
        int whiteTotal = 0;//sum of the position of the white pixels, measured from center = 0
        int numberOfWhite = 0;//running total of the number of white pixels

        for(int i=0; i<320; i++){
            take_picture();
            //get pixel "whiteness"
            //resolution of image is 320x240
            c = get_pixel(i,200,3);

            if(c<150){
                c = 0;  //Black pixel
            }
            else{
                c = 1;   //white pixel
                numberOfWhite++;
            }
            whiteTotal = whiteTotal + (i-160)*c; //add the position of the white pixels (if its white)
        }
        errorSignal = whiteTotal/numberOfWhite; //center of the white line, running from -160 through 0 to 160

        prop = (errorSignal*127/160);//proportional control
        //the *127/160 scales the value so the motor can handle it
        //equilibrium position: both motors are set to 127

        double rightMotor = 127-kp*prop;
        double leftMotor = 127+kp*prop;

        set_motor(1, rightMotor); //set motor speeds
        set_motor(2, leftMotor);

    }

    return 0;
}
