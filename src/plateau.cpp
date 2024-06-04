#include "plateau.h"

void Plateau::ajouter(Tuile& t, size_t ligne, size_t colonne){
    table_.at(ligne, colonne) = &t;

    //creation de nouvelles zones et gestion des zones déjà existantes
    creationZone(t);
    majZone(ligne, colonne);
}

bool Plateau::possedeVoisine(const Minituile& m) {
    for (unsigned int l = 0; l < taille; l++) {
        for (unsigned int c = 0; c < taille; c++) {
            Tuile* pt = table_.at(l, c);
            if (pt != nullptr && pt->possedeMinituile(m)) {
                Tuile* voisin = nullptr;
                int indice = pt->getIndiceMinituile(m);
                if (indice < 3 && indice > -1)
                    //voisin = tuile du haut
                    voisin = table_.at(l - 1, c);
                else if (indice < 6 && indice > 2)
                    //voisin = tuile de droite
                    voisin = table_.at(l, c + 1);
                else if (indice < 9 && indice > 5)
                    //voisin = tuile du bas
                    voisin = table_.at(l + 1, c);
                else if (indice < 12 && indice > 8)
                    //voisin = tuile de gauche
                    voisin = table_.at(l, c - 1);
                else if (indice == 12)
                    //minituile au centre de la tuile donc possede forcement des voisins
                    return true;
                return voisin != nullptr;
            }
        }
    }
    return false;
}

bool Plateau::estentoure(const Minituile& m) {
    for (unsigned int l = 0; l < taille; l++) {
        for (unsigned int c = 0; c < taille; c++) {
            Tuile* pt = table_.at(l, c);
            if (pt!=nullptr && pt->possedeMinituile(m)) {
                Tuile* voisin;
                if (table_.at(l - 1, c + 1)!= nullptr && table_.at(l - 1, c - 1)!= nullptr && table_.at(l - 1, c)!= nullptr && table_.at(l, c + 1) != nullptr && table_.at(l + 1, c+1)!= nullptr && table_.at(l + 1, c-1)!= nullptr && table_.at(l + 1, c)!= nullptr && table_.at(l, c - 1)!= nullptr) return true;
                else return false;
            }
        }
    }
    return false;
}



Plateau::~Plateau() {
    for (auto z : zones_) {
        delete z;
    }
}

Zone* Plateau::fabriqueZone(std::vector<Minituile*> minituiles,Plateau& p){
    Minituile* minituile = minituiles.front();
    Pre* pre = dynamic_cast<Pre*>(minituile);
    Ville* ville = dynamic_cast<Ville*>(minituile);
    Abbaye* abbe = dynamic_cast<Abbaye*>(minituile);
    Jardin* jardin = dynamic_cast<Jardin*>(minituile);
    Route* route = dynamic_cast<Route*>(minituile);
    if(pre != nullptr)
        return new Zone_Pre(minituiles,p);
    else if(ville != nullptr) return new Zone_Ville(minituiles,p);
    else if(abbe != nullptr) return new Zone_Abbe(minituiles,p);
    else if(route != nullptr) return new Zone_Route(minituiles,p);
    else if(jardin != nullptr) return new Zone_Abbe(minituiles,p);
    else return new Zone_non_evaluee(minituiles,p);

}

void Plateau::creationZone(Tuile& t) {
    for (const auto& liens : t.get_liens()) {
        std::vector<Minituile*> v;
        for (auto pos : liens) {
            v.push_back(&t.get_minituile(pos));
        }
        zones_.push_back(fabriqueZone(v,*this));
    }
}

void Plateau::majZone(unsigned int ligne, unsigned int colonne) {
    Tuile* pt = table_.at(ligne, colonne);

    Tuile* ptHaut = nullptr;
    Tuile* ptBas = nullptr;
    Tuile* ptDroit = nullptr;
    Tuile* ptGauche = nullptr;

    if (ligne != 0) {
        ptHaut = table_.at(ligne - 1, colonne);
    }
    if (ligne != table_.nb_lignes() - 1) {
        ptBas = table_.at(ligne + 1, colonne);
    }
    if (colonne != 0) {
        ptGauche = table_.at(ligne, colonne - 1);
    }
    if (colonne != table_.nb_colonnes() - 1) {
        ptDroit = table_.at(ligne, colonne + 1);
    }
    Zone* pz;
    Zone* pzVoisin = nullptr;

    if (ptHaut != nullptr) {
        for (unsigned int i = 0; i < 3; i++) {
            pz = pt->get_minituile(i).getZone();
            pzVoisin = ptHaut->get_minituile(8 - i).getZone();
            if (pz != pzVoisin) {
                *pz = *pz + *pzVoisin;
                suppressionZone(pzVoisin);
            }
        }
    }
    if (ptBas != nullptr) {
        for (unsigned int i = 0; i < 3; i++) {
            pz = pt->get_minituile(8 - i).getZone();
            pzVoisin = ptBas->get_minituile(i).getZone();
            if (pz != pzVoisin) {
                *pz = *pz + *pzVoisin;
                suppressionZone(pzVoisin);
            }
        }
    }
    if (ptDroit != nullptr) {
        for (unsigned int i = 0; i < 3; i++) {
            pz = pt->get_minituile(i + 3).getZone();
            pzVoisin = ptDroit->get_minituile(11 - i).getZone();
            if (pz != pzVoisin) {
                *pz = *pz + *pzVoisin;
                suppressionZone(pzVoisin);
            }
        }
    }
    if (ptGauche != nullptr) {
        for (unsigned int i = 0; i < 3; i++) {

            pz = pt->get_minituile(11 - i).getZone();
            pzVoisin = ptGauche->get_minituile(i + 3).getZone();
            if (pz != pzVoisin) {
                *pz = *pz + *pzVoisin;
                suppressionZone(pzVoisin);
            }
        }
    }
}

bool Plateau::compatibiliteGauche(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne) {
    if (colonne == 0) {
        return true;
    }
    Tuile* pt = table_.at(ligne, colonne - 1);
    if (pt != nullptr) {
        if (!(t.get_minituile(11) == pt->get_minituile(3) && t.get_minituile(10) == pt->get_minituile(4) && t.get_minituile(9) == pt->get_minituile(5)))
            return false;
    }
    return true;
}

bool Plateau::compatibiliteDroite(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne) {
    if (colonne == taille-1) {
        return true;
    }

    Tuile* pt = table_.at(ligne, colonne + 1);
    if (pt != nullptr) {
        if (!(t.get_minituile(3) == pt->get_minituile(11) && t.get_minituile(4) == pt->get_minituile(10) && t.get_minituile(5) == pt->get_minituile(9)))
            return false;
    }
    return true;

}

bool Plateau::compatibiliteBas(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne) {
    if (ligne == taille-1) {
        return true;
    }

    Tuile* pt = table_.at(ligne + 1, colonne);
    if (pt != nullptr) {
        if (!(t.get_minituile(6) == pt->get_minituile(2) && t.get_minituile(7) == pt->get_minituile(1) && t.get_minituile(8) == pt->get_minituile(0)))
            return false;
    }
    return true;
}

bool Plateau::compatibiliteHaut(const Tuile& t, const unsigned int& ligne, const unsigned int& colonne) {
    if (ligne == 0) {
        return true;
    }
    Tuile* pt = table_.at(ligne - 1, colonne);
    if (pt != nullptr) {
        if (!(t.get_minituile(0) == pt->get_minituile(8) && t.get_minituile(1) == pt->get_minituile(7) && t.get_minituile(2) == pt->get_minituile(6)))
            return false;
    }
    return true;
}

bool Plateau::compatibiliteRiviere(const Tuile &t, int ligne, int colonne){
    size_t cours_prolonge = 0;
    for(size_t n = 0; n < 13; n++){
        if(typeid(t.get_minituile(n)) == typeid(Riviere)){
            if(n >= 0 && n < 3){
                if(ligne-1 >= 0 && getTuile(ligne-1,colonne) == nullptr){
                    for(int k = -1; k < 2; k++){
                        if(colonne + k >= 0 && colonne + k < taille){
                            for(size_t m = 0; m < ligne; m++){
                                if(getTuile(m,colonne+k) != nullptr){
                                    return false;
                                }
                            }
                        }
                    }
                }
                else if (ligne == 0) return false;
                else cours_prolonge++;
            }
            if(n >= 3 && n < 6){
                if(colonne+1 < taille && getTuile(ligne,colonne+1) == nullptr){
                    for(int k = -1; k < 2; k++){
                        if(ligne + k >= 0 && ligne + k < taille){
                            for(size_t m = taille-1; m > colonne; m--){
                                if(getTuile(ligne+k,m) != nullptr){
                                    return false;
                                }
                            }
                        }
                    }
                }
                else if (colonne+1 == taille) return false;
                else cours_prolonge++;
            }
            if(n >= 6 && n < 9){
                if(ligne+1 < taille && getTuile(ligne+1,colonne) == nullptr){
                    for(int k = -1; k < 2; k++){
                        if(colonne + k >= 0 && colonne + k < taille){
                            for(size_t m = taille-1; m > ligne; m--){
                                if(getTuile(m,colonne+k) != nullptr){
                                    return false;
                                }
                            }
                        }
                    }
                }
                else if (ligne+1 == taille) return false;
                else cours_prolonge++;
            }
            if(n >= 9 && n < 12){
                if(colonne-1 >= 0 && getTuile(ligne,colonne-1) == nullptr){
                    for(int k = -1; k < 2; k++){
                        if(ligne + k >= 0 && ligne + k < taille){
                            for(size_t m = 0; m < colonne; m++){
                                if(getTuile(ligne+k,m) != nullptr){
                                    return false;
                                }
                            }
                        }
                    }
                }
                else if (colonne == 0) return false;
                else cours_prolonge++;
            }
        }
    }
    return cours_prolonge == 1;
}

bool Plateau::tuileCompatible(const Tuile &t, size_t l, size_t c) {
    bool gestionRiviere = true;
    if(typeid(t) == typeid(Tuile_Riviere)) gestionRiviere = compatibiliteRiviere(t,l,c);
    return compatibiliteBas(t, l, c) && compatibiliteDroite(t, l, c) && compatibiliteGauche(t, l, c) && compatibiliteHaut(t, l, c) && getTuile(l,c) == nullptr &&
            getTuilesAdjacentes(l,c).size() > 0 && gestionRiviere;
}

Tuile* Plateau::getTuile(size_t ligne, size_t colonne){
    Tuile* pt = table_.at(ligne, colonne);
    return pt;
}

void Plateau::suppressionZone(Zone* z){
    for(std::vector<Zone*>::iterator it = zones_.begin(); it != zones_.end(); it++){
        if(*it == z){
            zones_.erase(it);
            delete z;
            break;
        }
    }
    return;
}

void Plateau::afficher_zones() const{
    cout << "AFFICHAGE ZONES *******" << endl;
    cout << "nbre zones : " << zones_.size() << endl;
    int i = 1;
    for (auto z : zones_){
        cout << "zone " << i << " : ";
        z->affiche_zone();
        cout << endl;
        i++;
    }
}

Tuile* Plateau::getTuile(const Minituile& m) const{
    for(auto tuile : table_){
        if(tuile != nullptr && tuile->possedeMinituile(m)){
            return tuile;
        }
    }
    return nullptr;
}

std::list<Tuile*> Plateau::getTuilesAdjacentes(const Tuile& tuile) const{
    std::list<Tuile*> res;
    for(size_t ligne = 0; ligne < taille; ligne++){
        for(size_t colonne = 0; colonne < taille ; colonne ++){
            if(table_.at(ligne,colonne) == &tuile){
                //ligne du haut
                if(ligne > 0 && table_.at(ligne-1,colonne) != nullptr) res.push_back(table_.at(ligne-1,colonne));
                //ligne du bas
                if(ligne < taille-1 && table_.at(ligne+1,colonne) != nullptr) res.push_back(table_.at(ligne+1,colonne));
                //colonne de gauche
                if(colonne > 0 && table_.at(ligne,colonne-1) != nullptr) res.push_back(table_.at(ligne,colonne-1));
                //colonne de droite
                if(colonne < taille-1 && table_.at(ligne,colonne+1) != nullptr) res.push_back(table_.at(ligne,colonne+1));
            }
        }
    }
    return res;
}

std::list<Tuile*> Plateau::getTuilesAdjacentes(size_t ligne, size_t colonne) const{
    std::list<Tuile*> res;
    //ligne du haut
    if(ligne > 0 && table_.at(ligne-1,colonne) != nullptr) res.push_back(table_.at(ligne-1,colonne));
    //ligne du bas
    if(ligne < taille-1 && table_.at(ligne+1,colonne) != nullptr) res.push_back(table_.at(ligne+1,colonne));
    //colonne de gauche
    if(colonne > 0 && table_.at(ligne,colonne-1) != nullptr) res.push_back(table_.at(ligne,colonne-1));
    //colonne de droite
    if(colonne < taille-1 && table_.at(ligne,colonne+1) != nullptr) res.push_back(table_.at(ligne,colonne+1));
    return res;
}

size_t Plateau::getPosX(const Tuile& t) const {
    for(size_t ligne = 0; ligne < taille; ligne++){
        for(size_t colonne = 0; colonne < taille; colonne++){
            if(table_.at(ligne,colonne) == &t){
                return colonne;
            }
        }
    }
    return -1;
}

size_t Plateau::getPosY(const Tuile& t) const {
    for(size_t ligne = 0; ligne < taille; ligne++){
        for(size_t colonne = 0; colonne < taille; colonne++){
            if(table_.at(ligne,colonne) == &t){
                return ligne;
            }
        }
    }
    return -1;
}

bool Plateau::estPosable(Tuile& t){
    for(size_t i = 0; i <4; i++){
        for(size_t ligne = 0; ligne < taille; ligne++){
            for(size_t colonne = 0; colonne < taille; colonne++){
                if(tuileCompatible(t,ligne,colonne)) {
                    qDebug() << ligne << colonne;
                    for(size_t j = i; j < 4; j++) t.tourner();
                    return true;
                }
            }
        }
        t.tourner();
    }
    return false;
}


/*bool Plateau::checkminituilesadjacentes(Matrice<Tuile*, 143, 143> table_, size_t posx, size_t posy, size_t indice) {
bool check = false;
std::array<size_t,3> coord;
switch (indice)
{
    case 0: 
coord = getCoordMinituileHaut(table_,posx,posy,indice){}
if (type(minituile(coord)) == type(minituile(posx,poxy,indice) || coord=[1000,1000,1000]) check = true;
          break; 
    case 1: 
coord = getCoordMinituileHaut(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
           break;
    case 2: 
coord = getCoordMinituileHaut(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break; 
    case 3: 
coord = getCoordMinituileDroite(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break;
    case 4: 
coord = getCoordMinituileDroite(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break;
    case 5: 
coord = getCoordMinituileDroite(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break;
    case 6: 
coord = getCoordMinituileBas(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break; 
    case 7: 
coord = getCoordMinituilBas(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break;
    case 8: 
coord = getCoordMinituileBas(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break;
    case 9: 
coord = getCoordMinituileGauche(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break;
    case 10: 
coord = getCoordMinituileGauche(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break; 
    case 11: 
coord = getCoordMinituileGauche(table_,posx,posy,indice){}
if (table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][ndice] || coord=[1000,1000,1000]) check = true;
          break;
   default :
    break; 
}
return check;
}

bool Plateau::Tuilecompatible (const Tuile&, size_t posx, size_t posy){
    int compteur =0;
    bool check = false;
    if table_[posx][posy] != NULL {
        for (size_t i=0; i<12;i++){
            if (checkminituilesadjacentes(table_,posx,posy,i)) compteur++;
        }
        if (compteur==12) check = true;
    }
    return check;
}

//On retourne [1000,1000,1000] quand on arrive au bord du tableau ou alors minituile inexistante
std::array<size_t,3> Plateau::getCoordMinituileGauche(Matrice<Tuile*, 143, 143> table_, size_t posx, size_t posy, size_t indice){
std::array<size_t,3> coord;
if (indice ==11 ||indice ==10 ||indice ==9){
if (posx ==0 || table_[posx-1][posy] ==NULL) coord =[1000,1000,1000];
else coord = [posx-1,posy,(3+11-indice)];
}
else{
if (indice ==1 ||indice ==2 ||indice ==3) coord = [posx,posy,(indice-1)];
if (indice ==5 ||indice ==6 ||indice ==7||indice ==8) coord = [posx,posy,(indice+1)];
if (indice ==4) coord = [posx,posy,12];
}
return coord;
}

std::array<size_t,3> getCoordMinituileDroite(Matrice<Tuile*, 143, 143> table_, size_t posx, size_t posy, size_t indice){
std::array<size_t,3> coord;
if (indice ==3||indice ==4 ||indice ==5){
if (posx ==142|| table_[posx+1][posy] ==NULL) coord =[1000,1000,1000];
else coord = [posx+1,posy,(3+11-indice)];
}
else{
if (indice ==9 ||indice ==8 ||indice ==7||indice ==6) coord = [posx,posy,(indice-1)];
if (indice ==0 ||indice ==1 ||indice ==2) coord = [posx,posy,(indice+1)];
if (indice ==12) coord = [posx,posy,4];
if (indice ==10) coord = [posx,posy,12];
if (indice ==11) coord = [posx,posy,0];
}
return coord;
}

std::array<size_t,3> getCoordMinituileHaut(Matrice<Tuile*, 143, 143> table_, size_t posx, size_t posy, size_t indice){
std::array<size_t,3> coord;
if (indice ==0||indice ==1 ||indice ==2){
if (posy ==142|| table_[posx][posy+1] ==NULL) coord =[1000,1000,1000];
else coord = [posx,posy+1,(8-indice)];
}
else{
if (indice ==3 ||indice ==4 ||indice ==5||indice ==6) coord = [posx,posy,(indice-1)];
if (indice ==8 ||indice ==9 ||indice ==10) coord = [posx,posy,(indice+1)];
if (indice ==12) coord = [posx,posy,1];
if (indice ==7) coord = [posx,posy,12];
if (indice ==11) coord = [posx,posy,0];
}
return coord;
}

std::array<size_t,3> getCoordMinituileBas(Matrice<Tuile*, 143, 143> table_, size_t posx, size_t posy, size_t indice){
std::array<size_t,3> coord;
if (indice ==8||indice ==7 ||indice ==6){
if (posy ==0|| table_[posx][posy-1] ==NULL) coord =[1000,1000,1000];
else coord = [posx,posy-1,(8-indice)];
}
else{
if (indice ==3 ||indice ==4 ||indice ==5||indice ==2) coord = [posx,posy,(indice+1)];
if (indice ==11 ||indice ==9 ||indice ==10) coord = [posx,posy,(indice-1)];
if (indice ==0) coord = [posx,posy,11];
if (indice ==1) coord = [posx,posy,12];
if (indice ==12) coord = [posx,posy,7];
}
return coord;
}

std::vector<Minituile&> Plateau::returnzoneminituile (std::vector<Zone> zones_, Minituile& minituile){
std::vector<Minituile&> retourne;
for (auto& i : zones_)
{
    for (auto& y : zones[i]_)
{
    if (y==minituile) return zones[i];
}
}
return retourne;
}

std::vector<Zone&> Plateau::construirezone(std::vector<Zone> zones,Matrice<Tuile*, 143, 143> table_, size_t posx, size_t posy, size_t indice){
std::vector<Minituile&> zone;
creer 12 zones <=>  pushback 12 fois dans le vecteur zone renseigné
for (size_t i=0; i<=12;i++){
coord = getCoordMinituileGauche(table_,posx,posy,i);
    if coord != [1000,1000,1000]{
    if table_[coord[0]][coord[1]][coord[2]] == table_[posx][poxy][i]{
        if getZone(table_[coord[0]][coord[1]][coord[2]]) != NULL {
            getZone(table_[coord[0]][coord[1]][coord[2]]) = getZone(table_[coord[0]][coord[1]][coord[2]]) + table_[posx][poxy][i];
            lier la minituile à la zone de coord
            supprimer la zone de posx posy i
        }
    }
    }
        
        
    si type différent stop
    si vide stop
    
coord = getCoordMinituileDroite(table_,posx,posy,i){}
coord = getCoordMinituileHaut(table_,posx,posy,i){}
coord = getCoordMinituileBas(table_,posx,posy,i){}
}

return zone;
}

ostream& operator<<(ostream& os, const Plateau& plat) {
		plat.print(os);
		return os;
	}*/
