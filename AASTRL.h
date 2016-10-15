#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

const double a = 2;		//信息素权重
const double b = 2;		//启发因子权重
const int R = 150;		//半径
const int omega = 5;		//每个簇的宽度
const int K = 5;		//最大跨环数
const int sigma = 5;		//未知参数
const int epsilon = 50;		//传感器能量
const int phi = 400;		//信息量
const double E_elec = 0.000000005;		//传输耗能常数1
const double epsilon_fs = 0.00000000001;	//传输耗能常数2
const int Sensor_number_per_square_meter = 5;		//每平方米有的传感器数
const double gamma = 0.1;		//挥发系数