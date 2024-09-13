/**
* Programme qui comporte les 6 numéros du TP1 à remettre le 15 septembre 2024
* \file TP1.cpp
* \author Desjardins et
* \date 13 septembre 2024
* Créé le 3 septembre 2024
*/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;


void Numero4() {

	//On initialise nos valeurs du problème
	int nbADeviner;
	double essai = 1.5;   //On initialise à un nombre que l'
	int nEssais = 1;

	cout << "Entre votre nombre à deviner";
	cin >> nbADeviner;

	while (essai != nbADeviner) {
		cout << "Entrez votre tentative de deviner le nombre:";
		cin >> essai;
		if (essai < nbADeviner) 
			cout << "Votre nombre est trop petit\n";
		else if (essai > nbADeviner)
			cout << "Votre nombre est trop grand\n";
		nEssais += 1;
	}
	cout << "Vous avez deviné le bon nombre apres "<<nEssais-1<<" tentatives";

}

vector<int> Dedoubleur(int t[],int nbDeValeur) {
	vector<int> tableauSansDouble;
	for (int iteration = 0;iteration < nbDeValeur;iteration++) {
		if (find(tableauSansDouble.begin(), tableauSansDouble.end(), t[iteration])== tableauSansDouble.end())
			tableauSansDouble.push_back(t[iteration]);
	}
	return tableauSansDouble;
}

void Numero5() {
	int nValeurs=0;
	string chaineEntree;
	int positionTableauDoublon = 0;
	cout << "Entrez un nombre n et un tableau contenant n valeurs pour n plus petit que 100: ";
	cin >> nValeurs >> chaineEntree;
	int tableauAvecDoublons[100] = {};
	int i = 1;
	string valeurAjoutee = "";

	while (chaineEntree[i] != ']') {
		if (isdigit(chaineEntree[i]))
			valeurAjoutee += chaineEntree[i];
		else if (chaineEntree[i]==',') {
			tableauAvecDoublons[positionTableauDoublon] = stoi(valeurAjoutee);
			valeurAjoutee = "";
			positionTableauDoublon++;
		}
		i++;
	}

	tableauAvecDoublons[positionTableauDoublon] = stoi(valeurAjoutee);
	vector<int> listeDedoublee = Dedoubleur(tableauAvecDoublons,nValeurs);
	cout << "\nLe tableau sans les doublons est ["<<listeDedoublee[0];
	for (int j = 1; j <= size(listeDedoublee) - 1;j++) {
		cout << ","<<listeDedoublee[j] ;
	}
	cout << "]";
}

struct Etudiant{
	int matricule;
	double noteIntra;
	double moyLabs;
	double noteFinal;
	double moyFinale;
};
void Numero6() {
	ifstream lecture("notes.txt");
	Etudiant etudiants[10];
	int matricule;
	double noteIntra;
	double moyLabs;
	double noteFinal;
	
	int numEtudiant=0;
	while (lecture >> etudiants[numEtudiant].matricule
		>> etudiants[numEtudiant].noteIntra
		>> etudiants[numEtudiant].moyLabs 
		>> etudiants[numEtudiant].noteFinal) {
		double moyenneFinale = 0.3 * etudiants[numEtudiant].noteIntra + 0.3 * etudiants[numEtudiant].moyLabs + 0.4 * etudiants[numEtudiant].noteFinal;
		etudiants[numEtudiant].moyFinale=moyenneFinale;
		numEtudiant++;
	}
	int noteLabMax = 0;
	int matriculeLabMax = 0;
	double noteMoyMax = 0.0;
	int matriculeMoyMax = 0;
	double moyenneClasse = 0.0;
	for (int i = 0;i <= 9;i++) {
		if (etudiants[i].moyLabs > noteLabMax) {
			noteLabMax = etudiants[i].moyLabs;
			matriculeLabMax = etudiants[i].matricule;
		}
		if (etudiants[i].moyFinale > noteMoyMax) {
			noteMoyMax = etudiants[i].moyFinale;
			matriculeMoyMax = etudiants[i].matricule;
		}
		moyenneClasse += etudiants[i].moyFinale;
	}
	moyenneClasse /= 10;
	cout << moyenneClasse << "\n" << noteLabMax << "," << matriculeLabMax << "\n" << noteMoyMax << "," << matriculeMoyMax;
	ofstream ecriture("evelaution.txt");
	ecriture << "La moyenne de la classe est : " << moyenneClasse << endl;
	ecriture << "L'etudiant ayant le matricule " << matriculeLabMax <<" a la meilleure note "<< noteLabMax <<" dans les labs" << endl;
	ecriture << "L'etudiant ayant le matricule " << matriculeMoyMax << " a la meilleure moyenne " << noteMoyMax << endl;

}

int main() {
	Numero6();
}