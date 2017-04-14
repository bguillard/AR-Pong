
#include "Balle.h"




Balle::Balle()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.2, 0.8);
    this->Centre = cv::Point2f(WIDTH/2, HEIGHT/2);
    this->lastDrawn = cv::Point2f(Centre.x,Centre.y);
    this->lastPos = cv::Point2f(Centre.x, Centre.y);
    this->R = 10.f;
    this->r = 255;
    this->g = 0;
    this->b = 0;
    this->dirx = (float) dis(gen);
    this->diry =(float) sqrt( 1.0f-dirx*dirx);
    this->vit = 5;
    
}

Balle::~Balle() {  }
void Balle::affiche(cv::Mat frame) {
    clear(frame);
    cv::circle(frame, Centre, (int) R, cv::Scalar(b, g, r),-4,5);
    lastDrawn = Centre;
}

int Balle::updatePos() {
    lastPos = Centre;
    float x = Centre.x + vit*dirx;
    float y = Centre.y + vit*diry;
    
    // test aux bords
    
    
    //En haut
    if (y-R<0)
    {
        y =  R;
        if (diry < 0)
            diry = diry * (-1);
    }
    
    //En bas
    if (y + R >=  HEIGHT)
    {
        y =  HEIGHT - R;
        if (diry>0)
            diry= diry * (-1);
    }
    // A gauche
    if (x <  R)
    {
        x = R;
        if (dirx < 0)
            dirx = (-1)*dirx;
        
    }
    
    //A droite
    if (x +  R >=  WIDTH)
    {
        x = WIDTH - R;
        if (dirx > 0)
            dirx = dirx * (-1);
    }
    
    Centre.x = x;
    Centre.y = y;
    return 0;
}

void Balle::clear(cv::Mat frame)
{
    if(lastDrawn.x!=Centre.x || lastDrawn.y != Centre.y)
        cv::circle(frame, lastDrawn, (int)R, cv::Scalar(255, 255, 255), -4, 5);
}
