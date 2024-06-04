#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <QDebug>

#include "pion.h"

class Joueur {
 public:
  Joueur(std::string nom, int numero, int nbpions, std::vector<std::unique_ptr<Pion>>&& pions)
      : nom_(nom), numero_(numero), nbpionrestant_(nbpions), pions_(std::move(pions)) {
      for (auto& pion: pions_) {
          pion->setProprietaire(this);
      }
  }
  constexpr const auto& getName() const noexcept { return nom_; }

  constexpr auto getScore() const noexcept { return score_; }
  constexpr auto getNumero() const noexcept { return numero_; }

  constexpr auto getNbPion() const noexcept { return nbpionrestant_; }

  constexpr auto addScore(std::uint16_t score) noexcept {
    score_ += score;
    return score_;
  }
  /*Joueur (const Joueur &j){
      nom_=j.nom_;
      score_=j.score_;
      numero_=j.numero_;
      nbpionrestant_=j.nbpionrestant_;
      for(auto& p : j.pions_){
          pions_.push_back(std::move(p));
      }
  }*/
  //retirer pion du vecteur du joueur
  inline void retirerUnPion(int numero) {
    if (pions_.size() != 0) {
      auto i = findIndex(pions_, numero);
      pions_.erase(pions_.begin() + i);
      nbpionrestant_--;
    }
  }
  //recuperer un pion dans le vecteur
  inline void recupererUnPion(std::unique_ptr<Pion> pion) {
      // rajouter le pion à au vector
      pions_.push_back(std::move(pion));
      nbpionrestant_++;
  }
  //TODO
  std::unique_ptr<Pion> selectionnerPion();

  void setProprietairePion(){
      for(auto& p:pions_){
          p->setProprietaire(this);
      }
  }

  unsigned long findIndex(const std::vector<std::unique_ptr<Pion>>& arr, int item);
  void setScore(int score){score_=score;}
 private:
  std::string nom_;
  int score_{0};
  int numero_;
  int nbpionrestant_;
  std::vector<std::unique_ptr<Pion>> pions_;
};

#endif  // JOUEUR_HPP
