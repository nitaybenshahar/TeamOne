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
    float kp = 55;
    float ki = 0.0;//CHANGE THRESHOLD
    float kd = -1;
    int i;
    int leftCheck;
    int frontCheck;
    int rightCheck;
    bool left, front, right;

    int whiteTotal, prevWhiteLocation, whiteLocation, whiteLeft, whiteRight;
    int whiteLeft2, whiteLeft3, whiteRight2, whiteRight3;
    double whiteLocation2, whiteTotal2, whiteLocation3, whiteTotal3;
    //int motorOne, motorTwo;
    
    double whiteRatio;
    double prevRatio;
    double derivWhite;
    double integWhite;
    
    
    //Interface to maze
    bool DoNotEnterMaze = true;
    int redTotal;
    
    //Maze
    signed int leftSensor;
    signed int rightSensor;
   
    double leftSensorPrev;
    double rightSensorPrev;
    double error; 
    double previousError;
    double errorD;
    int whiteWall;
    bool noLeftWall, noRightWall, noWallAhead;
    int THRESHOLD = 250;
    int totalWidth;
    signed int leftMotor, rightMotor;
    
    //Primary Initialization
    init(1);
    
    //Networking Section
    
   // Send Signal to open gate
//    connect_to_server("130.195.6.196", 1024);
  //  send_to_server("Please");
    //receive_from_server(message);
   // send_to_server(message);
    
    //Line Following Section
    set_motor(1, 43);
    set_motor(2, -40);
    Sleep (5,0);
    //Loop runs until both sensors sense walls (start of maze)
    while(DoNotEnterMaze){

        //Set variables
        redTotal = 0;
        left = false;
        front = false;
        right = false;
        whiteTotal = 0;
        leftCheck = 0;
        frontCheck = 0;
        rightCheck = 0;
	whiteLeft = 0;
	whiteRight = 0;
        whiteLeft2 = 0;
	whiteLeft3 = 0;
	whiteRight2 = 0;
	whiteRight3 = 0;
	//Take readings
        take_picture();
        
        //Check for interface to the maze
        for(i = 0; i < 240; i++){
        	c = get_pixel(40, i, 0);//Gets redness of the pixel
		if(c > 170){
        		redTotal++;
        	}
    	}
    	if(redTotal>10){ //CHANGE THRESHOLD
            break;//break to maze code
    	}
    	
    	
    	

        for(i = 0; i < 240; i++){
        	c = get_pixel(40, i, 3);
		if(c > 120){
	          	whiteTotal++;
  		        whiteLocation = whiteLocation + (i-120);
        	}
		if((i<120 && c>120)){
			whiteLeft++;

		}
		if((i>=120 && c>120)){
			whiteRight++;
		}
    	}

	for(i = 0; i < 240; i++){
                c = get_pixel(25, i, 3);
                if(c > 120){
                        whiteTotal2++;
                        whiteLocation2 = whiteLocation2 + (i-120);
                }
                if((i<120 && c>120)){
                        whiteLeft2++;

                }
                if((i>=120 && c>120)){
                        whiteRight2++;
                }
        }
	for(i = 0; i < 240; i++){
                c = get_pixel(10, i, 3);
                if(c > 120){
                        whiteTotal3++;
                        whiteLocation3 = whiteLocation3 + (i-120);
                }
                if((i<120 && c>120)){
                        whiteLeft3++;

                }
                if((i>=120 && c>120)){
                        whiteRight3++;
                }
        }

	if(whiteRight2>whiteRight){
	    whiteRight = whiteRight2;
	}
	if(whiteRight3>whiteRight){
	    whiteRight = whiteRight3;
	}
	if(whiteLeft2>whiteLeft){
	    whiteLeft = whiteLeft2;
	}
	if(whiteLeft3>whiteLeft){
	    whiteLeft = whiteLeft3;
	}
	printf("Whiteleft: %d\n\nWhiteright: %d\n\n", whiteLeft, whiteRight);

    	for(i = 10; i < 190; i++){
    		c = get_pixel(i, 10, 3);
    		if(c > 120){
    			leftCheck++;
    		}
    	}

    	for(i = 10; i < 190; i++){
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
	if(leftCheck>5){
	        if(whiteLeft >85){
        		left = true;
        	}
	}
	
        if(frontCheck > 5){
        	front = true;
        }
	if(rightCheck>5){
       		if(whiteRight>85){
        		right = true;
        	}
	}

	if((front && right) || (front && left)){
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
            Sleep(0, 600000);
	    while(true){                           
		take_picture();
                c = get_pixel(40, 120,3);
		printf("Whiteness: %d\n\n", c);
                if(c>120){
                    break;
                }
            }                             //Left Sleep
        }
        /*else if(front && right){
            set_motor(1, 50);
            set_motor(2, -50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                           //Front Sleep
        }*/
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
            while(true){

                take_picture();
                c = get_pixel(40, 120, 3);
                if(c>120){
                    break;
                }
                Sleep(0, 100);
            }
            //Continues turning right until it finds the line
        }
        else{
        //follow the line
        derivWhite = ((double)whiteLocation - (double)prevWhiteLocation)/240;
	integWhite = integWhite + ((double)whiteLocation);
   	whiteLocation = whiteLocation/whiteTotal;

        rightMotor = (int) (45 + (-(whiteLocation*kp/120)-kd*derivWhite));
        leftMotor =  -((int) (45 + ((whiteLocation*kp/120)+kd*derivWhite)));
        set_motor(1, rightMotor);
        set_motor(2, leftMotor);
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
	 
	error = 0;
	
	for(int i = 120; i<128; i++){

	    c = get_pixel(300,i, 3);
 	    if(c>120){                                    //change white threshold
            	whiteWall++;
            }
    	}
    	
	//printf("whiteWall: %d\n", whiteWall);
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
	    set_motor(1, 25);//right motor
	    set_motor(2, -52);//left motor//CHANGE THRESHOLD
	    Sleep(0, 200000);//CHANGE THRESHOLD
	    leftSensorPrev = leftSensor;
	    while(true){
	        leftSensor = read_analog(0);
		printf("Left Sensor: %d\n\n", leftSensor);
	    	if(((leftSensor-leftSensorPrev)*(leftSensor-leftSensorPrev)<50) && (leftSensor > 300)){
			break;
		}
	        leftSensorPrev = leftSensor;
	    }

	    printf("turning right\n");
	}
	else if(noWallAhead){				//stay in the center of the maze
/*	    rightMotor = (leftSensor/10*1.1);      //change threshold
	    leftMotor = -(rightSensor/10);
	    set_motor(1, rightMotor);
	    set_motor(2,leftMotor);
	    
	    Sleep(0,1);
*/	    //rotate back to centre

	    error = (rightSensor-leftSensor)*7/250;
	   

	    errorD =(error - previousError);
	    printf("error: %f\nderivative error: %f\n", error, errorD);

	    leftMotor = -(42-error-errorD);      //change threshold
	    rightMotor = 40+error+errorD;
	    set_motor(1, rightMotor);
	    set_motor(2, leftMotor);
	    printf("forwarsdgsdjksdgr\n");
	    previousError = error;
	    Sleep(0, 1);
	}
	else if(noLeftWall){
	    set_motor(2, -25);//right motor
            set_motor(1, 52);//left motor//CHANGE THRESHOLD
            Sleep(0, 200000);//CHANGE THRESHOLD
            rightSensorPrev = rightSensor;
            while(true){
                rightSensor = read_analog(0);
                printf("Right Sensor: %d\n\n", rightSensor);
                if(((rightSensor-rightSensorPrev)*(rightSensor-rightSensorPrev)<50) && (rightSensor > 300)){
                        break;
                }
                rightSensorPrev = rightSensor;
            }
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
