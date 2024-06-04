


#include "Tuilewindow.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "minituile.h"
#include <typeinfo>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QTransform>

VueTuile::VueTuile(size_t x, size_t y, QWidget *parent) : QPushButton(parent), posX{x}, posY{y}{

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFixedSize(126,126);
    setEnabled(true);
    connect(this, SIGNAL(clicked()), this, SLOT(clickedEvent()));

    //découpage en 9 QPushButton pour poser un pion:

    layoutgrille = new QGridLayout;


    int k=0;
    for(int i=0;i<5;i++){
        for (int j =0;j<5;j++){

            les_boutons_n.push_back(new VueMinituile);
            layoutgrille->addWidget(les_boutons_n[k],i,j);
            les_boutons_n[k]->setFixedHeight(14);
            les_boutons_n[k]->setFixedWidth(14);
            les_boutons_n[k]->setEnabled(false);//bouton inactif
            les_boutons_n[k]->setFlat(true); //bouton invisible
            connect(les_boutons_n[k], SIGNAL(MiniTuileClicked(VueMinituile*)), this, SLOT(onMiniTuileClicked(VueMinituile*)));

            k++;
        }
    }
    vue_minituile.push_back(les_boutons_n[1]->setNum(0));  //0
    vue_minituile.push_back(les_boutons_n[2]->setNum(1));  //1
    vue_minituile.push_back(les_boutons_n[3]->setNum(2));  //2

    vue_minituile.push_back(les_boutons_n[9]->setNum(3));  //3
    vue_minituile.push_back(les_boutons_n[14]->setNum(4));  //4
    vue_minituile.push_back(les_boutons_n[19]->setNum(5));  //5

    vue_minituile.push_back(les_boutons_n[23]->setNum(6));  //6
    vue_minituile.push_back(les_boutons_n[22]->setNum(7));  //7
    vue_minituile.push_back(les_boutons_n[21]->setNum(8));  //8

    vue_minituile.push_back(les_boutons_n[15]->setNum(9));  //9
    vue_minituile.push_back(les_boutons_n[10]->setNum(10));  //10
    vue_minituile.push_back(les_boutons_n[5]->setNum(11));  //11

    vue_minituile.push_back(les_boutons_n[12]->setNum(12));  //12


    for(int i=0;i<13;i++){
        vue_minituile[i]->setText("P");

    }

    layoutgrille->setSpacing(20);
    this->setLayout(layoutgrille);

}


VueMinituile::VueMinituile(QWidget* parent):QPushButton(parent){

    connect(this,SIGNAL(clicked()),this,SLOT(clickedEvent()));

}
