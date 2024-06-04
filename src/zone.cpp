#include "zone.h"
#include "plateau.h"


Zone& Zone::operator+(const Zone& z){

    std::vector<Minituile*> v = z.getminituiles();


    //pour chaque minituile de la zone à additionner on l'ajoute à l'autre zone
    for (std::vector<Minituile*>::iterator it = v.begin(); it != v.end(); it++) {
        *this = *this + **it;
    }
    //maj du nbre de minituile dans la zone
    nbminituiles = minituiles_.size();

    //une fois que l'addition est faite il faut supprimer la zone en parametre du plateau :
    //a gerer depuis le plateau ou la partie

    return *this;
    
    
    /*for (auto i : z.getminituiles())
    {
        if (*std::find(this->begin(), this->end(), i) == i) {
              //Présent dans l'autre zone
        } 
        else {//Pas présent
            this->pushback(i);
        }
        (Zone*).~vector();
    }
    return this;*/
}

Zone& Zone::operator+(Minituile& m_) {
    for (auto m : getminituiles()) {
        if (m == &m_) {
            return *this;
        }
    }
    minituiles_.push_back(&m_);
    m_.setZone(*this);
    return *this;
}

  

bool Zone::estFermee() const {
    for (std::vector<Minituile*>::const_iterator it = minituiles_.cbegin(); it != minituiles_.cend(); it++) {
        if (!(*it)->estFermee())
            return false;
    }
    return true;
}

std::vector<Pion*> Zone::pionsOccupants()
{
    std::vector<Pion*> p;
    for (const auto& minituile: getminituiles()) {
        if (minituile->getPion() != nullptr) {
            p.push_back(minituile->getPion());
        }
    }
    return p;
}

void Zone::affiche_zone() const {
    for(auto m : getminituiles()){
        std::cout << m << " / ";
    }
    std::cout << std::endl;
}

std::list<Tuile*> Zone::getTuile() const {
    std::list<Tuile*> res;
    for(auto minituile : minituiles_){
        for(auto tuile : plateau->getTable()){
            if(tuile != nullptr && tuile->possedeMinituile(*minituile)){
                res.push_back(tuile);
            }
        }
    }
    res.unique();
    return res;
}

size_t Zone_Ville::getScore() const{
    bool cathedrale = false;
    size_t score = 0;
    for(auto tuile : getTuile()){
        if(tuile->possede_blason()){
            score += 2;
        }
        Tuile_Auberge* tuile_cathedrale = dynamic_cast<Tuile_Auberge*>(tuile);
        if(tuile_cathedrale != nullptr && cathedrale == false){
            cathedrale = tuile_cathedrale->possede_cathedrale();
        }
        score += 2;
    }
    if(cathedrale == false){
        return score;
    }
    else if(estFermee()){
        return (score /2)*3 ;
    }
    else return 0;
}

size_t Zone_Pre::getScore() const{
    std::list<Zone_Ville*> liste_ville_fermees_adjacentes;
    for(auto minituile : this->getminituiles()){
        for(auto minituiles_adjacentes : getPlateau()->getTuile(*minituile)->getMinituilesVoisines(*minituile)){
            Zone_Ville* z = dynamic_cast<Zone_Ville*>(minituiles_adjacentes->getZone());
            if(z != nullptr && z->estFermee()){
                liste_ville_fermees_adjacentes.push_back(z);
            }
        }
    }
    liste_ville_fermees_adjacentes.unique();
    return (liste_ville_fermees_adjacentes.size())*3;
}

size_t Zone_Route::getScore() const {
    bool auberge = false;
    size_t score = 0;
    for(auto tuile : getTuile()){
        Tuile_Auberge* tuile_auberge = dynamic_cast<Tuile_Auberge*>(tuile);
        if(tuile_auberge != nullptr && auberge == false){
            auberge = tuile_auberge->possede_auberge();
        }
        score += 1;
    }
    if(auberge == false){
        return score;
    }
    else if(estFermee()){
        return score*2 ;
    }
    else return 0;

}

size_t Zone_Abbe::getScore() const{
    size_t score = 0;
    if(getnbminituiles() != 1){
        return 0;
    }
    Tuile* tuile = getPlateau()->getTuile(*getminituiles().front());
    size_t ligne = getPlateau()->getPosY(*tuile);
    size_t colonne = getPlateau()->getPosX(*tuile);

    for(int i = -1; i <2; i++){
        for(int j = -1; j<2; j++){
            if(ligne+i >= 0 && ligne +i < Plateau::getTaille() && colonne+j >= 0 && colonne + j < Plateau::getTaille() && getPlateau()->getTable().at(ligne+i,colonne+j)!=nullptr){
                score+=1;
            }
        }
    }
    return score;
}
