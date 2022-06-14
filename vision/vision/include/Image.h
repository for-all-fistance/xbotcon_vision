#pragma once
#ifndef _IMAGE_H
#define _IMAGE_H
#include <opencv.hpp>
#include <vector>
#include "Edge.h"
#include "Target.h"
namespace xbot
{

	class Frame
	{
		friend class DEBUGABS;
	public:
		Frame();
		void InitEdges();//Ԥ����ͼ�񲢳�ʼ��ͼ��������Ϣ
		std::vector<Edge> edges;
		void show_info();
		void draw_target(xbot::Target& target);
	private:
		cv::Mat raw;
		cv::Mat image;
		cv::Size imageSize;
		void ImagePreprocess();
	};

}
#endif // !_IMAGE_H
