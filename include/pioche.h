#ifndef PIOCHE_H
#define PIOCHE_H

#include <queue>
#include <memory>
#include <list>
#include <queue>

#include "tuile.h"
#include "extension.h"

class Pioche
{
public:

    void ajouterTuiles(const std::vector<Tuile*>& tuiles) {
        // Mettre ces tuiles qui sont deja mélangé dans la pioche
        for (auto tuile: tuiles) {
            pioche_.push(tuile);
        }
    }


    Tuile* piocher(){
        Tuile* t=pioche_.front();
        pioche_.pop();
        return t;
    } //renvoi la tuile du début et l'enleve de la pioche

    void reposer(Tuile* tuile){
        pioche_.push(tuile);
    }

    bool vide(){
        return pioche_.empty();
    }

    decltype(auto) nbTuiles() const {
        return pioche_.size();
    }
    size_t getTaille() const {return pioche_.size();}

    
private :
    std::queue<Tuile*> pioche_;
};



#endif // PIOCHE_H
