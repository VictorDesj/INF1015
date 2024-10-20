#pragma once
#include <cstddef>
#include "Concepteur.hpp"

struct ListeConcepteurs
{
	std::size_t nElements, capacite;
	unique_ptr <shared_ptr <Concepteur>[]> elements_;
};