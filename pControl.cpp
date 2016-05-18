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
    int whiteTotal;
    int numberOfWhite;
    
    ///////////////////////
    //Derivative
    //int current_error = 0;
    //int previous_error;
    //int error_diff;
    //////////////////////

    while(true){
        //initialise values
        whiteTotal = 0;//sum of the position of the white pixels, measured from center = 0
        numberOfWhite = 0;//running total of the number of white pixels
        
        
        take_picture();
        ////////////////////////////////////////////////////////////////
        display_picture(2,0); //Display picture for debugging purposes
        ////////////////////////////////////////////////////////////////

        for(int i=0; i<320; i++){ 
            //Some ideas for hugging the left hand side of the white line:
            //using an if statement which breaks after a white pixel is found, so only one position is stored and use that as we are usinf whiteTotal now.
            //could also use a for statement if really wanted and make it that it is for whiteTotal < 1
            //GET THIS WORKING ASAP, as it will save a lot of coding relating to intersections.
        	
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
       		break; 
       		//should be an if statement using the sensors to see if there are walls on BOTH sides, can't be just one as it could just be passing by a wall
       		//and if there are then cut to maze method. If not then should break so the motors reverse to find the white line again.
       		//idea:
       		//if loop detecting walls using sensors
	       		//if no left wall turn left,
	       		//else if no wall ahead go forward,
	       		//else if no wall to right, go right,
	       		//else turn back.
	       	//else, break
       	}
        
        if ((whiteTotal >= 0) && (numberOfWhite >= 1)) { // Arthur's adjustment, adjusted a little more
        // && number of white clause is so that it doesn't try to divide by 0. I think this was our bug
        // Needs more review - we may still have no way to track when it's left of center
       	// might actually still be broken if we can't get exact center values or left values to print

        	errorSignal = whiteTotal/numberOfWhite; //center of the white line, running from -160 through 0 to 160
        	//would be the far left of the line ideally
        	
		////////////////////////////////////////////////////////////
		printf("%d\n", errorSignal); //Print error signal for Debugging purposes
		////////////////////////////////////////////////////////////
        } // likely else if statement for negative values to go here - need to test to make sure it's working
         else if ((whiteTotal < 0) && (numberOfWhite >= 1)) {
        	errorSignal = whiteTotal/numberOfWhite;
        	printf("%d", errorSignal);
        } // note - all this is super redundant if the bug is just to do with #numberOfWhite, & if it's not this may not work
        // if it works, delete this and change the first method (delete the whiteTotal portion, make condition "if numberOfWhite != 0"
        // or just >0 since it should never go negative
        
        ////////////////////////////////////
        //Derivative
        //for(int i=0; i<320; i++){ Pretty much was we had above but since we have proportional error could we combine 
        //	whiteTotal = whiteTotal + (i-160)*c;              this to what we have above?
        //	current error = whiteTotal;
        //	Sleep(0,100000); 
        //error_dif = current_error - prevous_error;
        //derivative_signal = (error_diff/errorSignal)*kd Not sure what you actually divide by
        //error_dif = current_error - errorSignal;
        //previous_error = current_error;
        // rightMotor and leftMotor + derivative_signal;
	/////////////////////////////////////////
        
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
    //ideally should be making motors move directly backwards until they hit the white line again.
    //so set_motor(1, -rightMotor); and set_motor(2, -leftMotor);
    //in comments as I'm not sure how to make it revert back to normal code once back on line, perhaps while(false)?
    /*
    while(false){ or maybe if(numberOfWhite = 0){
        set_motor(1,-rightMotor);
        set_motor(2, -leftMotor);
    }   <-- I have no idea if this would actually work in making the pi pay attention to whether it has got back to the white line or not.
    */
   set_motor(1, 0);
   set_motor(2, 0);
    printf("%s", "\nProgram fin");
    return 0;
}


