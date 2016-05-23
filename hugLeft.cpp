#include <stdio.h>
#include <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);
//networking stuff - now working!
extern "C" int connect_to_server( char server_addr[15], int port);
extern "C" int send_to_server( char message[24]);
extern "C" int receive_from_server( char message[24]);

int main(){
    //This sets up the RPi hardware 
    init(1);
    //Define variables
    char c; //stores whiteness of pixel
    float kp = 1; //constant of proportionality control
    float ki = 0;//constant of integral control, not used at the moment
    float kd = 0.5;//constant of derivative control, not used at the moment
    
   //connects to server with the ip address 130.195.6.196, but opens then closes super quick, need to fix that.
   //Could use the camera to detect the gate and while it sees it ask for it to be open?
   //need to actually check first though if it is too fast for Bambi to get through.
   connect_to_server("130.195.6.196", 1024);
   //sends a message to the connected server
   send_to_server("Hello server");
   //receives message from the connected server
   char message[24];
   receive_from_server(message);
   printf("%s", message);
    
    //Test: try changing the variable types to 'signed long' instead of 'double', commented out for now
    //signed long errorSignal;
    //signed long prop;
    //signed long rightMotor;
    //signed long leftMotor;

    double errorSignal;//the total distance away from the line
    
    double prop;//scaled error signal of proportionality, scaled for motor to handle
    // moved all these so they initialised here - initialising in a loop might cause weird memory stuff?
    double rightMotor;
    double leftMotor;
    int whiteNext;
    
    //////////////////////////////////Derivative////////////////////////////////
    int previousError;
    double der;//similar to prop but for derivative control
	////////////////////////////////////////////////////////////////////////////

    while(true){
        //initialise values
        whiteNext = 0;//To check if the next 3 pixels are white
        
        
        take_picture();
        ////////////////////////////////////////////////////////////////
        display_picture(2,0); //Display picture for debugging purposes
        ////////////////////////////////////////////////////////////////

        for(int i=0; i<240; i++){ //Changed to 240 as camera is set sideways
        	
            //get pixel "whiteness"
            //resolution of image is 320x240
            c = get_pixel(20,i,3);//middle bit may need to be 240-i in order to start from the left

            if(c>150){//threshold for white
				//Check that the next 3 pixels are also white
				for(int j=1; j<=3; j++){
					c = get_pixel(20, i+j, 3);
					if(c>150){
						whiteNext++;
					}
				}
				if(whiteNext>2){//can be changed to increase/decrease sensetivity to white along with the white threshold
					errorSignal = i-160;
					break;
				}
				else
				{
					whiteNext = 0;//reset whiteNext for next iteraton as it failed to detect white
				}
            }
        }
	
	///////////////////////////proportional////////////////////////////////////////
	
		prop = (errorSignal*127/160);//proportional control
        //the *127/160 scales the value so the motor can handle it
        //equilibrium position: both motors are set to 127
	
	////////////////////////////////Derivative////////////////////////////////////
		der = (prevError-errorSignal)/0.5;//Difference between current and previous error signal divided by time difference of 0.5s
		//der = <something>*kd;
		//HAVE TO ALLOCATE A CERTAIN AMOUNT OF THE MOTOR SPEED TO EACH CONTROL TYPE< THAT WILL define what <something> is
		//Then update rightMotor and leftMotor to include derivative control
	
		//IF the error signal is really small just keep driving straight ahead
		if(errorSignal<5 &&errorSignal>-5){
			set_motor(1, 200); //set motor speeds
			set_motor(2, -200);
		}
		else
		{
			rightMotor = 127-kp*prop;
			leftMotor = -(127+kp*prop);//negative so motors turn in the same direction

			set_motor(1, rightMotor); //set motor speeds
			set_motor(2, leftMotor);
		}
        
		previousError = errorSignal;//Update previous_error as it stores the error signal of the previous iteration
		Sleep(0,500000);//currently set at 0.5 between every itiration

    }
    // closing method once loop is exited - makes motors stop
    // should run if the camera ever detects 'no white'
    //ideally should be making motors move directly backwards until they hit the white line again.
    //so set_motor(1, -rightMotor); and set_motor(2, -leftMotor);
    //in comments as I'm not sure how to make it revert back to normal code once back on line, perhaps while(false)?
    //This could also go up top, where the loop for whiteTotal<=1. Have aan if it gets to one then follow, if not then do this:
    /*
    while(false){ or maybe if(numberOfWhite = 0){
        set_motor(1,-rightMotor);
        set_motor(2, -leftMotor);
    }   <-- I have no idea if this would actually work in making the pi pay attention to whether it has got back to the white line or not.
    */
   set_motor(1, 0); //I'm pretty sure that we never need to make the robot stop as it doesn't need to know when it gets to the end, it just needs to get there.
   set_motor(2, 0); //So currently this is just here so that in testing our robot doesn't run forever.
    printf("%s", "\nProgram fin");
    return 0;
}


