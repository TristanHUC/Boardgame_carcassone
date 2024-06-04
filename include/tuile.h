#ifndef TUILE_H
#define TUILE_H

#include <array>
#include <memory>
#include <vector>
#include <iostream>
#include <QString>
#include <string>

#include "minituile.h"

using namespace std;
class Tuile {
    /*Chaque minituile est identifiée par une position comprise entre 0 et 12

    Chaque vecteur à l'intérieur du vecteur liens contient les positions des
    minituiles liées*/

    /*
    Représentation d'une tuile en fonction des indices de minituiles:

      0  1  2
    11       3
    10   12  4
    9        5
      8  7  6
    */

    std::array<std::unique_ptr<Minituile>, 13> minituiles;
    std::vector<std::vector<unsigned int>> liens;
    bool blason;
    std::string chemin;
    int rotation {0};

public:
    virtual void foo() = 0;  // fonction virtuelle pour rendre la classe abstraite

    Tuile(const std::array<std::string, 13>& minituiles_, const std::vector<std::vector<unsigned int>>& liens, bool blason_ = false, const std::string& c=""); //ajout chemin
    Tuile(const Tuile&) = delete;
    Tuile& operator=(const Tuile&) = delete;

    Minituile& get_minituile(unsigned int pos) { return *(minituiles[pos]); }
    const Minituile& get_minituile(unsigned int pos) const { return *(minituiles[pos]); }
    std::vector<unsigned int> get_liens(unsigned int pos) const;
    std::vector<std::vector<unsigned int>> get_liens() const { return liens; }
    /*renvoie toutes les positions (comprises entre 0 et
    12) des minituiles liés à celle dont la position est
    passée en paramètre*/
    void tourner();        // tourne la tuile dans le sens horaire
    void reset();
    int getRotations() const {
        return rotation;
    }

    //bool est_placable(const Plateau& p, const int pos_x, const int pos_y) const;
    bool possede_blason() const { return blason; }
    bool possedeMinituile(const Minituile& m);//renvoie si la tuile possède une minituile en particulier
    int getIndiceMinituile(const Minituile& m);
    void affiche() const;
    const auto& getChemin() const {return chemin;}

    //methode qui renvoie la liste des minituiles voisines à une tuile(utile pour le score dans zone pre)
    std::list<Minituile*> getMinituilesVoisines(const Minituile&) const;
};

class Tuile_Base : public Tuile {
public:
    Tuile_Base(const std::array<std::string, 13>& minituiles_, const std::vector<std::vector<unsigned int>>& liens_, bool blason_ = false, const std::string& c="") :
        Tuile(minituiles_, liens_, blason_, c) {}
    Tuile_Base(const Tuile_Base&) = delete;
    Tuile_Base& operator=(const Tuile_Base&) = delete;

    void foo() override {}  // cette classe n'est plus abstraite
};

class Tuile_Riviere : public Tuile {
public:
    Tuile_Riviere(const std::array<std::string, 13>& minituiles_, const std::vector<std::vector<unsigned int>>& liens_, bool blason_ = false, const std::string& c="") :
        Tuile(minituiles_, liens_, blason_, c) {}
    Tuile_Riviere(const Tuile_Riviere&) = delete;
    Tuile_Riviere& operator=(const Tuile_Riviere&) = delete;

    void foo() override {}  // cette classe n'est plus abstraite
};

class Tuile_Auberge : public Tuile {
    // les tuiles auberges et cathédrales possèdent 2 nouveaux symboles (de même
    // nom que l'extension) avec des effets
    bool auberge;
    bool cathedrale;

public:
    Tuile_Auberge(const std::array<std::string, 13>& minituiles_, const std::vector<std::vector<unsigned int>>& liens_, bool blason_ = false, bool auberge_ = false, bool cathedrale_ = false,const std::string& c="") :
        Tuile(minituiles_, liens_, blason_,c), auberge(auberge_), cathedrale(cathedrale_) {}

    Tuile_Auberge(const Tuile_Auberge&) = delete;
    Tuile_Auberge& operator=(const Tuile_Auberge&) = delete;

    void foo() override {}  // cette classe n'est plus abstraite

    bool possede_auberge() const { return auberge; }
    bool possede_cathedrale() const { return cathedrale; }
};

class Tuile_Abbe : public Tuile {
public:
    Tuile_Abbe(const std::array<std::string, 13>& minituiles_, const std::vector<std::vector<unsigned int>>& liens_, bool blason_ = false,const std::string& c="") :
        Tuile(minituiles_, liens_, blason_,c) {}
    Tuile_Abbe(const Tuile_Abbe&) = delete;
    Tuile_Abbe& operator=(const Tuile_Abbe&) = delete;

    void foo() override {}  // cette classe n'est plus abstraite
};

void affiche_liens(std::vector<unsigned int>);

#endif  // TUILE_H
