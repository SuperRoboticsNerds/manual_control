#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "ras_arduino_msgs/PWM.h"
#include "ras_arduino_msgs/Encoders.h"
#include "geometry_msgs/Twist.h"
#include <sstream>
#include <math.h>
#include <termios.h>
//#include <conio.h>
//#include <ncurses.h>


class KeyboardController
{
public:

    ros::NodeHandle n;
    ros::Publisher cmd_vel_pub_;

    KeyboardController()
    {
        n = ros::NodeHandle("~");
        cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/motor_controller/twist", 1000);
        //cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);
	
    }

    ~KeyboardController()
    {
        //delete motor_controller_;
    }

    ////////////////////


    bool driveKeyboard()
    {
      std::cout << "Type a command and then press enter.  "
        "Use '+' to move forward, 'l' to turn left, "
        "'r' to turn right, '.' to exit.\n";

      //we will be sending commands of type "twist"
      //geometry_msgs::Twist twist_msg;

      //char cmd[50];
      char c;
      //cmd[0] = getch();

      while(n.ok()){

        //std::cin.getline(cmd, 50);
        std::cin >> (c);
        //if(cmd[0]!='w' && cmd[0]!='a' && cmd[0]!='d' && cmd[0]!='.')
        if(c!='w' && c!='a' && c!='d' && c!='.' && c!='s')
        {
          std::cout << "unknown command:" << c << "\n";
          continue;
        }

        //twist_msg.linear.x = twist_msg.linear.y = twist_msg.angular.z = 0;
        //move forward

        //if(cmd[0]=='w')
        if(c=='w'){                                                 //FORWARD
            twist_msg.linear.x = twist_msg.linear.x + 0.1;
        }
        //turn left (yaw) and drive forward at the same time
        //else if(cmd[0]=='a')
        else if(c=='a'){                                            //LEFT
          twist_msg.angular.z = twist_msg.angular.z + 0.1;
        }
        //turn right (yaw) and drive forward at the same time
        //else if(cmd[0]=='d')
        else if(c=='d'){                                            //RIGHT
          twist_msg.angular.z = twist_msg.angular.z - 0.1;
        }
        //quit
        //else if(cmd[0]=='.')
        else if(c=='s')
        {
            if (twist_msg.linear.x > 0)
            twist_msg.linear.x = twist_msg.linear.x - 0.1;
            else twist_msg.linear.x =0;
        }
        else if(c=='.')
        {
          break;
        }

        //publish the assembled command
        cmd_vel_pub_.publish(twist_msg);
      }
      return true;
    }



    ////////////////////

/*void twist_function()
    {
	twist_msg.linear.x=0.2;
	twist_msg.angular.z=0;
	twist_pub_.publish(twist_msg);
     }*/

private:

    geometry_msgs::Twist twist_msg;

};


int main(int argc, char **argv)
{
    
    ros::init(argc, argv, "keyboard_controller");
    KeyboardController keyboard_controller;

    // Control @ 10 Hz
    double control_frequency = 10.0;

    ros::Rate loop_rate(control_frequency);

    keyboard_controller.driveKeyboard();

    ros::spinOnce();
    loop_rate.sleep();
    

    return 0;
}
