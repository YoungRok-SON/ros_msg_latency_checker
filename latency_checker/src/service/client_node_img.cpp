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



double timeChecker(double time)
{
    return time; //change this value to time.
}


double latencyChecker(double start, double end)
{
    double timeGap = end - start;
    return timeGap;
}




int load_img()
{   
    cv::String imageName("/home/youngrok/Pictures/1080img.jpg");
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
    header.seq = 3;
    header.stamp = ros::Time::now();
    img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8,img);
    img_bridge.toImageMsg(img_msg);
}





int main(int argc, char **argv)
{    
    //init
    ros::init(argc, argv,"client_node_img");
    ros::NodeHandle nh;


    ros::ServiceClient client = nh.serviceClient<latency_checker::latency_check_img>("latency_check_img");
    double time = ros::Time::now().toSec();
    latency_checker::latency_check_img srv;

    load_img();
    cvtoros();

    srv.request.img = img_msg;

    ros::Rate r(100);

    while(ros::ok){
        //check ack and latency
        if (client.call(srv))
        {
            start_sent = timeChecker(ros::Time::now().toSec());
            end_recieved = timeChecker(ros::Time::now().toSec());
            latency = latencyChecker(start_sent, end_recieved);
            ROS_INFO_STREAM("seq : "<< seq+1 <<" start : " << start_sent << " end : " << end_recieved << " latency : " << latency );
            
            //calculate avg, min, max
            avgVec[seq] = latency;
            if(seq > 0){
                if(avgVec[seq-1] > avgVec[seq]){ min = avgVec[seq]; }
                if(avgVec[seq-1] < avgVec[seq]){ max = avgVec[seq]; }
            }else{
                min = avgVec[0];
                max = avgVec[0];
            }

            seq++;

        }else
        {
            ROS_ERROR("Failed to call service latency check.");
            return 1;
        }
        if(seq == target_seq){
            for(int i = 0; i < target_seq; i++){
                sum = sum + avgVec[i];
            } 
            avg = sum/target_seq;
            ROS_INFO_STREAM("avg : " << avg << " min :" << min << " max : " << max);
            ros::shutdown();
        }

        ros::spinOnce();
        r.sleep();
    }

    return 0;
}