/*
Ce TD nous precriavis l'utlisisation des notions de pointeurs intelligents, de surcharges d'opérateurs, des fonctions d'ordre supérieur,
de template, et de copie d'objets pour améliorer le code du TD2.
Author :
- Vincent Desjardins 2246973
- Ange Lares Simo Topi kenme 2290768
Date 22 Octobre 2024
Crée le 1 Octobre 2024 */


#include "Jeu.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cassert>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
#include"Liste.hpp"
#include <memory>
#include "Concepteur.hpp"
using namespace std;
using namespace iter;
using namespace gsl;

#pragma region "Fonctions de base pour vous aider"
template <typename T>
T lireType(istream& fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false&&(message)),terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream& fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete) {
	case enteteTailleVariableDeBase+0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase+1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase+2: return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion

//TODO: Fonction qui cherche un concepteur par son nom dans une ListeJeux.
// Cette fonction renvoie le pointeur vers le concepteur si elle le trouve dans
// un des jeux de la ListeJeux. En cas contraire, elle renvoie un pointeur nul.
shared_ptr<Concepteur> trouverConcepteur(const Liste<Jeu>& listeJeux, const string& nom) {
	// Itération sur la liste des jeux avec range
	for (size_t i : range(listeJeux.getnElements())) 
	{
		// Récupère le jeu i-ème dans la liste
		shared_ptr<Jeu> jeu = listeJeux[i];

		auto concepteur = jeu->rechercherConcepteur([&nom](const shared_ptr<Concepteur>& c) { return c->nom == nom; });
		if (concepteur != nullptr)
		{
			return concepteur;
		}
	}

	// Si aucun concepteur correspondant n'est trouvé, retourne nullptr
	return nullptr;
}


shared_ptr<Concepteur> lireConcepteur(istream& fichier, Liste<Jeu> & listeJeux)
{
	Concepteur concepteur = {}; // On initialise une structure vide de type Concepteur.
	concepteur.nom = lireString(fichier);
	concepteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	concepteur.pays = lireString(fichier);
	// Rendu ici, les champs précédents de la structure concepteur sont remplis
	// avec la bonne information.

	//TODO: Ajouter en mémoire le concepteur lu. Il faut revoyer le pointeur créé.
	// Attention, valider si le concepteur existe déjà avant de le créer, sinon
	// on va avoir des doublons car plusieurs jeux ont des concepteurs en commun
	// dans le fichier binaire. Pour ce faire, cette fonction aura besoin de
	// la liste de jeux principale en paramètre.
	// Afficher un message lorsque l'allocation du concepteur est réussie.
	shared_ptr<Concepteur> concepteurExistant = trouverConcepteur(listeJeux, concepteur.nom);
	if (concepteurExistant != nullptr)
	{
		return concepteurExistant;
	}
	
	concepteurExistant = make_shared<Concepteur>(concepteur);
	//cout << concepteur.nom << endl;  //TODO: Enlever cet affichage temporaire servant à voir que le code fourni lit bien les jeux.
	cout << "\033[92m" << "Allocation en mémoire du concepteur " << concepteur.nom
				<< "\033[0m" << endl;
	return concepteurExistant; //TODO: Retourner le pointeur vers le concepteur crée.
}



shared_ptr<Jeu> lireJeu(istream& fichier, Liste<Jeu>& listeJeux)
{
	Jeu jeu = {}; // On initialise une structure vide de type Jeu
	jeu.titre = lireString(fichier);
	jeu.anneeSortie = int(lireUintTailleVariable(fichier));
	jeu.developpeur = lireString(fichier);
	size_t nElements = lireUintTailleVariable(fichier);
	// Rendu ici, les champs précédents de la structure jeu sont remplis avec la
	// bonne information.

	//TODO: Ajouter en mémoire le jeu lu. Il faut revoyer le pointeur créé.
	// Attention, il faut aussi créer un tableau dynamique pour les concepteurs
	// que contient un jeu. Servez-vous de votre fonction d'ajout de jeu car la
	// liste de jeux participé est une ListeJeu. Afficher un message lorsque
	// l'allocation du jeu est réussie.
   // Ou allouer directement au début plutôt qu'en faire une copie ici.
	cout << "\033[96m" << "Allocation en mémoire du jeu " << jeu.titre
			  << "\033[0m" << endl;
	// cout << jeu.titre << endl;  //TODO: Enlever cet affichage temporaire servant à voir que le code fourni lit bien les jeux.
	//   // On n'a pas demandé de faire une réallocation dynamique pour les designers.
	for (size_t i = 0; i <nElements; i++)
	{
		jeu.concepteurs.ajouterUnElement(lireConcepteur(fichier, listeJeux));
	}
	shared_ptr<Jeu> ptrJeu = make_shared<Jeu>(jeu);
	return ptrJeu; //TODO: Retourner le pointeur vers le nouveau jeu.
}

Liste<Jeu> creerListeJeux(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	size_t nElements = lireUintTailleVariable(fichier);
	Liste<Jeu> listeJeux ;
	for([[maybe_unused]] size_t n : range(nElements))
	{
		listeJeux.ajouterUnElement(lireJeu(fichier, listeJeux));
	}

	return listeJeux; //TODO: Renvoyer la ListeJeux.
}



void afficherConcepteur(const Concepteur& d)
{
	cout << "\t" << d.nom << ", " << d.anneeNaissance << ", " << d.pays
			  << endl;
}

ostream& operator<<(ostream& os, const shared_ptr<Concepteur>& concepteur) {
	os << "\t" << concepteur->nom << ", " << concepteur->anneeNaissance << ", " << concepteur->pays << endl;
	return os;
}

ostream& operator<<(ostream& os, const Jeu& jeu) {
	os << "Titre: " << "\033[94m" << jeu.titre << "\033[0m" << endl;
	os << "Parution: " << "\033[94m" << jeu.anneeSortie << "\033[0m" << endl;
	for (size_t i : range(jeu.concepteurs.getnElements())) {
		os << jeu.concepteurs[i] << endl;
	}
	return os;
}

ostream& operator<<(ostream& os, const shared_ptr<Jeu>& jeu) {
	os << "Titre: " << "\033[94m" << jeu->titre << "\033[0m" << endl;
	os << "Parution: " << "\033[94m" << jeu->anneeSortie << "\033[0m" << endl;
	for (size_t i : range(jeu->concepteurs.getnElements())) {
		os << jeu->concepteurs[i] << endl;
	}
	return os;
}

ostream& operator<<(ostream& os, const Liste<Jeu> listeJeux) {
	static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
	os << ligneSeparation << endl;
	for (size_t i : range(listeJeux.getnElements())) {
		os << listeJeux[i];
		os << ligneSeparation << endl;
	}
	return os;

}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi(); 
	#pragma endregion


	Liste<Jeu> lj = creerListeJeux("jeux.bin"); //TODO: Appeler correctement votre fonction de création de la liste de jeux.

	static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
	cout << ligneSeparation << endl;
	cout << "Premier jeu de la liste :" << endl;
	//TODO: Afficher le premier jeu de la liste (en utilisant la fonction).  Devrait être Chrono Trigger.
	//cout << *lj[0] << ligneSeparation << endl;
	cout << ligneSeparation << endl;

	//TODO : Appel de la fonction affichage de la liste de jeux
	cout << ligneSeparation << lj << ligneSeparation;

	//test de la question 4
	if (lj[2]->titre == "Secret of Mana") 
	{
		cout << " Le test de l'operateur [] est un succes. " << endl;
	}
	else 
	{
		cout << "Le test de l'operateur [] est un echec." << endl;
	}

	cout << ligneSeparation << endl;

	if (lj[2]->concepteurs[1]->nom == "Hiromichi Tanaka") {
		cout << "fonction est un succès " << endl;
	}
	else {
		cout << "fonction est un echec partiel " << endl;
	}

	auto concepteur1 = lj[0]->rechercherConcepteur([](const shared_ptr<Concepteur>& c) { return c->nom == "Yoshinori Kitase"; });
	auto concepteur2 = lj[1]->rechercherConcepteur([](const shared_ptr<Concepteur>& c) { return c->nom == "Yoshinori Kitase"; });



	if (concepteur1 == concepteur2 && concepteur1->anneeNaissance == 1966) {
		cout << "Le test de la fonction lambda est une reussite." << endl;
	}
	else {
		cout << "Le test de la fonction lambda est un echec car les adresses des pointeurs different malgre qu'ils aient le meme contenu." << endl;
	}

	//test de la question 6 
	ofstream("sortie.txt") << lj;

	//test de la question 7
	Jeu copieJeu = *lj[2];

	copieJeu.concepteurs[2] = lj[0]->concepteurs[3];

	cout << "Le jeu original à l'indice 2 est: " << *lj[2] << endl;
	cout << "La copie modifiée est: " << copieJeu << endl;
	if ((lj[2]->concepteurs[0]) == (copieJeu.concepteurs[0])) {
		cout << "L'adresse du premier Concepteur dans les deux listes est la même!" << endl;
	}
	else {
		cout << "L'adresse du premier Concepteur dans les deux listes est la même!" << endl;


	}
}
