#ifndef TUILEWINDOW_H
#define TUILEWINDOW_H

#include <QPushButton>
#include "tuile.h"
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>



class  VueMinituile : public QPushButton{
    Q_OBJECT
public:

    VueMinituile(QWidget* parent=nullptr);
    VueMinituile* setNum(int num){
        numero_minituile_ = num;
        return this;
    }
    int getNum(){return numero_minituile_;}

private:
    int numero_minituile_;

signals:

    void MiniTuileClicked(VueMinituile*);

public slots:
private slots:
    void clickedEvent(){
        //qDebug() << "VM clickedEvent";
        emit MiniTuileClicked(this);
    }
    void disableButton(){this->setEnabled(false);};

};



class VueTuile : public QPushButton{
    Q_OBJECT
public:
    VueTuile(size_t x, size_t y, QWidget* parent = nullptr);

    void setTuile(Tuile& t) {
       if (a_tuile==false) {
            a_tuile = true;

            // Get pixmap from tuile
            QImage image(QString::fromStdString(t.getChemin()));
            auto pixmap = QPixmap::fromImage(image);

            // Rotate as much as needed
            int angle = 90 * t.getRotations();
            QTransform tr;
            tr.rotate(angle);
            pixmap = pixmap.transformed(tr);

            // Use pixmap as icon
            QIcon icon(pixmap);
            this->setIcon(icon);
            this->setIconSize(this->size());
       }
    }
    bool getaTuile(){
        return a_tuile;
    }

    size_t getposX() const noexcept {
        return posX;
    }
    size_t getposY() const noexcept {
        return posY;
    }

    QGridLayout* layoutgrille;
    vector<VueMinituile*> vue_minituile;
    vector<VueMinituile*> les_boutons_n;

private :
    size_t posX;
    size_t posY;
    QVBoxLayout * layout = nullptr;
    bool a_tuile = false;

signals:
    void tuileClicked(VueTuile*);
    void miniTuileClicked(VueTuile*,VueMinituile*);

public slots:
private slots:
    void clickedEvent(){
        emit tuileClicked(this);
    }
    void onMiniTuileClicked(VueMinituile* m) {
        //qDebug() << "VT onMinituileClicked";
        emit miniTuileClicked(this,m);
    };
};

  #endif // TUILEWINDOW_H
