#ifndef PARTIE_H
#define PARTIE_H

#include <QDebug>

#include <vector>
#include <string>
#include <list>
#include <optional>
#include <typeinfo>
#include <iostream>
#include <chrono>
#include <random>

#include "joueur.hpp"
#include "pioche.h"
#include "plateau.h"
#include "extension.h"
#include "tuile.h"


class Partie {
 public:
  Partie(std::vector<std::string> noms_joueurs, std::list<std::unique_ptr<Extension>> liste_extension);

  decltype(auto) piocher() {
      return pioche_.piocher();
  }

  decltype(auto) reposer(Tuile* t){
      t->reset();
      return pioche_.reposer(t);
  }

  bool piochevide(){
      return pioche_.vide();
  }
  size_t getTaillePioche() const{return pioche_.getTaille();}

  auto& getPlateau() {
      return plateau_;
  }

  inline int getnbJoueurs() const { return nb_joueurs_; }
  inline int getTour() const { return tour_; }
  inline int getnbPionsPartie() const { return nb_pions_partie_; }
  void tourSuivant(){tour_++;}
  Joueur& getJoueur(int num){return joueurs_[num];}

  const auto& getExtensions() const { return extensions_; }

 private:
  std::list<std::unique_ptr<Extension>> extensions_;
  std::vector<Joueur> joueurs_;
  int nb_joueurs_{0};
  int tour_{0};
  Pioche pioche_;
  Plateau plateau_;
  int nb_pions_partie_;

  //methode privée mélanger les tuiles
  std::vector<Tuile*> melange(std::vector<Tuile*>& tuiles);

  //methode privée pour indiquer si on joue avec l'extension riviere ou non (influe sur la pioche)
    bool possedeExtensionRiviere() const;


    //methode privée utile pour le melange de la pioche
    Tuile_Riviere* getTuileRiviereSource(std::vector<Tuile*>&) const;
    Tuile_Riviere* getTuileRiviereLac(std::vector<Tuile*>&) const;
    std::vector<Tuile*> getAllTuileRiviere(std::vector<Tuile*>&) const;

    //la tuile 0 de base est celle que l'on doit poser en premier sur le plateau si l'extension riviere n'est pas active
    Tuile_Base* getTuileBase0(std::vector<Tuile*>&) const;
};

#endif  // PARTIE_H
