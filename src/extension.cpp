#include "extension.h"

#include <QDebug>

std::unique_ptr<Tuile> Extension::fabriqueTuile(
    const std::array<string, 13>& m,
    const std::vector<std::vector<unsigned int>>& l,
    const string& chemin_,
    bool blason,
    bool auberge,
    bool cathedrale) const {  // ajout de String chemin_
  const auto& type = typeid(*this);

  if (type == typeid(Base))
    return std::make_unique<Tuile_Base>(m, l, blason, chemin_);  // ajout � chaque fois
  if (type == typeid(Rivieres))
    return std::make_unique<Tuile_Riviere>(m, l, blason, chemin_);
  if (type == typeid(Auberges))
    return std::make_unique<Tuile_Auberge>(m, l, blason, auberge, cathedrale, chemin_);
  if (type == typeid(Abbes))
    return std::make_unique<Tuile_Abbe>(m, l, blason, chemin_);
  // if (type == typeid(Paysans))
  //  return new Tuile_Paysan(m, l, blason, chemin_);
  return nullptr;
}

void Extension::initialisation(const string& nom_fichier) {
  QFile f(QString::fromStdString(nom_fichier));

  string temp;

    if(f.open(QIODevice::ReadOnly)){
        QTextStream contenu(&f);
        while (!contenu.atEnd()) {
            temp = contenu.readLine().toStdString();

            string tab_minituile;
            string tab_liens;
            string chemin;
            string booleen;// ajout

            size_t indice_fin_minituiles;
            size_t indice_fin_liens;  // ajout
            size_t indice_fin_chemin;
            size_t indice_fin_booleen;

            // isolement des deux donnees presentes dans une ligne
            indice_fin_minituiles = temp.find(")");
            indice_fin_liens = temp.find(")",indice_fin_minituiles + 1);
            indice_fin_chemin = temp.find(")",indice_fin_liens + 1);
            indice_fin_booleen = temp.find(")",indice_fin_chemin + 1);


            //on recupere les elements interessants grace aux indices :
            tab_minituile = temp.substr(1,indice_fin_minituiles - 1);
            tab_liens = temp.substr(indice_fin_minituiles + 2, indice_fin_liens - indice_fin_minituiles - 2);
            chemin = temp.substr(indice_fin_liens + 2, indice_fin_chemin - indice_fin_liens - 2);
            booleen = temp.substr(indice_fin_chemin + 2, indice_fin_booleen - indice_fin_chemin - 2);

            std::array<string, 13> minituiles;
            size_t m = 0;

            // insertion dans l'array minituiles les differentes informations presentes
            // dans tab_minituile
            for (string::iterator it = tab_minituile.begin(); it != tab_minituile.end();
               it++) {
            if (*it != ',') {
              minituiles[m].push_back(*it);
            } else {
              m += 1;
            }
            }

            std::vector<std::vector<unsigned int>> liens;
            std::vector<unsigned int> v;

            string nombre;

            // insertion dans l'array lien les informations de la ligne
            for (string::iterator it = tab_liens.begin(); it != tab_liens.end(); it++) {
                if (*it <= '9' && *it >= '0') {
                  nombre.push_back(*it);
                }
                else {
                  v.push_back(stoi(nombre));
                  nombre = "";
                  if (*it != ',') {
                    liens.push_back(v);
                    v.clear();
                  }
                }
            }
            v.push_back(stoi(nombre));
            liens.push_back(v);

            //gestion des booleens
            bool blason = false;
            bool auberge = false;
            bool cathedrale = false;

            if(booleen.find("blason") != string::npos){
              blason = true;
            }
            if(booleen.find("auberge") != string::npos){
              auberge = true;
            }
            if(booleen.find("cathedrale") != string::npos){
              cathedrale = true;
            }

            auto tuile = fabriqueTuile(minituiles, liens, chemin, blason, auberge, cathedrale);  // ajout

            if (tuile == nullptr) {
                cout << "ERREUR pointeur de tuile null" << endl;
                return;
            }
            tuiles.push_back(std::move(tuile));
        }
    } else {
        throw std::runtime_error("Impossible d'ouvrir le fichier de tuiles");
    }
}

//EXTENSION

std::vector<std::unique_ptr<Pion>> Extension::creerPions(){
    return doCreerPions();
}

// BASE

std::vector<std::unique_ptr<Pion>> Base::doCreerPions() {
  std::vector<std::unique_ptr<Pion>> vect_pions;
  for (int i = 0; i < 7; i++) {
    auto p = make_unique<Simple>(Simple(0));
    vect_pions.push_back(std::move(p));
  }
  return vect_pions;
}

//RIVIERE

//AUBERGES

std::vector<std::unique_ptr<Pion>> Auberges::doCreerPions() {
  std::vector<std::unique_ptr<Pion>> vect_pions;
    auto p = make_unique<Grand>(Grand(0));
    vect_pions.push_back(std::move(p));
  return vect_pions;
}

//ABBES

std::vector<std::unique_ptr<Pion>> Abbes::doCreerPions() {
  std::vector<std::unique_ptr<Pion>> vect_pions;
    auto p = make_unique<Abbe>(Abbe(0));
    vect_pions.push_back(std::move(p));
  return vect_pions;
}

//PAYSAN


/*Base::Base() {
  int k = 0;
  std::array<std::string, 13> tableau;
  std::vector<std::vector<unsigned int>> lien;

  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";
  tableau[3] = "pre";
  tableau[4] = "pre";
  tableau[5] = "pre";
  tableau[6] = "ville";
  tableau[7] = "ville";
  tableau[8] = "ville";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "ville";

  lien = {{0, 1, 2, 3, 4, 5}, {6, 7, 8, 9, 10, 11, 12}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, true);
    k++;
  }
  lien.clear();

  tableau[1] = "route";
  tableau[4] = "route";
  tableau[6] = "pre";
  tableau[7] = "route";
  tableau[8] = "pre";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "route";

  lien = {{1, 8}, {2, 3}, {5, 6}, {9, 10, 11}, {1, 4, 7, 12}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }
  lien.clear();

  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";
  tableau[3] = "ville";
  tableau[4] = "ville";
  tableau[5] = "ville";
  tableau[6] = "pre";
  tableau[7] = "pre";
  tableau[8] = "pre";
  tableau[12] = "pre";

  lien = {{0, 1, 2, 12, 6, 7, 8}, {3, 4, 5}, {9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "route";
  tableau[7] = "route";
  tableau[10] = "route";
  tableau[12] = "route";

  tableau[4] = "route";
  tableau[3] = "pre";
  tableau[5] = "pre";
  tableau[9] = "pre";
  tableau[11] = "pre";

  lien = {{1, 4, 7, 10, 12}, {0, 11}, {2, 3}, {5, 6}, {8, 9}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[7] = "pre";
  tableau[10] = "pre";
  lien = {{2, 3}, {1, 4, 12}, {0, 5, 6, 7, 8, 9, 10, 11}};

  for (int i = 0; i < 9; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "pre";
  tableau[10] = "route";
  lien = {{11, 0, 1, 2, 3}, {10, 4, 12}, {5, 6, 7, 8, 9}};

  for (int i = 0; i < 8; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "route";
  tableau[7] = "route";
  tableau[10] = "pre";
  lien = {{8, 9, 10, 11, 0}, {2, 3}, {5, 6}, {1, 4, 7, 12}};

  for (int i = 0; i < 4; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }
  lien.clear();
  tableau[0] = "ville";
  tableau[1] = "ville";
  tableau[2] = "ville";
  tableau[3] = "ville";
  tableau[4] = "ville";
  tableau[5] = "ville";
  tableau[6] = "ville";
  tableau[7] = "ville";
  tableau[8] = "ville";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "ville";

  lien = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}};
  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, true);
    k++;
  }
  lien.clear();

  tableau[3] = "pre";
  tableau[4] = "pre";
  tableau[5] = "pre";

  lien = {{0, 1, 2, 6, 7, 8, 9, 10, 11, 12}, {3, 4, 5}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, true);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }
  lien.clear();

  tableau[4] = "route";

  lien = {{0, 1, 2, 6, 7, 8, 9, 10, 11, 12}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, true);
    k++;
  }
  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }
  lien.clear();
  tableau[3] = "pre";
  tableau[4] = "pre";
  tableau[5] = "pre";
  tableau[6] = "pre";
  tableau[7] = "pre";
  tableau[8] = "pre";
  tableau[12] = "pre";

  lien = {{0, 1, 2}, {3, 4, 5, 6, 7, 8, 12}, {9, 10, 11}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";

  lien = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 12}, {9, 10, 11}};

  for (int i = 0; i < 5; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "route";
  tableau[7] = "route";
  tableau[12] = "route";

  lien = {{0, 8}, {2, 3, 4, 5, 6}, {1, 7, 12}, {9, 10, 11}};

  for (int i = 0; i < 4; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[7] = "pre";
  tableau[4] = "route";

  lien = {{2, 3}, {0, 8, 7, 5, 6}, {1, 4, 12}, {9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[7] = "route";
  tableau[1] = "pre";

  lien = {{5, 6}, {0, 8, 1, 2, 3}, {7, 4, 12}, {9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }

  lien.clear();

  tableau[0] = "ville";
  tableau[1] = "ville";
  tableau[2] = "ville";
  tableau[4] = "pre";
  tableau[5] = "pre";
  tableau[6] = "ville";
  tableau[7] = "ville";
  tableau[8] = "ville";
  tableau[9] = "pre";
  tableau[10] = "pre";
  tableau[11] = "pre";
  tableau[12] = "ville";

  lien = {{0, 1, 2, 6, 7, 8, 12}, {3, 4, 5}, {9, 10, 11}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, true);
    k++;
  }
  lien.clear();

  tableau[0] = "pre";
  tableau[1] = "route";
  tableau[2] = "pre";
  tableau[4] = "route";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "route";

  lien = {{0, 5}, {1, 4, 12}, {2, 3}, {6, 7, 8, 9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, false);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Base(tableau, lien, true);
    k++;
  }
}

Rivieres::Rivieres()  // CONSTRUCTION DE TOUTES LES TUILES DE L'EXTENSION
                      // CORRESPONDANTE
{
  std::array<std::string, 13> tableau;
  std::vector<std::vector<unsigned int>> lien;

  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";
  tableau[3] = "pre";
  tableau[4] = "riviere";
  tableau[5] = "pre";
  tableau[6] = "pre";
  tableau[7] = "pre";
  tableau[8] = "pre";
  tableau[9] = "pre";
  tableau[10] = "pre";
  tableau[11] = "pre";
  tableau[12] = "riviere";

  lien = {{0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11}};
  k = 0;
  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }
  lien.clear();
  tableau[10] = "riviere";
  lien = {{0, 1, 2, 3, 11}, {5, 6, 7, 8, 9}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[10] = "pre";
  tableau[1] = "riviere";
  lien = {{2, 3}, {0, 5, 6, 7, 8, 9, 10, 11}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }
  lien.clear();
  tableau[1] = "route";
  tableau[12] = "route";
  tableau[7] = "route";
  tableau[10] = "riviere";
  lien = {{11, 1}, {5, 6}, {8, 9}, {2, 3}, {1, 7, 12}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[0] = "ville";
  tableau[1] = "ville";
  tableau[2] = "ville";
  tableau[6] = "ville";
  tableau[7] = "ville";
  tableau[8] = "ville";
  tableau[12] = "riviere";
  lien = {{0, 1, 2}, {6, 7, 8}, {11, 3}, {9, 5}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }
  lien.clear();
  tableau[0] = "pre";
  tableau[1] = "riviere";
  tableau[2] = "pre";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  lien = {{0, 5}, {6, 7, 8, 9, 10, 11}, {2, 3}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "route";
  tableau[9] = "pre";
  tableau[10] = "riviere";
  tableau[11] = "pre";
  tableau[12] = "route";
  lien = {{1, 12}, {11, 0}, {2, 3}, {6, 7, 8}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "riviere";
  tableau[6] = "pre";
  tableau[7] = "route";
  tableau[8] = "pre";
  tableau[10] = "route";
  tableau[12] = "route";
  lien = {{11, 0, 6, 5}, {2, 3}, {8, 9}, {7, 12, 10}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "pre";
  tableau[4] = "pre";
  tableau[7] = "pre";
  tableau[10] = "riviere";
  tableau[12] = "riviere";
  lien = {{11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Riviere(tableau, lien, false);
    k++;
  }
}

Auberges::Auberges()  // CONSTRUCTION DE TOUTES LES TUILES ET PIONS DE
                      // L'EXTENSION CORRESPONDANTE
{
  int k = 0;
  for (int i = 0; i < 6; i++) {
    tuiles[k] = new Tuile_Auberge(PARAMETRE);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Auberge(PARAMETRE);
    k++;
  }
  for (int i = 0; i < 10; i++) {
    tuiles[k] = new Tuile_Auberge(PARAMETRE);
    k++;
  }
  for (c in couleur) {
    for (int m = 0; m < nombre de grand meeple; m++) {
      Grands[m] = new grand(parametre);
    }
  }
}

Abbes::Abbes()  // CONSTRUCTION DE TOUTES LES TUILES ET PIONS DE L'EXTENSION
                // CORRESPONDANTE
{
  int k = 0;
  std::array<std::string, 13> tableau;
  std::vector<std::vector<unsigned int>> lien;

  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";
  tableau[3] = "pre";
  tableau[4] = "pre";
  tableau[5] = "pre";
  tableau[6] = "pre";
  tableau[7] = "pre";
  tableau[8] = "pre";
  tableau[9] = "pre";
  tableau[10] = "pre";
  tableau[11] = "pre";
  tableau[12] = "abbaye";

  lien = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};

  for (int i = 0; i < 4; i++) {
    tuiles[k] = new Tuile_Abbe(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[4] = "route";

  lien = {{0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Abbe(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "riviere";
  tableau[7] = "riviere";

  lien = {{11, 10, 9, 0, 8}, {2, 3}, {5, 6}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Abbe(tableau, lien, false);
    k++;
  }

  for (c in couleur) {
    for (int m = 0; m < 1; m++) {
      Abbes[m] = new Abbe(parametre);
    }
  }
}

Paysans::Paysans() {  // CONSTRUCTION DE TOUTES LES TUILES ET PIONS DE
                      // L'EXTENSION CORRESPONDANTE
  for (int i = 0; i < 7; i++) {
    Pions_Paysans[i] = Simple(i);
  }
  int k = 0;
  std::array<std::string, 13> tableau;
  std::vector<std::vector<unsigned int>> lien;

  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";
  tableau[3] = "pre";
  tableau[4] = "pre";
  tableau[5] = "pre";
  tableau[6] = "ville";
  tableau[7] = "ville";
  tableau[8] = "ville";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "ville";

  lien = {{0, 1, 2, 3, 4, 5}, {6, 7, 8, 9, 10, 11, 12}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, true);
    k++;
  }
  lien.clear();

  tableau[1] = "route";
  tableau[4] = "route";
  tableau[6] = "pre";
  tableau[7] = "route";
  tableau[8] = "pre";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "route";

  lien = {{1, 8}, {2, 3}, {5, 6}, {9, 10, 11}, {1, 4, 7, 12}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }
  lien.clear();

  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";
  tableau[3] = "ville";
  tableau[4] = "ville";
  tableau[5] = "ville";
  tableau[6] = "pre";
  tableau[7] = "pre";
  tableau[8] = "pre";
  tableau[12] = "pre";

  lien = {{0, 1, 2, 12, 6, 7, 8}, {3, 4, 5}, {9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "route";
  tableau[7] = "route";
  tableau[10] = "route";
  tableau[12] = "route";

  tableau[4] = "route";
  tableau[3] = "pre";
  tableau[5] = "pre";
  tableau[9] = "pre";
  tableau[11] = "pre";

  lien = {{1, 4, 7, 10, 12}, {0, 11}, {2, 3}, {5, 6}, {8, 9}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[7] = "pre";
  tableau[10] = "pre";
  lien = {{2, 3}, {1, 4, 12}, {0, 5, 6, 7, 8, 9, 10, 11}};

  for (int i = 0; i < 9; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "pre";
  tableau[10] = "route";
  lien = {{11, 0, 1, 2, 3}, {10, 4, 12}, {5, 6, 7, 8, 9}};

  for (int i = 0; i < 8; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "route";
  tableau[7] = "route";
  tableau[10] = "pre";
  lien = {{8, 9, 10, 11, 0}, {2, 3}, {5, 6}, {1, 4, 7, 12}};

  for (int i = 0; i < 4; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }
  lien.clear();
  tableau[0] = "ville";
  tableau[1] = "ville";
  tableau[2] = "ville";
  tableau[3] = "ville";
  tableau[4] = "ville";
  tableau[5] = "ville";
  tableau[6] = "ville";
  tableau[7] = "ville";
  tableau[8] = "ville";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "ville";

  lien = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}};
  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, true);
    k++;
  }
  lien.clear();

  tableau[3] = "pre";
  tableau[4] = "pre";
  tableau[5] = "pre";

  lien = {{0, 1, 2, 6, 7, 8, 9, 10, 11, 12}, {3, 4, 5}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, true);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }
  lien.clear();

  tableau[4] = "route";

  lien = {{0, 1, 2, 6, 7, 8, 9, 10, 11, 12}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, true);
    k++;
  }
  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }
  lien.clear();
  tableau[3] = "pre";
  tableau[4] = "pre";
  tableau[5] = "pre";
  tableau[6] = "pre";
  tableau[7] = "pre";
  tableau[8] = "pre";
  tableau[12] = "pre";

  lien = {{0, 1, 2}, {3, 4, 5, 6, 7, 8, 12}, {9, 10, 11}};

  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[0] = "pre";
  tableau[1] = "pre";
  tableau[2] = "pre";

  lien = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 12}, {9, 10, 11}};

  for (int i = 0; i < 5; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[1] = "route";
  tableau[7] = "route";
  tableau[12] = "route";

  lien = {{0, 8}, {2, 3, 4, 5, 6}, {1, 7, 12}, {9, 10, 11}};

  for (int i = 0; i < 4; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[7] = "pre";
  tableau[4] = "route";

  lien = {{2, 3}, {0, 8, 7, 5, 6}, {1, 4, 12}, {9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();
  tableau[7] = "route";
  tableau[1] = "pre";

  lien = {{5, 6}, {0, 8, 1, 2, 3}, {7, 4, 12}, {9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }

  lien.clear();

  tableau[0] = "ville";
  tableau[1] = "ville";
  tableau[2] = "ville";
  tableau[4] = "pre";
  tableau[5] = "pre";
  tableau[6] = "ville";
  tableau[7] = "ville";
  tableau[8] = "ville";
  tableau[9] = "pre";
  tableau[10] = "pre";
  tableau[11] = "pre";
  tableau[12] = "ville";

  lien = {{0, 1, 2, 6, 7, 8, 12}, {3, 4, 5}, {9, 10, 11}};

  for (int i = 0; i < 1; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, true);
    k++;
  }
  lien.clear();

  tableau[0] = "pre";
  tableau[1] = "route";
  tableau[2] = "pre";
  tableau[4] = "route";
  tableau[9] = "ville";
  tableau[10] = "ville";
  tableau[11] = "ville";
  tableau[12] = "route";

  lien = {{0, 5}, {1, 4, 12}, {2, 3}, {6, 7, 8, 9, 10, 11}};

  for (int i = 0; i < 3; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, false);
    k++;
  }
  for (int i = 0; i < 2; i++) {
    tuiles[k] = new Tuile_Paysan(tableau, lien, true);
    k++;
  }
}
*/
