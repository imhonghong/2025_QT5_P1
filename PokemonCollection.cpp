#include "PokemonCollection.h"

PokemonCollection::PokemonCollection() {
    // 初始測試資料
    pokemons.append(new Pokemon("Charmander", ":/battle/data/battle/charmander.png"));
    pokemons.append(new Pokemon("Squirtle", ":/battle/data/battle/squirtle.png"));
    pokemons.append(new Pokemon("Charmander", ":/battle/data/battle/charmander.png"));
    pokemons.append(new Pokemon("Squirtle", ":/battle/data/battle/squirtle.png"));
}

void PokemonCollection::addPokemon(Pokemon *pokemon) {
    pokemons.append(pokemon);
}

QVector<Pokemon*> PokemonCollection::getAllPokemons() const {
    return pokemons;
}
