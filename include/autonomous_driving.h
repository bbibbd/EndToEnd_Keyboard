#include <iomanip>
#include <iostream>
#include <termios.h>

#include <fstream> 
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>   /* For open(), creat() */
#include "opencv2/opencv.hpp"
#include "etoeNet.h"

class autonomousDriving {

    public:
        autonomousDriving(int fd, cv::VideoCapture cap, int FPS, etoeNet ete_net, std::string onnx_file_path);
        ~autonomousDriving();

        cv::VideoCapture cap;
        etoeNet etoe_net;
        std::string onnx_file_path;
        std::string output_frame_path;

        float currentAngle = 0.0;

        int fd;
        int FPS;
        float drive();
        void set_init_angle(float init_angle);


    private:



};