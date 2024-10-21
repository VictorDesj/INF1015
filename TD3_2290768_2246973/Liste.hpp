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

	void setnElements(size_t nElements)  // Le lower Camel Case est +/- respecté pour que le nom de la variable soit le bon
	{
		while (nElements > capacite_) 
		{
			changerTaille(max(size_t(1), capacite_ * 2));
		}
		nElements_ = nElements;

	}

	void initelements(size_t n) 
	{     // Le lower Camel Case est +/- respecté pour que le nom de la variable soit le bon
		nElements_ = n;
		elements_ = make_shared<shared_ptr<T>[]>(nElements_);
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
				return elements_[i]; // Return the first matching element
			}
		}
		return nullptr; // No matching element found
	}

	void changerTaille(size_t nouvelleCapacite)
	{
		
		shared_ptr <shared_ptr <T>[]> nouveauxElements = make_shared<shared_ptr<T>[]>(nouvelleCapacite);
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

	void afficherCapacite() { cout << capacite_; }

	
	shared_ptr<T> operator[](size_t position)
	{
		return elements_[position];
	}
	shared_ptr<const T> operator[](size_t position)const
	{
		return elements_[position];
	}
	

	size_t getnElements() const { return nElements_; }  // Le lower Camel Case est +/- respecté pour que le nom de la variable soit le bon
	shared_ptr <shared_ptr <T>[]> getelements() const { return elements_; }  // Le lower Camel Case est +/- respecté pour que le nom de la variable soit le bon
		
private:
	shared_ptr <shared_ptr <T>[]> elements_;
	size_t nElements_ = 0;
	size_t capacite_ = 0;


	
};