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

void PokemonCollection::reorderPokemons() {
    std::stable_sort(pokemons.begin(), pokemons.end(), [](Pokemon* a, Pokemon* b) {
        return (a->getHp() > 0) && (b->getHp() == 0);
    });
}
