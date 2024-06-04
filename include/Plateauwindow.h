#ifndef PLATEAUWINDOW_H
#define PLATEAUWINDOW_H

#include <QWidget>
#include "plateau.h"
#include "tuile.h"
#include <vector>
#include "Tuilewindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

class VuePlateau : public QWidget{

    Q_OBJECT

public:
    explicit VuePlateau(QWidget * parent = nullptr);

    VueTuile* getCenter(){return vue_tuiles.at(Plateau::getTaille()/2, Plateau::getTaille()/2);} // For test, TODO change using plateau size
    void poserTuile(Tuile& tuile, int ligne, int colonne);
    VueTuile* getVueTuile(size_t l, size_t c) {return vue_tuiles.at(l,c);}

private:
    Matrice<VueTuile*, Plateau::getTaille(), Plateau::getTaille()> vue_tuiles;
    QGridLayout* layoutTuiles;

signals:
    void tuileClicked(VueTuile*);
    void miniTuileClicked(VueTuile*,VueMinituile*);

public slots:
    void onTuileClicked(VueTuile* vueTuile) {
        emit this->tuileClicked(vueTuile);
    }
    void onMiniTuileClicked(VueTuile* t, VueMinituile* m){
        //qDebug() << "VP onMiniTuileClicked";
        emit miniTuileClicked(t,m);
    }
};

#endif // PLATEAUWINDOW_H
