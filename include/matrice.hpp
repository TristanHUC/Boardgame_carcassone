#ifndef MATRICE_HPP
#define MATRICE_HPP

#include <array>
#include <stdexcept>

template<class T, int lignes, int colonnes>
class Matrice {
public:

    using container_type = std::array<T, lignes * colonnes>;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    constexpr T& at(std::size_t ligne, std::size_t colonne) {
        if (ligne >= lignes || colonne >= colonnes) {
            throw std::out_of_range {"Accès matrice out of range"};
        }
        return data_.at(ligne * colonnes + colonne);
    }
    constexpr const T& at(std::size_t ligne, std::size_t colonne) const {
        if (ligne >= lignes || colonne >= colonnes) {
            throw std::out_of_range {"Accès matrice out of range"};
        }
        return data_.at(ligne * colonnes + colonne);
    }

    constexpr int nb_lignes() const noexcept {return lignes;}

    constexpr int nb_colonnes() const noexcept {return colonnes;}

    constexpr iterator begin() noexcept {
        return data_.begin();
    }
    constexpr const_iterator begin() const noexcept {
        return data_.begin();
    }
    constexpr iterator end() noexcept {
        return data_.end();
    }
    constexpr const_iterator end() const noexcept {
        return data_.end();
    }
    constexpr const_iterator cbegin() const noexcept {
        return data_.cbegin();
    }
    constexpr const_iterator cend() const noexcept {
        return data_.cend();
    }

private:
    container_type data_;
};

#endif // MATRICE_HPP
