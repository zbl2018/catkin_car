/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

#include <ros/ros.h>
#include <ros/network.h>
#include <std_msgs/String.h>
#include "../include/car_map/qnode.hpp"

namespace car_map {
//=====init static number============
bool QNode::update_location = false;
bool QNode::update_planpath = false;
PointCoordinates QNode::location_point = {0,0};
QVector<PointCoordinates> QNode::PlanPath;
string QNode::topic_msg="";
int QNode::submap_number = 1;

QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
    init_argv(argv)
    //locker(&m_mutex)
{
    //locker.unlock();
//    m_stop = false;
//    myProcess = new QProcess;
//    roscore_process = new QProcess;
//    roscore_process->start("roscore");
    //connect()
    //ros::
}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool QNode::init() {
    //std::cout<<"1111check false"<<endl;
    ros::init(init_argc,init_argv,"car_qt_desktop");
    if (!ros::master::check()){
        //std::cout<<"check false"<<endl;
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;
    // Add your ros communications here.
    EndPoint_publisher = n.advertise<std_msgs::String>("EndPoint", 2);
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
    SubTopic_final_data = n.subscribe<std_msgs::String>("final_data",10,LocationHandler);
    SubTopic_DStar_PlanPath = n.subscribe<std_msgs::Float32MultiArray>("d_star_plan_path",5,DStarPathHandler);
    //std::cout<<"location"<<endl;
    SubTopic_map_id = n.subscribe<std_msgs::Int8>("map_index",5,MapIdHandler);
	start();
	return true;
}

bool QNode::init(const std::string &master_url, const std::string &host_url) {
	std::map<std::string,std::string> remappings;
	remappings["__master"] = master_url;
	remappings["__hostname"] = host_url;
	ros::init(remappings,"car_map");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
    SubTopic_final_data = n.subscribe<std_msgs::String>("final_data",5,LocationHandler);
	start();
	return true;
}

void QNode::run(){
    //ros::Rate loop_rate(50);
	int count = 0;
    while ( ros::ok() ){
        if (!ros::master::check()){
            break;
        }
        std::cout<<"location update: "<<count<<endl;
        ros::spinOnce();
		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();
		chatter_publisher.publish(msg);
        //log(Info,std::string("I sent: ")+msg.data);
        if(update_location)
        {
           std::cout<<"location update"<<endl;
           log(Info,topic_msg);
           update_location = false;
           Q_EMIT Signal_Carlocation(location_point,submap_number);
        }
        if(update_planpath){
            update_planpath =false;
            //Q_EMIT
        }
        ++count;
        //loop_rate.sleep();
        QThread::msleep(500);
	}
	std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown(S_RosShutDown); // used to signal the gui for a shutdown (useful to roslaunch)
}


void QNode::log( const LogLevel &level, const std::string &msg) {
	logging_model.insertRows(logging_model.rowCount(),1);
	std::stringstream logging_model_msg;
	switch ( level ) {
		case(Debug) : {
				ROS_DEBUG_STREAM(msg);
				logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Info) : {
				ROS_INFO_STREAM(msg);
				logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Warn) : {
				ROS_WARN_STREAM(msg);
				logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Error) : {
				ROS_ERROR_STREAM(msg);
				logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Fatal) : {
				ROS_FATAL_STREAM(msg);
				logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
				break;
		}
	}
	QVariant new_row(QString(logging_model_msg.str().c_str()));
	logging_model.setData(logging_model.index(logging_model.rowCount()-1),new_row);
	Q_EMIT loggingUpdated(); // used to readjust the scrollbar
}
//void QNode::execProcess(){
//    locker.relock();
//    QString Process = "source /home/chen/Project/catkin_sysu/devel/setup.bash && roslaunch loam_velodyne loam_velodyne.launch";
//    //qDebug()<<Process;
//    if(!m_stop){
//        qDebug()<<"status:"<<myProcess->state();
//        locker.unlock();
//        if(myProcess->state() == QProcess::NotRunning){
//             qDebug()<<Process;
//             myProcess->start(Process);
//        }
//        //qDebug()<<"555";
//        Q_EMIT QprocessStatus(S_RUNNING);
//    }
//    myProcess->kill();
//    qDebug()<<"666";
//    locker.unlock();
//    Q_EMIT QprocessStatus(S_STOPED);
//}
void QNode::LocationHandler(const std_msgs::String::ConstPtr &msg){
    topic_msg = msg->data+" zbl";
    std::cout<<"ppppp::"<<topic_msg<<endl;
    stringstream stringin(msg->data);
    stringin>>setprecision(12)>>location_point.x>>location_point.y;
    update_location = true;
}
void QNode::MapIdHandler(const std_msgs::Int8::ConstPtr &msg){
    submap_number = msg->data;
    qDebug()<<"submap:"<<submap_number;
//    topic_msg = msg->data+" zbl";
//    stringstream stringin(msg->data);
//    stringin>>setprecision(12)>>location_point.x>>location_point.y;
//    update_location = true;
}
void QNode::DStarPathHandler(const std_msgs::Float32MultiArray::ConstPtr &msg){
    std::cout<<"planpath::"<<topic_msg<<endl;
//    QVector<PointCoordinates> PlanPath;
    PointCoordinates point;
    for(int i = 0 ;i < msg->data.size();i+=2){
        point.x = msg->data[i];
        point.y = msg->data[i+1];
        PlanPath.append(point);
    }
    update_planpath = true;
}
void QNode::Slot_GetEndPointFromMainWin(string end_point_str){
    std_msgs::String msg;
//    std::stringstream ss;
//    ss << "hello world " << count;
    msg.data = end_point_str;
    EndPoint_publisher.publish(msg);
    std::cout<<"任务点："<<end_point_str<<endl;
}
bool QNode::ShutDownNode(){
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    //wait();
}
}  // namespace car_map
