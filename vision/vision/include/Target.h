#pragma once
#ifndef _TARGET_H
#define _TARGET_H
#include <opencv.hpp>
#include <vector>
#include "Edge.h"
namespace xbot
{

	class Target//抽象的目标类
	{
		friend class DEBUGABS;
	public:
		Target(cv::Point2f c=cv::Point2f(640, 320));
		void getTarget2dPosition();
		virtual void distance_detection();
		virtual void gravity_offset_composite();
		virtual void kalman_filter(Target*);
		virtual void angle_solver();
		cv::Point2f center;
		cv::Rect2f rect;
		std::vector<cv::Point2f> refer_imgPoint;//对应的装甲板2D图像点
		cv::Point2f point_pre;//预测点
		cv::Point2f point_angle;//yaw和pitch角
		float distance;//距离
	private:

	};

	class Armer : public Target//抽象的装甲板信息类
	{
	public:
		Armer(LightBar& lb1, LightBar& lb2);
		friend class DEBUGABS;
	private:

	};

	class Energy : public Target//抽象的能量块信息类
	{
	public:
		friend class DEBUGABS;
	private:

	};

}


#endif // !_TARGET_H

