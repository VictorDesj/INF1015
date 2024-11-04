#pragma once
#include <string>
#include "Liste.hpp"
#include "Concepteur.hpp"

struct Jeu
{
	std::string titre;
	int anneeSortie;
	std::string developpeur;
	Liste<Concepteur> concepteurs;

	friend std::ostream& operator<< (std::ostream& os, const Jeu& jeu);

	shared_ptr<Concepteur> rechercherConcepteur(const function<bool(const shared_ptr<Concepteur>&)> &critere) const
	{
		return concepteurs.rechercherElement(critere);
	}
};
