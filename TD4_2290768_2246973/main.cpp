/*
 TD4 nous utlisisons des notions de méthodes virtuelles, les objets polymorphes et l'héritage simple et multiple
Autheurd :
- Vincent Desjardins 2246973
- Ange Lares Simo Topi kenme 2290768
Date 06 Octobre 2024
Crée le 28 Octobre 2024 */


#include "lectureBinaire.hpp"
#include <fstream>
#include <sstream>
#include <cassert>
#include "bibliotheque_cours.hpp"
#include "VilainHeros.hpp"
#include <vector>

using namespace std;

static const string trait =
"═════════════════════════════════════════════════════════════════════════";


ifstream ouvrirFichierBinaire(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	return fichier;
}

void testsPourCouvertureLectureBinaire()
{
	istringstream iss("\xA0\x12\xA1\x21\x43\xA2\x98\xBA\xDC\xFE");
	assert(lireUintTailleVariable(iss) == 0x12);
	assert(lireUintTailleVariable(iss) == 0x4321);
	assert(lireUintTailleVariable(iss) == 0xFEDCBA98);
}

template <typename T>
void afficher(const vector<shared_ptr<T>>& listePerso) {
	for (const auto& personnagePtr : listePerso)
	{
		if (auto vilainHeros = dynamic_pointer_cast<VilainHeros>(personnagePtr)) {
			vilainHeros->changerCouleur(35);
			vilainHeros->afficher(cout);
			vilainHeros->changerCouleur(0);
		}
		else if (auto hero = dynamic_pointer_cast<Heros>(personnagePtr)) {
			hero->changerCouleur(34);
			hero->afficher(cout);
			hero->changerCouleur(0);
		}
		else if (auto vilain = dynamic_pointer_cast<Vilain>(personnagePtr)) {
			vilain->changerCouleur(31);
			vilain->afficher(cout);
			vilain->changerCouleur(0);
		}
		else {
			personnagePtr->afficher(cout);
		}
		cout << trait << endl;
	}

}

int main()
{
#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
#pragma endregion

	testsPourCouvertureLectureBinaire();
	
	// Ouverture des fichiers binaires
	ifstream fichierHeros = ouvrirFichierBinaire("heros.bin");
	ifstream fichierVilains = ouvrirFichierBinaire("vilains.bin");

	//TODO: Votre code pour le main commence ici (mais vous pouvez aussi ajouter/modifier du code avant si nécessaire)

	



	cout << trait << endl << "Test 1" << endl;
	Personnage perso1("Randi", "Secret of Mana");
	perso1.afficher(cout);
	perso1.changerCouleur(91);
	perso1.afficher(cout);
	cout << "\033[0m";

	cout << trait << endl << "Test 2" << endl;
	vector<string> allies = { "Red","Blue","Green" };
	Heros perso2("Larry Fitzgerald", "Madden", "Patriots", allies);
	perso2.afficher(cout);


	cout << trait << endl << "Test 3" << endl;
	Vilain perso3("Herobrine", "Minecraft", "Eliminier Steve");
	perso3.afficher(cout);
	perso3.changerCouleur(96);
	perso3.afficher(cout);
	cout << "\033[0m";

	cout << trait << endl << "Test 4" << endl;
	VilainHeros perso4(perso3,perso2);
	perso4.afficher(cout);

	cout << "\033[0m";


	vector <shared_ptr<Heros>> listeHeros = {};
	vector <shared_ptr<Vilain>> listeVilains = {};
	vector <shared_ptr<Personnage>> listePersonnages = {};


	// pour les heros 
	size_t nHeros = lireUintTailleVariable(fichierHeros);
	for (size_t i = 0; i < nHeros; i++)
	{
		string nom = lireString(fichierHeros);
		string jeu = lireString(fichierHeros);
		string ennemi = lireString(fichierHeros);
		vector <string> listeAllies = {};

		size_t nAllies = lireUintTailleVariable(fichierHeros);
		for (size_t j = 0; j < nAllies; j++)
		{
			listeAllies.push_back(lireString(fichierHeros));
		}

		Heros hero(nom, jeu, ennemi, listeAllies);
		listeHeros.push_back(make_shared<Heros>(hero));
	}

	// pour les vilains 
	size_t nVilains = lireUintTailleVariable(fichierVilains);
	for (size_t i = 0; i < nVilains; i++)
	{
		string nom = lireString(fichierVilains);
		string jeu = lireString(fichierVilains);
		string objectif = lireString(fichierVilains);

		Vilain vilain(nom, jeu, objectif);
		listeVilains.push_back(make_shared<Vilain>(vilain));
	}

	cout << "\n***Affichage de la liste des heros***\n";
	afficher(listeHeros);

	cout << "\n***Affichage de la liste des vilains***\n";
	afficher(listeVilains);

	for (const shared_ptr<Heros>& hero : listeHeros)
	{
		listePersonnages.push_back(hero);	
	}

	for (const shared_ptr <Vilain>& vilain : listeVilains)
	{
		listePersonnages.push_back(vilain);
	}

	
	
	VilainHeros mechantGentil(*listeVilains[2], *listeHeros[6]);
	listePersonnages.push_back(make_shared<VilainHeros>(mechantGentil));

	cout << "\n***Affichage de la liste des personnages***\n";
	afficher(listePersonnages);

}
