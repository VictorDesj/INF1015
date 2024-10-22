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

    Jeu() : titre(""), anneeSortie(0), developpeur(""), concepteurs() {}

    shared_ptr<Concepteur> chercherConcepteur(const function<bool(const shared_ptr<Concepteur>&)>& critere) {
        return concepteurs.chercherElement(critere);
    }

    Jeu operator=(const Jeu& autre) {
        Jeu copie;
 
        copie.titre = autre.titre;
        copie.anneeSortie = autre.anneeSortie;
        copie.developpeur = autre.developpeur;

      
        copie.concepteurs = Liste<Concepteur>();

        for (const shared_ptr<Concepteur>& concepteurPtr : autre.concepteurs.spanListe()) {
            concepteurs.ajouterUnElement(make_shared<Concepteur>(*concepteurPtr)); 
        }
        
        return copie; 
    }
};