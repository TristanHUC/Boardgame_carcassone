#ifndef ZONE_H
#define ZONE_H

#include <string>
#include <vector>
#include <memory>
#include <QDebug>

#include "matrice.hpp"
#include "minituile.h"
#include "tuile.h"
#include "pion.h"

class Plateau;

class Zone {
 public:
  Zone(const std::vector<Minituile*>& minituiles, Plateau& p)
      : minituiles_(minituiles), nbminituiles(minituiles.size()), plateau(&p) {
    // mettre la zone ds l'attribut zone de chaque minituiles
    for (size_t i = 0; i < minituiles.size(); i++) {
      minituiles[i]->setZone(*this);
    }
  }
  ~Zone() = default;

  Zone& operator+(const Zone&);       // joindre deux zones
  Zone& operator+(Minituile&);  // Ajouter une minituile à une zone
  size_t getnbminituiles() const { return nbminituiles; }
  std::vector<Minituile*> getminituiles() { return minituiles_; }
  std::vector<Minituile*> getminituiles() const { return minituiles_; }
  bool estFermee() const;                    // vérifie si la zone est fermée
  std::vector<Pion*> pionsOccupants();  // retourne un vecteur contenant les
                                       // pions occupants la zone, vide si libre
  
  const Plateau* getPlateau() const { return plateau; }
  Plateau* getPlateau() { return plateau; }
  // INUTILE :const int getnumProprio(std::vector<Pion&>pions);

  //methode test
  void affiche_zone() const;

  //methode pour obtenir le score actuel d'une zone (a redefinir dans les classes filles)
  virtual size_t getScore() const  = 0;

  //methode pour obtenir toutes les tuiles d'une zone, utile pour savoir si il y a blason/auberge/cathedrale
  std::list<Tuile*> getTuile() const;

 private:
  std::vector<Minituile*> minituiles_;

  size_t nbminituiles;
  Plateau* plateau;
};

class Zone_Ville : public Zone {
public:
    size_t getScore() const override;
    Zone_Ville(const std::vector<Minituile*>& minituiles, Plateau& p) : Zone(minituiles,p) {}
};

class Zone_Pre : public Zone {
public:
    size_t getScore() const override;
    Zone_Pre(const std::vector<Minituile*>& minituiles, Plateau& p) : Zone(minituiles,p) {}
};

class Zone_Route : public Zone {
public:
    size_t getScore() const override;
    Zone_Route(const std::vector<Minituile*>& minituiles, Plateau& p) : Zone(minituiles,p) {}
};

class Zone_Abbe : public Zone {
public:
    size_t getScore() const override;
    Zone_Abbe(const std::vector<Minituile*>& minituiles, Plateau& p) : Zone(minituiles,p) {}
};

class Zone_non_evaluee : public Zone {
public:
    size_t getScore() const override {return 0;}
    Zone_non_evaluee(const std::vector<Minituile*>& minituiles, Plateau& p) : Zone(minituiles,p) {}
};
#endif  // ZONE_H
// une zone est fermee si toutes les minituiles de la zone ont un voisin
