

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

	whiteWall = 0;
        //returns true if there isnt a wall
	noLeftWall = false;
	noRightWall = false;
	noWallAhead = false;

	//get data from sensors
        leftSensor = read_analog(0);
	rightSensor = read_analog(1);
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
	    printf("No wall ahead!!!!!!!!!\n\n\n");
	}
	

	if(leftSensor<THRESHOLD){
	     noLeftWall = true;
	}
	if(rightSensor<THRESHOLD){
	     noRightWall = true;
	}
	
	
	//////////////////////////////////MAZE CONTROL////////////////////////////////
	
	
	
	
	if(noWallAhead){				//stay in the center of the maze
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
	    //printf("forwarsdgsdjksdgr\n");
	    Sleep(0, 1);
	}
	else{//If there is a wall infront of it, turn towards the side that is open
	  if(right){
	    set_motor(1, 37);//right motor
	    set_motor(2, -67);//left motor//CHANGE THRESHOLD
	    Sleep(0,600000);//CHANGE THRESHOLD
	    //printf("turning right\n");
	    right = false;
	  }
	  else if(noLeftWall){
	    set_motor(1, 66);//right motor
	    set_motor(2, -32);//left motor               //Change thresholds
	    Sleep(0,900000);
	    //printf("left left left left\n");
	    left = false;
	  }
	  else //pop a u turn
	  {
	    //printf("pop a u turn\n");
	    set_motor(1, -50);
	    set_motor(2, -60);                            //bigger so the back doesn't hit the wall
	    Sleep(0,100000);               		  //Change thresholds
	  }
	}
    }   
