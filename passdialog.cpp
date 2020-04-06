#include "passdialog.h"
#include "ui_passdialog.h"
#include <QMessageBox>

PassDialog::PassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PassDialog)
{
    ui->setupUi(this);
    setModal(true);
    ui->lineEdit->setFocus();
}

PassDialog::~PassDialog()
{
    delete ui;
}

void PassDialog::on_buttonBox_accepted(){
    QString str = ui->lineEdit->text();

    if (str == "abracadabra"){
        //hide();
        ui->lineEdit->clear();
        emit passCorrectSig(true);
    }
    else {
        QMessageBox::critical(this,"Ошибка","Пароль введен неверно");
        emit passCorrectSig(false);
        show();

    }
}

void PassDialog::on_buttonBox_rejected()
{
    emit passCorrectSig(false);
}
