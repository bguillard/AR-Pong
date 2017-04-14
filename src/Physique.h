#pragma once
#include <vector>
#include "Objet.h"
#include "Balle.h"
#include "Player.h"
#include <limits>
class Physique
{
public:
    Physique();
    ~Physique();
    //Fonction qui met ˆ jour les objets
    static void updateObjets(std::vector<Balle> &balles, std::vector<Player> &players);
    
    
    static int collisionBalles(Balle a , Balle b );
    
private:
    //Fonction qui gere la collision avec l'objet i
    
    static bool collisionRect(Balle &b, cv::RotatedRect);
    static cv::Point2f rotate(cv::Point2f c, cv::Point2f p, float angle);
    static float getColTime(Balle a, Balle b );
    static void collide(Balle a, Balle b, float t);
    //Fonction qui teste la collision entre deux cercles
    
    static bool collisionCercle(Balle a, Balle b);
    
    static cv::RotatedRect rect(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3);
};
