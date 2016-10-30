#include "opencv2/highgui/highgui.hpp"
#include <unistd.h>
#include <ctime>
#include <string>
#include <iostream>
#include<fstream>

using namespace std;
using namespace cv;

void detectAndDisplay( Mat frame );

/** @function main */
int main( void )
{
    VideoCapture capture;
    Mat frame;

    capture.open( -1 );
    if ( ! capture.isOpened() ){
        cout << "! capture.isOpened() " << endl;
        return -1;
    }
    char filename[]="LastPicture.txt"; 
    fstream fp;

    while(1){
	fp.open(filename, ios::out|ios::trunc);

    	time_t     now = time(0);
    	struct tm  tstruct;
    	char       buf[80];
    	tstruct = *localtime(&now);
    	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    	string recordTime = buf;
    	string filename = buf;
	capture.read(frame);
    	capture.read(frame);
    	imwrite("picture/"+ filename +".png", frame);
    
	cout << "Time: "<<recordTime << endl;
	fp << filename + ".png" << endl;
	fp.close();
	sleep(1);

    }
    return 0;
}

