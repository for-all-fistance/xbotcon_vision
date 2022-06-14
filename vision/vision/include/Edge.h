#pragma once
#ifndef _EDGE_H
#define _EDGE_H
#include <opencv.hpp>
#include <vector>
namespace xbot
{

	class Edge//ͼ���Ե��
	{
		friend class DEBUGABS;
	public:
		Edge();
		Edge(std::vector<cv::Point>& _contour);
		//std::vector<cv::Point> contour;//����߽���Ϣ,����ʹ��point2f���ͣ�contours����point(int)����
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

	class LightBar : public Edge//������
	{
	public:
		LightBar(Edge&);
		friend class DEBUGABS;
	private:

	};


	class EnergyBlock : public Edge//�������������
	{
	public:
		friend class DEBUGABS;
	private:

	};

}
#endif // !_EDGE_H
