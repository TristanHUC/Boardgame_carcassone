#include "partie.h"
#include <vector>

Partie::Partie(std::vector<std::string> noms_joueurs, std::list<std::unique_ptr<Extension>> liste_extension) : extensions_ {std::move(liste_extension)}{

    int nb_pions=0;
    for(std::size_t i=0; i<noms_joueurs.size(); i++){
        std::vector<std::unique_ptr<Pion>> pions;
        for(auto& e : extensions_){
            auto pions_extension = e->creerPions();
            pions.insert(pions.end(), std::make_move_iterator(pions_extension.begin()), std::make_move_iterator(pions_extension.end()));
        }
        nb_pions=pions.size();
        for(size_t j=0;j<pions.size();j++){
               pions[j]->setNumero(j);
        }
        //remplir le vecteur de joueur avec ces infos
        joueurs_.emplace_back(noms_joueurs[i],i,nb_pions, std::move(pions));
    }
    for(auto& joueur:joueurs_){
            joueur.setProprietairePion();
    }
    nb_pions_partie_=nb_pions;
    nb_joueurs_=noms_joueurs.size();

    //prendre les tuiles des extensions présentes dans la list et les mélanger : OK
    //remplir la pioche : OK
    //générer plateau  : OK (se genere via les constructeurs par defaut)

    //remplissage de la pioche
    std::vector<Tuile*> pioche;
    for(auto& e : extensions_){
        for(auto t : e->getTuiles()){
            pioche.push_back(t);
        }
    }
    pioche = melange(pioche);
    pioche_.ajouterTuiles(pioche);

    qDebug() << "Nombre de tuiles dans la pioche au début de la partie" << pioche_.nbTuiles();

}

std::vector<Tuile*> Partie::melange(std::vector<Tuile*>& tuiles) { //TESTEE : CA MARCHE avec des int a la place des Tuiles*

    std::vector<Tuile*> newtuiles;
    int i;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);

    //on ajoute d'abord à la pioche toutes le tuiles de l'extension riviere
    if(possedeExtensionRiviere()){
        std::vector<Tuile*> pioche_riviere = getAllTuileRiviere(tuiles);

        //la premiere tuile à placer est la tuile source
        newtuiles.push_back(getTuileRiviereSource(pioche_riviere));
        Tuile* tuile_lac = getTuileRiviereLac(pioche_riviere);

        //placement au hasard du reste des tuiles rivieres
        while (!pioche_riviere.empty()) {

            i = generator() % pioche_riviere.size();
            newtuiles.push_back(pioche_riviere[i]);

            pioche_riviere.erase(pioche_riviere.begin() + i);

        }
        //la derniere tuile Riviere à placer est la tuile lac
        newtuiles.push_back(tuile_lac);

    }
    //si on ne joue pas avec l'extension riviere alors la premiere tuile à placer est la tuile base0
    else{
        newtuiles.push_back(getTuileBase0(tuiles));
    }

    //placement au hasard des autres tuiles (hors extension riviere)
    while (!tuiles.empty()) {

        i = generator() % tuiles.size();
        newtuiles.push_back((tuiles)[i]);

        tuiles.erase(tuiles.begin() + i);

    }

    for (size_t j = 0; j < newtuiles.size(); j++) {
            tuiles.push_back(newtuiles[j]);
    }
    return tuiles;
}

bool Partie::possedeExtensionRiviere() const{
    for(std::list<std::unique_ptr<Extension>>::const_iterator it = extensions_.cbegin(); it != extensions_.cend(); it++){

        if(typeid(*(it->get())) == typeid(Rivieres)){
            return true;
        }
    }
    return false;
}

Tuile_Riviere* Partie::getTuileRiviereSource(std::vector<Tuile*>& tuiles) const{
    for(std::vector<Tuile*>::const_iterator it = tuiles.cbegin(); it != tuiles.cend(); it++){
        if((*it)->getChemin().find("Riviere_source") != string::npos){
            Tuile_Riviere* temp = dynamic_cast<Tuile_Riviere*>(*it);
            tuiles.erase(it);
            return temp;
        }
    }
    return nullptr;
}

Tuile_Riviere* Partie::getTuileRiviereLac(std::vector<Tuile*>& tuiles) const{
    for(std::vector<Tuile*>::const_iterator it = tuiles.cbegin(); it != tuiles.cend(); it++){
        if((*it)->getChemin().find("Riviere_lac") != string::npos){
            Tuile_Riviere* temp = dynamic_cast<Tuile_Riviere*>(*it);
            tuiles.erase(it);
            return temp;
        }
    }
    return nullptr;
}

Tuile_Base* Partie::getTuileBase0(std::vector<Tuile*>& tuiles) const{
    for(std::vector<Tuile*>::const_iterator it = tuiles.cbegin(); it != tuiles.cend(); it++){
        if((*it)->getChemin().find("Base0") != string::npos){
            Tuile_Base* temp = dynamic_cast<Tuile_Base*>(*it);
            tuiles.erase(it);
            return temp;
        }
    }
}

std::vector<Tuile*> Partie::getAllTuileRiviere(std::vector<Tuile*>& tuiles) const{
    std::vector<Tuile*> res;
    for(std::vector<Tuile*>::const_iterator it = tuiles.cbegin(); it != tuiles.cend(); it++){
        if((*it)->getChemin().find("Riviere") != string::npos){
            res.push_back(*it);
            tuiles.erase(it);
            it--;
        }
    }
    return res;
}

