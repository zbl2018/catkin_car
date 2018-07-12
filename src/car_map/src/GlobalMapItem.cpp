#include "../include/car_map/GlobalMapItem.hpp"
#include <math.h>

GlobalMapItem::GlobalMapItem()
{
    GlobalMapPath = "./src/car_map/resources/global_map/";
    BaseMapName = "yingnuo.pgm";
    PointsInMap_FileName = "yingnuo_map.txt";
    OriginPoint = {126,108};
    //XRatio、YRatio分别对应界面坐标的放大倍数
    XRatio = 62;
    YRatio = 82;
    GetBaseMapInfo();
    GetPointsFromMapFile();
}
QRectF GlobalMapItem::boundingRect() const{
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2-OriginPoint.x, 0 - penWidth / 2-OriginPoint.y, PixmapWidth+ penWidth+500, PixmapHeigth + penWidth+500);
}  // 设置边界
void GlobalMapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,  QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //double ScaleWidth,ScaleHeight;
    //设置画笔，颜色、宽度
    QPen pen;
    pen.setWidth(1);
    pen.setBrush(Qt::red);
    painter->setPen(pen);
    //缩放地图，画出建筑地图
//    ScaleWidth = fabs(map_x_max - map_x_min);
//    ScaleHeight = fabs(map_y_max - map_y_min);
    if(!pixmap.isNull()){
         painter->drawPixmap(0-OriginPoint.x,0-OriginPoint.y,PixmapWidth,PixmapHeigth,pixmap);
    }
    else qDebug()<<"11111";
//    //画出规划的全局路径图
//    for(int i = 0 ; i < AllPointsInMap.size() ; i++){
//        painter->drawPoint(AllPointsInMap.at(i).y*XRatio,-AllPointsInMap.at(i).x*YRatio);
//    }
    //画出小车实时行走的轨迹图
    //小车的y坐标是界面的x坐标，x坐标是界面坐标的-y坐标
    if(DrivePath.size() >1 ){
        PointCoordinates start_point;
        start_point = DrivePath.at(0);
        for(int i = 0 ; i < DrivePath.size(); i++){
            painter->drawPoint(DrivePath.at(i).y*XRatio,-DrivePath.at(i).x*YRatio);
            //painter->drawLine(start_point.y*XRatio,-start_point.x*YRatio,DrivePath.at(i).y*XRatio,-DrivePath.at(i).x*YRatio);
            start_point = DrivePath.at(i);
        }
    }

}
void GlobalMapItem::GetPointsFromMapFile(){
    QString path = GlobalMapPath+QString("%1").arg(PointsInMap_FileName);
    QString LineData;
    QStringList LineDataList;
    PointCoordinates point;
    //double x_min,x_max,y_min,y_max;
    map_x_min = map_y_min = 99999;
    map_x_max = map_y_max = -99999;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        while(!file.atEnd()){
            LineData = file.readLine();
            LineDataList = LineData.split(" ");
            if(LineDataList.size() == 2){
                point.x = LineDataList.at(0).toDouble();
                point.y = LineDataList.at(1).toDouble();
                AllPointsInMap.append(point);
                //寻找外切矩阵
                if(map_x_min > point.x)
                    map_x_min = point.x;
                if(map_x_max < point.x)
                    map_x_max = point.x;
                if(map_y_min > point.y)
                    map_y_min = point.y;
                if(map_y_max < point.y)
                    map_y_max = point.y;
            }
            else{
                continue;
            }

        }
    }
    else{
        ShowTip("打开规划路径地图失败！");
    }
}
void GlobalMapItem::GetBaseMapInfo(){
    QString path = GlobalMapPath+QString("%1").arg(BaseMapName);
    pixmap.load(path);
    PixmapHeigth = pixmap.height();
    PixmapWidth = pixmap.width();
}
bool GlobalMapItem::ShowTip(QString tip){
    int res;
    QMessageBox msg;
    msg.setWindowTitle(QObject::tr("提示"));
    msg.setText(tr(tip.toStdString().c_str()));
    msg.setIcon(QMessageBox::Warning);
    msg.addButton(QObject::tr("确定"),QMessageBox::ActionRole);
    msg.addButton(QObject::tr("取消"),QMessageBox::ActionRole);

    res = msg.exec();
    if(res == tr("确定").toInt()){
        return true;
    }
    else {
        return false;
    }
}
void GlobalMapItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)//鼠标按下事件
{
    //鼠标右键事件，发送任务点
    //PointCoordinates point;
    QString message;
    if(mouseEvent->button() != Qt::LeftButton){
        //小车的y坐标是界面的x坐标，x坐标是界面坐标的-y坐标
        message = QString("%1 %2").arg(-mouseEvent->pos().y()/YRatio).arg(mouseEvent->pos().x()/XRatio);
        //qDebug()<<"location:"<< message ;
        bool res = ShowTip(QString("是否发布任务点：%1").arg(message));
        if(res){
            //qDebug()<<"1";
            Q_EMIT Signal_SendEndPoint(message.toStdString());
        }
    }
}
void GlobalMapItem::AddPointToDrivePath(PointCoordinates point){
   DrivePath.append(point);
   update();
}
//bool GlobalMapItem::JudgeNextPointStatus(PointCoordinates start_point,PointCoordinates end_point){
//    int orientation;
//    if(end_point.heading > 45 && end_point.heading < 135)
//        orientation = 2;//x轴正方向
//    else if(end_point.heading < -45 && end_point.heading > -135)
//            orientation = 3;//x轴负方向
//        else if(end_point.heading > -45 && end_point.heading < 45)
//                orientation = 0;//y轴正方向
//            else if((end_point.heading > 135 && end_point.heading < 180) || (end_point.heading > -180 && end_pointading < -135))
//                    orientation = 1;//y轴负方向
//    switch(orientation){
//        case 0 : {
//               if(end_point.y < MapPath.at(i).y)
//                    return false;
//                else return true;
//        }
//        case 1 : {
//                if(real_time_location.y > MapPath.at(i).y)
//                    return false;
//                else return true;
//        }
//        case 2 : {
//                if(real_time_location.x < MapPath.at(i).x)
//                    return false;
//                else return true;
//        }
//        case 3 : {
//                if(real_time_location.x > MapPath.at(i).x)
//                    return false;
//                else return true;
//        }
//    }
//}
