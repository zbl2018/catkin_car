/**
 * @file /include/car_map/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
#ifndef car_map_QNODE_HPP_
#define car_map_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include <QProcess>
#include <QMutex>
#include <QDebug>
#include <std_msgs/String.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32MultiArray.h>
#include <sstream>
#include <iostream>
#include "MapItem.hpp"
enum{
    S_RUNNING,
    S_STOPED,
    S_RosShutDown
};
using namespace std;
namespace car_map {

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
    //void execProcess();
	bool init(const std::string &master_url, const std::string &host_url);
	void run();
    bool ShutDownNode();
	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };
	QStringListModel* loggingModel() { return &logging_model; }
	void log( const LogLevel &level, const std::string &msg);
    static void LocationHandler(const std_msgs::String::ConstPtr &msg);
    static void MapIdHandler(const std_msgs::Int8::ConstPtr &msg);
    static void DStarPathHandler(const std_msgs::Float32MultiArray::ConstPtr &msg);
Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown(int);
    //void QprocessStatus(int);
    //void ThreadExit(bool);
    void Signal_Carlocation(PointCoordinates,int);
public Q_SLOTS:
    void Slot_GetEndPointFromMainWin(string);
private:
	int init_argc;
	char** init_argv;
    //发布句柄
	ros::Publisher chatter_publisher;
    ros::Publisher EndPoint_publisher;
    QStringListModel logging_model;
    ros::V_string nodes;

//    bool m_stop;
//    QMutex m_mutex;
//    QMutexLocker locker;
//    QProcess *myProcess;
//    QProcess *roscore_process;
    //位置更新标识
    static bool update_location;
    //规划路径更新标识
    static bool update_planpath;
    //存储从topic接收到的点的信息
    static PointCoordinates location_point;
    static QVector<PointCoordinates> PlanPath;
    //存储接收到的topic信息
    static string topic_msg;
    //存储接收到的子图编号
    static int submap_number;
    //订阅句柄
    ros::Subscriber SubTopic_final_data;
    ros::Subscriber SubTopic_map_id;
    ros::Subscriber SubTopic_DStar_PlanPath;
};

}  // namespace car_map

#endif /* car_map_QNODE_HPP_ */
