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
    bool left, right, forwards;
    int THRESHOLD = 200;
    int totalWidth;
    double leftMotor, rightMotor;


    while(true){
	//initialise
	whiteWall = 0;
        //returns true if there isnt a wall
	left =false;
	right = false;
	forwards = false;

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

	if(whiteWall <4){                                 //Change threshold if theres problems
	    forwards = true;
	}


	if(leftSensor>THRESHOLD){
	     left = true;
	}
	if(rightSensor>THRESHOLD){
	     right = true;
	}

	if(right){
//motor 1 is right motor (i think)
	    set_motor(1, 10);
	    set_motor(2, 100);               //Change thresholds
	    Sleep(2,0);
	    set_motor(1,0);
	    set_motor(2,0);



	}
	else if(forwards){//stay in the center of the maze

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
	    set_motor(1, 100);
	    set_motor(2, 10);               //Change thresholds
	    Sleep(2,0);
	    set_motor(1,0);
	    set_motor(2,0);
	}
	else //pop a u turn
	{
	    set_motor(1, -50);
	    set_motor(2, 60);               //Change thresholds
	    Sleep(2,0);
	    set_motor(1,0);
	    set_motor(2,0);

	}

        Sleep(0,500000);

    }
    return 0;
}
