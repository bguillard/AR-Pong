#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>
#include <iostream>

const float WIDTH = 640;
const float HEIGHT = 480;

class Objet
{
public:
    ~Objet();
    Objet();
    
    
    virtual void affiche(cv::Mat frame);
    virtual int updatePos();
    
    
};

