#pragma once
#include<cstddef>
#include<string>
#include<cppitertools/range.hpp>
#include <memory>
#include<gsl/span>

using namespace std;
using namespace iter;
using namespace gsl;

template <typename T> 
class Liste
{
public :

	Liste() = default; 

	void ajouterUnElement(const shared_ptr<T>& Element)
	{
		if (nElements_ == capacite_)
		{
			changerTaille(max(size_t(1), capacite_ * 2));
		}
		elements_[nElements_] = Element;
		nElements_++;
	}

	void enleverElement(const std::shared_ptr<T>& Element) 
	{
		for (size_t i : range(nElements_) )
		{
			if (elements_[i] = Element)
			{
				if (nElements_ < 1)
				{
					elements_[i] = elements_[i-1];
				}
				nElements_--;
			}
		}
	}

	shared_ptr<T> operator[](size_t position) const
	{
		return elements_[position];
	}

	shared_ptr<T> operator[](size_t position)
	{
		return elements_[position];
	}


	size_t getnElements() const
	{
		return nElements_;
	}

	size_t getCapacite() const
	{
		return capacite_;
	}



	shared_ptr<T> rechercherElement(const function<bool(const shared_ptr<T>&)> &critere) const {
		for (size_t i  : range(nElements_)) {
			if (critere(elements_[i])) {
				return elements_[i];
			}
		}
		return nullptr;
	}

	Liste(const Liste& autreliste)
	{
		capacite_ = autreliste.capacite_;
		nElements_ = autreliste.nElements_;
		elements_ = make_unique<shared_ptr<T>[]>(capacite_);
		for (size_t i = 0; i < nElements_; i++)
		{
			elements_[i] = autreliste.elements_[i];
		}

	}


	


private :

	size_t capacite_ = 0;
	size_t nElements_ = 0;
	unique_ptr <shared_ptr <T>[]> elements_;

	void changerTaille(const size_t nouvelleCapacite)
	{
		// Allocation correcte du nouveau tableau
		auto nouveauxElements = make_unique <shared_ptr<T>[]>(nouvelleCapacite);

		// Copier les anciens éléments dans le nouveau tableau
		for (size_t i : range(nElements_))
		{
			nouveauxElements[i] = elements_[i];
		}

		// Remplacer l'ancien tableau par le nouveau
		elements_ = move(nouveauxElements);
		capacite_ = nouvelleCapacite;
	}

};