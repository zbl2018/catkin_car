/**
 * @file /include/car_map/main_window.hpp
 *
 * @brief Qt based gui for car_map.
 *
 * @date November 2010
 **/
#ifndef car_map_MAIN_WINDOW_H
#define car_map_MAIN_WINDOW_H

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include <QProcess>
#include "MapItem.hpp"
#include "mod_mapitem.hpp"
#include <QtGlobal>
#include "GlobalMapItem.hpp"
#define foreach Q_FOREACH

namespace car_map {

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();
    //Load up qt program settings at startup（默认保存每次的操作配置）
    void ReadSettings();
    //Save qt program settings when closing
    void WriteSettings();
    //关闭界面函数
    void closeEvent(QCloseEvent *event);
	void showNoMasterMessage();
    //自定义提示框函数
    bool ShowTip(QString tip);
    //根据目录名称，获取指定目录下以yaml为后缀的文件数目
    int GetFileNumFromDir(QString dir_name);
    //文件对话框,返回选择的文件路径（若选择多个文件，则只返回第一个文件路径）
    QString selectFile();
    //以字符读取形式，读取指定文件的全部内容，返回QString类型的内容
    QString ReadAllInfoFromFile(QString file_path);
protected:
   //键盘事件
    void keyPressEvent(QKeyEvent *event);
public Q_SLOTS:
    //槽函数，接收信号消息
	void on_actionAbout_triggered();
    //连接按钮函数
	void on_button_connect_clicked(bool check );
    //可选按钮函数
	void on_checkbox_use_environment_stateChanged(int state);
    //更新logview界面
    void updateLoggingView(); // no idea why this can't connect automatically

    //void on_button_connect_clicked();
    void on_pushButton_clicked();
    //在界面显示画刷状态
    void Slot_ShowBrushStatus(int);

    void on_pushButton_2_clicked();
    //在view的地图上添加小车位置信息
    void Slot_DrawLocation(PointCoordinates,int);
    //打开了一个新的pgm文件
    void Slot_SelectNewMap(bool);
    //从item处获取到了任务点信息，将其转发给qnode对象
    void Slot_GetEndPointFromItem(string);
    //接收打开帮助文档的消息
    void Slot_OpenHelpManual(bool);
    //用来显示程序中出现的异常情况，在主界面中显示
    void Slot_ShowProcessStatus(int);

    //void on_command_panel_visibilityChanged(bool visible);
Q_SIGNALS:
    //void connect_clicked(QProcess[],int);
    //发送任务点给qnode对象
    void Signal_ContinueSendEndPoint(string);

private:
    //以下是定义item、scene、qnode对象
    //view在ui设计那里定义，直接通过窗口对象的指针ui调用
	Ui::MainWindowDesign ui;
	QNode qnode;
    QGraphicsScene * car_map_scene;
    MapItem *car_map_item;
    GlobalMapItem * car_global_map_item;
    Mod_MapItem *car_map_mod_item;
    QGraphicsScene *car_map_mod_scene;
    QGraphicsScene *car_global_map_scene;
    //地图总数，仅测试时需要到，可以忽略
    int map_num;
    //小车当前所处的子图编号
    int map_count;
    //int map_now;
    //mapitem对象的默认路径（仅用来测试）
    string map_dir;
    //文件选择框的默认搜索路径
    QString DefaultSelectFilePath;
    //帮助文档保存路径
    QString HelpManualPath;
    //connect按钮是否被按下
    bool HaveConnected;
};

}  // namespace car_map

#endif // car_map_MAIN_WINDOW_H
