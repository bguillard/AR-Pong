#pragma once
#include "Jeu.h"
#include "Objet.h"
class Moteur
{
    
private:
    // Le jeu
    Jeu* jeu ;
    
    // Sommes-nous dans le menu ?
    bool dans_menu;
    
    // Dessiner le jeu
    void dessineJeu(cv::Mat frame);
    // Dessiner le menu
    void dessineMenu();
    
public:
    // Le constructeur et destructeur
    Moteur();
    ~Moteur();
    
    // Mode de jeu
    
    void setMode(bool drawPlayers = false);
    
    // Gerer la scene (affichage + mise a jour)
    void Scene(cv::Mat frame);
    
    // Fonction d'initialisation
    bool init();
    
};

