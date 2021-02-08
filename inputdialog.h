#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class InputDialog;
}

extern void createinterface();
extern int ROW,COL,BOOMNUM;

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::InputDialog *ui;
};

#endif // INPUTDIALOG_H
