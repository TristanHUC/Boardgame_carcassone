#ifndef PION_H
#define PION_H

#include <string>

class Joueur;

class Pion {
  unsigned int poids_;
  Joueur* proprietaire_;
  int numero_ ;  //à initialiser dans extension avec la construction
                 // chaque pion aura un num a partir de 0

 public:
  virtual std::string type() = 0;
  // fonction virtuelle pour rendre la classe abstraite
  Pion(int numero,unsigned int poids=0):poids_(poids),proprietaire_(nullptr),numero_(numero){}
  int getNumero()const { return numero_; }
  void setNumero(int num) { numero_=num; }
  unsigned int getPoids() const { return poids_; }
  void setPoids(int poids){ poids_=poids; }
  Joueur* getProprietaire() const { return proprietaire_; }
  void setProprietaire(Joueur* joueur){ proprietaire_=joueur; }
};

class Simple : public Pion {
 public:
  Simple(int numero):Pion(numero,1){}
  std::string type() override { return "Simple"; }
};

class Abbe : public Pion {
 public:
  Abbe(int numero): Pion(numero,1){}
  std::string type() override { return "Abbé"; }

};

class Grand : public Pion {

 public:
  Grand(int numero): Pion(numero,2){}
  std::string type() override { return "Grand"; }
};

#endif  // PION_H
