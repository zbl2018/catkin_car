#ifndef MOD_MAPITEMHPP_H
#define MOD_MAPITEMHPP_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QApplication>
#include <QWidget>
#include <QFileInfo>
#include <QObject>
#include <QPainter>
#include <QMessageBox>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDebug>
#include <QKeyEvent>
#include <QTextCodec>
#include <QStack>
#include<QFileDialog>
#include <QProgressDialog>
#include <QDateTime>
//#include "MapItem.hpp"
using namespace std;
#define Color_Black 0
#define Color_White 255
//存储pgm图片上一点的信息
struct ModPointCoordinates {
    double x;
    double y;
    int color;
};
//存储pgm地图的结构体
struct PgmGrid
{
    QString map_head[4];
    unsigned char *data;
};
class Mod_MapItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
Q_SIGNALS:
    void Signal_OpenNewMap(bool);
    void Signal_BrushStatus(int);// 0 为黑色，1-10表示白色画刷范围
    void Signal_OpenHelpManual(bool);

public:
    Mod_MapItem();
    ~Mod_MapItem();
    QRectF boundingRect() const;  // 设置边界
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,  QWidget *widget); // 自动调用
    //void SetMapItemInfo(const int itemWidth, const int itemHeight, const QString path, const int mapNumber);
    void SetMapItemInfo(const int itemWidth, const int itemHeight, const QString all_path);//设置读取文件路径
//    void SetPath(const QString path);  // 设置目录
//    void SetMapNumber(int &mapNumber,int AddOrDec);  // 设置子图
//    void LoadMapPixmap();  // 加载图片
//    PointCoordinates GetMouseMapPoint();  // 获取鼠标刚刚点击位置的地图坐标
//    PointCoordinates GetMouseGlobalPoint();  // 获取鼠标刚刚点击位置的全局坐标
//    PointCoordinates PointGlobalToPixel(const PointCoordinates globalPoint);  // 全局坐标转图片坐标
    bool isFileExist(QString fullFilePath);
    QVector<ModPointCoordinates> GetPointsFromLine(ModPointCoordinates start_point,ModPointCoordinates end_point);
    bool LoadPgmMapInfo(QString MapPath,PgmGrid &map_content);//读取地图数据到数组中
    bool WritePgmMapInfo(QString backup_mapPath,PgmGrid map_content);//写入修改后的地图数据，重新生产地图
    bool ShowTip(QString tip);//提示框
    bool ModificateMap(QVector<ModPointCoordinates> real_map_boundary);//修改地图数据
    QString GetSystemTime();
//    void selectFile();//选择文件对话框
    //void SetDrivePath(PointCoordinates point);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);//鼠标按下事件
    void keyPressEvent(QKeyEvent *event);//键盘事件
private:
    QVector<ModPointCoordinates> drive_path;//the path of car running
    QVector<ModPointCoordinates> map_boundary;//在界面上显示的点
    //QVector<PointCoordinates> real_map_boundary;//实际上定位到pgm图上的点
    PgmGrid map_content;//存储PGM地图数据
    QString mod_map_path;//被修改过的子图存储地址
    int mod_count;//当前子图的修改版本(从0开始)
    int ModBrushSize;//修改画刷的大小，以像素为基本单位（默认为3，最小为1 ,最大为10），仅对将黑色区域修改成白色区域的时候起作用
    bool BlackBrush;//判断是否为黑色画刷，默认为黑色画刷
    QStack<int> NumPointsInPerStep;//每一步操作加入的点数
    QStack<int> NumPointsRetreated;//已经被回退的点数
    QStack<ModPointCoordinates> last_step_location_for_boundary;//存储上一步加入的边界点坐标
    //QStack<PointCoordinates> last_step_location_for_real;//存储上一步加入的边界点坐标
    QString map_all_path;//一个地图的全路径
    QString mod_map_name;//被修改的地图的名称
//    QString path;  //  子图目录
    int itemWidth;  // 边框的大小
    int itemHeight;  // 边框的大小
    //int mapNumber;  // 子图编号
    QPixmap pixmap;  // 子图
    double pixmapWidth;  // 子图宽度
    double pixmapHeight;  // 子图高度
//    double pixmapMultiple;  // 显示比例
    double pixelDistance;  // 每像素长度
    int MaxBrushSize;//最大画刷
    int MinBrushSize;//最小画刷
//    PointCoordinates mapOriginPoint;  // 地图原点
//    PointCoordinates globalOriginPoint;  // 原点的全局坐标
//    double globalOriginPointHeading; // 原点的全局坐标朝向
//    PointCoordinates mouseMapPoint;  // 鼠标点地图坐标
//    PointCoordinates mouseGlobalPoint; // 鼠标点全局坐标

//    PointCoordinates PointClickToMap(const PointCoordinates clickPoint); // 鼠标点击点转地图坐标
//    PointCoordinates PointMapToGlobal(const PointCoordinates mapPoint); // 地图坐标转全局坐标

};
#endif // MOD_MAPITEMHPP_H
