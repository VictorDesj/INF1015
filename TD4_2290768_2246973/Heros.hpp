#pragma once
#include <iostream>
#include <string>
#include "Personnage.hpp"
#include <vector>

using namespace std;

class Heros : virtual public Personnage {
public:
    Heros(const string& nom, const string& titreJeu, const string& ennemi, const vector<string>& allies)
        : Personnage(nom, titreJeu), ennemi_(ennemi), allies_(allies) {}

    ostream& afficher(ostream& out) const override {
        Personnage::afficher(out);
        out << "Ennemi : " << ennemi_ << endl << "Alli�s : ";
        for (const auto& allie : allies_) {
            out << allie << endl;
        }
        return out;
    }

    string getEnnemi() const {
        return ennemi_;
    }
    vector<string> getAllies() const {
        return allies_;
    }

protected:
    
private:
    string ennemi_;
    vector<string> allies_ = {};
};