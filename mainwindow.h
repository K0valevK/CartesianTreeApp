#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <string>
#include <memory>
#include <utility>
#include <unordered_map>

#include "chooseaction.h"
#include "choosenodexy.h"
#include "graphiccomponent.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void CreateRandomNode();
    void CreateChooseWindow();
    void CreateActionWindow();
    void ClearAll();
    void ExitApp();

    void RecieveDataNode(QString key, QString priority);
    void RecieveAction(GraphicProject::Actions action, int main_tree, int additional_tree, QString key, QString priority);

private:
    void EnableButtons();
    void CreateNode();
    void FillActions();
    bool IsCorrectFormat(const QString &value, bool iskey, int *result);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<ChooseNodeXY> choose_node_window_;
    std::unique_ptr<ChooseAction> choose_action_window_;
    std::pair<int, int> latest_input_;
    GraphicProject::CartesianTree *tree_;
    GraphicProject::GraphicComponent *drawer_;
    std::unordered_map<QObject*, GraphicProject::Actions> action_buttons_;
};

#endif // MAINWINDOW_H
