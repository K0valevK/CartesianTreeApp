#ifndef CHOOSENODEXY_H
#define CHOOSENODEXY_H

#include <QDialog>

namespace Ui {
class ChooseNodeXY;
} // namespace UI

class ChooseNodeXY : public QDialog {
    Q_OBJECT

public:
    explicit ChooseNodeXY(QWidget *parent = nullptr);
    ~ChooseNodeXY();

    void HidePriorityEditLine();

signals:
    void SendDataNode(QString key, QString priority);

public slots:
    void SendDataXY();

private:
    Ui::ChooseNodeXY *ui;
};

#endif // CHOOSENODEXY_H
