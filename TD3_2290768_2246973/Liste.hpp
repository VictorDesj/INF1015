#pragma once
#include <cstddef>
#include <string>
#include <cppitertools/range.hpp>
#include <memory>
#include <gsl/span>
#include <iostream>

using namespace std;
using namespace iter;
using namespace gsl;


template <typename T>
class Liste {
public:

	Liste() = default;

	Liste(const Liste<T>& autre)
	{
		nElements_ = autre.nElements_;
		capacite_ = autre.capacite_;
		elements_ = make_unique<shared_ptr<T>[]>(capacite_);
		*this = autre;
	}

	void setNElements(size_t nElements) 
	{
		while (nElements > capacite_) 
		{
			changerTaille(max(size_t(1), capacite_ * 2));
		}
		nElements_ = nElements;

	}

	void initElements(size_t n)  
	{     
		nElements_ = n;
		elements_ = make_unique<shared_ptr<T>[]>(nElements_);
	}

	void ajouterUnElement(const shared_ptr<T>& element)
	{
		if (nElements_ == capacite_)
		{
			changerTaille(max(size_t(1), capacite_ * 2));
		}
		elements_[nElements_] =element;
		nElements_++;
		
	}

	void enleverElement(const std::shared_ptr<T>& element)
	{
		for (size_t i : range(nElements_))
		{
			if (elements_[i] == element)
			{
				elements_[i] = move(elements_[nElements_ - 1]);
				elements_[nElements_ - 1].reset();
				nElements_--;
				break;
			}
		}
	}
	
	shared_ptr<T> chercherElement(const function<bool(const shared_ptr<T>&)>& critere) const {
		for (size_t i = 0; i < nElements_; ++i) {
			if (critere(elements_[i])) {
				return elements_[i]; 
			}
		}
		return nullptr; 
	}

	void changerTaille(size_t nouvelleCapacite)
	{
		
		unique_ptr <shared_ptr <T>[]> nouveauxElements = make_unique<shared_ptr<T>[]>(nouvelleCapacite);
		for (size_t i : range(nElements_))
		{
			nouveauxElements[i] = move(elements_[i]);
		}

		elements_ = move(nouveauxElements);
		capacite_ = nouvelleCapacite;
	}

	gsl::span<shared_ptr<T>> spanListe() const
	{
		return  gsl::span<shared_ptr<T>>(elements_.get(), nElements_);
	}

	void afficherCapacite() { cout << "Capacité: "<<capacite_; }
	void afficherNElements() { cout << "nÉléments: " << nElements_; }

	
	shared_ptr<T>& operator[](size_t position)
	{
		return elements_[position];
	}
	shared_ptr<const T>& operator[](size_t position)const
	{
		return elements_[position];
	}
	
	Liste<T>& operator=(const Liste<T>& autre) 
	{
		nElements_ = autre.nElements_;
		capacite_ = autre.capacite_;
		for (size_t i : range(autre.nElements_))
		{
			elements_[i] = autre.elements_[i];
			i++;
		}
		return *this;
	}


	size_t getNElements() const { return nElements_; }  
	unique_ptr <shared_ptr <T>[]> getElements() const { return elements_; } 
		


private:
	unique_ptr <shared_ptr <T>[]> elements_;
	size_t nElements_ = 0;
	size_t capacite_ = 0;
};