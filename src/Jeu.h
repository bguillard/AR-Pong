#pragma once
#include <vector>
#include "Objet.h"
#include "Balle.h"
#include "Player.h"
#include "Physique.h"
#include "Tracker.hpp"

class Jeu
{
    
    
private:
    
    
    //Initialisation des objets et des textures
    bool initObjets();
    bool initTextures();
    
    
public:
    std::vector<Balle> balles;
    std::vector<Player> players;
    // sélectionne le mode : true si on dessine les joueurs, false sinon
    void setDrawPlayers(bool b);
    //Constructeur et Destructeur
    Jeu();
    ~Jeu();
    //Initialisation de la classe
    bool init();
    int getNBalles();
    int getNPlayers();
    //Ajouter un objet dans le jeu, retourne l'indice de l'objet ajoute
    //Les objets
    
    Physique physique;
    Tracker tracker;
    
    
    //Pour la creation des balles
    const int MAX_BALLES=1;
    bool drawPlayers;
    int addBalle(Balle b);
    
    int addPlayer(Player p);
    
    //Afficher le jeu
    void affiche(cv::Mat frame);
    void affObjets(cv::Mat frame);
    
    //Mettre a jour les objets
    void updateObjets();
    
    //Gere la scene (mise a jour)
    void MAJScene();
    
};
