#include "Moteur.h"




Moteur::Moteur()
{
    this->dans_menu = false;
    this->jeu = new Jeu();
    
    
}


Moteur::~Moteur()
{
    //delete &jeu;
}

void Moteur::Scene(cv::Mat frame) {
    
    if (!dans_menu) {
        dessineJeu(frame);
    }
    else {
        dessineMenu();
    }
}

void Moteur::dessineMenu()
{
}

void Moteur::dessineJeu(cv::Mat frame)
{
    jeu->affiche(frame);
    jeu->MAJScene();
}

bool Moteur::init()
{
    return jeu->init();
}

void Moteur::setMode(bool setDrawPlayers) {
    this->jeu->setDrawPlayers(setDrawPlayers);
}
