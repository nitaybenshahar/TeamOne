
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
    
    char c;
    char cFuture;
    // keep the 'k' constants summing to 1 when testing - they're there for weighting, let's keep it simple
    float kp = 0.95;
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
		counter ++;
  		printf("%s\n", "50 loops completed"); // adjust to counter+" loops completed" when you can check C syntax
  		break;
    	}
        whiteTotal = 0;//sum of the position of the white pixels, measured from center = 0
        numberOfWhite = 0;//running total of the number of white pixels
       
        
        take_picture();
      
       for(int i=0; i<240; i++){
          
          //get pixel "whiteness"
          //resolution of image is 240x360
          c = get_pixel(320,i,3); //it's turning earlier than it should, indicating that it is seeing the turns earlier 
          
          if(c<200){ // 320/2 ie. if (c < half)
                c = 0;  //Black pixel
            }else{
              c = 1;   //white pixel
              numberOfWhite++;
          }
            
           whiteTotal = whiteTotal + (i-180)*c; //add the position of the white pixels (if its white)
       }
       
       if (numberOfWhite >= 1) { // no dividing by 0
        	currentError = whiteTotal/numberOfWhite; //center of the white line, running from -160 through 0 to 160
	      	printf("%f\n", currentError); //Print error signal for Debugging purposes
		
      		Sleep(0,500000);
        } 
       
        propSignal = currentError*kp;
        
        derivativeSignal = ((currentError-prev_error)/0.1)*kd;
      	printf("Derivative signal is: %f\n", derivativeSignal);
      	
      	finalSignal = propSignal+derivativeSignal;
      	
      	adjustment = (finalSignal*20/180); // the actual value for the motors to use
        //the *127/160 scales the value so the motor can handle it
        //equilibrium position: both motors are set to 127
        
        //currently bambi turns left but not right?!?!?! Nitay or Matt, any ideas?
        
       	rightMotor = 70.0+adjustment;
      	leftMotor = 70.0-adjustment;//I ran through the equations step by step and inside the brackets SHOULD be + yet when set like that it makes the motors equal???
        
      	printf("adjustment in else %f \n", adjustment);
      	printf("left motor else %f \n", leftMotor);
      	printf("right motor else %f \n", rightMotor); 
      	
      	set_motor(1, rightMotor); //set motor speeds
        set_motor(2, leftMotor);
        
        prev_error = currentError;
      
    } // end of primary loop
    // stop motors
    set_motor(1, 0);
    set_motor(2, 0);
    //printf("%s", "\nProgram fin"); // debugging - program complete msg
return 0;
}
