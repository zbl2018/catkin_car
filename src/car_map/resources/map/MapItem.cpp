#include "MapItem.h"
#include <QPainter>
#include <QMessageBox>
#include <cmath>
#include <fstream>
#include <sstream>

#include <iostream>

MapItem::MapItem() {

}

QRectF MapItem::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2, 0 - penWidth / 2, itemWidth + penWidth, itemHeight + penWidth);
}

void MapItem::SetItemBoundary(double itemWidth, double itemHeight){
    this->itemWidth = itemWidth;
    this->itemHeight = itemHeight;
    prepareGeometryChange();
    LoadMapPixmap();
    update();
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);  //标明该参数没有使用
    Q_UNUSED(widget);
    if( !pixmap.isNull() ) {
        //pixmap = pixmap.scaled(pixmapWidth*pixmapMultiple, pixmapHeight*pixmapMultiple, Qt::KeepAspectRatio);  //缩放并保持宽高比
        painter->drawPixmap(0, 0, pixmapWidth*pixmapMultiple, pixmapHeight*pixmapMultiple, pixmap);
    }

}

MapItem::MapItem(const int itemWidth, const int itemHeight, const QString path, const int mapNumber) {
    this->itemWidth = itemWidth;
    this->itemHeight = itemHeight;
    this->path = path;
    this->mapNumber = mapNumber;
    LoadMapPixmap();
}

void MapItem::SetPath(const QString path) {
    this->path = path;
    LoadMapPixmap();
}

void MapItem::SetMapNumber(const int mapNumber) {
    this->mapNumber = mapNumber;
    LoadMapPixmap();
}

void MapItem::LoadMapPixmap() {
    QString mapPath = path+QString("%1.pgm").arg(mapNumber);
    pixmap.load(mapPath);
    if( pixmap.isNull() ) {
//        QMessageBox::warning(this, QObject::tr("警告"), QObject::tr("图像不存在！"), QMessageBox::Ok);
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("错误"));
        msg.setText(QObject::tr("图像不存在！"));
        msg.setIcon(QMessageBox::Warning);
        msg.addButton(QObject::tr("确定"),QMessageBox::ActionRole);
        msg.exec();
    } else {
        // 读取地图信息文件
        QString mapMessagePath = path+QString("%1.yaml").arg(mapNumber);
        std::ifstream mapMessage(mapMessagePath.toStdString());
        std::string message;
        std::stringstream sstream;
        while( std::getline(mapMessage, message) ) {
            sstream.clear();
            sstream.str("");
            sstream << message;
            std::string data;
            sstream >> data;
            if( data == "resolution:" ) {  // 读取分辨率
                sstream >> pixelDistance;
//                std::cout << "pixelDistance: " << pixelDistance << std::endl;
            } else if( data == "origin:" ) {  // 读取地图原点
                for( unsigned int i = 0; i < message.length(); i++ ) {
                    if( (message[i] == ':') | (message[i] == '[') | (message[i] == ',') | (message[i] == ']')  ) {
                        message[i] = ' ';
                    }
                }
                sstream.clear();
                sstream.str("");
                sstream << message;
                sstream >> data;
                sstream >> mapOriginPoint.x;
                mapOriginPoint.x = std::fabs(mapOriginPoint.x);
                sstream >> mapOriginPoint.y;
                mapOriginPoint.y = std::fabs(mapOriginPoint.y);
//                std::cout << "mapOriginPointX: " << mapOriginPointX << std::endl;
//                std::cout << "mapOriginPointY: " << mapOriginPointY << std::endl;
            } else {
                continue;
            }
        }
        // 获取原点全局信息
        QString originPointMessagePath = path+QString("map_spoint.txt");
        std::ifstream originPointMessage(originPointMessagePath.toStdString());
        for( int i = 0; i <= mapNumber; i++ ) {
            std::getline(originPointMessage, message);
        }
        sstream.clear();
        sstream.str("");
        sstream << message;
        sstream >> globalOriginPoint.x;
        sstream >> globalOriginPoint.y;
        sstream >> globalOriginPointHeading;
        // 设置地图信息
        pixmapWidth = pixmap.width(); //图片的宽度
        pixmapHeight = pixmap.height();  //图片的高度
        if( pixmapWidth > pixmapHeight ) {
            pixmapMultiple = (double)itemWidth / (double)pixmapWidth;
        } else {
            pixmapMultiple = (double)itemHeight / (double)pixmapHeight;
        }
    }
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    // 计算鼠标位置
    // 坐标原点在地图左下角！！！
    PointCoordinates clickPoint;
    clickPoint.x = (mouseEvent->pos().x()-this->pos().x())*(pixelDistance/pixmapMultiple);
    clickPoint.y = (itemHeight-mouseEvent->pos().y()-this->pos().y())*(pixelDistance/pixmapMultiple);
    // 注意地图坐标与小车坐标X、Y是反的！

//    std::cout << "mouseEventPoint: " << mouseEvent->pos().x() << ", " << mouseEvent->pos().y() << std::endl;
//    std::cout << "clickPoint: " << clickPoint.x << ", " << clickPoint.y << std::endl;

    mouseMapPoint = PointClickToMap(clickPoint);

    mouseGlobalPoint = PointMapToGlobal(mouseMapPoint);

//    // 计算实际坐标
//    mouseMapPoint.y = clickPoint.x*std::cos(globalOriginPointHeading) - clickPoint.y*std::sin(globalOriginPointHeading);
//    mouseMapPoint.x = clickPoint.y*std::cos(globalOriginPointHeading) + clickPoint.x*std::sin(globalOriginPointHeading);

//    mouseGlobalPoint.x = mouseMapPoint.x + globalOriginPoint.x;
//    mouseGlobalPoint.y = mouseMapPoint.y + globalOriginPoint.y;

//    std::cout << x << ", " << y << "\n" << mousePoint.x << ", " << mousePoint.y << std::endl;

//    PointCoordinates testPoint;
//    testPoint.x = mouseGlobalPoint.x;
//    testPoint.y = mouseGlobalPoint.y;
//    PointCoordinates resultPoint = PointGlobalToPixel(testPoint);
//    resultPoint = resultPoint;

//    QString message = QString("%1, %2").arg(resultPoint.x).arg(resultPoint.y);
//    QMessageBox msg;
//    msg.setWindowTitle(QObject::tr("坐标"));
//    msg.setText(message);
//    msg.setIcon(QMessageBox::Warning);
//    msg.addButton(QObject::tr("确定"),QMessageBox::ActionRole);
//    msg.exec();
}

PointCoordinates MapItem::PointClickToMap(const PointCoordinates clickPoint) {
    PointCoordinates mapPoint;
    // 计算地图上实际坐标
    mapPoint.x = clickPoint.x - mapOriginPoint.x;
    mapPoint.y = clickPoint.y - mapOriginPoint.y;
    PointCoordinates mapCorrectPoint;
    // 修正Heading后的坐标
    mapCorrectPoint.x = mapPoint.y*std::cos(globalOriginPointHeading) + mapPoint.x*std::sin(globalOriginPointHeading);
    mapCorrectPoint.y = mapPoint.x*std::cos(globalOriginPointHeading) - mapPoint.y*std::sin(globalOriginPointHeading);
//    std::cout << "mapPoint: " << mapPoint.x << ", " << mapPoint.y << "\nmapCorrectPoint: " << mapCorrectPoint.x << ", " << mapCorrectPoint.y << std::endl;
    return mapCorrectPoint;
}

PointCoordinates MapItem::PointMapToGlobal(const PointCoordinates mapPoint) {
    PointCoordinates globalPoint;
    globalPoint.x = mapPoint.x + globalOriginPoint.x;
    globalPoint.y = mapPoint.y + globalOriginPoint.y;
//    std::cout << "globalPoint: " << globalPoint.x << ", " << globalPoint.y << std::endl << std::endl;
    return globalPoint;
}

PointCoordinates MapItem::GetMouseMapPoint() {
    return mouseMapPoint;
}

PointCoordinates MapItem::GetMouseGlobalPoint() {
    return mouseGlobalPoint;
}

PointCoordinates MapItem::PointGlobalToPixel(const PointCoordinates globalPoint, int type = 1){
    // 全局坐标转修正地图坐标
    PointCoordinates mapCorrectPoint;
    mapCorrectPoint.x = globalPoint.x - globalOriginPoint.x;
    mapCorrectPoint.y = globalPoint.y - globalOriginPoint.y;
//    std::cout << "TEST: mapCorrectPoint: " << mapCorrectPoint.x << ", " << mapCorrectPoint.y << std::endl;

    // 修正地图坐标转地图坐标
    if(type != 0){//type为0 表示此时是静态图的坐标，暂时不用旋转角度
        PointCoordinates mapPoint;
        mapPoint.x = mapCorrectPoint.y*std::cos(-globalOriginPointHeading) - mapCorrectPoint.x*std::sin(-globalOriginPointHeading);
        mapPoint.y = mapCorrectPoint.x*std::cos(-globalOriginPointHeading) + mapCorrectPoint.y*std::sin(-globalOriginPointHeading);
    }
//    std::cout << "TEST: mapPoint: " << mapPoint.x << ", " << mapPoint.y << std::endl;

    // 地图坐标转鼠标点击坐标
    PointCoordinates clickPoint;
    clickPoint.x = mapPoint.x + mapOriginPoint.x;
    clickPoint.y = mapPoint.y + mapOriginPoint.y;
//    std::cout << "TEST: clickPoint: " << clickPoint.x << ", " << clickPoint.y << std::endl;

    // 鼠标单击坐标转图像坐标
    PointCoordinates pixelPoint;
    pixelPoint.x = (clickPoint.x/pixelDistance*pixmapMultiple)+this->pos().x();
    pixelPoint.y = itemHeight-this->pos().y()-(clickPoint.y/pixelDistance*pixmapMultiple);
//    std::cout << "TEST: pixelPoint: " << pixelPoint.x << ", " << pixelPoint.y << std::endl << std::endl << std::endl;

    return pixelPoint;
}

























