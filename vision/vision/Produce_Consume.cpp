#include "Produce_consume.h"
#include <mutex>
#include <condition_variable>
using namespace xbot;
const cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 1.2853517927598091e+03, 0., 3.1944768628958542e+02, 0.,
	1.2792339468697937e+03, 2.3929354061292258e+02, 0., 0., 1.);//外参
const cv::Mat distCoeff = (cv::Mat_<double>(1, 5) << -6.3687295852461456e-01, -1.9748008790347320e+00,
	3.0970703651800782e-02, 2.1944646842516919e-03, 0.);//内参：畸变矩阵

void Producer::produce(std::vector<Target>& Targets){
	armer_Finder finder;
	while (true)
	{
		Frame frame;
		frame.InitEdges();
		finder.find(frame, Targets);
		frame.draw_target(Targets.back());
		frame.show_info();
	}
}
extern std::mutex mtx;
extern std::condition_variable cond_var;
void Consumer::consume(std::vector<Target>& Targets) {
	while (true)
	{
		std::unique_lock<std::mutex> lock(mtx);
		cond_var.wait(lock);
		//lock.lock();
		Target* targetptr = &Targets.back();
		targetptr->getTarget2dPosition();
		undistortPoints(targetptr->refer_imgPoint, targetptr->refer_imgPoint, cameraMatrix, distCoeff, cv::noArray(), cameraMatrix);
		targetptr->center = (targetptr->refer_imgPoint[1] + targetptr->refer_imgPoint[3]) / 2.0;
		targetptr->gravity_offset_composite();
		lock.unlock();
		targetptr->kalman_filter(targetptr - 1);
	}
}

