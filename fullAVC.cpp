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
    char cFuture;
    float kp = 0.83;
    float ki = 0.02;
    float kd = 0.17;
    int i;
    int totalSide;
    int leftCheck;
    int frontCheck;
    int rightCheck;
    int lineCheck;
    
    int whiteTotal, numberOfWhite, futureNumberOfWhite, futureWhiteTotal;
    int rightWhite;
    int counter = 0;
    
    double whiteRatio;
    double prevRatio;
    double derivRatio;
    double finalRatio;
    
    float timeInterval = 0.0005; // update when changing primary sleep duration
    
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
    while((read_analog(0) < THRESHOLD) && (read_analog(1) < THRESHOLD)){
        
        //Set variables
        whiteTotal = 0;
        rightWhite = 0;
        whiteRatio = 0.0;
        leftCheck = 0;
        frontCheck = 0;
        rightCheck = 0;
        lineCheck = 0;
        
        //Take readings
        take_picture();
        
        //Line ahead
        
        //Has a line Check
        for(i = 0; i < 240; i++){
            c = get_pixel(0, i, 3);
            if(c > 150){
                lineCheck++;
            }
        }
        
        //Left intersection
        for(i = 0; i < 100; i++){
            c = get_pixel(i, 0, 3); // check this (i moves)
            if(c > 150){
                leftCheck++;
            }
        }
        
        //Ahead intersection
        for(i = 0; i < 140; i++){
            c = get_pixel(150, i + 50, 3);
            if(c > 150){
                frontCheck++;
            }
        }
        
        //Right intersection
        for(i = 0; i < 100; i++){
            c = get_pixel(i, 239, 3); // check
            if(c > 150){
                rightCheck++;
            }
        }
        
        //No Line Statement
        
        if(lineCheck < 6){
            set_motor(1, -50);
            set_motor(2, 50);
            Sleep(1,0);                                                   //Reverse Sleep
        }
        
        //Left Path Statement
        else if((leftCheck > 5) && (lineCheck > 5)){
            set_motor(1, 100);
            set_motor(2, 0);
            Sleep(1, 0);                                                             //Turn Left Sleep
        }
        
        //Forward Path Statement
        else if((frontCheck > 5) && (rightCheck > 5) && (lineCheck > 5)){
            set_motor(1, 100);
            set_motor(2, -100);
            Sleep(1, 0);                                                            //Forward Step Sleep
        }
        
        //Right Path Statement
        else if((rightCheck > 5) && (lineCheck > 5)){
            set_motor(1, 0);
            set_motor(2, 100);
            Sleep(1, 0);                                                             //Turn Right Sleep
        }
        
        //Path Ends Statement 
        else if((leftCheck < 6) && (frontCheck < 6) && (rightCheck < 6)){
            set_motor(1, 100);
            set_motor(2, 100);
            Sleep(1, 0);                                                             //Turn Around Sleep
        }
        
        //Continue Following Statment
        else{
            for(i = 0; i < 240; i++){
                c = get_pixel(0, i, 3);
                if(c > 150){
                
                    whiteTotal++;
                
                    if(i < 120){
                        rightWhite++;
                    }
                }
            }
            
            if(whiteTotal < 1){
                whiteTotal = 1;
            }
            
            whiteRatio = (double)rightWhite / (double)whiteTotal;
            
            derivRatio = (((double)whiteRatio - (double)prevRatio)/timeInterval);
            
            prevRatio = whiteRatio;
            
            finalRatio = (whiteRatio*kp)+(derivRatio*kd); // k values scale - sum to 1
            
            set_motor(1, ((int)(whiteRatio * 100)));
            set_motor(2, -((int)((1-whiteRatio) * 100)));
            Sleep(0, 5000);
            
        }
    }
    
    //presumably for maze - commented out for now
    //while(true){
        
        
        
    //}
    return 0;
}

