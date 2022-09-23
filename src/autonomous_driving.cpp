#include "autonomous_driving.h"

autonomousDriving::autonomousDriving(int fd, cv::VideoCapture cap, int FPS, etoeNet etoe_net, std::string onnx_file_path){
    this -> fd = fd;
    this -> cap = cap;
    this -> etoe_net = etoe_net;
    this -> onnx_file_path = onnx_file_path;
    this -> FPS = FPS;

}

autonomousDriving::~autonomousDriving(){
    

}

float autonomousDriving::drive(){
    char key;
    cv::Mat img;
    etoe_net.loadOnnxFile(onnx_file_path);

    float actualAngle = 0.0;
    float diffAngle = 0.0;

    char go[1]= {'w'};
    char left[1] = {'a'};
    char right[1] = {'d'};
    char trash[1] = {'b'};
    char stop[1] = {'s'};
    char clear[1] = {0x0D};

    while(1){

        cap >> img;
        cv::imshow("current_framte", img);
        key = cv::waitKey(1000/FPS);

        etoe_net.runInference(img);
        actualAngle = etoe_net.getActualAngle_ver2();
        diffAngle = (int)((currentAngle - actualAngle)/5);

        std::cout << "current_angle: " << currentAngle << " actual_angle: " << actualAngle << " diff_angle: " << diffAngle << std::endl;        
        currentAngle = actualAngle;

        if(diffAngle == 0){
            std::cout << "do not write any data" << std::endl;
	    
        }
        else if(diffAngle < 0){
            for(int i=0; i< -diffAngle; i++){
                std::cout << "write: d (" << i+1 << ")" << std::endl;
                write(fd, right, 1);
            }
        }else{
            for(int i=0; i< diffAngle; i++){
                std::cout << "write: a (" << i+1 << ")" << std::endl;
                write(fd, left, 1);
            }
        }
        
        if(key == 27){
            write(fd, stop, 1);
            currentAngle = 0.0;
            break;
        }

        
    }

    return currentAngle/20.0;
}

void autonomousDriving::set_init_angle(float angle){
    this -> currentAngle = angle;
}
