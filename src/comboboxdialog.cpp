#include "comboboxdialog.h"
#include "ui_comboboxdialog.h"

ComboBoxDialog::ComboBoxDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComboBoxDialog)
{
    ui->setupUi(this);
}

ComboBoxDialog::~ComboBoxDialog()
{
    delete ui;
}
