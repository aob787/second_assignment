Second assignment - Research Track 1
================================

This respiratory was froked from https://github.com/CarmineD8/second_assignment

Tasks
--------------------
-Making the robot to keep running in the arena with same direction (i.e., counter-clockwise or clockwise).
-The user interface can take an input from user for increase/decrease the speed or reset the position of robot.

Installing and running
----------------------
To run you need to build/complie first using
```Shell
catkin_make
```
Then you can run the code by using
```Shell
roscore
rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
rosrun second_assignment controller_node
rosrun second_assignment ui_node
```
Once the program run correctly there are 2 console: controller node and User input node.

Pseudocode
--------------------
This pseudocode consist of 2 program.
controller.cpp
```
main(){
  init Publisher
  init service
  While(true){

  }
}

call-back function for laser scanner
    recive msg

    for i in range of dectection (e.g., 1.5 -> 0.8){
      for loop for find minDist
      if there is no obstacle in fron of the robot (minDist > range of detection)
        move forward(speed*speed multiplier)
      else
        for loop to find other path
          if cw is clear (avgDist > range of detection)
            turn right (speed*speed multiplier)
          else if ccw is clear (avgDist > range of detection)
            turn left (speed*speed multiplier)
    }
call-back function for speed adjust
  if sig == increase speed
    speed multiplier ++
  else if sig == decrease speed
    speed multiplier --
  put the speed multiplier back to Service
```
ui.cpp
```
main(){
  init service(speed_adj, reset_pos)
  While(true){
    take user user input
    switch case(user input)
      case reset
        call reset service
      case increase
        call speed_regulator(increase)
      case decrease
        call speed_regulator(decrease)
  }
}
```


Implemented code
--------------------

There are comments in codes. So, I have tuned some code and add some feature to get robot work reliably.
