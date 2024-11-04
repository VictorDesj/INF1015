#pragma once
#include <iostream>
#include <string>

using namespace std;


class Affichable {
public:
	virtual ostream& afficher(ostream& out) const = 0;
	virtual void changerCouleur(int couleur) = 0;
	virtual ~Affichable() = default;
};