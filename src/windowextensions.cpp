#include "windowextensions.h"
#include "ui_windowextensions.h"

windowextensions::windowextensions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windowextensions)
{
    ui->setupUi(this);
}

windowextensions::~windowextensions()
{
    delete ui;
}
