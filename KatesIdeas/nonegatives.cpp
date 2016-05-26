
//Here is my ideas for a method that doesn't use negatives, i.e. treats the middle as it is
//I'm doing this as we aren't sure why Bambi isn't working and are wondering if the negative might have a part in that
//There is no kp or kd to keep it simple

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
    double currentError = 0;
    double adjustment = 0;
    
    double leftMotor = 0;
    double rightMotor = 0;
    
    int whiteTotal;
    int numberOfWhite;
    int counter = 0;
    
   while(true){
      if (counter >= 50) { // main exit loop command
  	   	printf("%s\n", "50 loops completed"); // adjust to counter+" loops completed" when you can check C syntax
    		break;
      	}
      	
      	counter ++;
      	
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
            
           whiteTotal = whiteTotal + i*c; //add the position of the white pixels (if its white)
        }
       if (numberOfWhite >= 1) { // no dividing by 0
        	currentError = whiteTotal/numberOfWhite; //center of the white line, running from 0 through to 240
	      	printf("%f\n", currentError); //Print error signal for Debugging purposes
		      
		      if(currentError > 240){
		        printf("The image is portrait.");//this is here in case my logic regarding the reading of the camera is wrong.
		      }
		      
      		Sleep(0,500000);
        }
        
        adjustment = 120 - currentError; //middle of the picture (i.e current location) minus the location of the line and therefor the change that needs to happen
        //if adjustment is negative then the line is to the right and if it is positive then the line is to the left. 
         
        rightMotor = 70.0+adjustment;
      	leftMotor = -70.0+adjustment;
      	
      	printf("adjustment in else %f \n", adjustment);
      	printf("left motor else %f \n", leftMotor);
      	printf("right motor else %f \n", rightMotor); 
      	
      	set_motor(1, rightMotor); //set motor speeds
        set_motor(2, leftMotor);
     }
  set_motor(1, 0);
  set_motor(2, 0);
  //printf("%s", "\nProgram fin"); // debugging - program complete msg
  return 0;
}
