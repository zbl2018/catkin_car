#include "../include/car_map/MapItem.hpp"
MapItem::MapItem() {
   //drive_path = ReadMapTxt("/home/deepdriving/shenrk/xc_Project/catkin_car/src/obstacle_runout/output/static_grid_map.txt");

}

QRectF MapItem::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2, 0 - penWidth / 2, pixmapWidth*pixmapMultiple+ penWidth, pixmapHeight*pixmapMultiple + penWidth);
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
    if( !pixmap.isNull() ){
        //pixmap = pixmap.scaled(pixmapWidth*pixmapMultiple, pixmapHeight*pixmapMultiple, Qt::KeepAspectRatio);  //缩放并保持宽高比
        painter->drawPixmap(0,0, pixmapWidth*pixmapMultiple, pixmapHeight*pixmapMultiple, pixmap);
        QPen pen;
        pen.setWidth(2);
        pen.setBrush(Qt::red);
        painter->setPen(pen);
        PointCoordinates point;
        if(drive_path.size() > 0){
//            qDebug()<<"1666666666666666666666666666666666666666666666666666";
            qDebug()<<"size:"<<drive_path.size();
        }
        for(int i = 0;i<drive_path.size();i++){
            //qDebug()<<drive_path.at(i).x<<" "<<drive_path.at(i).y;
            //point = PointGlobalToPixel(drive_path.at(i));
            painter->drawPoint(drive_path.at(i).x,drive_path.at(i).y);
            //painter->drawText(point.x,point.y,QString("%1,%2").arg(point.x).arg(point.y));
            //qDebug()<<"1111:"<<point.x<<" "<<point.y;
        }
    }
}

MapItem::MapItem(const int itemWidth, const int itemHeight, const QString path, const int mapNumber) {
    this->itemWidth = itemWidth;
    this->itemHeight = itemHeight;
    this->path = path;
    this->mapNumber = mapNumber;
   // connect()
    LoadMapPixmap();
}
void MapItem::SetMapItemInfo(const int itemWidth, const int itemHeight, const QString path, const int mapNumber) {
    this->itemWidth = itemWidth;
    this->itemHeight = itemHeight;
    this->path = path;
    this->mapNumber = mapNumber;
   // connect()
    LoadMapPixmap();
}
void MapItem::SetPath(const QString path) {
    this->path = path;
    LoadMapPixmap();
}

void MapItem::SetMapNumber(int &mapNumber, int AddOrDec) {
    this->mapNumber = mapNumber;
    QString mapPath = path+QString("%1.pgm").arg(mapNumber);
    if(!isFileExist(mapPath)){
        //qDebug()<<"1111";
        if(AddOrDec == 0)//decline option
            mapNumber++;
        else mapNumber--;//add option
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("error"));
        msg.setText(QObject::tr("map inexistence"));
        msg.setIcon(QMessageBox::Warning);
        msg.addButton(QObject::tr("Yes"),QMessageBox::ActionRole);
        msg.exec();
        return;
    }else{
        LoadMapPixmap();
    }
}

void MapItem::LoadMapPixmap() {
    QString mapPath = path+QString("%1.pgm").arg(mapNumber);
    //qDebug()<<"file:"<<mapPath<<endl;
    if(!isFileExist(mapPath)) {
//        QMessageBox::warning(this, QObject::tr("警告"), QObject::tr("图像不存在！"), QMessageBox::Ok);
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("error"));
        msg.setText(QObject::tr("map inexistence"));
        msg.setIcon(QMessageBox::Warning);
        msg.addButton(QObject::tr("Yes"),QMessageBox::ActionRole);
        msg.exec();
    } else {
        // 读取地图信息文件
        pixmap.load(mapPath);
//        QImage image(mapPath);
//        QRgb pixel=image.pixel(1,1);
//        int r=qRed(pixel);
//        int g=qGreen(pixel);
//        int b=qBlue(pixel);
//        qDebug()<<"rgb:"<<r<<" "<<g<<" "<<b;
        QString mapMessagePath = path+QString("%1.yaml").arg(mapNumber);
        std::ifstream mapMessage(mapMessagePath.toStdString().c_str());
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
//                std::cout << "mapOriginPoint.x: " << mapOriginPoint.x << std::endl;
//                std::cout << "mapOriginPoint.y: " << mapOriginPoint.y << std::endl;
            } else {
                continue;
            }
        }
        // 获取原点全局信息
        QString originPointMessagePath = path+QString("map_spoint.txt");
        std::ifstream originPointMessage(originPointMessagePath.toStdString().c_str());
        //determine the number of the first map : 1 or 0
        QString FirstFileNmae = path+QString("%1.pgm").arg(0);
        int FirstFileNumber = 0;
        if(!isFileExist(FirstFileNmae)){
            FirstFileNumber =1;
        }
        //qDebug()<<"1111:"<<FirstFileNumber;
        for( int i = FirstFileNumber; i <= mapNumber; i++ ) {
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
    update();
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    // 计算鼠标位置
    // 坐标原点在地图左下角！！！
    //this->pos().y()，为item的原点坐标
    PointCoordinates clickPoint;
    clickPoint.x = (mouseEvent->pos().x()-this->pos().x())*(pixelDistance/pixmapMultiple);
    clickPoint.y = (pixmapHeight*pixmapMultiple-mouseEvent->pos().y()-this->pos().y())*(pixelDistance/pixmapMultiple);
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
    QString message = QString("%1 %2").arg(mouseGlobalPoint.x).arg(mouseGlobalPoint.y);
    qDebug()<<message;
    //鼠标右键事件，发送任务点
    if(mouseEvent->button() != Qt::LeftButton){
        bool res = ShowTip(QString("是否发布任务点：%1").arg(message));
        if(res){
            qDebug()<<"1";
            Q_EMIT Signal_SendEndPoint(message.toStdString());
        }
    }
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

PointCoordinates MapItem::PointGlobalToPixel(const PointCoordinates globalPoint ,int type){
    // 全局坐标转修正地图坐标
    PointCoordinates mapCorrectPoint;
    mapCorrectPoint.x = globalPoint.x - globalOriginPoint.x;
    mapCorrectPoint.y = globalPoint.y - globalOriginPoint.y;
//    std::cout << "TEST: mapCorrectPoint: " << mapCorrectPoint.x << ", " << mapCorrectPoint.y << std::endl;

    // 修正地图坐标转地图坐标
    PointCoordinates mapPoint;
    if(type != 0){//type为0时，表示需要转全局的坐标是一个静态地图，暂时不需要转全局角度
        mapPoint.x = mapCorrectPoint.y*std::cos(-globalOriginPointHeading) - mapCorrectPoint.x*std::sin(-globalOriginPointHeading);
        mapPoint.y = mapCorrectPoint.x*std::cos(-globalOriginPointHeading) + mapCorrectPoint.y*std::sin(-globalOriginPointHeading);
    }
    else{
        mapPoint.x = mapCorrectPoint.y;
        mapPoint.y = mapCorrectPoint.x;
    }

//    std::cout << "TEST: mapPoint: " << mapPoint.x << ", " << mapPoint.y << std::endl;

    // 地图坐标转鼠标点击坐标
    PointCoordinates clickPoint;
    clickPoint.x = mapPoint.x + mapOriginPoint.x;
    clickPoint.y = mapPoint.y + mapOriginPoint.y;
    //std::cout << "TEST: clickPoint: " << clickPoint.x << ", " << clickPoint.y << std::endl;

    // 鼠标单击坐标转图像坐标
    PointCoordinates pixelPoint;
    pixelPoint.x = (clickPoint.x/pixelDistance*pixmapMultiple)+this->pos().x();
    pixelPoint.y = pixmapHeight*pixmapMultiple-this->pos().y()-(clickPoint.y/pixelDistance*pixmapMultiple);
    //std::cout << "TEST: pixelPoint: " << pixelPoint.x << ", " << pixelPoint.y << std::endl << std::endl << std::endl;

    return pixelPoint;
}
bool MapItem::isFileExist(QString fullFilePath)
{
    QFileInfo fileInfo(fullFilePath);
    if(fileInfo.exists())
    {
        return true;
    }
    return false;
}
//void MapItem::Slot_DrawLocation(PointCoordinates global_point){
//    PointCoordinates pixel_point = PointGlobalToPixel(global_point);
//    QPainter painter;
//    QPen pen;
//    pen.setWidth(5);
//    pen.setBrush(Qt::yellow);
//    painter.setPen(pen);
//    painter.drawPoint(pixel_point.x,pixel_point.y);
//}
void MapItem::SetDrivePath(PointCoordinates point){
    PointCoordinates pixel_point = PointGlobalToPixel(point);
    drive_path.append(pixel_point);
    update();
}
QVector<PointCoordinates> MapItem::ReadMapTxt(QString MapTxtPath){
    drive_path.clear();
    QFile file(MapTxtPath);
    QString line;
    QStringList LineList;
    PointCoordinates point;
    QVector<PointCoordinates> point_vector;
    QStringList FileNameList;
    FileNameList = MapTxtPath.split("/");
    QString file_name = FileNameList.at(FileNameList.size()-1);
    qDebug()<<"filename:"<<file_name;
    QString x,y,degree;
    if(file.open(QIODevice::ReadOnly)){
        qDebug()<<"yes";
        while(!file.atEnd()){
            line = file.readLine();
            if(line.isEmpty()){
                continue;
            }
            LineList = line.split(" ");
            if(LineList.size()<2){
                continue;
            }
            x = LineList.at(0);
            y = LineList.at(1);
            //qDebug()<<"333:"<<x;
            if(file_name.compare("global_obstacle.txt") == 0||file_name.compare("local_obstacle.txt") == 0)
            {
                point.x = x.toDouble();
                point.y = y.toDouble();
                point = PointGlobalToPixel(point);//转全局
                point_vector.append(point);
                //qDebug()<<"3:"<<point.x<<" "<<point.y;
                continue;
            }
            if(file_name.compare("dynamic_grid_map.txt") == 0)
            {
                degree = LineList.at(2);
                if(degree.toDouble()== -1){
                    point.x = x.toDouble();
                    point.y = y.toDouble();
                    point = PointGlobalToPixel(point,0);//转全局
                    point_vector.append(point);
                    //continue;
                    qDebug()<<"4:"<<point.x<<" "<<point.y;
                    continue;
                }

            }
            if(file_name.compare("dstar_path.txt") == 0){
                point.x = x.toDouble();
                point.y = LineList.at(2).toDouble();
                point = PointGlobalToPixel(point);//转全局
                point_vector.append(point);
                //qDebug()<<"2:"<<point.x<<" "<<point.y;
                continue;
            }
            if(file_name.compare("static_grid_map.txt") == 0){
                degree = LineList.at(2);
                if(degree.toDouble()/144 > 0.1){
                    point.x = x.toDouble();
                    point.y = y.toDouble();
                    point = PointGlobalToPixel(point,0);//转全局
                    point_vector.append(point);
                    continue;
                    //qDebug()<<"4:"<<point.x<<" "<<point.y;
                }
            }
            else{
                file_name = file_name = FileNameList.at(FileNameList.size()-2);
                if(file_name.compare("global_obstacle") == 0){
                        point.x = x.toDouble();
                        point.y = y.toDouble();
                        point = PointGlobalToPixel(point,0);//转全局
                        point_vector.append(point);
                        continue;
                        //qDebug()<<"4:"<<point.x<<" "<<point.y;
                }
                if(file_name.compare("dynamic_grid_map") == 0){
                    degree = LineList.at(2);
                    if(degree.toDouble()== -1){
                        point.x = x.toDouble();
                        point.y = y.toDouble();
                        point = PointGlobalToPixel(point,0);//转全局
                        point_vector.append(point);
                        //continue;
                        qDebug()<<"4:"<<point.x<<" "<<point.y;
                        continue;
                    }

                }
                else{
                    if(LineList.size()<3)
                        continue;
                    point.x = x.toDouble();
                    point.y = LineList.at(2).toDouble();
                    point = PointGlobalToPixel(point);//转全局
                    point_vector.append(point);
                    //qDebug()<<"2:"<<point.x<<" "<<point.y;
                    continue;
                }
            }
        }
        file.close();
        return point_vector;
    }
    else qDebug()<<"no";
}
void MapItem::DrawPath(QString file_path){
    drive_path = ReadMapTxt(file_path);
    update();
}
bool MapItem::ShowTip(QString tip){
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



















