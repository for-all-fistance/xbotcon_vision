#include <opencv.hpp>
#include <ctime>
#include <thread>
#include "Produce_consume.h"
using namespace xbot;
int main()
{
	std::vector<Target> Targets;
	std::thread Target_Produce_Thread(&Producer::produce,std::ref(Targets));//get image and preprocess ,detect Target from the preprocessed image
	std::thread Target_consume_Thread(&Consumer::consume, std::ref(Targets));//process the detected Target info,send Target info(maybe include get aommand from the commander)
	Target_Produce_Thread.join();
	Target_consume_Thread.join();
	return 0;
}

