#include "etoeNet.h"

etoeNet::etoeNet() : tensorNet(){

}

etoeNet::~etoeNet(){

}

void etoeNet:: loadOnnxFile(const std::string &onnx_file_path){
    std::vector<std::string> input_blocbs = {"input"};
    std::vector<std::string> output_blobs = {"output"};
    LoadNetwork(NULL, onnx_file_path.c_str(), NULL, input_blocbs, output_blobs, 1, TYPE_FP32);

    m_img_cropped_rgb_f_mat = cv::Mat(cv::Size(320, 70), CV_32FC3, mInputs[0].CPU);
}

void etoeNet::runInference(const cv::Mat &img_mat){


    //전처리
    //-------------------------------------------
    cv::Mat img_resized_mat;
    cv::resize(img_mat, img_resized_mat, cv::Size(320,160));    //resize the original image to 320x160
    //cv::imshow("test", img_resized_mat);
    cv::Mat img_cropped_mat = img_resized_mat(cv::Rect(0, 35, 320, 70));    //crop resized image (0, 65, 320, 70)
    cv::Mat img_cropped_rgb_mat;
    cv::cvtColor(img_cropped_mat, img_cropped_rgb_mat, cv::COLOR_BGR2RGB);
    img_cropped_rgb_mat.convertTo(m_img_cropped_rgb_f_mat, CV_32FC3, (1.0 / 127.5), -1.0);
    //--------------------------------------------

    ProcessNetwork(true); //sync = TRUE

    // TODO : inference 결과 가져오고난 뒤 Postprocess

    m_network_output = *(mOutputs[0].CPU);

   
}

float etoeNet::getModelOutput(){
    
    return m_network_output;
}

float etoeNet::getActualAngle(){

    float actual_angle;
    if(m_network_output < -0.75)
        actual_angle = -1.00;
    else if(m_network_output < -0.25)
        actual_angle = -0.5;
    else if(m_network_output < 0.25)
        actual_angle = 0.0;
    else if(m_network_output < 0.75)
        actual_angle = 0.5;
    else
        actual_angle = 1.0;
    return actual_angle * 20;
}

float etoeNet::getActualAngle_ver2(){

    float actual_angle;
    if(m_network_output < -0.875)
        actual_angle = -1.00;
    else if(m_network_output < -0.625)
        actual_angle = -0.75;
    else if(m_network_output < -0.375)
        actual_angle = -0.5;
    else if(m_network_output < -0.125)
        actual_angle = -0.25;
    else if(m_network_output < 0.125)
        actual_angle = 0.0;
    else if(m_network_output < 0.375)
        actual_angle = 0.25;
    else if(m_network_output < 0.625)
        actual_angle = 0.5;
    else if(m_network_output < 0.875)
        actual_angle = 0.75;    
    else
        actual_angle = 1.0;
    return actual_angle * 20;
}
