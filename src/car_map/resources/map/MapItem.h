#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QApplication>
#include <QWidget>

struct PointCoordinates {
    double x;
    double y;
};

class MapItem : public QGraphicsItem
{

public:
    MapItem();
    QRectF boundingRect() const;  // 设置边界
    void SetItemBoundary(double itemWidth, double itemHeight);  // 改变边界
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,  QWidget *widget); // 自动调用

    MapItem(const int itemWidth, const int itemHeight, const QString path, const int mapNumber);  // 初始化
    void SetPath(const QString path);  // 设置目录
    void SetMapNumber(const int mapNumber);  // 设置子图
    void LoadMapPixmap();  // 加载子图
    PointCoordinates GetMouseMapPoint();  // 获取鼠标刚刚点击位置的地图坐标
    PointCoordinates GetMouseGlobalPoint();  // 获取鼠标刚刚点击位置的全局坐标
    PointCoordinates PointGlobalToPixel(const PointCoordinates globalPoint);  // 全局坐标转图片坐标

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QString path;  //  子图目录
    int itemWidth;  // 边框的大小
    int itemHeight;  // 边框的大小
    int mapNumber;  // 子图编号
    QPixmap pixmap;  // 子图
    double pixmapWidth;  // 子图宽度
    double pixmapHeight;  // 子图高度
    double pixmapMultiple;  // 显示比例
    double pixelDistance;  // 每像素长度
    PointCoordinates mapOriginPoint;  // 地图原点
    PointCoordinates globalOriginPoint;  // 原点的全局坐标
    double globalOriginPointHeading; // 原点的全局坐标朝向
    PointCoordinates mouseMapPoint;  // 鼠标点地图坐标
    PointCoordinates mouseGlobalPoint; // 鼠标点全局坐标

    PointCoordinates PointClickToMap(const PointCoordinates clickPoint); // 鼠标点击点转地图坐标
    PointCoordinates PointMapToGlobal(const PointCoordinates mapPoint); // 地图坐标转全局坐标


};

#endif // MAPITEM_H
