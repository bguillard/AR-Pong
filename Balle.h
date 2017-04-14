#pragma once
#include "Objet.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>
#include <iostream>
#include <random>

class Balle : public Objet
{
public:
    
    cv::Point2f Centre;
    cv::Point2f lastDrawn;
    cv::Point2f lastPos;
    float R;
    float vit;
    float dirx;
    float diry;
    int r, g, b;
    
    
    Balle();
    ~Balle();
    
    
    virtual void affiche(cv::Mat frame);
    virtual int updatePos();
    void clear(cv::Mat frame);
    
    
};

