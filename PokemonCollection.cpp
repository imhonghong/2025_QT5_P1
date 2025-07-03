#include "PokemonCollection.h"

PokemonCollection::PokemonCollection() {
    // 初始測試資料

}

void PokemonCollection::addPokemon(Pokemon *pokemon) {
    pokemons.append(pokemon);
}

QVector<Pokemon*> PokemonCollection::getAllPokemons() const {
    return pokemons;
}
