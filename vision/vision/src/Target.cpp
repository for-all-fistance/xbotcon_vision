#include <opencv.hpp>
#include "C:\Users\Li Jialiang\source\repos\xbotcon_vision\vision\vision\include\Target.h"
#include <Eigen/Core>
#include <Eigen/src/Core/DiagonalMatrix.h>
#include <Eigen/Dense>  //稠密矩阵的代数运算
#include <opencv2/core/eigen.hpp>
#include <cmath>
using namespace xbot;
Armer::Armer(LightBar& lb1, LightBar& lb2){
			cv::Rect2f r1 = lb1.minAreaRect.boundingRect2f();
			cv::Rect2f r2 = lb2.minAreaRect.boundingRect2f();
			rect = r1 | r2;
			center = (rect.br() + rect.tl())/2;
			std::cout << center << std::endl;
}
Target::Target(cv::Point2f c) {
	center = c;
}
void Target::getTarget2dPosition() {
	using namespace cv;
	std::vector<cv::Point2f> temp;
	temp.push_back(rect.tl() + Point2f(0, rect.height) );//bl
	temp.push_back(rect.br() );//br
	temp.push_back(rect.tl() + Point2f(rect.width, 0) );//tr
	temp.push_back(rect.tl() );//tl
	refer_imgPoint = temp;
}

const cv::Point3f ARMER_BL(-33.75, -13.25, 0);
const cv::Point3f ARMER_BR(33.75, -13.25, 0);
const cv::Point3f ARMER_TR(33.75, 13.25, 0);
const cv::Point3f ARMER_TL(-33.75, 13.25, 0);
void Target::distance_detection()
{
	const cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 61.2853517927598091e+03, 0., 3.1944768628958542e+02, 0.,
		1.2792339468697937e+03, 2.3929354061292258e+02, 0., 0., 1.);//外参
	const cv::Mat distCoeff = (cv::Mat_<double>(1, 5) << -6.3687295852461456e-01, -1.9748008790347320e+00,
		3.0970703651800782e-02, 2.1944646842516919e-03, 0.);//内参：畸变矩阵
	cv::Mat rvecs;//旋转向量
	cv::Mat tvecs;//平移向量
	std::vector<cv::Point3f> refer_objectPoints;//自定义的3D坐标点
	cv::Mat rmats;//旋转矩阵
	refer_objectPoints.push_back(ARMER_BL);
	refer_objectPoints.push_back(ARMER_BR);
	refer_objectPoints.push_back(ARMER_TR);
	refer_objectPoints.push_back(ARMER_TL);
	solvePnP(refer_objectPoints, refer_imgPoint, cameraMatrix, distCoeff, rvecs, tvecs, true, cv::SOLVEPNP_EPNP);//solvepnp解算旋转向量和平移向量
	Rodrigues(rvecs, rmats);
	Eigen::Matrix3f R_n;
	Eigen::Vector3f T_n;
	cv2eigen(rmats, R_n);
	cv2eigen(tvecs, T_n);
	Eigen::Vector3f P_oc;
	P_oc = -R_n.inverse() * T_n;
	std::cout<<"当前3D位姿坐标为："<<"["<<P_oc<<"]"<<std::endl;
	distance=P_oc.z() * 0.0001;//m
}

const float SPEED_BULLET = 14.0;
const float REAL_HEIGHT_ARMER = 0.07;//7cm
void Target::gravity_offset_composite()
{
	distance_detection();
	float gravity_offset = (9.8 * distance * distance * rect.height) / (2.0	* SPEED_BULLET * SPEED_BULLET * REAL_HEIGHT_ARMER);
	center.y -= gravity_offset;
}

const int stateNum = 4;                                      //状态值10×1向量(x,y,weight,height,angle,△x,△y......)
const int measureNum = 4;                                    //测量值10×1向量(x,y,weight,height,angle,△x,△y......)	                               
cv::KalmanFilter KF(stateNum, measureNum, 0);					//创建一个卡尔曼滤波器，****无需重复定义****

void Target::kalman_filter(Target* past_pos) {
		KF.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0,
														0, 1, 0, 1,
														0, 0, 1, 0,
														0, 0, 0, 1);                              //转移矩阵A
		cv::Mat measurement = cv::Mat::zeros(measureNum, 1, CV_32F); //初始测量值x'(0)为0矩阵，因为后面要更新这个值，所以必须先定义
		setIdentity(KF.measurementMatrix);       //测量矩阵H    setIdentity()创建初始值为单位阵
		setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-3));     //系统噪声方差矩阵Q  Q越大越相信观测值
		setIdentity(KF.measurementNoiseCov, cv::Scalar::all(5e-5)); //测量噪声方差矩阵R  R越小越信任观测值
		setIdentity(KF.errorCovPost, cv::Scalar::all(1));   //后验错误估计协方差矩阵P  
		///////////////////KF预测矩形中心点位置/////////////////
		measurement.at<float>(0) = this->center.x;
		measurement.at<float>(1) = this->center.y;
		measurement.at<float>(2) = this->center.x - past_pos->center.x;
		measurement.at<float>(3) = this->center.y - past_pos->center.y;
		//update
		KF.correct(measurement);
		//kalman prediction
		KF.predict();
		if (KF.statePre.at<float>(0) != KF.statePre.at<float>(0) || KF.statePre.at<float>(1) != KF.statePre.at<float>(1))
		{
			KF.statePre.at<float>(0) = this->center.x;
			KF.statePre.at<float>(1) = this->center.y;
		}
		if (abs(KF.statePre.at<float>(0)) >1000 || abs(KF.statePre.at<float>(1)) >1000)
		{
			KF.statePre.at<float>(0) = this->center.x;
			KF.statePre.at<float>(1) = this->center.y;
		}
		std::cout << "预测下一帧位于：" << cv::Point2f(KF.statePre.at<float>(0), KF.statePre.at<float>(1)) << std::endl;
		this->point_pre = cv::Point2f(KF.statePre.at<float>(0), KF.statePre.at<float>(1));
}

void Target::angle_solver()//计算角度
{
	const cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 61.2853517927598091e+03, 0., 3.1944768628958542e+02, 0.,
		1.2792339468697937e+03, 2.3929354061292258e+02, 0., 0., 1.);//外参
	const cv::Mat distCoeff = (cv::Mat_<double>(1, 5) << -6.3687295852461456e-01, -1.9748008790347320e+00,
		3.0970703651800782e-02, 2.1944646842516919e-03, 0.);//内参：畸变矩阵
	static double cx = cameraMatrix.at<double>(0, 2);
	static double cy = cameraMatrix.at<double>(1, 2);
	static double fx = cameraMatrix.at<double>(0, 0);
	static double fy = cameraMatrix.at<double>(1, 1);
	float rx = (center.x - cx) / fx;
	float ry = (center.y - cy) / fy;
	point_angle=cv::Point2f(atan(rx) / CV_PI * 180, atan(ry) / CV_PI * 180);
}