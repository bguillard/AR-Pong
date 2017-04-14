#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "Moteur.h"
#include "Balle.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>
#include <iostream>

// Global HEIGHT and WIDTH (in px) of the playscreen are defined in Objet.h

int main()
{
	char keyboard;
    int done=0;
    cv::Mat frame(HEIGHT, WIDTH, CV_8UC3, cv::Scalar(255, 255, 255)); //current frame
	//create GUI windows
    cv::namedWindow("Pong", cv::WINDOW_NORMAL);
   
	Moteur Moteur;
	if (Moteur.init() == false)
		done = 1;
	Moteur.setMode(true); // affichage des joueurs à l'écran;
	while (done == 0) {
        
       
        
		Moteur.Scene(frame);
		imshow("Pong", frame);
        
		
        keyboard = cv::waitKey(1);
        if(keyboard == 'q')
            done = 1;
	}

	cv::destroyAllWindows();
	return EXIT_SUCCESS;

}

