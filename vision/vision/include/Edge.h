#pragma once
#ifndef _EDGE_H
#define _EDGE_H
#include <opencv.hpp>
#include <vector>
namespace xbot
{

	class Edge//图像边缘类
	{
		friend class DEBUGABS;
	public:
		Edge();
		Edge(std::vector<cv::Point>& _contour);
		//std::vector<cv::Point> contour;//储存边界信息,不能使用point2f类型，contours都是point(int)类型
		virtual	void show_info();
		float area;
		cv::RotatedRect minAreaRect;
		float areaRate;
		float height;
		float width;
		float lenthRate;
		float solidity;
		float angle;
	private:

	};

	class LightBar : public Edge//灯条类
	{
	public:
		LightBar(Edge&);
		friend class DEBUGABS;
	private:

	};


	class EnergyBlock : public Edge//具体的能量块类
	{
	public:
		friend class DEBUGABS;
	private:

	};

}
#endif // !_EDGE_H
