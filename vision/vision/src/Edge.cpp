#include <opencv.hpp>
#include "C:\Users\Li Jialiang\source\repos\xbotcon_vision\vision\vision\include\Edge.h"
using namespace xbot;

Edge::Edge()//默认构造函数
{
	area = 1;
	height = 1;
	width = 1;
	angle = 1;
	solidity = 1;
	areaRate = 0;
	lenthRate = 0;
}
Edge::Edge(std::vector<cv::Point> &_contour){
	//contour = _contour;
	area = contourArea(_contour);
	angle = minAreaRect.angle;
	minAreaRect = cv::minAreaRect(_contour);
	height = minAreaRect.size.height;
	width = minAreaRect.size.width;
	if (height <= width)
		std::swap(height, width);
	areaRate = height * width / area;//最小外接矩形与轮廓的面积比值
	lenthRate = height / width;//矩形长宽比
	std::vector<cv::Point> HullPoints(_contour.size());
	cv::convexHull(cv::Mat(_contour), HullPoints, false);//寻找凸包
	solidity = cv::contourArea(HullPoints) / area;//凸度
	//show_info();
}

void Edge::show_info() {
	using std::cout;
	cout << "\narea:" << area
		<< "\narearate:" << areaRate
		<< "\nheight:" << height
		<< "\nwidth:" << width
		<< "\nlenthrate:" << lenthRate
		<< "\nsolidity:" << solidity
		<< "\nangle:" << angle;
}

LightBar::LightBar(Edge& father)//拷贝函数
{
	area = father.area;
	minAreaRect = father.minAreaRect;
	height = father.height;
	width = father.width;
	angle = father.angle;
	solidity = father.solidity;
}
