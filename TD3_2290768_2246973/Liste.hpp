#pragma once
#include <cstddef>
#include <string>
#include <cppitertools/range.hpp>
#include <memory>
#include <gsl/span>

using namespace std;
using namespace iter;
using namespace gsl;


template <typename T>
class Liste {
public:

	Liste() = default;

	void setnElements(size_t nElements)  // Le lower Camel Case est +/- respecté pour que le nom de la variable soit le bon
	{
		nElements_ = nElements;
	}
	void ajouterUnElement(const shared_ptr<T>& element)
	{
		if (nElements_ == capacite_)
		{
			changerTaille(max(size_t(1), capacite_ * 2));
		}
		elements_[nElements_++] = element;
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
	shared_ptr<T> operator[](size_t position)const
	{
		return elements_[position];
	}


	void changerTaille(size_t nouvelleCapacite)
	{
		shared_ptr <shared_ptr <T>[]> nouveauxElements = make_unique<shared_ptr<T>[]>(nouvelleCapacite);
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
	size_t getnElements() const { return nElements_; }  // Le lower Camel Case est +/- respecté pour que le nom de la variable soit le bon
	 shared_ptr <shared_ptr <T>[]> getelements() const { return elements_; }  // Le lower Camel Case est +/- respecté pour que le nom de la variable soit le bon
		
private:
	 shared_ptr <shared_ptr <T>[]> elements_;
	size_t nElements_ = 0;
	size_t capacite_ = 1;


	
};