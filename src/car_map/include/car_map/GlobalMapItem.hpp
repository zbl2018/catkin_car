#ifndef GLOBALMAPITEM_HPP
#define GLOBALMAPITEM_HPP

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QPainter>
#include "MapItem.hpp"
#include <QMessageBox>
class GlobalMapItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
Q_SIGNALS:
    //发送任务点给main_window对象
    void Signal_SendEndPoint(string);
public:
    GlobalMapItem();
    //设置边界
    QRectF boundingRect() const;
    //画图函数
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,  QWidget *widget);
    //自定义提示框
    bool ShowTip(QString tip);
    //添加坐标点，显示小车当前行走轨迹
    void AddPointToDrivePath(PointCoordinates point);
protected:
    //从txt文件中获取坐标点，在地图上显示（仅测试使用）
    void GetPointsFromMapFile();
    //获取全局底图
    void GetBaseMapInfo();
    //bool JudgeNextPointStatus(PointCoordinates start_point,PointCoordinates end_point);
    //鼠标事件函数
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);//鼠标按下事件
private:
    QPixmap pixmap;//加载图片对象
    double PixmapWidth,PixmapHeigth;//图片高度与宽度
    QString GlobalMapPath;//图片路径
    QString BaseMapName;//底图名称
    QString PointsInMap_FileName;//全局规划图的名称
    QVector<PointCoordinates> AllPointsInMap;//存储txt文件中的所有点
    double map_x_min,map_x_max,map_y_min,map_y_max;
    QVector<PointCoordinates> DrivePath;//保存小车行走轨迹的坐标点

    PointCoordinates OriginPoint;//图片上的真实原点位置
    double XRatio,YRatio;//地图比例尺（为82.3时，表示，用地图的82.3个像素长度代表现实世界的1m长度）
};

#endif // GLOBALMAPITEM_HPP
