#include <opencv.hpp>
#include "C:\Users\Li Jialiang\source\repos\xbotcon_vision\vision\vision\include\Finder.h"
#include <mutex>
#include <condition_variable>
using namespace xbot;

const float MINAREARATE_ARMER = 1.0;//最小外接矩形与轮廓的面积比值在一定范围内
const float MAXAREARATE_ARMER = 1.3;
const float MINLENTHRATE_ARMER = 4;//长宽比大于一定值
const float MAXLENTHRATE_ARMER = 6;//长宽比小于一定值
const float MINSOLIDITY_ARMER = 1.02;//轮廓的凸度(Solidity)大于一定值
const float MAXSOLIDITY_ARMER = 1.2;

const float MAX_ANGLE_ERROR = 15;//灯条angle误差最大值
const float MIN_HEIGHT_RATE = 0.85;
const float MAX_HEIGHT_RATE = 1.2;//灯条高度应近似相等

const float MINAREARATE_ENERGY = 2;
const float MAXAREARATE_ENERGY = 2;
const float MINLENTHRATE_ENERGY = 1.5;
const float MINSOLIDITY_ENERGY = 1;
const float MAXSOLIDITY_ENERGY = 1;
bool armer_Finder::check(Edge* target) {
        if (target->area > 30 && target->area < 250000/*去除整个图片的边框！*/)
        {
            if (target->areaRate > MINAREARATE_ARMER && target->areaRate < MAXAREARATE_ARMER)
            {
                if (target->lenthRate > MINLENTHRATE_ARMER && target->lenthRate < MAXLENTHRATE_ARMER)
                {
                    if (target->solidity > MINSOLIDITY_ARMER&& target->solidity< MAXSOLIDITY_ARMER)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
}
std::mutex mtx;
std::condition_variable cond_var;
void armer_Finder::find(xbot::Frame &image, std::vector<Target> &Target_container) {
    using namespace cv;
    using namespace std;  
    vector<LightBar> lightBars;
    for (int i=0; i < image.edges.size(); ++i)
    {
        if (check(&image.edges[i]))
        {
            cout << "lightbar detected!" << endl;
            lightBars.push_back(LightBar(image.edges[i]));
        }
    }
    vector<LightBar>::iterator now = lightBars.begin();
    vector<vector<int>> all_pos;
    vector<float> all_error;
    bool  paired = false;
    vector<LightBar>::iterator i_small ;
    vector<LightBar>::iterator j_small ;
    int height_rate_small = 180;
    for (; now < lightBars.end(); ++now)
    {
        for (vector<LightBar>::iterator j = now + 1; j < lightBars.end(); ++j)
        {
            float angle_error = abs(now ->angle -  j->angle);
            float height_rate = now ->height /  j->height;
            if (angle_error < MAX_ANGLE_ERROR&&height_rate<MAX_HEIGHT_RATE&&height_rate>MIN_HEIGHT_RATE)
            {
                if (height_rate_small > angle_error)
                {
                    height_rate_small = angle_error;
                    i_small = now;
                    j_small = j;
                }
                paired = true;
            }
        }
    }
    if (paired)
    {
        cout << "paired\n";
        Target_container.push_back(Armer(*i_small, *j_small));
        cond_var.notify_one();
    }
    lightBars.clear();
}

bool energy_Finder::check(Edge* target) {
    return false;
}