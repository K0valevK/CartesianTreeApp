#include "chooseaction.h"
#include "ui_chooseaction.h"

ChooseAction::ChooseAction(GraphicProject::Actions action, size_t num_of_roots, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseAction), action_(action) {
    ui->setupUi(this);
    switch (action) {
    case GraphicProject::Actions::Merge:
        ui->KeyLabel->hide();
        ui->KeyEdit->hide();
        break;
    case GraphicProject::Actions::Split:
        ui->AdditionalBox->hide();
        ui->AdditionalLabel->hide();
        ui->MainLabel->setText("Tree");
        break;
    case GraphicProject::Actions::Insert:
        ui->AdditionalBox->hide();
        ui->AdditionalLabel->hide();
        ui->MainLabel->setText("Tree");
        break;
    case GraphicProject::Actions::Erase:
        ui->AdditionalBox->hide();
        ui->AdditionalLabel->hide();
        ui->MainLabel->setText("Tree");
        break;
    default:
        break;
    }
    for (size_t i = 0; i < num_of_roots; ++i) {
        ui->MainBox->addItem(QString::number(i + 1));
        ui->AdditionalBox->addItem(QString::number(i + 1));
    }
}

ChooseAction::~ChooseAction() {
    delete ui;
}

void ChooseAction::SendAction() {
    emit SendActionSignal(action_, ui->MainBox->currentIndex(), ui->AdditionalBox->currentIndex(), ui->KeyEdit->text());
}
