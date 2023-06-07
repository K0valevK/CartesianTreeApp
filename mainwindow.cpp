#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->GraphView->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);
    setWindowTitle("Cartesian Tree Drawer");

    tree_ = new GraphicProject::CartesianTree();
    drawer_ = new GraphicProject::GraphicComponent(*tree_, new QGraphicsScene());
    FillActions();
    EnableButtons();

    ui->GraphView->setScene(drawer_->GetScene());
    ui->GraphView->setCacheMode(QGraphicsView::CacheBackground);
    ui->GraphView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->GraphView->setRenderHint(QPainter::Antialiasing);
    ui->GraphView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->GraphView->scale(qreal(1), qreal(1));
    ui->GraphView->setMinimumSize(640, 480);

    connect(ui->AddRandomNodeButton, &QPushButton::clicked, this, &MainWindow::CreateRandomNode);
    connect(ui->AddNodeKeyButton, &QPushButton::clicked, this, &MainWindow::CreateChooseWindow);
    connect(ui->AddNodeButton, &QPushButton::clicked, this, &MainWindow::CreateChooseWindow);
    connect(ui->MergeButton, &QPushButton::clicked, this, &MainWindow::CreateActionWindow);
    connect(ui->SplitButton, &QPushButton::clicked, this, &MainWindow::CreateActionWindow);
    connect(ui->InsertButton, &QPushButton::clicked, this, &MainWindow::CreateActionWindow);
    connect(ui->EraseButton, &QPushButton::clicked, this, &MainWindow::CreateActionWindow);
    connect(ui->ClearButton, &QPushButton::clicked, this, &MainWindow::ClearAll);
    connect(ui->ExitButton, &QPushButton::clicked, this, &MainWindow::ExitApp);
}

MainWindow::~MainWindow() {
    delete ui;
    delete tree_;
    delete drawer_;
}

void MainWindow::CreateRandomNode() {
    latest_input_ = {GraphicProject::Random::GetInstance().GetInt(), GraphicProject::Random::GetInstance().GetInt()};
    CreateNode();
}

void MainWindow::CreateChooseWindow() {
    choose_node_window_ = std::unique_ptr<ChooseNodeXY>(new ChooseNodeXY(action_buttons_[sender()]));
    choose_node_window_->setModal(true);

    connect(choose_node_window_.get(), &QDialog::accepted, choose_node_window_.get(), &ChooseNodeXY::SendDataXY);
    connect(choose_node_window_.get(), &ChooseNodeXY::SendDataNode, this, &MainWindow::RecieveDataNode);

    choose_node_window_->exec();
}

void MainWindow::CreateActionWindow() {
    choose_action_window_ = std::unique_ptr<ChooseAction>(new ChooseAction(action_buttons_[sender()], tree_->GetSize()));
    choose_action_window_->setModal(true);

    connect(choose_action_window_.get(), &QDialog::accepted, choose_action_window_.get(), &ChooseAction::SendAction);
    connect(choose_action_window_.get(), &ChooseAction::SendActionSignal, this, &MainWindow::RecieveAction);

    choose_action_window_->exec();
}

void MainWindow::ClearAll() {
    tree_->Clear();
    drawer_->Clear();
    EnableButtons();
}

void MainWindow::ExitApp() {
    QApplication::quit();
}

void MainWindow::RecieveDataNode(QString key, QString priority) {
    if (!IsCorrectFormat(key, true, &latest_input_.first)) {
        return;
    }
    if (priority.isEmpty()) {
        latest_input_.second = GraphicProject::Random::GetInstance().GetInt();
    } else {
        if (!IsCorrectFormat(priority, false, &latest_input_.second)) {
            return;
        }
    }
    CreateNode();
}

void MainWindow::RecieveAction(GraphicProject::Actions action, int main_tree, int additional_tree, QString key, QString priority) {
    switch (action) {
    case GraphicProject::Actions::Merge:
        if (main_tree == additional_tree) {
            QMessageBox::warning(this, "Error:", "Tree can not be merged with itself!");
            return;
        }
        if (!tree_->Merge(main_tree, additional_tree)) {
            QMessageBox::warning(this, "Error:", "Theese trees cannot be merged!");
        }
        break;
    case GraphicProject::Actions::Split:
        if (!IsCorrectFormat(key, true, &latest_input_.first)) {
            return;
        }
        tree_->Split(main_tree, latest_input_.first);
        break;
    case GraphicProject::Actions::Insert:
        if (!IsCorrectFormat(key, true, &latest_input_.first)) {
            return;
        }
        if (!IsCorrectFormat(priority, false, &latest_input_.second)) {
            return;
        }
        tree_->Insert(main_tree, latest_input_.first, latest_input_.second);
        break;
    case GraphicProject::Actions::Erase:
        if (!IsCorrectFormat(key, true, &latest_input_.first)) {
            return;
        }
        tree_->Erase(main_tree, latest_input_.first);
        break;
    default:
        break;
    }
    EnableButtons();
}

void MainWindow::EnableButtons() {
    if (tree_->GetSize() >= 1) {
        ui->SplitButton->setEnabled(true);
        ui->InsertButton->setEnabled(true);
        ui->EraseButton->setEnabled(true);
        if (tree_->GetSize() >= 2) {
            ui->MergeButton->setEnabled(true);
        }
        return;
    }
    ui->SplitButton->setEnabled(false);
    ui->InsertButton->setEnabled(false);
    ui->EraseButton->setEnabled(false);
    if (tree_->GetSize() < 2) {
        ui->MergeButton->setEnabled(false);
    }
}

void MainWindow::CreateNode() {
    tree_->Add(latest_input_);
    EnableButtons();
}

void MainWindow::FillActions() {
    action_buttons_[ui->AddNodeKeyButton] = GraphicProject::Actions::CreateNodeKey;
    action_buttons_[ui->AddNodeButton] = GraphicProject::Actions::CreateNodeCustom;
    action_buttons_[ui->MergeButton] = GraphicProject::Actions::Merge;
    action_buttons_[ui->SplitButton] = GraphicProject::Actions::Split;
    action_buttons_[ui->InsertButton] = GraphicProject::Actions::Insert;
    action_buttons_[ui->EraseButton] = GraphicProject::Actions::Erase;
}

bool MainWindow::IsCorrectFormat(const QString &value, bool iskey, int *result) {
    bool is_convertable;
    int tmp = value.toInt(&is_convertable);
    if (!is_convertable) {
        if (iskey) {
            QMessageBox::warning(this, "Error:", "Not acceptable key value was provided!");
        } else {
            QMessageBox::warning(this, "Error:", "Not acceptable priority value was provided!");
        }
        return false;
    }
    if (!((value[0] == '-' && value.size() <= 5) || value.size() <= 4)) {
        QMessageBox::warning(this, "Error:", "Please provide number under 5 digits!");
        return false;
    }
    if (result) {
        *result = tmp;
    }
    return true;
}
