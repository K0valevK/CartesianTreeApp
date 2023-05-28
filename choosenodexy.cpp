#include "choosenodexy.h"
#include "ui_choosenodexy.h"

ChooseNodeXY::ChooseNodeXY(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseNodeXY) {
    ui->setupUi(this);
    setWindowTitle("Node parametrs");
}

ChooseNodeXY::~ChooseNodeXY() {
    delete ui;
}

void ChooseNodeXY::HidePriorityEditLine() {
    ui->PriorityLable->hide();
    ui->PriorityLineEdit->hide();
}

void ChooseNodeXY::SendDataXY() {
    emit SendDataNode(ui->KeyLineEdit->text(), ui->PriorityLineEdit->text());
}
