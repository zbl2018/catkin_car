#include "../include/car_map/mod_mapitem.hpp"
#include<stdio.h>

Mod_MapItem::Mod_MapItem(){
    //setSelected(true);
    setFlag(ItemIsFocusable, true);
    mod_map_path = "./src/car_map/resources/modificate_map/";
    mod_count = 0;
    BlackBrush = true;
    MaxBrushSize = 10;
    MinBrushSize = 0;
    ModBrushSize = 3;
    pixelDistance = 0.05;
}
Mod_MapItem::~Mod_MapItem(){

}
QRectF Mod_MapItem::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2, 0 - penWidth / 2, pixmapWidth+ penWidth, pixmapHeight + penWidth);
}

void Mod_MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);  //标明该参数没有使用
    Q_UNUSED(widget);
    ModPointCoordinates start_point;
    QVector<ModPointCoordinates> line_points;
    if(!pixmap.isNull()){
        painter->drawPixmap(0,0, pixmapWidth, pixmapHeight, pixmap);
        painter->setPen(Qt::white);
        //以下画出手动修改的地图边界
//        if(map_boundary.size()>0){
//             start_point= map_boundary.at(0);
//        }
        int i;
        //找到第一个黑色的点
        for(i = 0; i<map_boundary.size() ; i++){
            if(map_boundary.at(i).color == Color_Black){
                start_point = map_boundary.at(i);
                break;
            }
            else{
                //白色点不需要获取直线上的点
                painter->setPen(Qt::white);
                painter->drawPoint(map_boundary.at(i).x,map_boundary.at(i).y);
            }
        }
        for(i += 1 ; i < map_boundary.size() ; i++ ){//获取黑色点上所有直线上的点
            if(map_boundary.at(i).color == Color_White){
                painter->setPen(Qt::white);
                painter->drawPoint(map_boundary.at(i).x,map_boundary.at(i).y);
            }
            else{
                painter->setPen(Qt::black);
                line_points=GetPointsFromLine(start_point,map_boundary.at(i));
                for(int j = 0 ;j<line_points.size();j++){
                    painter->drawPoint(line_points.at(j).x,line_points.at(j).y);
                }
                start_point = map_boundary.at(i);
            }
        }
//        for(int i = 0;i<map_boundary.size();i++){
//            pen.setBrush(Qt::red);
//            //qDebug()<<map_boundary.at(i).x<<" "<<map_boundary.at(i).y;
//            //painter->drawPoint(drive_path.at(i).x,drive_path.at(i).y);
//            if(start_point.x != map_boundary.at(i).x && start_point.y != map_boundary.at(i).y){
//                pen.setBrush(Qt::black);
//                line_points = GetPointsFromLine(start_point,map_boundary.at(i));
//                for(int j = 0 ;j<line_points.size();j++){
//                    painter->drawPoint(line_points.at(j).x,line_points.at(j).y);
//                }
//                start_point = map_boundary.at(i);
//            }
//        }
    }
}

void Mod_MapItem::SetMapItemInfo(const int itemWidth, const int itemHeight, const QString all_path){
    QStringList file_path_split;
    this->itemWidth = itemWidth;
    this->itemHeight = itemHeight;
    this->map_all_path = all_path;
    map_boundary.clear();
    last_step_location_for_boundary.clear();
    file_path_split = all_path.split("/");
    this->mod_map_name = file_path_split.at(file_path_split.size()-1);
    //加载地图
    pixmap.load(map_all_path);
    // 设置地图信息
    pixmapWidth = pixmap.width(); //图片的宽度
    pixmapHeight = pixmap.height();  //图片的高度
//    if( pixmapWidth > pixmapHeight ) {
//        pixmapMultiple = (double)itemWidth / (double)pixmapWidth;
//    } else {
//        pixmapMultiple = (double)itemHeight / (double)pixmapHeight;
//}
    //创建空间存储地图的二进制数据
    map_content.data = new unsigned char[pixmap.width()*pixmap.height()];
    if(!LoadPgmMapInfo(map_all_path,map_content)){
             qDebug()<<"load pgm fail";
    }
    update();
}

void Mod_MapItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    ModPointCoordinates clickPoint;
    //鼠标右键事件，修改地图，画出地图边界
    if(mouseEvent->button() != Qt::LeftButton){
        if(BlackBrush){//添加障碍物范围
            clickPoint.x = mouseEvent->pos().x();
            clickPoint.y = mouseEvent->pos().y();
            clickPoint.color = Color_Black;
            map_boundary.append(clickPoint);
            NumPointsInPerStep.push(1);//此时仅加入一个点
            last_step_location_for_boundary.clear();
            NumPointsRetreated.clear();
            //qDebug()<<"color:"<<clickPoint.color;
            update();
        }
        else{//扫除障碍物
            int num_point = 0;
            int top_left_x,top_left_y,right_down_x,right_down_y;
            top_left_x = mouseEvent->pos().x() - ModBrushSize;
            top_left_y = mouseEvent->pos().y() - ModBrushSize;
            right_down_x = mouseEvent->pos().x() + ModBrushSize;
            right_down_y = mouseEvent->pos().y() + ModBrushSize;
            //map_boundary.append(clickPoint);
            for(int i = top_left_x; i<=right_down_x ; i++){//根据画刷大小，修改当前点的位置周围画刷大小范围的点为白色
                for(int j = top_left_y; j<= right_down_y; j++){
                    clickPoint.x = i;
                    clickPoint.y = j;
                    clickPoint.color = Color_White;
                    map_boundary.append(clickPoint);
                    num_point++;
                }
            }
            NumPointsInPerStep.push(num_point);//记录此步操作加入的点数
            last_step_location_for_boundary.clear();
            NumPointsRetreated.clear();
            update();
        }

    }
    else{
        // 计算鼠标位置
        // 坐标原点在地图左下角！！！
        // 将鼠标与地图的坐标系统统一
        ModPointCoordinates clickPoint;
        clickPoint.x = mouseEvent->pos().x()*pixelDistance;
        clickPoint.y = (pixmapHeight-mouseEvent->pos().y())*pixelDistance;
//        // 注意地图坐标与小车坐标X、Y是反的！
//        mouseMapPoint = PointClickToMap(clickPoint);
//        mouseGlobalPoint = PointMapToGlobal(mouseMapPoint);
        QString message = QString("%1, %2").arg( clickPoint.x).arg(clickPoint.y);
        qDebug()<<message;
        return;
        // 鼠标左键事件
    }

}
void Mod_MapItem::keyPressEvent(QKeyEvent *event){
    bool res;
    //保存修改的图片
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_S))
        {
            res = ShowTip("确定现在对地图进行修改吗？\n注意：修改后无法回退到上一步操作，若取消，需重新画边界");
            if(res == true){
                //qDebug()<<"11111";
                ModificateMap(map_boundary);
                return;
            }       
        }
    //切换操作：对地图添加障碍物范围
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_B)){
           BlackBrush = true;
           Q_EMIT Signal_BrushStatus(0);
           return;
        }
    //切换操作：扫除地图的障碍物
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_P)){
           BlackBrush = false;
           Q_EMIT Signal_BrushStatus(ModBrushSize);
           return;
     }
    //增大画刷的范围
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_M)){
        //此时若处于添加障碍物状态，则返回
        if(BlackBrush)
            return;
        if(ModBrushSize < MaxBrushSize){
            ModBrushSize++;
            Q_EMIT Signal_BrushStatus(ModBrushSize);
            return;
        }
        else{
            return;
        }
      }
    //缩小画刷的范围
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_L)){
        //此时若处于添加障碍物状态，则返回
        if(BlackBrush)
            return;
        if(ModBrushSize > MinBrushSize){
            Q_EMIT Signal_BrushStatus(ModBrushSize);
            ModBrushSize--;
            return;
        }
        else{
            return;
        }
      }
    //回退上一步的画图操作
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Z)){
        if(map_boundary.size() == 0 || NumPointsInPerStep.size() == 0){
             return;
        }
        int num_point = NumPointsInPerStep.pop();//本次需要回退多少个点
        NumPointsRetreated.push(num_point);//记录回退的点数
        for(int i = 0; i <num_point; i++){
            last_step_location_for_boundary.push(map_boundary.at(map_boundary.size()-1));
            map_boundary.pop_back();
        }
        update();
        return;
    }
    //回到下一步操作
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Y)){
        if(last_step_location_for_boundary.size() == 0 ||NumPointsRetreated.size() == 0){
             return;
        }
        int num_retreated = NumPointsRetreated.pop();//回到下一步的点数
        NumPointsInPerStep.push(num_retreated);//记录此步的点数
        for(int i = 0; i <num_retreated; i++){
            map_boundary.append(last_step_location_for_boundary.pop());
        }
        update();
        return;
    }
    //打开文件对话框
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_F)){
        //Q_EMIT Signal_OpenNewMap(true);
        //qDebug()<<"4444";
    }
    //打开帮助文档
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_H)){
        Q_EMIT Signal_OpenHelpManual(true);
        //qDebug()<<"4444";
    }
}

bool Mod_MapItem::isFileExist(QString fullFilePath)
{
    QFileInfo fileInfo(fullFilePath);
    if(fileInfo.exists())
    {
        return true;
    }
    return false;
}
bool Mod_MapItem::ModificateMap(QVector<ModPointCoordinates> map_boundary){
    ModPointCoordinates start_point;
    QVector<ModPointCoordinates> points_map_boundary;
    unsigned short BlackCellValue = 0;
    unsigned short WhiteCellValue = 255;
    //qDebug()<<"2222:"<<real_map_boundary.size();
    int i;
    //找到第一个黑色的点
    for(i = 0; i<map_boundary.size() ; i++){
        if(map_boundary.at(i).color == Color_Black){
            start_point = map_boundary.at(i);
            break;
        }
        else{
            //白色点不需要获取直线上的点
            points_map_boundary.append(map_boundary.at(i));
        }
    }
    for(i += 1 ; i < map_boundary.size() ; i++ ){//获取黑色点上所有直线上的点
        if(map_boundary.at(i).color == Color_White){
            points_map_boundary.append(map_boundary.at(i));
        }
        else{
            points_map_boundary+=GetPointsFromLine(start_point,map_boundary.at(i));
            start_point = map_boundary.at(i);
        }
    }
    //修改地图数据
    for(int i=0;i<points_map_boundary.size();i++){
        //qDebug()<<"points:"<<points_map_boundary.at(i).color;
        if(points_map_boundary.at(i).color == Color_Black){
            map_content.data[(int)floor(points_map_boundary.at(i).y)*pixmap.width()+(int)floor(points_map_boundary.at(i).x)] = (unsigned char)BlackCellValue;
        }
        else {
            map_content.data[(int)floor(points_map_boundary.at(i).y)*pixmap.width()+(int)floor(points_map_boundary.at(i).x)] = (unsigned char)WhiteCellValue;
        }
        //qDebug()<<"value:"<<(unsigned short)map_content.data[(int)floor(points_map_boundary.at(i).y)*pixmap.width()+(int)floor(points_map_boundary.at(i).x)];
    }
    //qDebug()<<"size:"<<points_map_boundary.size();
    //重新生成地图
    QString now_time = GetSystemTime();
    QString modificate_map_path = mod_map_path+QString("%1_%2").arg(now_time).arg(mod_map_name);
    mod_count++;
    //QString backup_mapPath = "./backup.pgm";
    if(!WritePgmMapInfo(modificate_map_path,map_content))
         qDebug()<<"load pgm fail";
    return true;
}
QVector<ModPointCoordinates> Mod_MapItem::GetPointsFromLine(ModPointCoordinates start_point,ModPointCoordinates end_point){
    double x_len,y_wid;
    //double x,y;
    QVector<ModPointCoordinates> PointsOnLine;
    ModPointCoordinates point_temp;
    x_len = fabs(start_point.x - end_point.x);
    y_wid = fabs(start_point.y - end_point.y);
    //根据相似三角形定理，求直线上的所有点(间隔为0.1个像素)
    if(x_len < 1){//x的距离小于1个像素，认为其是一条直线
        point_temp.x = start_point.x;
        if(start_point.y < end_point.y){
            for(double i=0.3;i<y_wid;i+=0.3){
                point_temp.y = start_point.y + i;
                point_temp.color = Color_Black;
                PointsOnLine.append(point_temp);
            }
        }
        else{
            for(double i=0.3;i<y_wid;i+=0.3){
                point_temp.y = start_point.y - i;
                point_temp.color = Color_Black;
                PointsOnLine.append(point_temp);
            }
        }
    }
    else{
        for(double i = 0.1; i<x_len ; i+=0.1){
            if(start_point.x < end_point.x)
                point_temp.x = start_point.x + i;
            else point_temp.x = start_point.x - i;
            if(start_point.y < end_point.y)
                point_temp.y = start_point.y + i/x_len*y_wid;
            else point_temp.y = start_point.y - i/x_len*y_wid;
                point_temp.color = Color_Black;
                PointsOnLine.append(point_temp);
        }
    }
    return PointsOnLine;
}
bool Mod_MapItem::LoadPgmMapInfo(QString MapPath,PgmGrid &map_content){
    QFile pgm_file(MapPath);
    char temp;
    if(pgm_file.open(QIODevice::ReadOnly)){
        //QTextStream readin(&pgm_file);
            for(int i = 0;i<4;i++)
               map_content.map_head[i]= pgm_file.readLine();
            for(int col = 0;col<pixmap.height();col++)
                for(int row = 0;row<pixmap.width();row++){
                    pgm_file.getChar(&temp);
                    map_content.data[col*pixmap.width()+row] = (unsigned char)temp;
//                    if((unsigned short)map_content.data[col*pixmap.width()+row]!=128)
//                    qDebug()<<"int:"<<(unsigned short)map_content.data[col*pixmap.width()+row];
                }

    }else{
        return false;
    }
    pgm_file.close();
    return true;
}
bool Mod_MapItem::WritePgmMapInfo(QString backup_mapPath, PgmGrid map_content){
    //进度条
    QProgressDialog status_process;
    status_process.setLabelText(tr("正在修改图片，请稍后..."));
    status_process.setRange(0,pixmapWidth*pixmapHeight);
    status_process.setModal(true);
    //status_process.setCancelButtonText(tr("取消"));
    status_process.setMinimumDuration(0);

//    status_process.setValue(pixmapWidth*pixmapHeight);
    QFile pgm_file(backup_mapPath);
    //QString line;
    if(pgm_file.open(QIODevice::WriteOnly)){
        //QTextStream WriteIn(&pgm_file);
        for(int i = 0 ; i< 4;i++){
//            line =  map_content.map_head[i]+"\n";
            pgm_file.write(map_content.map_head[i].toStdString().c_str());
        }
        for(int col = 0;col<pixmap.height();col++)
            for(int row = 0;row<pixmap.width();row++){
                 pgm_file.putChar((char)map_content.data[col*pixmap.width()+row]);
                 //status_process.setValue(col*pixmap.width()+row);
            }
        //qDebug()<<"222";
    }
    else{
        return false;
    }
    ShowTip("图片已生成，生成的图片保存路径为："+backup_mapPath);
    pgm_file.close();
    return true;
}
bool Mod_MapItem::ShowTip(QString tip){
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
QString Mod_MapItem::GetSystemTime(){
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd_hh:mm:ss_ddd");
    return current_date;
}
