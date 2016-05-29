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
    float ki = 0.02;
    float kd = 0.04;
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
    int leftSensor, rightSensor;
    int whiteWall;
    bool noLeftWall, noRightWall, noWallAhead;
    int THRESHOLD = 250;                                                               //Sensor Threshold
    int totalWidth;
    
    //Primary Initialization
    init(1);
    
    //Networking Section
    
    //Send Signal to open gate
    connect_to_server("130.195.6.196", 1024);
    send_to_server("please");
    receive_from_server(message);
    send_to_server(message);
    
    //Line Following Section
    
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
    		c = get_pixel(i, 60, 3);
    		if(c > 120){
    			leftCheck++;
    		}
    	}
    		
    	for(i = 60; i < 70; i++){
    		c = get_pixel(i, 180, 3);
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
        
        if(leftCheck > 5){
        	left = true;
        }
        if(frontCheck > 10){
        	front = true;
        }
        if(rightCheck > 5){
        	right = true;
        }
        
        if(left){
            set_motor(1, 50);
            set_motor(2, 0);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                            //Left Sleep
        }
        else if(front && right){
            set_motor(1, 50);
            set_motor(2, -50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 500000);                           //Front Sleep
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
            set_motor(2, 50);
            derivWhite = 0.0;
            integWhite = 0.0;
            Sleep(0, 100000);                           //Turn around Sleep
        }
        else{
        	derivWhite = ((double)whiteLocation - (double)prevWhiteLocation)/0.01;
		integWhite = integWhite + ((double)whiteLocation * 0.01);
		whiteLocation = whiteLocation/whiteTotal;
		
		// set_motor(1, ((int) ((-(whiteLocation*40/120)*kp+kd*derivWhite)+40)));
		// set_motor(2, -((int) (((whiteLocation*40/120)*kp+kd*derivWhite)+40)));
		
		// motorOne = (-( ( (whiteLocation*1/3)*kp) + (derivWhite * kd) + (integWhite * ki) + 40))
		// motorTwo = (((whiteLocation*1/3)*kp)+(derivWhite * kd) + (integWhite * ki) + 40)
		// set_motor(1, motorOne);
		// set_motor(2, -motorTwo);
		
		set_motor(1, ((int)(-( ( (whiteLocation*1/3)*kp) + (derivWhite * kd) + (integWhite * ki) + 40))));
		set_motor(2, -((int) (((whiteLocation*1/3)*kp)+(derivWhite * kd) + (integWhite * ki) + 40)));
			      
		prevWhiteLocation = whiteLocation;
        	Sleep(0,1000);
        }
    }
    
    while(true){ // [insert maze method, if we want to test one]
        
    }
        
        
    
    return 0;
}
