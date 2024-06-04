#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include <vector>
#include <string>

#include "Tuilewindow.h"
#include "partie.h"
#include "joueur.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void creerPartie();
    bool unTour();
    void tourJoueur(Joueur& j);
    bool piocheVide(){return partie_->piochevide();}
    void piocher();
    void finPartie();
    int getnbJoueurs(){return partie_->getnbJoueurs();}
    Joueur& getJoueur(int numero){return partie_->getJoueur(numero);}


public slots:
    void on_pivoter_clicked();
    void on_repiocher_clicked();
    void on_vuePlateau_tuileClicked(VueTuile* vueTuile);
    void on_bouton_suivant_clicked();
    void on_vuePlateau_miniTuileClicked(VueTuile* t,VueMinituile* m);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Partie> partie_=nullptr;
    QStandardItemModel tableau_joueurs {0, 3};
    Tuile* tuile_courante_ = nullptr;
    int num_joueur_courant_ = 0;
    VueTuile* derniere_vue_tuile_ = nullptr;
    bool tuile_posee_=false;
    bool pion_pose_=false;
    bool pioche_vide_=false;
    QWidget* fenetre_fin;
    void majNbrePionTableau();
    void majScoreTableau();
    void recuperationAutoPion();
    void majScoreInterne(Zone* z);

};
#endif // MAINWINDOW_H
