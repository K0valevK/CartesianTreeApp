#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->GraphView->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);

    tree_ = new GraphicProject::CartesianTree();
    drawer_ = new GraphicProject::GraphicComponent(*tree_, new QGraphicsScene());
    FillActions();

    ui->GraphView->setScene(drawer_->GetScene());
    ui->GraphView->setCacheMode(QGraphicsView::CacheBackground);
    ui->GraphView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->GraphView->setRenderHint(QPainter::Antialiasing);
    ui->GraphView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->GraphView->scale(qreal(1), qreal(1));
    ui->GraphView->setMinimumSize(640, 480);

    connect(ui->AddRandomNodeButton, &QPushButton::clicked, this, &MainWindow::CreateRandomNode);
    connect(ui->AddNodeKeyButton, &QPushButton::clicked, this, &MainWindow::OpenChooseKeyWindow);
    connect(ui->AddNodeButton, &QPushButton::clicked, this, &MainWindow::OpenChooseWindow);
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

void MainWindow::OpenChooseWindow() {
    choose_node_window_ = std::make_unique<ChooseNodeXY>(new ChooseNodeXY());
    choose_node_window_->setModal(true);

    connect(choose_node_window_.get(), &QDialog::accepted, choose_node_window_.get(), &ChooseNodeXY::SendDataXY);
    connect(choose_node_window_.get(), &ChooseNodeXY::SendDataNode, this, &MainWindow::RecieveDataNode);

    choose_node_window_->exec();
}

void MainWindow::OpenChooseKeyWindow() {
    choose_node_window_ = std::make_unique<ChooseNodeXY>(new ChooseNodeXY());
    choose_node_window_->setModal(true);
    choose_node_window_->HidePriorityEditLine();

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
}

void MainWindow::ExitApp() {
    QApplication::quit();
}

void MainWindow::RecieveDataNode(QString key, QString priority) {
    bool is_convertable;
    latest_input_.first = key.toInt(&is_convertable);
    if (!is_convertable) {
        QMessageBox::warning(this, "Error:", "Not acceptable key value was provided!");
        return;
    }
    if (priority.isEmpty()) {
        latest_input_.second = GraphicProject::Random::GetInstance().GetInt();
    } else {
        latest_input_.second = priority.toInt(&is_convertable);
        if (!is_convertable) {
            QMessageBox::warning(this, "Error:", "Not acceptable priority value was provided!");
            return;
        }
    }
    CreateNode();
}

void MainWindow::RecieveAction(GraphicProject::Actions action, int main_tree, int additional_tree, QString value) {
    bool is_convertable;
    int key = value.toInt(&is_convertable);
    if (!value.isEmpty() && !is_convertable) {
        QMessageBox::warning(this, "Error:", "Not acceptable key value was provided!");
        return;
    }
    switch (action) {
    case GraphicProject::Actions::Merge:
        if (main_tree == additional_tree) {
            QMessageBox::warning(this, "Error:", "Tree can not be merged with itself!");
            return;
        }
        tree_->Merge(main_tree, additional_tree);
        break;
    case GraphicProject::Actions::Split:
        tree_->Split(main_tree, key);
        break;
    case GraphicProject::Actions::Insert:
        tree_->Insert(main_tree, key);
        break;
    case GraphicProject::Actions::Erase:
        tree_->Erase(main_tree, key);
        break;
    default:
        break;
    }
}



void MainWindow::CreateNode() {
    tree_->Add(latest_input_);
}

void MainWindow::FillActions() {
    action_buttons_[ui->MergeButton] = GraphicProject::Actions::Merge;
    action_buttons_[ui->SplitButton] = GraphicProject::Actions::Split;
    action_buttons_[ui->InsertButton] = GraphicProject::Actions::Insert;
    action_buttons_[ui->EraseButton] = GraphicProject::Actions::Erase;
}

