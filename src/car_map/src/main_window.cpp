/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/car_map/main_window.hpp"

namespace car_map {

using namespace Qt;

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
    // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    ui.setupUi(this);
    // qApp is a global variable for the application
    QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    if(ui.checkbox_remember_settings->isChecked()){
        //程序启动时读取上一次保存的配置信息
        ReadSettings();
    }
	setWindowIcon(QIcon(":/images/icon.png"));
	ui.tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).

	ui.view_logging->setModel(qnode.loggingModel());
    QObject::connect(&qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));

    if ( ui.checkbox_remember_settings->isChecked() ) {
        on_button_connect_clicked(true);
    }
    //ui.map_view->resize(541,558);
    //=============init class member variable==========
    HaveConnected = false;//connect 按钮未被选中
    HelpManualPath = "./src/car_map/resources/help/help.txt";
    map_count = 1;
    //map_now = 0;
    map_dir = "./src/car_map/resources/yingno/data/";
    map_num = GetFileNumFromDir(map_dir.c_str());
    DefaultSelectFilePath = "/home/deepdriving/shenrk/xc_Project/catkin_car/src/obstacle_runout/output";
    //============add item to map view==================
    car_map_scene = new QGraphicsScene;
    car_map_mod_scene = new QGraphicsScene;
    car_global_map_scene = new QGraphicsScene;
    //car_map_item = new MapItem[map_num+1];
    car_map_item = new MapItem;
    car_map_mod_item = new Mod_MapItem;
    car_global_map_item = new GlobalMapItem;
    //设置mapitem对象信息
    car_map_item->SetMapItemInfo(500,500,map_dir.c_str(),1);
    //将item对象添加到scene对象中
    car_map_scene->addItem(car_map_item);
    car_map_mod_scene->addItem(car_map_mod_item);
    car_global_map_scene->addItem(car_global_map_item);

    //设置整个view的背景颜色
    car_map_scene->setBackgroundBrush(QColor(128,128,128));
    car_map_mod_scene->setBackgroundBrush(QColor(128,128,128));
    //car_global_map_scene->setBackgroundBrush(QColor(0,0,0));
    //将scene对象添加view中
    ui.map_view->setScene(car_map_scene);
    ui.map_mod_view->setScene(car_map_mod_scene);
    ui.global_map_view->setScene(car_global_map_scene);
    //=============connect function================
    //接收qnode处发来的小车位置、当前子图信息，在地图上显示
    connect(&qnode,SIGNAL(Signal_Carlocation(PointCoordinates,int)),this,SLOT(Slot_DrawLocation(PointCoordinates,int)));
    //接收mod_mapitem对象传来的打开文件选择框的命令，打开文件选择框
    connect(car_map_mod_item,SIGNAL(Signal_OpenNewMap(bool)),this,SLOT(Slot_SelectNewMap(bool)));
    //接收mod_mapitem对象传来的显示画刷状态的消息，在主界面显示
    connect(car_map_mod_item,SIGNAL(Signal_BrushStatus(int)),this,SLOT(Slot_ShowBrushStatus(int)));
    //接收mapitem出传来的任务点消息，此处只做测试使用
    connect(car_map_item,SIGNAL(Signal_SendEndPoint(string)),this,SLOT(Slot_GetEndPointFromItem(string)));
    //接收全局地图传来的任务点信息
    connect(car_global_map_item,SIGNAL(Signal_SendEndPoint(string)),this,SLOT(Slot_GetEndPointFromItem(string)));
    //将任务点信息转发给qnode对象，让其发布节点消息
    connect(this,SIGNAL(Signal_ContinueSendEndPoint(string)),&qnode,SLOT(Slot_GetEndPointFromMainWin(string)));
    //打开帮助文档
    connect(car_map_mod_item,SIGNAL(Signal_OpenHelpManual(bool)),this,SLOT(Slot_OpenHelpManual(bool)));
    //接收与ros断开的消息
    connect(&qnode, SIGNAL(rosShutdown(int)), this, SLOT(Slot_ShowProcessStatus(int)));
    //new_thread->start();//start the new thread
}

MainWindow::~MainWindow() {}

void MainWindow::showNoMasterMessage(){
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    //close();
}

void MainWindow::on_button_connect_clicked(bool check){
    if (ui.checkbox_use_environment->isChecked()){
        if(!HaveConnected){
            if (!qnode.init()){
                showNoMasterMessage();
            } else {
                HaveConnected = true;
                ui.checkbox_use_environment->setEnabled(false);
                ui.button_connect->setText(QString("disconnect"));
                //ui.button_connect->setEnabled(false);
            }
        }
        else{
            //断开与ros的连接
            qnode.ShutDownNode();
            HaveConnected = false;
            ui.checkbox_use_environment->setEnabled(true);
            ui.button_connect->setText(QString("connect"));
        }

    }else{
        if(!HaveConnected){
            if (!qnode.init(ui.line_edit_master->text().toStdString(),
                       ui.line_edit_host->text().toStdString())){
                showNoMasterMessage();
            }else{
                HaveConnected = true;
                ui.checkbox_use_environment->setEnabled(false);
                ui.button_connect->setText(QString("disconnect"));
                //ui.button_connect->setEnabled(false);
                ui.line_edit_master->setReadOnly(true);
                ui.line_edit_host->setReadOnly(true);
                ui.line_edit_topic->setReadOnly(true);
            }
        }
        else{
            //断开与ros的连接
            qnode.ShutDownNode();
            HaveConnected = false;
            ui.checkbox_use_environment->setEnabled(true);
            ui.button_connect->setText(QString("connect"));
            ui.line_edit_master->setReadOnly(false);
            ui.line_edit_host->setReadOnly(false);
            ui.line_edit_topic->setReadOnly(false);
        }
	}
}

void MainWindow::on_checkbox_use_environment_stateChanged(int state){
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui.line_edit_master->setEnabled(enabled);
	ui.line_edit_host->setEnabled(enabled);
	//ui.line_edit_topic->setEnabled(enabled);
}

void MainWindow::updateLoggingView() {
        ui.view_logging->scrollToBottom();
}

void MainWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "car_map");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    //ui.line_edit_topic->setText(topic_name);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);
    	ui.line_edit_host->setEnabled(false);
    	//ui.line_edit_topic->setEnabled(false);
    }
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", "car_map");
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));

}

void MainWindow::closeEvent(QCloseEvent *event)
{
	WriteSettings();
	QMainWindow::closeEvent(event);
}

void MainWindow::on_pushButton_clicked()
{

    map_count++;
    if(map_count > map_num){
        map_count--;//retreat
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("error"));
        msg.setText(QObject::tr("map inexistence"));
        msg.setIcon(QMessageBox::Warning);
        msg.addButton(QObject::tr("Yes"),QMessageBox::ActionRole);
        msg.exec();
        return;
    }else{
//        car_map_scene->removeItem(&car_map_item[map_count-1]);
//        car_map_scene->addItem(&car_map_item[map_count]);
        //map_count = map_count%map_num;
    }

//    car_map_item->SetMapNumber(map_count,1);
//    qDebug()<<map_num;
}

void MainWindow::on_pushButton_2_clicked()
{
    map_count--;
    if(map_count == 0){
        map_count++;//retreat
        QMessageBox msg;
        msg.setWindowTitle(QObject::tr("error"));
        msg.setText(QObject::tr("map inexistence"));
        msg.setIcon(QMessageBox::Warning);
        msg.addButton(QObject::tr("Yes"),QMessageBox::ActionRole);
        msg.exec();
        return;
        }else{
//            car_map_scene->removeItem(&car_map_item[map_count+1]);
//            car_map_scene->addItem(&car_map_item[map_count]);
            //map_count = map_count%map_num;
        }
}
void MainWindow::Slot_DrawLocation(PointCoordinates location_point,int submap_number){
    //将获取到的小车的实时位置在全局地图上显示
    car_global_map_item->AddPointToDrivePath(location_point);
    //将获取到的小车实时位置在测试地图（mapitem对象）上显示
    car_map_item->SetDrivePath(location_point);
    //记录当前子图编号
    map_count = submap_number;
}

int MainWindow::GetFileNumFromDir(QString dir_name){
    QDir dir(dir_name);
    dir.setFilter(QDir::Files);
    //加过滤器，选择hou缀为yaml的文件
    QStringList filters;
    filters << "*.yaml";
    dir.setNameFilters(filters);
    QFileInfoList list=dir.entryInfoList();
    QFileInfo fileInfo;
    QStringList filelist;
    foreach (fileInfo, list)
    {
        filelist << fileInfo.fileName();
    }
    int count = filelist.count();
    return count;
}
QString MainWindow::selectFile()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog;
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开图片"));
    //设置默认文件路径
    fileDialog->setDirectory(DefaultSelectFilePath);
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*pgm *.png *.jpg *.jpeg *.bmp *.txt)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //返回被选择的文件路径
//    return fileDialog->selectedFilter();
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }
    if(fileNames.isEmpty())
        return NULL;
    return fileNames.at(0);

}
void MainWindow::keyPressEvent(QKeyEvent *event){
    //打开文件对话框选择文件
    QStringList file_path_split_list;
    QString file_type;
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_F)){
        QString file_path = selectFile();
        //qDebug()<<file_path;
        if(file_path.isEmpty()){
            return;
        }
        else{
            file_path_split_list = file_path.split(".");
            file_type = file_path_split_list.at(file_path_split_list.size()-1);
            if(file_type.compare("txt") == 0){//判断文件类型
                car_map_item->DrawPath(file_path);
                ui.tab_manager->setCurrentWidget(ui.tab_map);
            }
            else{
                car_map_mod_item->SetMapItemInfo(500,500,file_path);
                ui.tab_manager->setCurrentWidget(ui.tab_mod_map);
            }
            //修改下次的默认打开文件路径
            file_path_split_list = file_path.split("/");
            file_path.replace(file_path_split_list.at(file_path_split_list.size()-1),QString(""));
            DefaultSelectFilePath = file_path;
            qDebug()<<"default:"<<DefaultSelectFilePath;
        }
        return;
    }
    //显示帮助文档
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_H)){
        QString info = ReadAllInfoFromFile(HelpManualPath);
        QMessageBox::about(NULL,"help manual",info);
        return;
    }
    //隐藏comman panel
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Q)){
        //ui.command_panel->close();
        //ui.command_panel->close();
        return;
    }
    //显示comman panel
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_E)){
        ui.command_panel->setVisible(true);
        return;
    }
}
void MainWindow::Slot_SelectNewMap(bool status){
    QStringList file_path_split_list;
    QString file_type;
    QString file_path;
    if(status){
        file_path = selectFile();
        if(file_path.isEmpty()){
            return;
        }
        else{
            file_path_split_list = file_path.split(".");
            file_type = file_path_split_list.at(file_path_split_list.size()-1);
            if(file_type.compare("txt") == 0){//判断文件类型
                ui.tab_manager->setCurrentWidget(ui.tab_map);
                car_map_item->DrawPath(file_path);
            }
            else{
                car_map_mod_item->SetMapItemInfo(500,500,file_path);
                ui.tab_manager->setCurrentWidget(ui.tab_mod_map);
            }
            //修改下次的默认打开文件路径
            file_path_split_list = file_path.split("/");
            file_path.replace(file_path_split_list.at(file_path_split_list.size()-1),QString(""));
            DefaultSelectFilePath = file_path;
            qDebug()<<"default:"<<DefaultSelectFilePath;
        }
        return;
    }
}
void MainWindow::Slot_ShowBrushStatus(int status){
    if(status == 0){
        ui.groupBox_mod_map->setTitle(tr("正在修改地图：添加障碍物"));
    }
    else{
        QString tip = QString(tr("正在修改地图：去除障碍物，画刷大小为%1")).arg(status);
        ui.groupBox_mod_map->setTitle(tip);
    }
}
void MainWindow::Slot_GetEndPointFromItem(string end_point_str){
    qDebug()<<"main window have sent";
    if(HaveConnected){
        Q_EMIT Signal_ContinueSendEndPoint(end_point_str);
    }
    else{
        ShowTip(QString("QT-DESKTOP未与ros连接，发送任务点失败，请先与ros master建立连接!"));
    }
}
QString MainWindow::ReadAllInfoFromFile(QString file_path){
    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly)){
       QTextStream in(&file);
       return in.readAll();
    }
    else{
        ShowTip(file_path.append(" this file isn't exist!"));
    }
}
bool MainWindow::ShowTip(QString tip){
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
void MainWindow::Slot_OpenHelpManual(bool status){
    Q_UNUSED(status);
    QString info = ReadAllInfoFromFile(HelpManualPath);
    QMessageBox::about(NULL,"help manual",info);
}
void MainWindow::Slot_ShowProcessStatus(int status){
    QString TipInfo;
    switch (status){
    case S_RosShutDown:{
        //被动与ros断开连接
        if(HaveConnected){
            HaveConnected = false;
            ui.checkbox_use_environment->setEnabled(true);
            ui.button_connect->setText(QString("connect"));
        }
        TipInfo = "已与ros断开连接！";
        break;
        }
    default:
        break;
    }
    ShowTip(TipInfo);
}
}  // namespace car_map

//void car_map::MainWindow::on_command_panel_visibilityChanged(bool visible)
//{
//    if(visible){
//        qDebug()<<"1111";
//    }
//    else{
//        qDebug()<<"2222";
//    }
//}
