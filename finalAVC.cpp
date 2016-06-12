# include <stdio.h> include <time.h>

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

    int prevWhiteLocation = 0;
    int whiteTotal, whiteLocation, whiteLeft, whiteRight;
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
    //set_motor(1, 43);
    //set_motor(2, -40);
    //Sleep (5,0);
    //Loop runs until both sensors sense walls (start of maze)
    while(read_analog(0)<THRESHOLD || read_analog(1)<THRESHOLD){

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
    	if(redTotal>150){ //CHANGE THRESHOLD
            printf("number of red: %d\n", redTotal);//break to maze code
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
            c = get_pixel(5, i, 3);
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
            c = get_pixel(40, i, 3);
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

    	for(i = 0; i < 190; i++){
           c = get_pixel(i, 10, 3);
           if(c > 120){
           	 leftCheck++;
           }
    	}
    	for(i = 0; i < 190; i++){
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
    	if(leftCheck>3){
    	    if(whiteLeft > 95){
       	    	left = true;
    	    }
        }

    	if(frontCheck > 5){
            front = true;
    	}
    	if(rightCheck>3){
    	    if(whiteRight>95){
                if(leftCheck>40){//If there are left whitepixels, turn left instead
            	    left = true;
	    	    printf("left check: %d\n", leftCheck);
            	}
            	else
	    	{
            	    right = true;
                }
    	    }
        }
    	printf("Right white: %d\nLeftWhite: %d\n", whiteRight, whiteLeft);
	printf("LeftCheck: %d\n", leftCheck);
	/*if((front && right) || (front && left)){
            set_motor(1, 50);
            set_motor(2, -50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                           //Front Sleep
        }*/
        if(left){
            set_motor(1, 50);
            set_motor(2, 0);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 200000);
            while(true){
                take_picture();
                c = get_pixel(10, i,3);
                printf("Whiteness: %d\n\n", c);
                    if(c>120){
                    	break;
                    }
		}
            set_motor(1, -25);
	    set_motor(2, 25);
	    Sleep(0, 300000);
 }                             //Left Sleep
        
        else if(front && right){
            set_motor(1, 50);
            set_motor(2, -50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                           //Front Sleep
        }
        else if(right){
            set_motor(1, 0);
            set_motor(2, -60);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 200000); 
	    while(true){
                take_picture();
                c = get_pixel(10, 120,3);
                printf("Whiteness: %d\n\n", c);
                if(c>120){
                    break;
		}
            }
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
         }
		set_motor(1, -30);
		set_motor(2, 30);
		Sleep(0, 200000);
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
        printf("left sensor: %d\nright sensor: %d\n", leftSensor, rightSensor);
        //get data from camera
        take_picture();

        error = 0;

        for(int i = 120; i<128; i++){
            c = get_pixel(200,i, 3);
            if(c>190){                                    //change white threshold
            	whiteWall++;
            }
    	}

        printf("whiteWall: %d\n", whiteWall);
        if(whiteWall < 5){                                 //Change threshold if theres problems
            noWallAhead = true; //rename
        //  printf("No wall ahead!!!!!!!!!\n\n\n");
        }


        if(leftSensor<400){
            noLeftWall = true;
        }
        if(rightSensor<400){
            noRightWall = true;
        }
        if(noRightWall){
            int counter = 0;

            set_motor(1, 26);//right motor
            set_motor(2, -54);//left motor//CHANGE THRESHOLD
            Sleep(0, 200000);//CHANGE THRESHOLD
	    leftSensorPrev = leftSensor;
            while(true){
                leftSensor = read_analog(0);
                printf("Left Sensor: %d\n\nChange in error: %d\n", leftSensor, (leftSensor-leftSensorPrev)*(leftSensor-leftSensorPrev));
                if((((leftSensor-leftSensorPrev)*(leftSensor-leftSensorPrev)<50) && leftSensor>350) || (counter>100)){
                break;
                }
	        leftSensorPrev = leftSensor;
	        counter++;
		printf("Counter: %d\n\n", counter);
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
            int counter = 0;
            set_motor(2, -24);//right motor
            set_motor(1, 59);//left motor//CHANGE THRESHOLD
            Sleep(0, 700000);//CHANGE THRESHOLD
            rightSensorPrev = rightSensor;
            //while(true){
              //  rightSensor = read_analog(0);
               // printf("Right Sensor: %d\n\nChange in error squared: %d\n\n", rightSensor, (rightSensor-leftSensor)*(rightSensor-leftSensor));
                //if((((rightSensor-rightSensorPrev)*(rightSensor-rightSensorPrev)<50) && (rightSensor > 350)) || (counter>100)){//CHANGE THRESHOLDS
                  //  break;
                //}
                //rightSensorPrev = rightSensor;
                //counter++;
	//	printf("Counter: %d\n\n", counter);
          //  }
            printf("left left left left\n");
        }
	    else //pop a u turn
	    {
            printf("pop a u turn\n");
            set_motor(1, -40);
            set_motor(2, -62);                            //bigger so the back doesn't hit the wall
            Sleep(0,900000);               		  //Change thresholds
	    set_motor(1, -50);
	    set_motor(2, 0);
	    Sleep(0, 800000);
	    set_motor(1, -50);
	    set_motor(2, -59);
	   Sleep(0, 900000);
	    set_motor(1, -50);
	    set_motor(2, 0);
	    Sleep(0, 800000);
        }
    }

return 0;
}
