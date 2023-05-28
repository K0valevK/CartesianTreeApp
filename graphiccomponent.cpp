#include "graphiccomponent.h"

#include <QtDebug>

namespace GraphicProject {

GraphicComponent::GraphicComponent(CartesianTree &tree, QGraphicsScene *scene) : tree_(tree), scene_(scene) {
    tree_.Subscribe(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 2048, 2048);
    nodes_.reserve(10);
    nodes_data_.reserve(10);
}

GraphicComponent::~GraphicComponent() {
    delete scene_;
}

void GraphicComponent::Notify() {
    Clear();
    border_x_coord_.resize(tree_.GetSize(), {INT_MAX, INT_MIN});
    for (size_t i = 0; i < tree_.GetSize(); ++i) {
        nodes_.push_back({});
        nodes_data_.push_back({});
        nodes_text_.push_back({});
        edges_.push_back({});
        CalculateCoords(i, tree_[i], 1024, 1024);
    }
    DrawGraph();
}

QGraphicsScene *GraphicComponent::GetScene() const {
    return scene_;
}

void GraphicComponent::CalculateCoords(size_t index, const std::unique_ptr<CartesianTree::Node> &node, int x, int y, size_t parent) {
    if (!node) {
        return;
    }
    nodes_data_[index].push_back({node->GetKey(), node->GetPriority(), x, y, parent});
    int new_parent = nodes_data_[index].size() - 1;
    border_x_coord_[index].first = std::min(x, border_x_coord_[index].first);
    border_x_coord_[index].second = std::max(x, border_x_coord_[index].second);
    CalculateCoords(index, node->GetLeft(), x - dist_between_nodes_x, y + dist_between_nodes_y, new_parent);
    CalculateCoords(index, node->GetRight(), x + dist_between_nodes_x, y + dist_between_nodes_y, new_parent);
}

void GraphicComponent::CalcualteOffset() {
    int offset_old = 0;
    int offset_new = 0;
    for (size_t i = 0; i < nodes_data_.size(); ++i) {
        if (i != 0) {
            offset_new = dist_between_trees - border_x_coord_[i].first + border_x_coord_[i].second + offset_old;
        }
        for (size_t j = 0; j < nodes_data_[i].size(); ++j) {
            nodes_data_[i][j].coord_x += offset_new;
        }
        border_x_coord_[i] = { border_x_coord_[i].first + offset_new, border_x_coord_[i].second + offset_new };
        offset_old = offset_new;
    }
}

void GraphicComponent::DrawGraph() {
    CalcualteOffset();
    for (size_t i = 0; i < nodes_data_.size(); ++i) {
        for (size_t j = 0; j < nodes_data_[i].size(); ++j) {
            if (nodes_data_[i][j].parent != j) {
                edges_[i].push_back(
                    scene_->addLine(nodes_data_[i][nodes_data_[i][j].parent].coord_x + radius / 2,
                                    nodes_data_[i][nodes_data_[i][j].parent].coord_y + radius / 2,
                                    nodes_data_[i][j].coord_x + radius / 2,
                                    nodes_data_[i][j].coord_y + radius / 2,
                                    QPen(Qt::white))
                    );
            }
        }
    }
    for (size_t i = 0; i < nodes_data_.size(); ++i) {
        for (size_t j = 0; j < nodes_data_[i].size(); ++j) {
            nodes_[i].push_back(
                scene_->addEllipse(nodes_data_[i][j].coord_x, nodes_data_[i][j].coord_y,
                                   radius, radius, QPen(Qt::white),
                                   color_palette[i % color_palette.size()])
                );
            qreal offset_key = (5 - QString::number(nodes_data_[i][j].key).size()) * radius / 10;
            qreal offset_priority = (5 - QString::number(nodes_data_[i][j].priority).size()) * radius / 10;
            nodes_text_[i].push_back({
                scene_->addText(QString::number(nodes_data_[i][j].key)),
                scene_->addText(QString::number(nodes_data_[i][j].priority))
            });
            nodes_text_[i].back().first->setPos(nodes_data_[i][j].coord_x + offset_key,
                                          nodes_data_[i][j].coord_y + radius / 8);
            nodes_text_[i].back().second->setPos(nodes_data_[i][j].coord_x + offset_priority,
                                                nodes_data_[i][j].coord_y + 7 * radius / 16);
        }
    }
}

void GraphicComponent::Clear() {
    for (size_t i = 0; i < nodes_.size(); ++i) {
        for (size_t j = 0; j < nodes_[i].size(); ++j) {
            scene_->removeItem(nodes_[i][j]);
            scene_->removeItem(nodes_text_[i][j].first);
            scene_->removeItem(nodes_text_[i][j].second);
            delete nodes_[i][j];
            delete nodes_text_[i][j].first;
            delete nodes_text_[i][j].second;
        }
    }
    for (size_t i = 0; i < edges_.size(); ++i) {
        for (size_t j = 0; j < edges_[i].size(); ++j) {
            scene_->removeItem(edges_[i][j]);
            delete edges_[i][j];
        }
    }
    nodes_data_.clear();
    nodes_.clear();
    nodes_text_.clear();
    edges_.clear();
    border_x_coord_.clear();
}

} // namespace GraphicProject
