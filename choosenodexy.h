#ifndef CHOOSENODEXY_H
#define CHOOSENODEXY_H

#include <QDialog>

#include "actionbuttons.h"

namespace Ui {
class ChooseNodeXY;
} // namespace UI

class ChooseNodeXY : public QDialog {
    Q_OBJECT

public:
    ChooseNodeXY(GraphicProject::Actions action, QWidget *parent = nullptr);
    ~ChooseNodeXY();

signals:
    void SendDataNode(QString key, QString priority);

public slots:
    void SendDataXY();

private:
    void HidePriorityEditLine();
    void UpdateSize();

private:
    Ui::ChooseNodeXY *ui;
};

#endif // CHOOSENODEXY_H
