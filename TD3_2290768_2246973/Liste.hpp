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
class Liste {
public:
	//std::size_t nElements, capacite;
	//T** elements;
	Liste() = default;
	void ajouterUnElement(const shared_ptr<T>* Element)
	{
		if (nElements == capacite)
		{
			changerTaille(max(size_t(1), capacite_ * 2));
		}
		elements_[nElements] = Element;
		nElements_++;
		
	}
	void enleverElement(const std::shared_ptr<T>& Element) {
		for (size_t i : range(nElements_)
		{
			if (elements_[i] = Element)
			{
				elements_[i]=move(elements_[nElements]);


				nElements_--;
					break;
			}
		}
	}

	shared_ptr<T> operator[](size_t position)const
	{
		return elements_[position];
	}

private:
	unique_ptr <shared_ptr <T>[]> elements_;
	size_t nElements = nElements=0;
	size_t capacite = capacite=0;
	void changerTaille(size_t nouvelleCapacite)
	{
		auto nouveauxElements = make_unique <shared_ptr <T>[nouvelleCapacite]>;
		for (size_t i : range(elements_))
		{
			nouveauxElements.elements_[i] = elements_[i];
		}

		elements_ = move(nouveauxElements);
		capacite_ = nouvelleCapacite;
	}

};