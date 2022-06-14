#include "Image.h"
#include <opencv.hpp>
using namespace xbot;

const std::string imageSource = "";
const std::string videoSource = "C:/Users/Li Jialiang/Desktop/armer test.avi";
const int imageWidth = 640; //����ͼƬ��С��������ͷ�ķֱ���
const int imageHeight = 480;
cv::VideoCapture cap = cv::VideoCapture(videoSource);
Frame::Frame() {
    imageSize = cv::Size(imageWidth, imageHeight);
    cap >> raw;
    if (raw.empty())
    {
        std::cout << "no image!\n";
        exit(0);
    }
    ImagePreprocess();
}
const int threash_num = 80;
void Frame::ImagePreprocess() {
    std::vector<cv::Mat> channels;
    split(raw, channels);//����ͨ��
    cv::Mat blueImg = channels.at(2) - channels.at(0);
    //threshold(blueImg,threash,0,255,THRESH_OTSU);
    threshold(blueImg, image, threash_num, 255, cv::THRESH_BINARY);
    //Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
    //dilate(threash, dst, element);
}


void Frame::InitEdges() {
    
    std::vector<std::vector<cv::Point>> contours;//����߽���Ϣ,����ʹ��point2f���ͣ�contours����point(int)����
    std::vector<cv::Vec4i> hierarchy;//����߽��������Ϣ����ǰһ����������һ����������������
    cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);//Ѱ�ұ߽�
    cv::drawContours(raw, contours, -1, cv::Scalar(255, 0, 0));
    for (int i = 0; i < contours.size(); ++i)
        edges.push_back(Edge(contours[i]));
}

void Frame::show_info() {

    cv::imshow("raw", raw);
    cv::imshow("image", image);
    cv::waitKey(20);
}

void Frame::draw_target(xbot::Target &target) {
    cv::circle(raw, target.center, 5, cv::Scalar(255, 0, 0), cv::FILLED);
    //cv::circle(raw, target.point_pre, 5, cv::Scalar(0, 0, 255), cv::FILLED);

}
