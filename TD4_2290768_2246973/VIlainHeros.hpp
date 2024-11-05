#pragma once
#include "Personnage.hpp"
#include "Vilain.hpp"
#include "Heros.hpp"


class VilainHeros : virtual public Vilain, virtual public Heros {
public:
	VilainHeros(const Vilain& vilain, const Heros& hero) :Personnage(vilain.getNom() + "-" + hero.getNom(), vilain.getTitreJeu() + "-" + hero.getTitreJeu()),
		Vilain(vilain.getNom() + "-" + hero.getNom(), vilain.getTitreJeu() + "-" + hero.getTitreJeu(), vilain.getObjectif()),
		Heros(vilain.getNom() + "-" + hero.getNom(), vilain.getTitreJeu() + "-" + hero.getTitreJeu(), hero.getEnnemi(), hero.getAllies()),
		missionSpeciale_(vilain.getObjectif() + " dans le monde de " + hero.getTitreJeu()) {}

	ostream& afficher(ostream& out) const override {
		Vilain::afficher(out);
		out << "Ennemi : " << Heros::getEnnemi() << endl<< "Allies : ";
		
		for (string allie : Heros::getAllies()) {
			out << allie << endl;
		}
		out << "Mission speciale : " << missionSpeciale_<<endl;
		return out;
	}
	string getMissionSpeciale() {
		return missionSpeciale_;
	}
protected:
	void setMissionSpeciale(const string& mission) {
		missionSpeciale_ = mission;
	}
private:
	string missionSpeciale_;
};

