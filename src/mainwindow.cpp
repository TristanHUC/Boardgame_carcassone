#include "mainwindow.h"
#include "windowextensions.h"
#include "windowplayer.h"
#include "./ui_mainwindow.h"

#include "ui_windowextensions.h"
#include "ui_windowplayer.h"
#include "extension.h"
#include "SpinBoxDelegate.hpp"
#include "Plateauwindow.h"

#include <QTransform>
#include <QScrollBar>
#include <QBrush>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TODO reglage scrollbar
}

void MainWindow::creerPartie(){
    windowextensions w;
    auto res = w.exec();
    if (res != QDialog::Accepted) {
        throw "???"; // TODO treat non acceptation
    }
    const auto nb_joueur = w.getUi()->QSpinBox_nb_joueurs->value();
    qDebug() << "Extension window accepted. Number of players is " << nb_joueur;

    windowplayer wp {nb_joueur};
    res = wp.exec();
    if (res != QDialog::Accepted) {
        throw "???"; // TODO treat non acceptation
    }


   std::vector<std::string> noms;
   noms.push_back(wp.getUi()->QLineEdit_joueur1->text().toStdString());
   noms.push_back(wp.getUi()->QLineEdit_joueur2->text().toStdString());
   if(nb_joueur >= 3){
       noms.push_back(wp.getUi()->QLineEdit_joueur3->text().toStdString());
   }
   if(nb_joueur >= 4){
       noms.push_back(wp.getUi()->QLineEdit_joueur4->text().toStdString());
   }

    std::list<std::unique_ptr<Extension>> extensions;
    extensions.push_back(std::make_unique<Base>());

    if (w.getUi()->QCheckBox_abbes->isChecked()) {
        extensions.push_back(std::make_unique<Abbes>());
    }
    if (w.getUi()->QCheckBox_riviere->isChecked()) {
        extensions.push_back(std::make_unique<Rivieres>());
    }
    if (w.getUi()->QcheckBox_paysan->isChecked()) {
        extensions.push_back(std::make_unique<Paysans>());
    }
    if (w.getUi()->QcheckBox_auberges->isChecked()) {
        extensions.push_back(std::make_unique<Auberges>());
    }

    // Creation de la partie
    partie_ = std::make_unique<Partie>(noms, std::move(extensions));

    //affichage informations joueurs
    tableau_joueurs.setHeaderData( 0, Qt::Horizontal, "Nom" );
    tableau_joueurs.setHeaderData( 1, Qt::Horizontal, "Pion" );
    tableau_joueurs.setHeaderData( 2, Qt::Horizontal, "Score" );

    ui->tableView->setItemDelegateForColumn(2, new SpinBoxDelegate);

    for (const auto& nom: noms) {
        auto name = new QStandardItem(QString::fromStdString(nom));
        name->setEditable(false);
        auto pions = new QStandardItem(QString("%1").arg(partie_->getnbPionsPartie()));
        pions->setEditable(false);
        auto score = new QStandardItem(QString("%1").arg(0));

        QList<QStandardItem*> row;
        row << name << pions << score;

        tableau_joueurs.appendRow(row);
    }

    tableau_joueurs.setHeaderData( 0, Qt::Vertical, "Joueur 1" );
    tableau_joueurs.item(0,0)->setBackground(QBrush(Qt::darkRed));
    tableau_joueurs.setHeaderData( 1, Qt::Vertical, "Joueur 2" );
    tableau_joueurs.item(1,0)->setBackground(QBrush(Qt::blue));
    if(nb_joueur >= 3){
        tableau_joueurs.setHeaderData( 2, Qt::Vertical, "Joueur 3" );
        tableau_joueurs.item(2,0)->setBackground(QBrush(Qt::green));
    }
    if(nb_joueur >= 4){
        tableau_joueurs.setHeaderData( 3, Qt::Vertical, "Joueur 4" );
        tableau_joueurs.item(3,0)->setBackground(QBrush(Qt::yellow));
    }
    ui->tableView->setModel(&tableau_joueurs);


    // placer la premiere tuile
    tuile_courante_ = partie_->piocher();

    VueTuile* vue_centre = ui->vuePlateau->getCenter();
    partie_->getPlateau().ajouter(*tuile_courante_, vue_centre->getposY(), vue_centre->getposX());
    vue_centre->setTuile(*tuile_courante_);
    for(auto& m:vue_centre->vue_minituile){
        m->setText(" ");
    }
    piocher();

}

void MainWindow::on_bouton_suivant_clicked(){
    if(partie_->piochevide()==true){

        if(derniere_vue_tuile_!=nullptr){
            for(auto& m:derniere_vue_tuile_->vue_minituile){
                m->setEnabled(false);
            }
        }
        //maj des scores
        finPartie();
        qDebug() << "Fin de la partie";

    }
    else if(tuile_posee_==true){
        //init
        tuile_posee_=false;
        pion_pose_=false;
        ui->repiocher->setEnabled(true);
        ui->pivoter->setEnabled(true);
        //update du surlignage
        if(num_joueur_courant_==0)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::red));
        }
        else if(num_joueur_courant_==1)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::blue));
        }
        else if(num_joueur_courant_==2)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::green));
        }
        else if(num_joueur_courant_==3)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::yellow));
        }
        //joueur suivant
        ++num_joueur_courant_;
        num_joueur_courant_ %= partie_->getnbJoueurs();
        if(num_joueur_courant_==0)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::darkRed));
        }
        else if(num_joueur_courant_==1)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::darkBlue));
        }
        else if(num_joueur_courant_==2)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::darkGreen));
        }
        else if(num_joueur_courant_==3)
        {
            tableau_joueurs.item(num_joueur_courant_,0)->setBackground(QBrush(Qt::darkYellow));
        }
        qDebug() << "Tour du joueur " << QString::fromStdString(getJoueur(num_joueur_courant_).getName());
        recuperationAutoPion();

        if(derniere_vue_tuile_!=nullptr){
            const auto& tuile = partie_->getPlateau().getTuile(derniere_vue_tuile_->getposY(), derniere_vue_tuile_->getposX());
            for (int i = 0; i < 13; ++i) {
                if (!tuile->get_minituile(i).possedePion() || typeid (*(tuile->get_minituile(i).getPion())) != typeid(Abbe)) {
                    derniere_vue_tuile_->vue_minituile[i]->setEnabled(false);
                }
                if(!tuile->get_minituile(i).possedePion()){
                    derniere_vue_tuile_->vue_minituile[i]->setText(" ");
                }
            }
        }
        piocher();
    }else{
        QMessageBox::warning(this, "Attention", "Vous devez poser une tuile");
        return;

    }
}

void MainWindow::piocher() {
    if (!partie_->piochevide()) {
        tuile_courante_ = partie_->piocher();
        QImage image(QString::fromStdString(tuile_courante_->getChemin()));
        ui->label_pioche->setPixmap(QPixmap::fromImage(image));
        ui->repiocher->setText(QString("repiocher ")+QString::number(partie_->getTaillePioche()));
    }else{
        tuile_courante_=nullptr;
        pioche_vide_=true;
    }
}

void MainWindow::on_repiocher_clicked() {
    qDebug() << "Repiocher";
    if (tuile_courante_ != nullptr) {
        if(tuile_posee_==true){
            QMessageBox::warning(this, "Attention", "Vous avez deja posé une tuile ce tour, pas possible de repiocher");
            ui->repiocher->setEnabled(false);
            return;
        }
        else if(partie_->getPlateau().estPosable(*tuile_courante_)){
            QMessageBox::warning(this, "Attention", "La tuile actuelle est posable sur le plateau");
            ui->repiocher->setEnabled(false);
            return;
        }
    }
    piocher();
}

void MainWindow::on_vuePlateau_tuileClicked(VueTuile* vueTuile) {
    //qDebug() << "clique tuile";
    if (tuile_courante_ != nullptr && vueTuile->getaTuile()==false) {
        if(tuile_posee_==false){
            if(!partie_->getPlateau().tuileCompatible(*tuile_courante_, vueTuile->getposY(), vueTuile->getposX())) {
                QMessageBox::warning(this, "Attention", "La tuile ne peut pas être posée à cet emplacement");
                return;
            }

            vueTuile->setTuile(*tuile_courante_);
            partie_->getPlateau().ajouter(*tuile_courante_, vueTuile->getposY(), vueTuile->getposX());
            for(auto& m:vueTuile->vue_minituile){
                m->setEnabled(true);
            }

            // Si pas abbaye ou jardin, désactiver bouton central
            auto& mt = tuile_courante_->get_minituile(12);
            if (typeid(mt) != typeid(Abbaye) && typeid(mt) != typeid(Jardin)) {
                vueTuile->vue_minituile[12]->setEnabled(false);
            }

            // Vérifier si l'extension paysans est activée
            bool paysans = false;
            for (auto& extension: partie_->getExtensions()) {
                if (typeid(*extension) == typeid(Paysans)) {
                    paysans = true;
                }
            }
            // Si non, désactiver les boutons des minituiles paysans
            if (!paysans) {
                for (int i = 0; i < 12; ++i) {
                    if (typeid(tuile_courante_->get_minituile(i)) == typeid(Pre)) {
                        vueTuile->vue_minituile[i]->setEnabled(false);
                    }
                }
            }

            derniere_vue_tuile_=vueTuile;
            tuile_posee_=true;
        }else{
            QMessageBox::warning(this, "Attention", "Vous avez deja posé une tuile ce tour");
            return;
        }
    }
    if(partie_->piochevide()==true){
        QMessageBox::warning(this, "Attention", "Dernier tour");
        return;
    }
}

void MainWindow::on_vuePlateau_miniTuileClicked(VueTuile* t,VueMinituile* m){

    Tuile* tuile = partie_->getPlateau().getTuile(t->getposY(),t->getposX());
    int num_mtuile = m->getNum();
    Minituile& mini_tuile = tuile->get_minituile(num_mtuile);

    if (pion_pose_) {
        QMessageBox::warning(this, "Attention", "Vous avez deja effectué une action");
        return;
    }

    if (mini_tuile.possedePion() && typeid(*mini_tuile.getPion()) == typeid(Abbe)) {
        if (mini_tuile.getPion()->getProprietaire() != &partie_->getJoueur(num_joueur_courant_)) {
            QMessageBox::warning(this, "Attention", "Impossible de récupérer le pion d'un autre joueur");
            return;
        }
        majScoreInterne(mini_tuile.getZone());
        mini_tuile.recupererPion(partie_->getJoueur(num_joueur_courant_));
        m->setText("P");
        auto pal = m->palette();
        pal.setColor(QPalette::Button, QColorConstants::Transparent);
        m->setPalette(pal);
        m->setEnabled(false);
        majNbrePionTableau();
        pion_pose_=true;
        QMessageBox::information(this, "Info", "Pion récupéré !");
        return;
    }

    bool reussi = mini_tuile.poserPion(getJoueur(num_joueur_courant_));
    QColor color;
    if(reussi==true){
        switch(num_joueur_courant_) {
        case 0:
            color = QColorConstants::Red;
            break;
        case 1:
            color = QColorConstants::Blue;
            break;
        case 2:
            color = QColorConstants::Green;
            break;
        case 3:
            color = QColorConstants::Yellow;
            break;
        }
        auto pal = m->palette();
        pal.setColor(QPalette::Button, color);
        m->setAutoFillBackground(true);
        m->setPalette(pal);
        //maj texte du bouton
        auto pion=mini_tuile.getPion();
        if(typeid(*pion)==typeid(Simple)){
            m->setText("S");
        }
        else if(typeid(*pion)==typeid(Abbe)){
            m->setText("A");
        }
        else if(typeid(*pion)==typeid(Grand)){
            m->setText("G");
        }
        m->update();

        //maj du nb de pions dans le tableau de joueur
        majNbrePionTableau();
        pion_pose_=true;
    } else {
        QMessageBox::warning(this, "Attention", "Impossible de poser le pion");
    }
}

void MainWindow::on_pivoter_clicked() {
    qDebug() << "Pivoter";
    if (tuile_courante_ != nullptr) {
        if(tuile_posee_==false){
            tuile_courante_->tourner();
            auto pixmap = ui->label_pioche->pixmap();
            QTransform tr;
            tr.rotate(90);
            pixmap = pixmap.transformed(tr);
            ui->label_pioche->setPixmap(pixmap);
        }else{
            QMessageBox::warning(this, "Attention", "Vous ne pouvez pas tourner cette tuile");
            ui->pivoter->setEnabled(false);
            return;
        }
    }
}

void MainWindow::finPartie(){

        int max=-1;
        vector<string> vainqueurs;

        //maj score:
        for(auto zone : partie_->getPlateau().getZones()){
            majScoreInterne(zone);
        }



         for(int i=0;i<getnbJoueurs();i++){
            if(partie_->getJoueur(i).getScore()>max){
                vainqueurs.clear();
                max=partie_->getJoueur(i).getScore();
                vainqueurs.push_back(partie_->getJoueur(i).getName());
                //qDebug() << "test";
            }
            else{                                            //en cas de plusieurs vainqueurs (ex aequo)
                if (partie_->getJoueur(i).getScore()==max){
                    vainqueurs.push_back(partie_->getJoueur(i).getName());
                    //qDebug() << "test3";
                }
            }

        }
        QString message("La partie est finie ! Les vainqueur.e.s : ");
        QString message2;
        for (size_t i=0;i<vainqueurs.size();i++){
            message2=(QString::fromStdString(vainqueurs[i]));
            message = message + message2;
            message2 = " ";
            message = message + message2;
            //qDebug() << "test2";
        }
        QMessageBox::information(this,"Fin de la Partie",message);

}

void MainWindow::majNbrePionTableau(){
    for(int i=0;i<partie_->getnbJoueurs();i++){
        int nb_pion_update= getJoueur(i).getNbPion();
        QString s = QString::number(nb_pion_update);
        tableau_joueurs.item(i,1)->setText(s);
    }
}

void MainWindow::majScoreTableau(){
    for(int i=0;i<partie_->getnbJoueurs();i++){
        size_t score_update = getJoueur(i).getScore();
        QString s = QString::number(score_update);
        tableau_joueurs.item(i,2)->setText(s);
    }
}

void MainWindow::recuperationAutoPion(){
    std::vector<Pion*> pion_j1;
    std::vector<Pion*> pion_j2;
    std::vector<Pion*> pion_j3;
    std::vector<Pion*> pion_j4;
    //si la zone est complète il faut renvoyer les meeple de la zone dans la main du joueur
    for(auto zone : partie_->getPlateau().getZones()){
        if(zone->estFermee()){
            majScoreInterne(zone);
            for(auto minituile : zone->getminituiles()){
                if(minituile->possedePion()){
                    Tuile* tuile = partie_->getPlateau().getTuile(*minituile);
                    size_t ligne = partie_->getPlateau().getPosY(*tuile);
                    size_t colonne = partie_->getPlateau().getPosX(*tuile);

                    //on met à jour la couleur du bouton sur le plateau
                    VueMinituile* vue_minituile = ui->vuePlateau->getVueTuile(ligne,colonne)->vue_minituile[tuile->getIndiceMinituile(*minituile)];
                    auto pal = vue_minituile->palette();
                    pal.setColor(QPalette::Button, QColorConstants::Transparent);
                    vue_minituile->setPalette(pal);

                    //on redonne le pion au joueur
                    Joueur* j = minituile->getPion()->getProprietaire();
                    minituile->recupererPion(*j);

                    //on met a jour le tableau des joueurs avec le nouveau nbre
                    majNbrePionTableau();
                    //affichage des pions recup
                    if(j->getNumero()==0){
                        pion_j1.push_back(minituile->getPion());
                    }
                    else if(j->getNumero()==1){
                        pion_j2.push_back(minituile->getPion());
                    }
                    else if(j->getNumero()==2){
                        pion_j3.push_back(minituile->getPion());
                    }
                    else if(j->getNumero()==3){
                        pion_j4.push_back(minituile->getPion());
                    }
                }
            }
        }
    }
    if(pion_j1.size()!=0 || pion_j2.size()!=0 || pion_j3.size()!=0 || pion_j4.size()!=0){
        QString message;
        for(int i=0;i<partie_->getnbJoueurs();i++){
            message+="Joueur ";
            message+=QString::number(i+1);
            message+=" : ";
            if(i==0){
                message+=QString::number(pion_j1.size());
            }
            else if(i==1){
                message+=QString::number(pion_j2.size());
            }
            else if(i==2){
                message+=QString::number(pion_j3.size());
            }
            else if(i==3){
                message+=QString::number(pion_j4.size());
            }
            message+=" ";
        }
        QMessageBox::information(this,"Pions récupérés",message);
    }
}

void MainWindow::majScoreInterne(Zone* z){
    //choix du ou des joueurs a qui attribuer les points
    size_t poids_joueur1 = 0;
    size_t poids_joueur2 = 0;
    size_t poids_joueur3 = 0;
    size_t poids_joueur4 = 0;
    for(auto pion : z->pionsOccupants()){
        if(pion->getProprietaire()->getNumero() == 0){
            poids_joueur1 += pion->getPoids();
        }
        if(pion->getProprietaire()->getNumero() == 1){
            poids_joueur2 += pion->getPoids();
        }
        if(pion->getProprietaire()->getNumero() == 2){
            poids_joueur3 += pion->getPoids();
        }
        if(pion->getProprietaire()->getNumero() == 3){
            poids_joueur4 += pion->getPoids();
        }
    }

    //attribution des points en fonction du poids
    size_t poids_max = std::max(std::max(poids_joueur1,poids_joueur2),std::max(poids_joueur3,poids_joueur4));
    if(poids_max == 0) return; //on ne fait pas de maj si il n'y a pas de pions à retirer
    size_t score = z->getScore();
    if(poids_joueur1 == poids_max){
        partie_->getJoueur(0).setScore(score + partie_->getJoueur(0).getScore());
    }
    if(poids_joueur2 == poids_max){
        qDebug() << "joueur 2";
        partie_->getJoueur(1).setScore(score + partie_->getJoueur(1).getScore());
    }
    if(poids_joueur3 == poids_max){
        qDebug() << "joueur 3";
        partie_->getJoueur(2).setScore(score + partie_->getJoueur(2).getScore());
    }
    if(poids_joueur4 == poids_max){
        qDebug() << "joueur 4";
        partie_->getJoueur(3).setScore(score + partie_->getJoueur(3).getScore());
    }

    //mise a jour du tableau
    majScoreTableau();
}

MainWindow::~MainWindow()
{
    delete ui;
}

