#include <ros/ros.h>
#include <iostream>
#include <latency_checker/latency_check.h>
#include <stdio.h>
#include <cstdlib>
#include <ros/console.h>
#include <vector>


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


double timeChecker(double time)
{
    return time; //change this value to time.
}

double latencyChecker(double start, double end)
{
    double timeGap = end - start;
    return timeGap;
}

int main(int argc, char **argv)
{    
    //init
    ros::init(argc, argv,"client_node");
    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<latency_checker::latency_check>("latency_check_int64");
    double time = ros::Time::now().toSec();
    latency_checker::latency_check srv;
    
    //get a data from user
    srv.request.data = atoll(argv[1]);

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