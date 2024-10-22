#pragma once
#include <string>
#include "Liste.hpp" 


struct Jeu;

struct Concepteur
{
    std::string nom;
    int anneeNaissance;
    std::string pays;
    Liste<Jeu> jeuxConcus; 
    Concepteur() : nom(""), anneeNaissance(0), pays(""), jeuxConcus() {}
};