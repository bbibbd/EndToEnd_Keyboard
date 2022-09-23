#include "keyboard_driving.h"

keyboardDriving::keyboardDriving(int fd, cv::VideoCapture cap, int FPS, std::string save_path){
    this -> m_fd = fd;
    this -> cap = cap;
    this -> save_path = save_path;
    this -> FPS = FPS;
}

keyboardDriving::~keyboardDriving(){

}

float keyboardDriving::drive(){

    char go[1]= {'w'};
    char left[1] = {'a'};
    char right[1] = {'d'};
    char stop[1] = {'s'};
    char back[1] = {'x'};
    char center[1] = {'f'};

    int recording;

    std::string output_frame_file_path = save_path + "/frame";
    std::string output_log_file_path = save_path + "/driving_log.csv";
    std::string driving_log;

    while(1){
        
        cap >> img_pre;
        cv::resize(img_pre, img, cv::Size(320, 160));
        img_cropped_mat = img(cv::Rect(0, 35, 320, 70));    //set ROI
        cv::cvtColor(img_cropped_mat, img_cropped_rgb_mat, cv::COLOR_BGR2RGB);
        img_cropped_rgb_mat.convertTo(m_img_cropped_rgb_f_mat, CV_32FC3, (1.0 / 127.5), -1.0);
        cv::imshow("cropped img", img_cropped_rgb_mat); //this image will be inputed to the pilotNet
        cv::imshow("camera img", img);

        key = cv::waitKey(1000/FPS);
        if(key == 'w'){
            write(m_fd, go, 1);
        } else if(key == 'd'){
            angle += 0.25;
            if(angle >= 1)
                angle = 1;
            write(m_fd, right, 1);
        } else if(key == 'a'){
            angle -= 0.25;
                if(angle <= -1)
                    angle = -1;
            write(m_fd, left, 1);
        
        } else if(key == 's'){
            angle = 0;
            write(m_fd, stop, 1);
        } else if(key == 'x'){
            write(m_fd, back, 1);
        } else if(key == 'f'){
            angle = 0;
            write(m_fd, center, 1);
        } else if(key == 27){
            write(m_fd, stop, 1);
            break;
        } else if(key == 'p'){
            recording = PAUSE;
        } else if(key == 'r'){
            recording = RECORD;
        }else{
            key = 0;
        }
        
        std::string file_name = output_frame_file_path + std::to_string(m_frame_no) + ".jpg";
        driving_log = file_name + ',' + std::to_string(angle);
        std::cout << file_name << ", " << angle << std::endl;
        //std::cout << "key: " << key << std::endl;
        
        if(recording == RECORD){
            cv::imwrite(file_name, img);
            save_driving_log(output_log_file_path, driving_log);
            m_frame_no ++;
        }
    }
    std::cout << "done.." << std::endl;
    write(m_fd, stop, 1);
    angle = 0.0;

    return angle*20.0;


}
void keyboardDriving::save_driving_log(std::string file_name, std::string driving_log){

    std::ofstream fout;
    fout.open(file_name, std::ios_base::out | std::ios_base::app);
    fout << driving_log << std::endl;
    fout.close();
}

void keyboardDriving::set_init_angle(float angle){
    this -> angle = angle;
}