#include <stdio.h>
#include <time.h>

extern "C" char get_pixel(int row,int col,int colour);
extern "C" int init(int d_lev);
extern "C" int read_analog(int ch_adc);
extern "C" int take_picture();
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );

int main(){

    init(0);

    int leftSensor;
    int rightSensor;
    char c;
    int whiteWall;
    bool noLeftWall, 
    bool noRightWall, 
    bool noWallAhead;
    int THRESHOLD = 200;
    int totalWidth;
    double leftMotor, rightMotor;


    while(true){
	//initialise
	whiteWall = 0;
        //returns true if there isnt a wall
	noLeftWall =false;
	noRightWall = false;
	noWallAhead = false;

	//get data from sensors
        leftSensor = read_analog(0);
	rightSensor = read_analog(1);
	//get data from camera
	take_picture();

	for(int i = 1; i<5; i++){

	    c = get_pixel(340,i*45, 3);
 	    if(c>150){                                    //change white threshold
            	whiteWall++;
	    }
    	}

	if(whiteWall < 4){                                 //Change threshold if theres problems
	    noWallAhead = true; //rename
	}

	if(leftSensor>THRESHOLD){
	     noLeftWall = true;
	}
	if(rightSensor>THRESHOLD){
	     noRightWall = true;
	}

	if(noRightWall){

	    set_motor(1, 10);//right motor
	    set_motor(2, 100);//left motor               //Change thresholds
	    Sleep(2,0);
	    set_motor(1,0);
	    set_motor(2,0);
	}
	else if(noWallAhead){//stay in the center of the maze

	    totalWidth = leftSensor + rightSensor;
	    rightMotor = (leftSensor/totalWidth)*50;      //vhange threshold
	    leftMotor = (rightSensor/totalWidth)*50;

	    Sleep(0,50000);
	    //rotate back to centre
	    leftMotor = (leftSensor/totalWidth)*50;      //vhange threshold
	    rightMotor = (rightSensor/totalWidth)*50;
	    Sleep(0,50000);

	}
	else if(left){
	    set_motor(1, 100);//right motor
	    set_motor(2, 10);//left motor               //Change thresholds
	    Sleep(2,0);
	    set_motor(1,0);
	    set_motor(2,0);
	}
	else //pop a u turn
	{
	    set_motor(1, -50);
	    set_motor(2, 60);//bigger so the back doesn't hit the wall               //Change thresholds
	    Sleep(2,0);
	    set_motor(1,0);
	    set_motor(2,0);

	}

        Sleep(0,500000);
    }
    return 0;
}
