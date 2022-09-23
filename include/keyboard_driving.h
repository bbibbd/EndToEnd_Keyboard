#define PAUSE 0
#define RECORD 1

#include <iomanip>
#include <iostream>
#include <termios.h>

#include <fstream> 
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>   /* For open(), creat() */
#include "opencv2/opencv.hpp"

class keyboardDriving {
    public:
        keyboardDriving(int fd, cv::VideoCapture cap, int FPS, std::string save_path);
        ~keyboardDriving();

        float drive();
        void save_driving_log(std::string, std::string);
        void set_init_angle(float angle);


        int m_fd;
        int m_frame_no = 0;
        int FPS;
        
        float m_network_output = 0.0;
        float angle;

        cv::VideoCapture cap;
        cv::Mat img;
        cv::Mat img_pre;
        cv::Mat img_cropped_mat;
        cv::Mat img_cropped_rgb_mat;
        cv::Mat m_img_cropped_rgb_f_mat;

        char key;
        std::string save_path;

    private:

};
