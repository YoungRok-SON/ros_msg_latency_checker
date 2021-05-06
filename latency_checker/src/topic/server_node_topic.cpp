#include <stdio.h>
#include <ros/ros.h>
#include <latency_checker/latency_check_int64_topic.h>
#include <latency_checker/latency_check_img_topic.h>
#include <std_msgs/Header.h>
#include <string.h>
#include <std_msgs/Duration.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <vector>


ros::Time start;
ros::Time end;
ros::Duration latency;
std::string test;
int seq = 0;
int client_seq = 0;
int data_loss;
std::vector<double> avgVec(10000,0);
double sum = 0;
double avg = 0;
double min = 10;
double max = 0;
image_transport::Publisher pub;







void recieve_img_cb(const sensor_msgs::Image::ConstPtr& msg);

void recieve_int64_cb(const std_msgs::Header::ConstPtr& msg)
{
    //get latency
    start = msg->stamp;
    test = msg->frame_id;
    end = ros::Time::now();
    client_seq = msg->seq;
    
    latency = end - start;

    ROS_INFO_STREAM("lost message : " << client_seq-seq << " seq : " << seq << " client_seq : " << client_seq <<" frame_id : " << test << " start : " << start << " end : " << end << " latency : " << latency);
    avgVec[seq] = latency.toSec();
    
    // Cal avg, min, max
    if(client_seq == 9999 )
    {   
        for(int i = 0; i<seq+1; i++)
        {
            sum = sum + avgVec[i];
            if(min > avgVec[i]){min = avgVec[i];}
            if(max < avgVec[i]){max = avgVec[i];}
        }
        avg = sum/(seq+1);
        
        ROS_INFO_STREAM("avg : " << avg << " min :" << min << " max : " << max);
    }
    
    seq++;
   
}




int main(int argc, char **argv)
{
    ros::init(argc, argv, "server_node_topic");
    ros::NodeHandle nh;

    image_transport::ImageTransport it(nh);
    pub = it.advertise("image",1);

    ros::Subscriber sub_int = nh.subscribe("latency_check_int64_topic",1000,recieve_int64_cb);
    ros::Subscriber sub_img = nh.subscribe("latency_check_img_topic",1000,recieve_img_cb);

    ROS_INFO("Ready to receive data.");
    ros::spin();

    return 0;  
}


void recieve_img_cb(const sensor_msgs::Image::ConstPtr& msg)
{
    //get latency
    start = msg->header.stamp; // get timestamp of the client
    test = msg->header.frame_id; // 
    end = ros::Time::now(); 
    client_seq = msg->header.seq;
    
    latency = end - start;

    ROS_INFO_STREAM(" seq : " << seq << " client_seq : " << client_seq <<" frame_id : " << test << " start : " << start << " end : " << end << " latency : " << latency);
    avgVec[seq] = latency.toSec();
    
    // Cal avg, min, max
    if(client_seq == 9999 )
    {   
        for(int i = 0; i<seq+1; i++)
        {
            sum = sum + avgVec[i];
            if(min > avgVec[i]){min = avgVec[i];}
            if(max < avgVec[i]){max = avgVec[i];}
        }
        avg = sum/(seq+1);
        
        ROS_INFO_STREAM("lost message : " << client_seq-seq << " avg : " << avg << " min :" << min << " max : " << max);
    }
    //pub.publish(msg);
    
    seq++;
}