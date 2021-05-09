#include <ros/ros.h>
#include <iostream>
#include <latency_checker/latency_check.h>
#include <stdio.h>
#include <cstdlib>
#include <ros/console.h>
#include <vector>
#include <std_msgs/Header.h>


double start_sent ;
double end_recieved ;
double latency ;
int seq = 0;
int target_seq = 10000;
double sum = 0;
double avg;
double min;
double max;
std::vector<double> avgVec(100000,0);
std_msgs::Header msg;


int main(int argc, char **argv)
{    
    //init

    ros::init(argc, argv,"client_node_topic");
    ros::NodeHandle nh;
    ros::Duration(3).sleep();
    
    ros::Publisher pub_time = nh.advertise<std_msgs::Header>("latency_check_int64_topic",1000);



    ros::Rate r(100);

    while(ros::ok){
        //check ack and latency

        msg.seq = seq;
        msg.stamp = ros::Time::now();
        msg.frame_id = "test";
        
        if( seq < 10000){
        pub_time.publish(msg);
        
        std::cout <<" seq : " << seq <<std::endl;
        }
        ros::spinOnce();
        r.sleep();
        seq++;

    }

    return 0;
}