#ifndef PASSDIALOG_H
#define PASSDIALOG_H

#include <QDialog>

namespace Ui {
class PassDialog;
}

class PassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PassDialog(QWidget *parent = nullptr);
    ~PassDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PassDialog *ui;

signals:
    void passCorrectSig(bool);
};

#endif // PASSDIALOG_H
