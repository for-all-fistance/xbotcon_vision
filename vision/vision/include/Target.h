#pragma once
#ifndef _TARGET_H
#define _TARGET_H
#include <opencv.hpp>
#include <vector>
#include "Edge.h"
namespace xbot
{

	class Target//�����Ŀ����
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
		std::vector<cv::Point2f> refer_imgPoint;//��Ӧ��װ�װ�2Dͼ���
		cv::Point2f point_pre;//Ԥ���
		cv::Point2f point_angle;//yaw��pitch��
		float distance;//����
	private:

	};

	class Armer : public Target//�����װ�װ���Ϣ��
	{
	public:
		Armer(LightBar& lb1, LightBar& lb2);
		friend class DEBUGABS;
	private:

	};

	class Energy : public Target//�������������Ϣ��
	{
	public:
		friend class DEBUGABS;
	private:

	};

}


#endif // !_TARGET_H

