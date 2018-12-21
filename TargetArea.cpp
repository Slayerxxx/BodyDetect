#include <cv.hpp>
#include <iostream>
#include "Defs.h"
using namespace cv;

void getMiddleArea(Mat& src) {
	int height = src.rows;
	int width = src.cols;
	std::cout << height << " " << width;
	for (int i = 0; i < height; i++) {
		uchar* p = src.ptr<uchar>(i);
		for (int j = 0; j < width; j++) {
			if (j < 4000) {
				//cout << (int)p[j] << "\n";
				p[j] = 0;
			}
		}
	}
}
void RemoveSmallRegion2(Mat& src, Mat& dest, int area) {
	int remove_count = 0;       //记录除去的个数  

	//point_status用于记录每个像素点的检验状态：
	//- UNCHECKED：		未检查
	//- CHECKED：		正在检查
	//- UNQUALIFIED：	不合格（需要反转颜色）
	//- QUALIFIED：		合格（即不需要检查）  
	Mat  point_status = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows; ++i) {
		uchar* row_data = src.ptr<uchar>(i);
		uchar* row_status = point_status.ptr<uchar>(i);
		for (int j = 0; j < src.cols; ++j) {
			// 先将像素值为黑色的像素点置为QUALIFIED，这些点不要进行检查
			if (row_data[j] == 0) {
				row_status[j] = QUALIFIED;
			}
		}
	}

	//记录邻域点位置 
	std::vector<Point2i> neighbor_position;
	// 将8个邻接点加入数组
	neighbor_position.push_back(Point2i(-1, 0));
	neighbor_position.push_back(Point2i(1, 0));
	neighbor_position.push_back(Point2i(0, -1));
	neighbor_position.push_back(Point2i(0, 1));
	neighbor_position.push_back(Point2i(-1, -1));
	neighbor_position.push_back(Point2i(-1, 1));
	neighbor_position.push_back(Point2i(1, -1));
	neighbor_position.push_back(Point2i(1, 1));

	int current_x = 0, current_y = 0;

	//开始检测  
	for (int i = 0; i < src.rows; ++i) {
		uchar* row_status = point_status.ptr<uchar>(i);
		for (int j = 0; j < src.cols; ++j) {
			//如果当前点为检测过，则开始检测该点
			if (row_status[j] == UNCHECKED) {
				std::vector<Point2i> buffer;
				//这里特别注意：因为左上角为坐标原点，当前像素点第i行第j列
				//换算成坐标之后，一般先确定x坐标，再确定y坐标，就是(j,i)，所以压入buffer的坐标就是(j,i)
				buffer.push_back(Point2i(j, i));
				//将当前点的状态设置为已检测（CHECKED）
				point_status.at<uchar>(i, j) = CHECKED;

				for (int m = 0; m < buffer.size(); m++) {
					//检查8个邻域点  
					for (int n = 0; n < 8; n++) {
						//得到当前像素点的8个邻接点的真正坐标值：
						//-	x坐标为：当前像素点x + 相对位移 x
						//- y坐标为：当前像素点y + 相对位移 y
						current_x = buffer.at(m).x + neighbor_position.at(n).x;
						current_y = buffer.at(m).y + neighbor_position.at(n).y;
						//防止越界 
						if (current_x >= 0 && current_x < src.cols && current_y >= 0 && current_y < src.rows) {
							// 如果当前点为检测，则进行检测
							if (point_status.at<uchar>(current_y, current_x) == UNCHECKED) {
								//将邻接点加入buffer
								buffer.push_back(Point2i(current_x, current_y));
								//更新邻接点的检查状态，避免重复检查  
								point_status.at<uchar>(current_y, current_x) = CHECKED;
							}
						}
					}
				}

				//判断结果（是否超出限定的area大小）
				//- 如果超出，则将结果置为UNIQUALIFED
				//- 如果没有超出，就说明当前点的检测时合格的
				int check_result;
				if (buffer.size() > area)
					check_result = UNQUALIFIED;
				else
					check_result = CHECKED;

				//更新status记录  
				for (int z = 0; z < buffer.size(); z++) {
					current_x = buffer.at(z).x;
					current_y = buffer.at(z).y;
					point_status.at<uchar>(current_y, current_x) += check_result;
				}
			}
		}
	}

	//开始反转面积过小的区域  
	for (int i = 0; i < src.rows; ++i) {
		uchar* row_data = src.ptr<uchar>(i);
		uchar* dest_row_data = dest.ptr<uchar>(i);
		uchar* row_status = point_status.ptr<uchar>(i);
		for (int j = 0; j < src.cols; ++j) {
			if (row_status[j] == UNQUALIFIED) {
				dest_row_data[j] = 0;
			}
			else if (row_status[j] == QUALIFIED) {
				dest_row_data[j] = row_data[j];
			}
		}
	}
}