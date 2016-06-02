# include <stdio.h>
# include <time.h>

//FUNCTION INITIALIZATION

//Networking
extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15], int port);
extern "C" int send_to_server( char message[24]);
extern "C" int receive_from_server( char message[24]);

//Line Follow
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);

//Maze
extern "C" int read_analog(int ch_adc);

int main(){

    //VARIABLE INITIALIZATION
    
    //Networking
    char message[24];
    
    //Line Following
    char c;
    float kp = 0.80;
    float ki = 0.0;
    float kd = 0.0;
    int i;
    int leftCheck;
    int frontCheck;
    int rightCheck;
    bool left, front, right;
    
    int whiteTotal, prevWhiteLocation, whiteLocation;
    //int motorOne, motorTwo;
    
    double whiteRatio;
    double prevRatio;
    double derivWhite;
    double integWhite;
    
    //Maze
    signed int leftSensor;
    signed int rightSensor;

    int whiteWall;
    bool noLeftWall, noRightWall, noWallAhead;
    int THRESHOLD = 250;
    int totalWidth;
    signed int leftMotor, rightMotor;
    
    //Primary Initialization
    init(1);
    
    //Networking Section
    
   // Send Signal to open gate
    connect_to_server("130.195.6.196", 1024);
    send_to_server("Please");
    receive_from_server(message);
    send_to_server(message);
    
    //Line Following Section
    set_motor(1, 43);
    set_motor(2, -40);
    Sleep (5,0);
    //Loop runs until both sensors sense walls (start of maze)
    while((read_analog(0) < THRESHOLD) || (read_analog(1) < THRESHOLD)){
        
        //Set variables
        left = false;
        front = false;
        right = false;
        whiteTotal = 0;
        leftCheck = 0;
        frontCheck = 0;
        rightCheck = 0;
        
        //Take readings
        take_picture();
        
        for(i = 0; i < 240; i++){
        	c = get_pixel(40, i, 3);
		if(c > 120){
        		whiteTotal++;
			whiteLocation = whiteLocation + (i-120);
        	}
    	}
    		
    	for(i = 60; i < 70; i++){
    		c = get_pixel(i, 10, 3);
    		if(c > 120){
    			leftCheck++;
    		}
    	}
    		
    	for(i = 60; i < 70; i++){
    		c = get_pixel(i, 230, 3);
    		if(c > 120){
    			rightCheck++;
    		}
    	}
        
        for(i = 30; i < 210; i++){
        	c = get_pixel(160, i, 3);
        	if(c > 120){
                	frontCheck++;
            	}
        }
        
        if(leftCheck > 3){
        	left = true;
        }
        if(frontCheck > 10){
        	front = true;
        }
        if(rightCheck > 5){
        	right = true;
        }
        
        else if((front && right) || (front && left)){
            set_motor(1, 50);
            set_motor(2, -50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                           //Front Sleep
        }
        else if(left){
            set_motor(1, 50);
            set_motor(2, 0);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                            //Left Sleep
        }
        else if(right){
            set_motor(1, 0);
            set_motor(2, -50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                           //Right Sleep
        }
        else if(whiteTotal < 1){
            set_motor(1, -50);
            set_motor(2, -50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 300000);                           //Turn around Sleep
        }
        else{
        	derivWhite = ((double)whiteLocation - (double)prevWhiteLocation)/0.01;
		integWhite = integWhite + ((double)whiteLocation * 0.01);
		whiteLocation = whiteLocation/whiteTotal;
		
		 set_motor(1, ((int) ((-(whiteLocation*40/120)*kp+kd*derivWhite)+40)));
		 set_motor(2, -((int) (((whiteLocation*40/120)*kp+kd*derivWhite)+40)));
		
		// motorOne = (-( ( (whiteLocation*1/3)*kp) + (derivWhite * kd) + (integWhite * ki) + 40))
		// motorTwo = (((whiteLocation*1/3)*kp)+(derivWhite * kd) + (integWhite * ki) + 40)
		// set_motor(1, motorOne);
		// set_motor(2, -motorTwo);
		
		//set_motor(1, ((int)(-( ( (whiteLocation*1/3)*kp) + (derivWhite * kd) + (integWhite * ki) + 40))));
		//set_motor(2, -((int) (((whiteLocation*1/3)*kp)+(derivWhite * kd) + (integWhite * ki) + 40)));
			      
		prevWhiteLocation = whiteLocation;
        	Sleep(0,1000);
        }
    }
    
    while(true){
		 whiteWall = 0;
        //returns true if there isnt a wall
	noLeftWall =false;
	noRightWall = false;
	noWallAhead = false;

	//get data from sensors
        leftSensor = read_analog(0);
	rightSensor = read_analog(1);
	//printf("left sensor: %d\nright sensor: %d\n", leftSensor, rightSensor);
	//get data from camera
	take_picture();

	for(int i = 120; i<128; i++){

	    c = get_pixel(300,i, 3);
 	    if(c>120){                                    //change white threshold
            	whiteWall++;
            }
    	}
    	
	printf("whiteWall: %d\n", whiteWall);
	if(whiteWall < 5){                                 //Change threshold if theres problems
	    noWallAhead = true; //rename
	  //  printf("No wall ahead!!!!!!!!!\n\n\n");
	}
	

	if(leftSensor<THRESHOLD){
	     noLeftWall = true;
	}
	if(rightSensor<THRESHOLD){
	     noRightWall = true;
	}
	if(noRightWall){
	   set_motor(1, 32);
	    set_motor(2, -30);
	    Sleep(0, 300000);

	    set_motor(1, 37);//right motor
	    set_motor(2, -67);//left motor//CHANGE THRESHOLD
	    Sleep(0,900000);//CHANGE THRESHOLD
	    printf("turning right\n");
	}
	else if(noWallAhead){				//stay in the center of the maze
	    rightMotor = (leftSensor/10*1.1);      //change threshold
	    leftMotor = -(rightSensor/10);
	    set_motor(1, rightMotor);
	    set_motor(2,leftMotor);
	    
	    Sleep(0,1);
	    //rotate back to centre
	    leftMotor = -(leftSensor/10);      //change threshold
	    rightMotor = (rightSensor/10*1.1);
	    set_motor(1, rightMotor);
	    set_motor(2, leftMotor);
	    printf("forwarsdgsdjksdgr\n");
	    Sleep(0, 1);
	}
	else if(noLeftWall){
	    set_motor(1, 40);
	    set_motor(2,-42);

	    Sleep(0,450000);
	    set_motor(1, 66);//right motor
	    set_motor(2, -32);//left motor               //Change thresholds
	    Sleep(0,900000);
	   printf("left left left left\n");
	}
//	else //pop a u turn
/*	{
	   printf("pop a u turn\n");
	    set_motor(1, -50);
	    set_motor(2, -60);                            //bigger so the back doesn't hit the wall
	    Sleep(0,100000);               		  //Change thresholds
	}*/

        
    }   
    
return 0;    
}
