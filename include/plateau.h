#ifndef PLATEAU_H
#define PLATEAU_H

#include <memory>
#include <vector>

#include "matrice.hpp"
#include "tuile.h"
#include "zone.h"
#include "minituile.h"

class Plateau {
private:
    static const size_t taille = 25;
 std::vector<Zone*> zones_;
 Matrice<Tuile*, taille, taille> table_;

 //methodes utile pour tester la placabilité d'une tuile :
 bool compatibiliteGauche(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne);
 bool compatibiliteDroite(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne);
 bool compatibiliteBas(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne);
 bool compatibiliteHaut(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne);
 bool compatibiliteRiviere(const Tuile &t, int ligne, int colonne);

 //appelé après l'ajout d'une tuile sur le plateau
 void creationZone(Tuile& t);
 void majZone(unsigned int ligne, unsigned int colonne);

 //methode appelee lors de la maj d'une zone
 void suppressionZone(Zone* z);

 //methode appelee pour fabriquer le bon type de tuile
 Zone* fabriqueZone(std::vector<Minituile*> minituiles, Plateau &p);


public:
    Plateau() {
        for (auto& cell: table_) {
            cell = nullptr;
        }
    };
    ~Plateau();
    //Plateau& operator=(const Plateau&) = delete;
    Plateau(const Plateau& p) = delete;
    std::vector<Zone*> getZones() const { return zones_; }
    Matrice<Tuile*, taille, taille> getTable() const { return table_; }

    void ajouter(Tuile&, size_t ligne, size_t colonne); //plutot pointeur non ?
    bool tuileCompatible(const Tuile&, size_t ligne, size_t colonne);  // prend en compte le cas position déjà occupée

    bool a_tuile(std::size_t ligne, std::size_t colonne) const {
    return table_.at(ligne, colonne) != nullptr;
    }
    bool estentoure(const Minituile& m); //indique si la tuile est entourée
    bool possedeVoisine(const Minituile& m);
    constexpr static size_t getTaille() { return taille; }
    Tuile* getTuile(size_t ligne, size_t colonne);

    //renvoie la tuile qui possede cette minituile
    Tuile* getTuile(const Minituile& m) const;

    size_t getPosX(const Tuile&) const;
    size_t getPosY(const Tuile&) const;

    //methode qui retourne au plus 4 tuiles adjacentes à une tuile (haut,bas,gauche,droite)
    std::list<Tuile*> getTuilesAdjacentes(const Tuile& t) const;
    std::list<Tuile*> getTuilesAdjacentes(size_t ligne, size_t colonne) const;

    //methode qui indique si une tuile est posable ou non (dans n'importe quel sens de rotation)
    bool estPosable(Tuile &t);


    //methode de test
    void afficher_zones() const;

//peut etre à supprimer : 
/*std::vector<Zone&> construirezone(std::vector<Zone&> zones, Matrice<Tuile*, 143, 143> table_,
                                         size_t posx, size_t posy,
                                         size_t indice);

bool checkminituilesadjacentes(Matrice<Tuile*, 143, 143> table_, size_t posx,size_t posy, size_t indice);

std::array<size_t,3> getCoordMinituileGauche(Matrice<Tuile*, 143, 143> table_,size_t posx, size_t posy, size_t indice);
std::array<size_t,3> getCoordMinituileDroite(Matrice<Tuile*, 143, 143> table_,
                                    size_t posx, size_t posy, size_t indice);
std::array<size_t,3> getCoordMinituileHaut(Matrice<Tuile*, 143, 143> table_, size_t posx,
                                  size_t posy, size_t indice);
std::array<size_t,3> getCoordMinituileBas(Matrice<Tuile*, 143, 143> table_, size_t posx,
                                 size_t posy, size_t indice);
  // retourne les coordonnées des minituiles adjacentes (pratique pour parcourir
  // les minituiles)
  
  void print(ostream& os = cout) const;
  */

};

ostream& operator<<(ostream&, const Plateau&);
#endif  // PLATEAU_H
