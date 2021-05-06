#include <ros/ros.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ros/console.h>
#include <vector>

#include <std_msgs/Header.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <latency_checker/latency_check_img.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>




double start_sent ;
double end_recieved ;
double latency ;
int seq = 0;
int target_seq = 10000;
double sum = 0;
double avg;
double min;
double max;
std::vector<double> avgVec(10000,0);

cv::Mat img;
cv_bridge::CvImage img_bridge;
sensor_msgs::Image img_msg;
std_msgs::Header header;




int load_img()
{   
    cv::String imageName("/home/youngrok/Pictures/train.jpg");
    cv::String Windowname( "Display Window ");
    img = cv::imread(imageName, cv::IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "could not open or find the image." << std::endl;
        return -1;
    }

    cv::namedWindow( Windowname, cv::WINDOW_AUTOSIZE);
    cv::imshow(Windowname , img);
    cv::waitKey(0);

    return 0;
}

void cvtoros()
{   
    img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8,img);
    img_bridge.toImageMsg(img_msg);
}





int main(int argc, char **argv)
{    
    //init
    ros::init(argc, argv,"client_node_img");
    ros::NodeHandle nh;
    sensor_msgs::Image msg;


    ros::Publisher pub_img = nh.advertise<sensor_msgs::Image>("latency_check_img_topic",1000);

    load_img();
    cvtoros();

    //give a data to msg
    msg = img_msg;
    msg.header.frame_id = "test";
    ros::Duration(3).sleep();

    
    ros::Rate r(100);

    while(ros::ok){
        //check ack and latency

        msg.header.stamp = ros::Time::now();
        msg.header.seq = seq;
        
        if( seq < 10000){
            pub_img.publish(msg);
        
         std::cout <<" seq : " << seq <<std::endl;
        }

        ros::spinOnce();
        r.sleep();
        seq++;
    }

    return 0;
}