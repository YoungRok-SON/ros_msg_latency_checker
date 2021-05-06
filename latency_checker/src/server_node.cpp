#include <stdio.h>
#include <ros/ros.h>
#include <latency_checker/latency_check.h>
#include <latency_checker/latency_check_img.h>
#include <std_msgs/Header.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>

sensor_msgs::Image img;
image_transport::Publisher pub;




bool recieve_img(latency_checker::latency_check_img::Request &req,
                  latency_checker::latency_check_img::Response &res);

bool recieve_int64(latency_checker::latency_check::Request &req,
                  latency_checker::latency_check::Response &res)
{
    if(req.data == 1){
        res.ack = 1;
        std::cout << "data recevied." << std::endl;
        std::cout << req.data << std::endl;
    }

    return true;
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "server_node");
    ros::NodeHandle nh;

    image_transport::ImageTransport it(nh);
    pub = it.advertise("image",1);


    ros::ServiceServer service_int64 = nh.advertiseService("latency_check_int64", recieve_int64);
    ros::ServiceServer service_img = nh.advertiseService("latency_check_img", recieve_img);



    

    ROS_INFO("Ready to send data.");
    ros::spin();

    return 0;  
}




bool recieve_img(latency_checker::latency_check_img::Request &req,
                  latency_checker::latency_check_img::Response &res)
{
    if(req.img.header.seq ==3){
        res.ack = 1;
        std::cout << "img : data recevied." << std::endl;
    }else if(req.img != img)
    {
        ROS_ERROR("topic has dameged.");
        ros::shutdown();
    }

    img = req.img;
    //pub.publish(img);


    return true;
    
}

