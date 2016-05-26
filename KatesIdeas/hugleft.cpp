
//Here is my ideas for a method that hugs the left most white pixel in the line
//I'm doing this as we haven't done intersections yet and this is my approach to simplify it
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
  	counter ++;
  	printf("%s\n", "50 loops completed"); // adjust to counter+" loops completed" when you can check C syntax
	break;
    }
      
    whiteTotal = 0;//sum of the position of the white pixels, measured from center = 0
    numberOfWhite = 0;//running total of the number of white pixels
        
    take_picture();
        
    	for(int i=0; i<=240; i++){
          
	   while(numberOfWhite < 1){
	          
		//get pixel "whiteness"
	        //resolution of image is 240x360
	        c = get_pixel(320,i,3); //it's turning earlier than it should, indicating that it is seeing the turns earlier 
	            
	        if(c<200){ // 320/2 ie. if (c < half)
	               	c = 0;  //Black pixel
	               	whiteTotal = 0;
	        }else{
	               	c = 1;   //white pixel
	              		whiteTotal = i;
	        }
	    }
    	}
       	if (numberOfWhite >= 1) { // no dividing by 0
		printf("%f\n", whiteTotal); //Print error signal for Debugging purposes
		      
		if(whiteTotal > 240){
			printf("The image is portrait.");//this is here in case my logic regarding the reading of the camera is wrong.
		}
		      
      		Sleep(0,500000);
        }
        
        adjustment = 120 - whiteTotal;//middle of the picture (i.e current location) minus the location of the line and therefor the change that needs to happen
        //if adjustment is negative then the line is to the right and if it is positive then the line is to the left. 
         
        if(whiteTotal>0){
        	rightMotor = 70.0+adjustment;
        	leftMotor = -70.0+adjustment;
        }else{ //if there's no white it will turn right
        	rightMotor = 70.0;
        	leftMotor = -90.0;
        }
      	
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
