#include <iostream>
#include "autonomous_driving.h"
#include "keyboard_driving.h"
#include "filesystem.h"
#include "opencv2/opencv.hpp"
#include <termios.h>

#define FPS 20
#define WEBCAM

int flag = 1.0;
int m_fd;
float current_angle = 0.0;

void openSerialPort();

int main(int argc, char** argv) {

    if(argc < 2){
        std::cout << "usage: ./trt_interface_app PATH_TO_OUTPUT " << std::endl;
        return 0;
    }

    std::string onnx_file_path = "/home/cgv/slam/onnx/nth_large_ver1.simplified.onnx";
    std::string output_frame_path = argv[1];
    
#ifdef WEBCAM
    cv::VideoCapture cap(0);
#else
    cv::VideoCpature cap("");
#endif

    openSerialPort();

    etoeNet etoeNet;
    keyboardDriving keyboard_driving(m_fd, cap, FPS, output_frame_path);
    autonomousDriving autonomous_driving(m_fd, cap, FPS, etoeNet, onnx_file_path);

    while(1){
        if(flag == 0){      //end to end driving
            autonomous_driving.set_init_angle(current_angle);
            current_angle = autonomous_driving.drive();
            flag = 1;
        }
        else if(flag == 1){  //keyboard driving
            keyboard_driving.set_init_angle(current_angle);
            current_angle = keyboard_driving.drive();
            flag = 0;
        }
        else{               

        }
    }
}

void openSerialPort(){
    struct termios toptions;

     /* open serial port */
    m_fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);   
    std::cout << "FD: " << m_fd << std::endl;

    /* get current serial port settings */
    tcgetattr(m_fd, &toptions);

    /* set 9600 baud both ways */
    cfsetispeed(&toptions, B9600);
    cfsetospeed(&toptions, B9600);

    /* 8 bits, no parity, no stop bits */
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;

    /* Canonical mode */
    toptions.c_lflag |= ICANON;

    /* commit the serial port settings */
    tcsetattr(m_fd, TCSANOW, &toptions);
}
