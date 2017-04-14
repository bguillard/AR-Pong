#include "Jeu.h"
Jeu::Jeu()
{
    
}
Jeu::~Jeu() {
}
void Jeu::setDrawPlayers(bool b) {
    this->drawPlayers = b;
}

bool Jeu::init()
{
    return initObjets() && initTextures() && tracker.initHomography();
}


bool Jeu::initTextures() { return true; }

bool Jeu::initObjets()
{
    balles.clear();
    
    players.clear();
    
    return true;
}

int Jeu::addBalle(Balle o)
{
    
    // On copie l'objet dans la case vide
    balles.push_back(o);
    //On rend l'indice de l'objet insere
    return (int) balles.size()-1;
    
}
int Jeu::getNBalles()
{
    return (int) balles.size();
}

int Jeu::addPlayer(Player p) {
    players.push_back(p);
    return getNPlayers() - 1;
}

int Jeu::getNPlayers() {
    return (int) players.size();
}
void Jeu::affiche(cv::Mat frame)
{
    //Afficher les objets
    affObjets(frame);
}

void Jeu::affObjets(cv::Mat frame)
{
    
    int i;
    //On affiche les balles
    for (i = 0; i<(int)getNBalles(); i++)
    {
        
        balles[i].affiche(frame);
        
    }
    // On affiche les joueurs si on est dans ce mode
    if (this->drawPlayers) {
        for (i = 0; i < (int)getNPlayers(); i++)
        {
            players[i].affiche(frame);
        }
    }
    
}
void Jeu::updateObjets()
{
    this->physique.updateObjets(balles, players);
    
    for (int j = 0; j < (int) getNPlayers(); j++)
    {
        players[j].updatePos();
        
    }
}

void Jeu::MAJScene()
{
    // mise à jour des coordonnées du joueur par le tracker
    if ((int) getNPlayers() == 0) {
        Player p;
        addPlayer(p);
    }
    
     // mise à jour des coordonnées du joueur par le tracker

     if(balles.size() >0)
     {
     std::vector<cv::RotatedRect> shape = tracker.getRectangles(balles[0]);
     players[0].setShape(shape);
     }
     else
     {
     std::vector<cv::RotatedRect> shape = tracker.getRectangles(*new Balle);
     players[0].setShape(shape);
     }
    
    /*
    std::vector<cv::RotatedRect> v;
    cv::RotatedRect a(cv::Point2f(150, 150), cv::Size2f(50,50), 12);
    v.push_back(a);
    players[0].setShape(v);
     */
    
    /*
    cv::Mat img(480, 640, CV_8UC3, cv::Scalar(255, 255, 255));
    for (size_t i = 0; i < players[0].shape.size(); i++)
    {
        cv::Point2f vtx[4];
        players[0].shape[i].points(vtx);
        for (int j = 0; j < 4; j++)
            line(img, vtx[j], vtx[(j + 1) % 4], cvScalar(0, 255, 0), 1, cv::LINE_AA);
    }
    if(getNBalles() > 0)
        cv::circle(img, balles[0].Centre, (int) 10.f, cv::Scalar(255, 0, 255),4,5);
    
    cv::imshow("img",img);
    */
    
    // On met à jour les objets (notamment collisions)
    updateObjets();
    
    // si il n'y a pas de balle on en crée une.
    // idem avec le joueur
    
    if ((int) getNBalles() < MAX_BALLES) {
        Balle balle;
        
        int i = addBalle(balle);
        for (int j = 0; j < getNBalles(); j++) {
            if (i != j && physique.collisionBalles(balles[j], balles[i]) != -1)
                balles.pop_back();
        }
    }
    
    
}
