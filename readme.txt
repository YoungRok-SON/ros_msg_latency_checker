there are two parts

1. For topic messages
rosrun latency_check server_node_topic : get time for one way with the msg from client nodes.
rosrun latency_check client_node_topic : send header msg to server_node_topic
rosrun latency_check client_node_img_topic : send the msg with img. the image directory needs to be set on the code.

2. For Service messages(RTT)
rosrun latency_check server_node : get time for one way with the msg from client nodes.
rosrun latency_check client_node [number]: send header msg to server_node_topic. *** you need to put a number for run properly at the end of the setence.
rosrun latency_check client_node_img : send the msg with img. the image directory needs to be set on the code.
