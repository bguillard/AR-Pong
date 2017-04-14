//
//  Tracker.cpp
//  Projet
//
//  Created by Benoît Guillard on 18/03/2017.
//
//

#include "Tracker.hpp"

#include <chrono>

    
Tracker::Tracker()
    {
        // Capture from the webcam
        capWebcam.open(0);
		if (!capWebcam.isOpened())
        {
            std::cout << "webcam non trouvee" << std::endl;
        }
        else
        {
            std::cout << "flux webcam ouvert" << std::endl;
        }
        
        //Reduce image size, to improve calculation speed
        std::cout << capWebcam.set(cv::CAP_PROP_FRAME_WIDTH, 640) << std::endl;
        std::cout << capWebcam.set(cv::CAP_PROP_FRAME_HEIGHT, 480) << std::endl;
        
        capWebcam >> trackerFrame;
	
        
        frameWidth = capWebcam.get(cv::CAP_PROP_FRAME_WIDTH);
        frameHeight = capWebcam.get(cv::CAP_PROP_FRAME_HEIGHT);
        frameType = trackerFrame.type();
        
        // The kernels for morphology transformations :
        kernel1 = getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
        
        // MOG2 Background subtractor (history, size, shadow)
        pMOG2 = cv::createBackgroundSubtractorMOG2(200,124,false);
    }
    
Tracker::~Tracker()
    {
        // Delete capture object
        capWebcam.release();
    }


    
std::vector<cv::RotatedRect> Tracker::getRectangles(Balle b)
    {
        std::vector<cv::RotatedRect> ans;

        capWebcam >> trackerFrame;
        
        // Pour la demo :
        //imshow("trackerFrame1", trackerFrame);
        
        /* For debugging : mesure du temps écolé dans une fonction
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        fonctionATester();
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::cout << duration << std::endl;
        */
        
        // On applique l'homographie pour n'analyser que l'image de l'écran de jeu
        warpPerspective(trackerFrame, trackerFrame, H, trackerFrame.size() , 1, cv::BORDER_TRANSPARENT );
        
        // Pour la demo :
        //imshow("trackerFrame2", trackerFrame);
        
        // Application du tracker
        pMOG2->apply(trackerFrame, fgMaskMOG2);
        
        // Pour la demo :
        //imshow("fgMaskMOG21", fgMaskMOG2);
      
        
        // Masquage brutal, à la main, de l'image de la balle,
        // à partir de la position actuelle de la balle et de sa vitesse (on efface sa traîne également)
    
        /*
        cv::circle(fgMaskMOG2, b.Centre, 13, cv::Scalar(0), -1);
        cv::circle(fgMaskMOG2, cv::Point(b.Centre.x - b.vit*b.dirx , b.Centre.y - b.vit*b.diry), 24, cv::Scalar(0), -1);
        cv::circle(fgMaskMOG2, cv::Point(b.Centre.x - 2*b.vit*b.dirx , b.Centre.y - 2*b.vit*b.diry), 24, cv::Scalar(0), -1);
        cv::circle(fgMaskMOG2, cv::Point(b.Centre.x - 3*b.vit*b.dirx , b.Centre.y - 3*b.vit*b.diry), 24, cv::Scalar(0), -1);
        cv::circle(fgMaskMOG2, cv::Point(b.Centre.x - 4*b.vit*b.dirx , b.Centre.y - 4*b.vit*b.diry), 24, cv::Scalar(0), -1);
        cv::circle(fgMaskMOG2, cv::Point(b.Centre.x - 5*b.vit*b.dirx , b.Centre.y - 5*b.vit*b.diry), 24, cv::Scalar(0), -1);
        */
        /*
        // Autre mask, basé sur la couleur
        cvtColor(trackerFrame, trackerFrame, cv::COLOR_BGR2HSV);
        // On trouve le rouge et on le supprime : suppression des balles
        cv::Mat1b mask1, mask2;
        cv::inRange(trackerFrame, cv::Scalar(0, 70, 50), cv::Scalar(20, 255, 255), mask1);
        cv::inRange(trackerFrame, cv::Scalar(160, 70, 50), cv::Scalar(180, 255, 255), mask2);
        cv::Mat1b mask = mask1 | mask2;
        // On dilate la balle rouge detectée
        cv::Mat element = cv::getStructuringElement( cv::MORPH_ELLIPSE,cv::Size( 6, 6 ));
        cv::dilate( mask, mask, element );
        // On inverse le mask et on le superpose à fgMaskMOG2
        cv::bitwise_not(mask, mask);
        fgMaskMOG2 = fgMaskMOG2 & mask;
        // Affichage du mask
        //imshow("Mask", mask);
        */
        
        
        // Remove noise and fill holes in the mask
        morphologyEx(fgMaskMOG2, fgMaskMOG2, cv::MORPH_OPEN, kernel1);
        morphologyEx(fgMaskMOG2, fgMaskMOG2, cv::MORPH_CLOSE, kernel1);
        
        // Soften the mask
        GaussianBlur( fgMaskMOG2, fgMaskMOG2, cv::Size( 5, 5 ), 0, 0 );
        
        // Show the final mask, on which the contours are computed
        //cv::imshow("mask",fgMaskMOG2);
        
        // Find contours
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i > hierarchy;
        findContours( fgMaskMOG2, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1 );
        
        std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
        std::vector<cv::Rect> boundRect( contours.size() );
        
        
        // Draw Contours
        
        // Pour la demo :
        /// Draw contours
        /*
        cv::Mat drawing = cv::Mat::zeros( cv::Size(640,480), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
            cv::Scalar color = cv::Scalar( 0,55,0 );
            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
        }
        imshow( "Contours", drawing );
        */
        
        // Bounding BOX method (not right up rectangles : sweeeet :) )
        for( size_t i = 0; i < contours.size(); i++ )
        {
            size_t count = contours[i].size();
            if( count < 6 )
                continue;
            
            cv::Mat pointsf;
            cv::Mat(contours[i]).convertTo(pointsf, CV_32F);
            
            cv::RotatedRect box = fitEllipse(pointsf);
            
            if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 || (box.size.width <35 && box.size.height<35 && fabs(b.Centre.x - box.center.x) < 20 && fabs(b.Centre.y - box.center.y) < 20))
                continue;
            
            ans.push_back(box);
        }
        
        // Pour la demo :
        // Show players to debug
        /*
        cv::Mat img(480, 640, CV_8UC3, cv::Scalar(255, 255, 255)); //current frame
        for (size_t i = 0; i < ans.size(); i++)
        {
            cv::Point2f vtx[4];
            ans[i].points(vtx);
            for (int j = 0; j < 4; j++)
                line(img, vtx[j], vtx[(j + 1) % 4], cvScalar(0, 255, 0), 1, cv::LINE_AA);
        }
        imshow("ImgTracker", img);
        */
        
        return ans;
    }

bool Tracker::initHomography()
{
    // Show target
    //cv::Mat target = cv::imread("/Users/benoitguillard/Documents/X/INF473I/Projet5homography/tetris.png", CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat target = cv::imread("../../tetris.png", CV_LOAD_IMAGE_GRAYSCALE);
    cv::resize(target, target, cv::Size(WIDTH,HEIGHT));
    imshow("Pong", target);
    
    char keyboard= -1;
    cv::Mat acquired;
    
    while(keyboard==-1)
    {
        capWebcam >> acquired;
        imshow("Acquired", acquired);
        keyboard = cv::waitKey(1);
    }
    
    cv::destroyWindow("Acquired");
    
    cvtColor(acquired, acquired, CV_BGR2GRAY);
    
    // Methode AKAZE :
    //Ptr<AKAZE> D = AKAZE::create();
    // Methode ORB :
    cv::Ptr<cv::ORB> D = cv::ORB::create();
    std::vector<cv::KeyPoint> m1, m2;
    cv::Mat desc1, desc2;
    
    D->detectAndCompute(target, cv::noArray(), m1, desc1);
    D->detectAndCompute(acquired, cv::noArray(), m2, desc2);
    
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    
    std::vector<cv::DMatch>  matches;
    matcher.match(desc1, desc2, matches);
    
    std::vector<cv::Point2f> src, dst;
    
    for(auto m :matches)
    {
        src.push_back(m1[m.queryIdx].pt);
        dst.push_back(m2[m.trainIdx].pt);
    }
    
    cv::Mat mask;
    H = findHomography(dst, src, CV_RANSAC, 5.0, mask);
    
    // For demo:
    //warpPerspective(acquired, acquired, H, acquired.size() , 1, cv::BORDER_TRANSPARENT );
    //imshow("Transformed",acquired);

    return true;
}
