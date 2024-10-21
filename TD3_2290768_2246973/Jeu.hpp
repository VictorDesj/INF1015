#pragma once
#include <string>
#include "Liste.hpp"

struct Concepteur;

struct Jeu
{
    std::string titre;
    int anneeSortie;
    std::string developpeur;
    Liste<Concepteur> concepteurs; 

    shared_ptr<Concepteur> chercherConcepteur(const function<bool(const shared_ptr<Concepteur>&)>& critere) {
        return concepteurs.chercherElement(critere);
    }
};