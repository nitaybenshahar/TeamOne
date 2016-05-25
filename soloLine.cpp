
# include <stdio.h>
# include <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);

int main (){
    
    init(1);
//hi    
    char c;
    char cFuture;
    // keep the 'k' constants summing to 1 when testing - they're there for weighting, let's keep it simple
    float kp = 0.95;
    float ki = 0.3;
    float kd = 0.04;
    
    double current_error = 0;
    double prev_error = 0;

	double totalCount;
	double currentError;
    
    double propSignal;
    double derivativeSignal;
    double inteSignal;
    double finalSignal;
    double adjustment = 0;
    
    double errorTotal;
    
    double rightMotor = 0;
    double leftMotor = 0;
    
    int whiteTotal;
    int numberOfWhite;
    int futureNumberOfWhite;
    int futureWhiteTotal;
    int counter = 0;
    
    while(true){
        if (counter >= 50) { // main exit loop command
		printf("%s\n", "50 loops completed"); // adjust to counter+" loops completed" when you can check C syntax
		break;
	}
        whiteTotal = 0;//sum of the position of the white pixels, measured from center = 0
        numberOfWhite = 0;//running total of the number of white pixels
       
        
        take_picture();
        //display_picture(2,0); //Display picture for debugging purposes

        for(int i=0; i<240; i++){
        	
            //get pixel "whiteness"
            //resolution of image is 320x240
            c = get_pixel(230,i,3);

            if(c<200){ // 320/2 ie. if (c < half)
                c = 0;  //Black pixel
            }
            else{
                c = 1;   //white pixel
                numberOfWhite++;
            }
            whiteTotal = whiteTotal + (i-120)*c; //add the position of the white pixels (if its white)
            
            // the 'check ahead' method
            cFuture = get_pixel(i, 230, 3); // currently close to the initial pixel read – may work better if they’re further apart 
            // – adjust with testing
            if(c<160){
                cFuture = 0;  //Black pixel
            }
            else{
                cFuture = 1;   //white pixel
                futureNumberOfWhite++;
            }
            double whiteFutureTotal= whiteFutureTotal + (i-120)*c; 
        
	// intersection navigation methods
        /*if (numberOfWhite == 0) { // go left for a while
		set_motor(1, -40); 
	      	set_motor(2, 40);
		Sleep(2,0); // adjust to go ~180 degrees; maze variant will be more complex (& likely the biggest hurdle)

	} else if ((numberOfWhite > 40) && (propSignal <= 0)) { // should go left at any crossroads 
		set_motor(1, -40); 
        	set_motor(2, 40);
		Sleep(1,0);// hard left for sleep duration

	} else if ((numberOfWhite > 40) && (futureNumberOfWhite > 0)) { // else go straight if line continues
		set_motor(1, 40); 
        	set_motor(2, 40);
		Sleep(2,0); // may be too long; adjust
	} else if ((numberOfWhite > 40) && (propSignal > 0)) { // else turn right if line ONLY goes right
		rightMotor = 40;
		leftMotor = -40;
		Sleep(1,0); // hard right for sleep duration
	}*/
	// end of navigation method
        
        if (numberOfWhite >= 1) { // no dividing by 0
        	currentError = whiteTotal/numberOfWhite; //center of the white line, running from -160 through 0 to 160
		//printf("%d\n", currentError); //Print error signal for Debugging purposes
		counter++;
		
		Sleep(0,100000);


        } 
        errorTotal = errorTotal + currentError;
	totalCount = totalCount + 1;
        
        propSignal = currentError*kp;
        
        derivativeSignal = (currentError-prev_error/0.1)*kd;
	printf("Derivative signal is: %d", derivativeSignal);
	
	//inteSignal = (errorTotal/totalCount)*ki; // integral signal; running average
	
	//finalSignal = (currentError+derivativeSignal+inteSignal)/3; // the cumulative signal result
	finalSignal = propSignal+derivativeSignal;        

        adjustment = (finalSignal*20/120); // the actual value for the motors to use
        //the *127/160 scales the value so the motor can handle it
        //equilibrium position: both motors are set to 127
      
//        if (propSignal>-10 && propSignal < 10) { // adjust as necessary
//        	rightMotor = 70.0;
//        	leftMotor = 70.0;
//	printf("left motor if %f \n", leftMotor);
//	printf("right motor if %f \n", rightMotor);
//        } else {
        	rightMotor = 70.0+adjustment;
        	leftMotor = -(70.0-adjustment);//negative so motors turn in the same direction

	printf("adjustment in else %f \n", adjustment);
	printf("left motor else %f \n", leftMotor);
	printf("right motor else %f \n", rightMotor);        
//}

        set_motor(1, rightMotor); //set motor speeds
        set_motor(2, leftMotor);
        
	prev_error = currentError;
	printf("adjustment: %f", adjustment);
        }	
    } // end of primary loop
    // stop motors
    set_motor(1, 0);
    set_motor(2, 0);
    //printf("%s", "\nProgram fin"); // debugging - program complete msg
return 0;
}
