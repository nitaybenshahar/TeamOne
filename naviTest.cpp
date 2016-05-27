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
    float kp = 0.8;
    float ki = 0.02;
    float kd = 0;
    int i;
    int totalSide;
    int leftCheck;
    int frontCheck;
    int rightCheck;
    int lineCheck;
    
    int whiteTotal, numberOfWhite, futureNumberOfWhite, futureWhiteTotal;
    int rightWhite;
    int counter = 0;
    double prevWhiteLocation;
    double whiteLocation;
    double whiteRatio;
    double prevRatio;
    double derivRatio;
    double finalRatio;
    double derivWhite = 0;

    float timeInterval = 0.0005; // update when changing primary sleep duration
    
    //Maze
    int leftSensor, rightSensor;
    int whiteWall;
    bool noLeftWall, noRightWall, noWallAhead;
    int THRESHOLD = 200;                                                               //Sensor Threshold
    int totalWidth;
    
    //Primary Initialization
    init(1);
    
    
        while(true){
        
	whiteLocation = 0;
        whiteTotal = 0;
        rightWhite = 0;
        whiteRatio = 0.0;
        leftCheck = 0;
        frontCheck = 0;
        rightCheck = 0;
        lineCheck = 0;
        
        take_picture();
            
            for(i = 0; i < 240; i++){
                c = get_pixel(40, i, 3);
                if(c > 120){
                
                    whiteTotal++;
		    whiteLocation = whiteLocation + (i-120);
                
                    if(i < 120){
                        rightWhite++;
                    }
                }
            }
	printf("White Total: %d\n\n",whiteTotal);
           
 if(whiteTotal < 1){
                set_motor(1, -60);
                set_motor(2, 60);
           printf("reverse beep beep!! \n\n\n");
           Sleep(1,0);
}
            else{
		
                whiteRatio = (double)rightWhite / (double)whiteTotal;
             
                derivRatio = (((double)whiteRatio - (double)prevRatio)/timeInterval);
                
                prevRatio = whiteRatio;
                
                finalRatio = (whiteRatio*kp)+(derivRatio*kd); // k values scale - sum to 1
	       

		derivWhite = ((double)whiteLocation - (double)prevWhiteLocation)/0.01;
		whiteLocation = whiteLocation/whiteTotal;
		
		int motorOne =  ((-(whiteLocation*40/120)*kp+kd*derivWhite)+40)));
		

		 else {
		 
		printf("Motor One: %f", motorOne);
		    set_motor(1, motorOne);
		    set_motor(2, -motorOne);
       }
   
         }
            Sleep(0, 1000);
                    prevWhiteLocation = whiteLocation;
        }
    
    
  }
