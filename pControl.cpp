#include <stdio.h>
#include <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);
//Networks, sending signal
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

int main(){
    //This sets up the RPi hardware 
    init(0);
    
    //Send signal to gate
    connect_to_server("192.168.1.2", 1024);
	send_to_server("please"); 
	char message[24];
	receive_from_server(message);
    
    
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
            ////////////////////////////////////////////////////////////////
            display_picture(2,0); //Display picture for debugging purposes
            ////////////////////////////////////////////////////////////////
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
		////////////////////////////////////////////////////////////
		printf("%f", errorSignal); //Print error signal for Debugging purposes
		////////////////////////////////////////////////////////////
        prop = (errorSignal*127/160);//proportional control
        //the *127/160 scales the value so the motor can handle it
        //equilibrium position: both motors are set to 127

        double rightMotor = 127-kp*prop;
        double leftMotor = -(127+kp*prop);//negative so motors turn in the same direction

        set_motor(1, rightMotor); //set motor speeds
        set_motor(2, leftMotor);

    }

    return 0;
}
