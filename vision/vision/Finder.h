#pragma once
#ifndef _FINDER_H
#define _FINDER_H
#include "Edge.h"
#include <vector>
#include "Target.h"
#include "Image.h"
namespace xbot
{

	class Finder
	{
	public:
		bool virtual check(Edge* target) = 0;
		void virtual find(xbot::Frame& image, std::vector<Target>& Targets) = 0;
	private:

	};



	class armer_Finder : public Finder
	{
	public:
		bool check(Edge* target);
		void find(xbot::Frame &image, std::vector<Target> &Targets);//寻找符合条件的目标
	private:

	};



	class energy_Finder : public Finder
	{
	public:
		bool check(Edge* target);
		/*void find(xbot::Frame &image, std::vector<Target> &Targets);*/
	private:

	};
}

#endif