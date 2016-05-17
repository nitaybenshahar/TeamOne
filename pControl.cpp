#include <stdio.h>
#include <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);
//Networks, sending signal

int main(){
    //This sets up the RPi hardware 
    init(1);
    //Define variables
    char c; //stores whiteness of pixel
    float kp = 1; //constant of proportionality control
    float ki = 0;//constant of integral control, not used at the moment
    float kd = 0.5;//constant of derivative control, not used at the moment

    //Test: try changing the variable types to 'signed long' instead of 'double', commented out for now
    //signed long errorSignal;
    //signed long prop;
    //signed long rightMotor;
    //signed long leftMotor;

    double errorSignal;//the total distance away from the line
    double prop;//scaled error signal of proportionality, scaled for motor to handle
    // moved all these so they initialised here - initialising in a loop might cause weird memory stuff? iuno
    double rightMotor;
    double leftMotor;
    int whiteTotal;
    int numberOfWhite;
    
    //int current_error;
    //int error_diff;

    while(true){
        //initialise values
        whiteTotal = 0;//sum of the position of the white pixels, measured from center = 0
        numberOfWhite = 0;//running total of the number of white pixels
        
        
        take_picture();
        ////////////////////////////////////////////////////////////////
        display_picture(2,0); //Display picture for debugging purposes
        ////////////////////////////////////////////////////////////////

        for(int i=0; i<320; i++){
        	
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
        if (numberOfWhite == 0){ // this should never happen for first 3 quadrants - useful for debugging, though
       		printf("%s\n", "No white detected");
       		break; // should become 'cut to maze method' later, will just make a 'motorspeed = 0' method for now
       	}
        
        if ((whiteTotal >= 0) && (numberOfWhite >= 1)) { // Arthur's adjustment, adjusted a little more
        // && number of white clause is so that it doesn't try to divide by 0. I think this was our bug
        // Needs more review - we may still have no way to track when it's left of center
       	// might actually still be broken if we can't get exact center values or left values to print

        	errorSignal = whiteTotal/numberOfWhite; //center of the white line, running from -160 through 0 to 160
        	
		////////////////////////////////////////////////////////////
		printf("%d\n", errorSignal); //Print error signal for Debugging purposes
		////////////////////////////////////////////////////////////
        } // likely else if statement for negative values to go here - commented out because tired & may not work
         else if ((whiteTotal < 0) && (numberOfWhite >= 1)) {
        	errorSignal = whiteTotal/numberOfWhite;
        	printf("%d", errorSignal);
        } // note - all this is super redundant if the bug is just to do with #numberOfWhite, & if its not this may not work
        // if it works, delete this and change the first method (delete the whiteTotal portion, make condition "if numberOfWhite != 0"
        // or just >0 since it should never go negative
        
        //error_dif = current_error - errorSignal;
        //derivative_signal = (error_diff/errorSignal)*kd
        //errorSignal = current_error;
        
        //need to find current error
        
        prop = (errorSignal*127/160);//proportional control
        //the *127/160 scales the value so the motor can handle it
        //equilibrium position: both motors are set to 127

        rightMotor = 127-kp*prop;
        leftMotor = -(127+kp*prop);//negative so motors turn in the same direction

        set_motor(1, rightMotor); //set motor speeds
        set_motor(2, leftMotor);

    }
    // closing method once loop is exited - makes motors stop
    // should run if the camera ever detects 'no white'
   set_motor(1, 0);
   set_motor(2, 0);
    printf("%s", "\nProgram fin");
    return 0;
}


