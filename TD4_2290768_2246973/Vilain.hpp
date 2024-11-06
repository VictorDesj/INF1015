#pragma once
#include "Personnage.hpp"


class Vilain :virtual public Personnage {
public:
	Vilain(const string& nom, const string& titreJeu, const string objectif) :Personnage(nom, titreJeu), objectif_(objectif) {}
	ostream& afficher(ostream& out) const override{
		Personnage::afficher(out);
		cout << "Objectif : " << objectif_ << endl;
		return out;
	}
	string getObjectif() const {
		return objectif_;
	}
protected:
	

private:
	string objectif_;
};