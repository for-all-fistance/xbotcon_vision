#pragma once
#ifndef _PRODUCE_CONSUME_H
#define _PRODUCE_CONSUME_H
#include <cstddef>
#include <vector>
#include "Target.h"
#include "Image.h"
#include"Edge.h"
#include "Finder.h"
namespace xbot
{

	class Producer
	{
	public:
		static void produce(std::vector<Target>&);

	private:

	};



	class Consumer
	{
	public:
		static void consume(std::vector<Target>&);

	private:

	};
}



#endif // !_PRODUCE_CONSUME_H
