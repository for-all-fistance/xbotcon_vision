#include "C:\Users\Li Jialiang\source\repos\xbotcon_vision\vision\vision\include\Image.h"
#include <opencv.hpp>
using namespace xbot;

const std::string Source = "C:/Users/Li Jialiang/Desktop/armer test.avi";//"C:/Users/Li Jialiang/Downloads/视觉开源加分项图片.png";//
const int imageWidth = 640; //定义图片大小，即摄像头的分辨率
const int imageHeight = 480;
cv::VideoCapture cap = cv::VideoCapture(Source);
Frame::Frame() {
    imageSize = cv::Size(imageWidth, imageHeight);
    cap >> raw;
    /*raw = cv::imread(Source);*/
    if (raw.empty())
    {
        std::cout << "no image!\n";
        cv::waitKey();
        exit(0);
    }
    ImagePreprocess();
}
const int threash_num = 80;
void Frame::ImagePreprocess() {
    std::vector<cv::Mat> channels;
    split(raw, channels);//分离通道
    cv::Mat blueImg = channels.at(2) - channels.at(0);
    //threshold(blueImg,threash,0,255,THRESH_OTSU);
    threshold(blueImg, image, threash_num, 255, cv::THRESH_BINARY);
    //Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
    //dilate(threash, dst, element);
}


void Frame::InitEdges() {
    
    std::vector<std::vector<cv::Point>> contours;//储存边界信息,不能使用point2f类型，contours都是point(int)类型
    std::vector<cv::Vec4i> hierarchy;//储存边界的拓扑信息，如前一个轮廓，后一个轮廓，父轮廓等
    cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);//寻找边界
    cv::drawContours(raw, contours, -1, cv::Scalar(100, 0, 100),cv::FILLED);
    for (int i = 0; i < contours.size(); ++i)
        edges.push_back(Edge(contours[i]));
}

void Frame::show_info() {

    cv::imshow("raw", raw);
    cv::imshow("image", image);
    cv::waitKey(100);
}

void Frame::draw_target(xbot::Target &target) {
    cv::circle(raw, target.center, 5, cv::Scalar(255, 0, 0), cv::FILLED);
    cv::rectangle(raw, target.rect, cv::Scalar(255, 0, 0), 2);
    //cv::circle(raw, target.point_pre, 5, cv::Scalar(0, 0, 255), cv::FILLED);

}
