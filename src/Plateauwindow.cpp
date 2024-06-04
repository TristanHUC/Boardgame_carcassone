#include "Plateauwindow.h"
#include "Tuilewindow.h"

#include <QMessageBox>


VuePlateau::VuePlateau(QWidget *parent) : QWidget(parent){

    layoutTuiles = new QGridLayout;
    for(int ligne = 0; ligne < vue_tuiles.nb_lignes() ; ++ligne){
        for (int colonne = 0; colonne < vue_tuiles.nb_colonnes(); ++colonne){

            // Attention x = colonne y = ligne
            auto vue = new VueTuile(colonne, ligne);

            vue_tuiles.at(ligne, colonne) = vue;
            layoutTuiles->addWidget(vue, ligne, colonne);

            connect(vue, SIGNAL(tuileClicked(VueTuile*)), this, SLOT(onTuileClicked(VueTuile*)));
            connect(vue, SIGNAL(miniTuileClicked(VueTuile*, VueMinituile*)), this, SLOT(onMiniTuileClicked(VueTuile*, VueMinituile*)));
        }
    }

    layoutTuiles->setSpacing(0);
    this->setLayout(layoutTuiles);
    this->adjustSize();
}

void VuePlateau::poserTuile(Tuile& tuile, int ligne, int colonne) {
    vue_tuiles.at(ligne, colonne)->setTuile(tuile);
}

/*void VuePlateau::affichage_tuile(VueTuile* tu){


    if (P.tuileCompatible(*(this->t),tu->getposY(),tu->getposX())){
        P.ajouter(*(this->t),tu->getposY(),tu->getposX());
        tu->SetTuile(*(this->t),*(this->j));
        //this->setEnabled(false); // on peut pas faire ca car ca desactive aussi les boutons internes
        for(int i =0;i<13;i++){

            tu->vue_minituile[i]->setEnabled(true);
            tu->vue_minituile[i]->setMini(tu->getT().get_minituile(i));

        }
        if ((typeid(tu->vue_minituile[12]->getMini()) != typeid(Jardin)) || (typeid(tu->vue_minituile[12]->getMini())!=typeid(Abbaye))){
            tu->vue_minituile[12]->setEnabled(false);

        }
    }
    else{
        QMessageBox::warning(this, "Impossible de poser la tuile ici", "La tuile n'est pas compatible avec ses voisines");
    }
}*/
