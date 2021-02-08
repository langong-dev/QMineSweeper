#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::on_buttonBox_accepted()
{
    ROW = this->ui->row->value();
    COL = this->ui->col->value();
    BOOMNUM = this->ui->bomb->value();
    createinterface();
    this->close();
}
