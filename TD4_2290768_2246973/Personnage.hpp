#pragma once
#include <string>
#include "Affichable.hpp"

using namespace std;

class Personnage : public Affichable {
public:
	Personnage(const string& nom, const string titreJeu) :nom_(nom), titreJeu_(titreJeu) {};

	ostream& afficher(ostream& out) const override {
		out << "Nom : " << nom_ << endl << "Parution : " << titreJeu_ << endl;
		return out;
	}

	void changerCouleur(int couleur) override {
		cout << "\033[" << couleur << "m";
	}

	virtual string getNom() const {
		return nom_;
	}
	virtual string getTitreJeu() const {
		return titreJeu_;
	}

protected:
	
private:
	string nom_;
	string titreJeu_;
};