#ifndef MINITUILE_H
#define MINITUILE_H

#include <typeinfo>
#include <string>
#include <iostream>
#include <QDebug>

#include "joueur.hpp"
#include "pion.h"
class Zone;

class Minituile {
 public:
  bool poserPion(Joueur& joueur);
  void recupererPion(Joueur& joueur);
  inline void setPion(std::unique_ptr<Pion> pion) {
    pion_ = std::move(pion);
  }
  Zone* getZone() { return zone_; }
  void setZone(Zone& zone) { zone_ = &zone; }
  Pion* getPion() {
      return pion_.get();
  }
  virtual bool estFermee(); //utile pour savoir si une zone est fermee
  bool possedePion() const {return pion_!=nullptr;}
 protected :
  std::unique_ptr<Pion> pion_ = nullptr;
 private:
  virtual bool doPoserPion(Joueur& joueur) = 0;
  virtual void doRecupererPion(Joueur& joueur) = 0;
  Zone* zone_ = nullptr;  // zone de laquel la minituile fait parti (une seule forcement)
};

class Pre : public Minituile {
 private:
  bool doPoserPion(Joueur& joueur) override ;
  void doRecupererPion(Joueur& joueur) override;
  bool estFermee() override {return false;}
};

class Ville : public Minituile {
 private:
  bool doPoserPion(Joueur& joueur) override;
  void doRecupererPion(Joueur& joueur) override;
};

class Route : public Minituile {
 private:
  bool doPoserPion(Joueur& joueur) override;
  void doRecupererPion(Joueur& joueur) override ;
};

class Riviere : public Minituile {
 private:
  bool doPoserPion(Joueur& joueur) override;
  void doRecupererPion(Joueur& joueur) override;
};

class Jardin : public Minituile {
 private:
  bool doPoserPion(Joueur& joueur) override;
  void doRecupererPion(Joueur& joueur) override;
  public:
  bool estFermee() override;
};
class Abbaye : public Minituile {
 private:
  bool doPoserPion(Joueur& joueur) override ;
  void doRecupererPion(Joueur& joueur) override;
  public:
  bool estFermee()override;
};

bool operator==(const Minituile& lhs, const Minituile& rhs);

class FabriqueMinituile {
public:
    Minituile* methodeFabrication(const std::string s);
};

#endif  // MINITUILE_H
