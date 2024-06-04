#ifndef WINDOWPLAYER_H
#define WINDOWPLAYER_H

#include <QDialog>

namespace Ui {
class windowplayer;
}

class windowplayer : public QDialog
{
    Q_OBJECT

public:
    explicit windowplayer(int nb_joueurs,QWidget *parent = nullptr);
    ~windowplayer();

    void accept() override;

    const Ui::windowplayer* getUi() const {return ui;}
private:
    Ui::windowplayer *ui;
};

#endif // WINDOWPLAYER_H
