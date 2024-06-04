#include "windowplayer.h"
#include "ui_windowplayer.h"

#include <QMessageBox>

windowplayer::windowplayer(int nb_joueurs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windowplayer)
{
    ui->setupUi(this);

    if (nb_joueurs >= 3) {
        ui->QLabel_joueur3->setEnabled(true);
        ui->QLineEdit_joueur3->setEnabled(true);
    }
    if (nb_joueurs >= 4) {
        ui->QLabel_joueur4->setEnabled(true);
        ui->QLineEdit_joueur4->setEnabled(true);
    }
}

void windowplayer::accept(){
    if (ui->QLineEdit_joueur1->text().isEmpty() ||
        ui->QLineEdit_joueur2->text().isEmpty() ||
        (ui->QLineEdit_joueur3->isEnabled() && ui->QLineEdit_joueur3->text().isEmpty()) ||
        (ui->QLineEdit_joueur4->isEnabled() && ui->QLineEdit_joueur4->text().isEmpty())
     ){
        QMessageBox::critical(this, "Erreur formulaire", "Merci de remplir tout les champs du formulaire");
    }
    else
        return QDialog::accept();
}

windowplayer::~windowplayer()
{
    delete ui;
}
