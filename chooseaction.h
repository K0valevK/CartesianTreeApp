#ifndef CHOOSEACTION_H
#define CHOOSEACTION_H

#include <QDialog>
#include <QString>

#include "actionbuttons.h"

namespace Ui {
class ChooseAction;
}

class ChooseAction : public QDialog {
    Q_OBJECT

public:
    ChooseAction(GraphicProject::Actions action, size_t num_of_roots, QWidget *parent = nullptr);
    ~ChooseAction();

signals:
    void SendActionSignal(GraphicProject::Actions action, int main_tree, int additional_tree, QString key, QString priority);

public slots:
    void SendAction();

private:
    void HideKey();
    void HidePriority();
    void HideAdditionalBox();
    void UpdateSize();

private:
    Ui::ChooseAction *ui;
    GraphicProject::Actions action_;
};

#endif // CHOOSEACTION_H
