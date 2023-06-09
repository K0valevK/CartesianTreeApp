#ifndef GRAPHICPROJECT_GRAPHICCOMPONENT_H
#define GRAPHICPROJECT_GRAPHICCOMPONENT_H

#include <QColor>
#include <QString>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

#include <array>
#include <vector>

#include "cartesiantree.h"

namespace GraphicProject {

static constexpr int radius = 50;
static constexpr int dist_between_nodes_x = 50;
static constexpr int dist_between_nodes_y = 75;
static constexpr int dist_between_trees = 200;

static constexpr std::pair<int, int> start_scene_size = {512, 512};
static constexpr int start_pos_x = start_scene_size.first / 2;
static constexpr int start_pos_y = start_scene_size.second / 2;

static const std::array<QColor, 5> color_palette = { Qt::blue, Qt::darkGreen, Qt::red, Qt::magenta, Qt::gray };

struct NodeGraphicData {
    int key;
    int priority;
    int coord_x;
    int coord_y;
    size_t parent;
};

class GraphicComponent : public Observer {
public:
    template<class T>
    using vector2D = std::vector<std::vector<T>>;

    using UniqueEllipse = std::unique_ptr<QGraphicsEllipseItem>;

public:
    GraphicComponent(CartesianTree &tree, QGraphicsScene *scene);

    ~GraphicComponent();

    void Notify() override;

    QGraphicsScene *GetScene() const;

    void Clear();

signals:
    void CreateError(QString error_message);

private:
    void CalculateCoords(size_t index, const std::unique_ptr<CartesianTree::Node> &node, int x, int y, size_t parent = 0);
    void CalcualteOffset();
    void DrawGraph();
    void ResizeScene();

private:
    CartesianTree &tree_;
    QGraphicsScene *scene_;
    std::pair<int, int> scene_size = start_scene_size;
    //vector2D<UniqueEllipse> nodes_;
    //std::vector<std::vector<std::unique_ptr<QGraphicsEllipseItem>>> nodes_;
    std::vector<std::vector<QGraphicsEllipseItem *>> nodes_;
    std::vector<std::vector<NodeGraphicData>> nodes_data_;
    //std::vector<std::vector<std::unique_ptr<QGraphicsTextItem>>> nodes_text_;
    std::vector<std::vector<std::pair<QGraphicsTextItem *, QGraphicsTextItem *>>> nodes_text_;
    std::vector<std::vector<QGraphicsLineItem *>> edges_;
    std::vector<std::pair<int, int>> border_x_coord_;
    int border_y_;
};

} // namespace GraphicProject

#endif // GRAPHICPROJECT_GRAPHICCOMPONENT_H
