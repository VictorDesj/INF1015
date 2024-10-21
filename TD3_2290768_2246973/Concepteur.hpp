#pragma once
#include <string>
#include "Liste.hpp" // Include the header for your Liste class

// Forward declaration of Jeu to avoid circular dependency
struct Jeu;

struct Concepteur
{
    std::string nom;
    int anneeNaissance;
    std::string pays;
    Liste<Jeu> jeuxConcus; // Use the forward-declared type
};