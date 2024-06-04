#include "minituile.h"
#include "zone.h"
#include "plateau.h"

#include <iostream>
#include <vector>

bool Minituile::poserPion(Joueur& joueur) {
  // Si il y a deja un pion
  if (pion_ != nullptr) {
    std::cout << "Il y a deja un pion sur la minituile\n";
    return false;
  }
  // verification des pions de la zone pour savoir si on peut poser

  auto pion_zone = zone_->pionsOccupants();
  // s'il y a des pions pas possible de poser
  if (pion_zone.size() == 0 && joueur.getNbPion()!=0 && zone_->estFermee()==false) {
    return doPoserPion(joueur);
  } else {
      return false;
  }
}

void Minituile::recupererPion(Joueur& joueur) {
  // Si il n'y a pas de un pion
  if (pion_ == nullptr) {
    std::cout << "Il n'y a pas de pion sur la minituile\n";
    return;
  }
  doRecupererPion(joueur);
}

bool Minituile::estFermee() {
	//renvoie true si la minituile possède une minituile voisine sur une Tuile différente que la sienne
	//cette methode doit être redéfinie pour la tuile abbé et jardin (concept différent de fermeture)
	return getZone()->getPlateau()->possedeVoisine(*this);
}

bool Abbaye::estFermee() {
	//renvoie true si la minituile possède une minituile voisine sur une Tuile différente que la sienne
	//cette methode doit être redéfinie pour la tuile abbé et jardin (concept différent de fermeture)
	return getZone()->getPlateau()->estentoure(*this);
}
bool Jardin::estFermee() {
	//renvoie true si la minituile possède une minituile voisine sur une Tuile différente que la sienne
	//cette methode doit être redéfinie pour la tuile abbé et jardin (concept différent de fermeture)
	return getZone()->getPlateau()->estentoure(*this);
}

//PRE
bool Pre::doPoserPion(Joueur& joueur){
    pion_ = joueur.selectionnerPion();
    if (pion_!=nullptr && typeid(*pion_) == typeid(Abbe)){
        joueur.recupererUnPion(std::move(pion_));
        pion_ = nullptr;
    }
    return pion_ != nullptr;
}

void Pre::doRecupererPion(Joueur& joueur){
  // pour recup un pion: pion abbe
  if (pion_->type() == "A") {
    joueur.recupererUnPion(std::move(pion_));
    // mettre pion à nullptr
    pion_ = nullptr;
  }
}

//VILLE

bool Ville::doPoserPion(Joueur& joueur){
  pion_ = joueur.selectionnerPion();
  if (pion_!=nullptr && typeid(*pion_) == typeid(Abbe)){
      joueur.recupererUnPion(std::move(pion_));
      pion_ = nullptr;
  }
  return pion_ != nullptr;
}
void Ville::doRecupererPion(Joueur& joueur){
  // pour recup un pion: zone fermée ou pion abbe
  if (getZone()->estFermee() == true || typeid(*pion_) == typeid(Abbe)) {
    joueur.recupererUnPion(std::move(pion_));
    // mettre pion à nullptr
    pion_ = nullptr;
  }
}

//ROUTE
bool Route::doPoserPion(Joueur& joueur){
  pion_ = joueur.selectionnerPion();
  if (pion_!=nullptr && typeid(*pion_) == typeid(Abbe) ){
      joueur.recupererUnPion(std::move(pion_));
      pion_ = nullptr;
  }
  return pion_ != nullptr;
}
void Route::doRecupererPion(Joueur& joueur){
  // pour recup un pion: zone fermée ou pion abbe
  if (getZone()->estFermee() == true || typeid(*pion_) == typeid(Abbe)) {
      joueur.recupererUnPion(std::move(pion_));
      // mettre pion à nullptr
      pion_ = nullptr;
  }
}


//JARDIN
bool Jardin::doPoserPion(Joueur& joueur) {
  pion_ = joueur.selectionnerPion();
  return pion_ != nullptr;
}

//que abbe dedans?
void Jardin::doRecupererPion(Joueur& joueur) {
  // pour recup un pion: zone fermée ou pion abbe
  if (getZone()->estFermee() == true || typeid(*pion_) == typeid(Abbe)) {
      joueur.recupererUnPion(std::move(pion_));
      // mettre pion à nullptr
      pion_ = nullptr;
  }
}

//ABBAYE
bool Abbaye::doPoserPion(Joueur& joueur) {
  pion_ = joueur.selectionnerPion();
  return pion_ != nullptr;
}
void Abbaye::doRecupererPion(Joueur& joueur){
  // pour recup un pion: zone fermée ou pion abbe
  if (getZone()->estFermee() == true || typeid(*pion_) == typeid(Abbe)) {
      joueur.recupererUnPion(std::move(pion_));
      // mettre pion à nullptr
      pion_ = nullptr;
  }
}

bool Riviere::doPoserPion(Joueur& joueur) {
    return false;
}

void Riviere::doRecupererPion(Joueur& joueur)  {
    throw std::runtime_error("This should never happend");
}

Minituile* FabriqueMinituile::methodeFabrication(const std::string s) {
	if (s == "pre") {
		return (new Pre());
	}
	else if (s == "ville") {
		return new Ville();
	}
	else if (s == "route") {
		return new Route();
	}
	else if (s == "riviere") {
		return new Riviere();
	}
	else if (s == "abbaye") {
		return new Abbaye();
	}
	else if (s == "jardin") {
		return new Jardin();
	}
	else {
		return nullptr;
	}
}

bool operator==(const Minituile& lhs, const Minituile& rhs) {
	return (typeid(lhs) == typeid(rhs));
}
