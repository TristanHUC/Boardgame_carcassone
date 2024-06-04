#ifndef WINDOWEXTENSIONS_H
#define WINDOWEXTENSIONS_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class windowextensions;
}

class windowextensions : public QDialog
{
    Q_OBJECT

public:
    explicit windowextensions(QWidget *parent = nullptr);
    ~windowextensions();

    const Ui::windowextensions* getUi() const {return ui;}

private:
    Ui::windowextensions *ui;
};

#endif // WINDOWEXTENSIONS_H
