#pragma once
#include <vector>
#include "Objet.h"

class Player : public Objet
{
public:
    Player(std::vector<cv::Point2f>);
    Player();
    ~Player();
    std::vector<cv::Point2f> points;
    std::vector<cv::RotatedRect> shape;
    std::vector<cv::RotatedRect> prevShape;
    
    virtual void affiche(cv::Mat frame);
    virtual int updatePos();
    void clear(cv::Mat frame);
    
    void setShape(std::vector<cv::RotatedRect> s);
    
    virtual void afficheConv(cv::Mat frame);
    virtual int updatePosConv();
    
    static int orientation( cv::Point2f const p, cv::Point2f const q, cv::Point2f const r);
    std::vector<int> convexHull(std::vector<cv::Point2f> points);
    
};

