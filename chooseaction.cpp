#include "chooseaction.h"
#include "ui_chooseaction.h"

#include <QDebug>

ChooseAction::ChooseAction(GraphicProject::Actions action, size_t num_of_roots, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseAction), action_(action) {
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString(GraphicProject::action_names[action] + " parametrs"));
    switch (action) {
    case GraphicProject::Actions::Merge:
        HideKey();
        HidePriority();
        break;
    case GraphicProject::Actions::Split:
        HideAdditionalBox();
        HidePriority();
        ui->MainLabel->setText("Tree");
        break;
    case GraphicProject::Actions::Insert:
        HideAdditionalBox();
        ui->MainLabel->setText("Tree");
        break;
    case GraphicProject::Actions::Erase:
        HideAdditionalBox();
        HidePriority();
        ui->MainLabel->setText("Tree");
        break;
    default:
        break;
    }
    for (size_t i = 0; i < num_of_roots; ++i) {
        ui->MainBox->addItem(QString::number(i + 1));
        ui->AdditionalBox->addItem(QString::number(i + 1));
    }
    UpdateSize();
}

ChooseAction::~ChooseAction() {
    delete ui;
}

void ChooseAction::SendAction() {
    emit SendActionSignal(action_, ui->MainBox->currentIndex(), ui->AdditionalBox->currentIndex(), ui->KeyEdit->text(), ui->PriorityEdit->text());
}

void ChooseAction::HideKey() {
    ui->KeyLabel->hide();
    ui->KeyEdit->hide();
}

void ChooseAction::HidePriority() {
    ui->PriorityLabel->hide();
    ui->PriorityEdit->hide();
}

void ChooseAction::HideAdditionalBox() {
    ui->AdditionalLabel->hide();
    ui->AdditionalBox->hide();
}

void ChooseAction::UpdateSize() {
    QSize new_size = size();
    auto children = findChildren<QLabel *>();
    for (auto child : children) {
        if (child->isHidden()) {
            new_size.setHeight(new_size.height() - child->height());
        }
    }
    resize(new_size);
}
