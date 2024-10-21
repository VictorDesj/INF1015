#include "Jeu.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cassert>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "ListeDeveloppeurs.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
#include "Liste.hpp"
#include "Jeu.hpp"
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
	case enteteTailleVariableDeBase + 0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase + 1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase + 2: return lireType<uint32_t>(fichier);
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

shared_ptr<Concepteur> trouverConcepteur(const Liste<Jeu>& listeJeux, string nom) {
	auto critere = [nom](const shared_ptr<Concepteur>& concepteur) {
		return concepteur->nom == nom;
		};
	for (const shared_ptr<Jeu> j : listeJeux.spanListe()) {
		shared_ptr<Concepteur> concepteurTrouve = j->chercherConcepteur(critere);
		if (concepteurTrouve != nullptr) {
			return concepteurTrouve; 
		}
	}
	return nullptr;
}


shared_ptr<Concepteur> lireConcepteur(istream& fichier, Liste<Jeu>& listeJeux)
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
		return concepteurExistant;

	//std::cout << concepteur.nom << endl;  //TODO: Enlever cet affichage temporaire servant à voir que le code fourni lit bien les jeux.
	std::cout << "\033[92m" << "Allocation en mémoire du concepteur " << concepteur.nom
		<< "\033[0m" << endl;
	return make_shared<Concepteur>(concepteur);//TODO: Retourner le pointeur vers le concepteur crée.
}






shared_ptr<Jeu> lireJeu(istream& fichier, Liste<Jeu>& listeJeux)
{
	Jeu jeu = {}; // On initialise une structure vide de type Jeu
	jeu.titre = lireString(fichier);
	jeu.anneeSortie = int(lireUintTailleVariable(fichier));
	jeu.developpeur = lireString(fichier);
	jeu.concepteurs.setNElements(lireUintTailleVariable(fichier));
	// Rendu ici, les champs précédents de la structure jeu sont remplis avec la
	// bonne information.

	shared_ptr<Jeu> ptrJeu = make_shared<Jeu>(jeu);

	std::cout << "\033[96m" << "Allocation en mémoire du jeu " << jeu.titre
		<< "\033[0m" << endl;
	// std::cout << jeu.titre << endl;  //TODO: Enlever cet affichage temporaire servant à voir que le code fourni lit bien les jeux.
	// On n'a pas demandé de faire une réallocation dynamique pour les designers.
	ptrJeu->concepteurs.initElements(ptrJeu->concepteurs.getNElements());
	for (shared_ptr<Concepteur>& c : ptrJeu->concepteurs.spanListe()) {
		c = lireConcepteur(fichier, listeJeux);
		c->jeuxConcus.ajouterUnElement(ptrJeu);
	}
	return ptrJeu; //TODO: Retourner le pointeur vers le nouveau jeu.
}

Liste<Jeu> creerListeJeux(const string & nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	size_t nElements = lireUintTailleVariable(fichier);
	Liste<Jeu> listeJeux = {};
	for ([[maybe_unused]] size_t n : iter::range(nElements))
	{
		listeJeux.ajouterUnElement(lireJeu(fichier, listeJeux)); //TODO: Ajouter le jeu à la ListeJeux.
		cout << n;
	}

	return listeJeux; //TODO: Renvoyer la ListeJeux.
}


//TODO: Fonction pour détruire une ListeJeux et tous ses jeux.

void afficherConcepteur(const Concepteur& d)
{
	std::cout << "\t" << d.nom << ", " << d.anneeNaissance << ", " << d.pays
		<< endl;
}


void afficherJeu(const Jeu& j)
{
	std::cout << "Titre : " << "\033[94m" << j.titre << "\033[0m" << endl;
	std::cout << "Parution : " << "\033[94m" << j.anneeSortie << "\033[0m"<< endl;
	std::cout << "Concepteurs du jeu :" << "\033[94m" << endl;
	for (const shared_ptr<Concepteur> c : j.concepteurs.spanListe())
		afficherConcepteur(*c);
	std::cout << "\033[0m";
}

ostream& operator<<(ostream& os, const Concepteur& concepteur) {
	os << concepteur.nom << " (né en " << concepteur.anneeNaissance << ")";
	return os;
}
ostream& operator<<(ostream& os, const Jeu& jeu) {
	os << "Titre: " << jeu.titre << "\n"
		<< "Annee de sortie: " << jeu.anneeSortie << "\n"
		<< "Concepteurs:\n";
	for (const auto& concepteur : jeu.concepteurs.spanListe()) {
		if (concepteur) {
			os << "  - " << *concepteur << "\n";  // Utilise la surcharge pour Concepteur
		}
	}
	return os;
}

template <typename T>
ostream& operator<<(ostream& os, const Liste<T>& liste)
{
	static const string ligneSeparation = "\n\033[95m"
		"══════════════════════════════════════════════════════════════════════════"
		"\033[0m\n";
	for (shared_ptr<T> j : liste.spanListe())
	{
		os << *j << endl<<ligneSeparation<<endl;  // Affiche chaque élément en supposant que T surcharge aussi <<
	}
	return os;
}


//TODO: Fonction pour afficher tous les jeux de ListeJeux, séparés par un ligne.
// Servez-vous de la fonction d'affichage d'un jeu crée ci-dessus. Votre ligne
// de séparation doit être différent de celle utilisée dans le main.
void afficherListeJeux(const Liste<Jeu>& listeJeux)
{
	static const string ligneSeparation = "\n\033[95m"
		"══════════════════════════════════════════════════════════════════════════"
		"\033[0m\n";
	std::cout << ligneSeparation << endl;
	for (const shared_ptr<Jeu> j : listeJeux.spanListe())
	{
		afficherJeu(*j);
		std::cout << ligneSeparation << endl;
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
#pragma endregion

	//int* fuite = new int;  // Pour vérifier que la détection de fuites fonctionne; un message devrait dire qu'il y a une fuite à cette ligne.



	Liste<Jeu> lj = creerListeJeux("jeux.bin"); //TODO: Appeler correctement votre fonction de création de la liste de jeux.

	static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
	std::cout << ligneSeparation << endl;
	std::cout << "Premier jeu de la liste :" << endl;
	//TODO: Afficher le premier jeu de la liste (en utilisant la fonction).  Devrait être Chrono Trigger.
	afficherJeu(*lj[0]);
	std::cout << ligneSeparation << endl;


	//afficherListeJeux(lj);
	cout << ligneSeparation << endl << "Test #3"<<endl;
	lj.afficherCapacite();

	cout << ligneSeparation << endl<<"Test #4"<<endl;
	
	cout << lj[2]->titre << endl << lj[2]->concepteurs[1]->nom<<endl;
	
	cout << ligneSeparation << endl << "Test #5" << endl;
	auto critere = [](const shared_ptr<Concepteur>& concepteur) {
		return concepteur->nom == "Yoshinori Kitase";
		};

	shared_ptr<Concepteur> concepteur1 = lj[0]->chercherConcepteur(critere);
	if (concepteur1) {
		cout << "Concepteur trouvé dans le jeu à l'indice 0 : " << concepteur1->nom
			<< ", Année de naissance : " << concepteur1->anneeNaissance << endl;
	}


	shared_ptr<Concepteur> concepteur2 = lj[1]->chercherConcepteur(critere);
	if (concepteur2) {
		cout << "Concepteur trouvé dans le jeu à l'indice 1 : " << concepteur2->nom
			<< ", Année de naissance : " << concepteur2->anneeNaissance << endl;
	}


	if (concepteur1 == concepteur2) {
		cout << "Les pointeurs vers le concepteur sont identiques." << endl;
	}
	
	cout << ligneSeparation << endl << "Test #6" << endl;

	cout << lj;
	/*
	std::ofstream fichierSortie("sortie.txt");
	if (fichierSortie.is_open()) {
		fichierSortie << lj;
		fichierSortie.close();
		std::cout << "Liste de jeux écrite dans 'sortie.txt'.\n";
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier.\n";
	}
	*/
	cout << ligneSeparation << endl << "Test #7" << endl;

	Jeu copieJeu = *lj[2];

	// Remplacer le deuxième concepteur dans copieJeu par un autre venant du jeu à l'indice 0
	if (copieJeu.concepteurs.getNElements() > 1 && lj[0]->concepteurs.getNElements() > 1) {
		copieJeu.concepteurs[1] = lj[0]->concepteurs[1];  // Remplacement du 2e concepteur par celui du jeu à l'indice 0
	}

	// Afficher le jeu à l'indice 2
	std::cout << "Jeu original à l'indice 2 :\n";
	std::cout << *lj[2] << "\n";

	// Afficher la copie modifiée
	std::cout << "Copie du jeu avec modification :\n";
	std::cout << copieJeu << "\n";

	// Vérification que l'adresse du premier concepteur est la même
	if (lj[2]->concepteurs[0] == copieJeu.concepteurs[0]) {
		std::cout << "L'adresse du premier concepteur est la même.\n";
	}
	else {
		std::cout << "L'adresse du premier concepteur est différente.\n";
	}

	// Vérification que les listes de concepteurs sont bien différentes
	if (lj[2]->concepteurs[1] != copieJeu.concepteurs[1]) {
		std::cout << "Les listes de concepteurs sont différentes.\n";
	}
	else {
		std::cout << "Les listes de concepteurs sont identiques (ce qui ne devrait pas être le cas).\n";
	}
}
