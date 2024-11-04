#pragma once
#include "Personnage.hpp"


class Vilain :public Personnage {
public:
	Vilain(const string& nom, const string& titreJeu,const string objectif):Personnage(nom,titreJeu),objectif_(objectif){}
	ostream& afficher(ostream& out) {
		Personnage::afficher(out);
		cout << "Objectif : " << objectif_ << endl;
		return out;
	}
	string getObjectif() {
		return objectif_;
	}
protected:
	void setObjectif(string objectif) {
		objectif_ = objectif;
	}

private:
	string objectif_;
};