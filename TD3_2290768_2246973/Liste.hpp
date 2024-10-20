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
	//std::size_t nElements, capacite;
	//T** elements;
	Liste() = default;

	void ajouterUnElement(const shared_ptr<T> element)
	{
		if (nElements_ == capacite_)
		{
			changerTaille(max(size_t(1), capacite_ * 2));
		}
		elements_[nElements_++] = move(element);
	}
	void enleverElement(const std::shared_ptr<T>& element) 
	{
		for (size_t i : range(nElements_))
		{
			if (elements_[i] == element)
			{
				elements_[i]=move(elements_[nElements_-1]);
				elements_[nElements_-1].reset();
				nElements_--;
				break;
			}
		}
	}
	shared_ptr<T> operator[](size_t position)const
	{
		return elements_[position];
	}
	unique_ptr <shared_ptr <T>[]> elements_;
	size_t nElements_ = 0;
	size_t capacite_ = 0;
private:
	

	void changerTaille(size_t nouvelleCapacite)
	{
		auto nouveauxElements = make_unique<shared_ptr<T>[]>(nouvelleCapacite);
		for (size_t i : range(nElements_))
		{
			nouveauxElements[i] = elements_[i];
		}

		elements_ = move(nouveauxElements);
		capacite_ = nouvelleCapacite;
	}

};
