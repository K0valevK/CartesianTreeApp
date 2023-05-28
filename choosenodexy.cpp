#include "choosenodexy.h"
#include "ui_choosenodexy.h"

ChooseNodeXY::ChooseNodeXY(GraphicProject::Actions action, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseNodeXY) {
    ui->setupUi(this);
    setWindowTitle("Node parametrs");

    switch (action) {
    case GraphicProject::Actions::CreateNodeKey:
        HidePriorityEditLine();
        break;
    default:
        break;
    }
    UpdateSize();
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

void ChooseNodeXY::UpdateSize() {
    QSize new_size = size();
    auto children = findChildren<QLabel *>();
    for (auto child : children) {
        if (child->isHidden()) {
            new_size.setHeight(new_size.height() - child->height());
        }
    }
    resize(new_size);
}
