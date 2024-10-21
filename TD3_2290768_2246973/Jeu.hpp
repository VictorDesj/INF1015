#pragma once
#include <string>
#include "Liste.hpp"

// Forward declaration of Concepteur
struct Concepteur;

struct Jeu
{
    std::string titre;
    int anneeSortie;
    std::string developpeur;
    Liste<Concepteur> concepteurs; // Use the forward-declared type
};