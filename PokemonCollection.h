#pragma once

#include <QVector>
#include "Pokemon.h"

class PokemonCollection {
public:
    PokemonCollection();

    void addPokemon(Pokemon *pokemon);
    QVector<Pokemon*> getAllPokemons() const;
    void reorderPokemons();

private:
    QVector<Pokemon*> pokemons;
};
