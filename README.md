# XBOTCON2022无限机甲杯视觉开源代码说明(第二版)

## 一.代码运行环境：

| 操作系统 | 运行库 |
| --- | --- |
| windows11 | OpenCv-4.5.4   |
|  | Eigen3 |

## 二.编译安装

直接从github下载源码更改文件路径后编译运行

# 三.主要代码原理：

- 图像预处理：
    
    
    1.由于装甲板的颜色为蓝色，故将图像进行通道分离，将蓝色通道减去红色通道得到最好的成像效果。
    2.之后对图像进行阈值化，阈值化准备有两套方案，手动设定阈值和自适应阈值化，手动调参鲁棒性较差，而OTSU法自适应阈值化在背景光比较强时分割效果很差，且增加了时间开销，需要根据具体情况考虑选择哪种方案。
    
- 装甲板检测与筛选：
    
    筛选装甲板灯条的条件有：轮廓面积（可以筛除一些小干扰点）轮廓最小外接矩形的长宽比，轮廓最小外接矩形和轮廓面积的比值，轮廓的凸度（最小外接凸多边形与轮廓面积比值）
    
    灯条匹配的条件：两个倾角相差最小的灯条匹配为一个完整的装甲板。
    
- 卡尔曼滤波器预测目标运动轨迹：
    
    卡尔曼滤波的原理参考：[【学习笔记】卡尔曼滤波超详细推导和理解举例（以RoboMaster目标预测为例）_安河桥北以北的博客-CSDN博客_卡尔曼滤波适用条件](https://blog.csdn.net/Fosu_Chenai/article/details/113112833)
    
    [https://www.researchgate.net/publication/356209829_jiyukaermanlubodemubiaoshibiegenzongyushejixitongsheji?utm_oi=1333859106986459136](https://www.researchgate.net/publication/356209829_jiyukaermanlubodemubiaoshibiegenzongyushejixitongsheji?utm_oi=1333859106986459136)
    
- pnp测距和重力误差消除：
    
    使用OpenCV中的solvepnp（）通过装甲板的四个角点解算旋转向量rvecs，经过rodrigues变换获得旋转矩阵，根据以下公式可以算出三维点坐标信息。
    
    ![Untitled](XBOTCON2022%E6%97%A0%E9%99%90%E6%9C%BA%E7%94%B2%E6%9D%AF%E8%A7%86%E8%A7%89%E5%BC%80%E6%BA%90%E4%BB%A3%E7%A0%81%E8%AF%B4%E6%98%8E(%E7%AC%AC%E4%BA%8C%E7%89%88)%204318165687fb4b4dbb0f95ada8367e9f/Untitled.png)
    
    得到距离后由简单的物理知识可以得到重力引起的误差。重力误差体现在图像上可以由以下公式近似计算(其中H1为装甲板实际高度，hscreen为装甲板在图像上的高度):
    
    $Δyscreen=ΔH*H1/hscreen$
    
- 角度计算
    
    角度计算公式：
    
    $$
    tanθx=Z/X=(xscreen−cx)/fx
    
    $$
    
    $$
    tanθy=Y/X=(yscreen−cy)/fy
    $$
    
    $$
    θx​=arctan(tanθx​)
    $$
    
    $$
    θy​=arctan(tanθy​)
    $$
    

# 四.目前代码仍存在的问题和未来的优化方向：

1. pnp测距精度不高，后续会考虑使用激光测距模块提高测距精度。
2. 考虑到装甲板的运动速度并不快，可以在检测到装甲板后，取装甲板周围一定区域为ROI，以减少图像处理的时间开销。此外为了防止跟丢，需要在一定帧后对全图进行一次检测。
3. 由于此时已经临近考试周，没有时间过多的考虑代码的实现细节，调参也很粗糙，仅仅只是能够运行而已。后续大概率要重新写过。
4. 最初版本的程序是在linux平台上编写，但应举办方的要求重新编写，仓促之下只能在windows上完成，原先实现的一些功能也没来得及完全移植。更多细节见：[https://github.com/for-all-fistance/test_code.git](https://github.com/for-all-fistance/test_code.git)

## 五.Notes

1. 受到疫情的影响，本次比赛中出现了很多意外，视觉没能在赛场发挥作用，最后居然变成了编程大赛，有点让人遗憾。
2. 感谢东南大学SEU-SuperNova-CVRA战队开源代码[https://github.com/SEU-SuperNova-CVRA/Robomaster2018-SEU-OpenSource.git](https://github.com/SEU-SuperNova-CVRA/Robomaster2018-SEU-OpenSource.git)和上海交通大学RoboMaster 2019赛季 视觉代码[https://github.com/xinyang-go/SJTU-RM-CV-2019.git](https://github.com/xinyang-go/SJTU-RM-CV-2019.git)。前辈的开源代码让我受益匪浅。

**作者：碱基互补配队  自动化科学与工程学院 21级 李佳梁**