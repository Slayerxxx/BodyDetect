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
	int remove_count = 0;       //��¼��ȥ�ĸ���  

	//point_status���ڼ�¼ÿ�����ص�ļ���״̬��
	//- UNCHECKED��		δ���
	//- CHECKED��		���ڼ��
	//- UNQUALIFIED��	���ϸ���Ҫ��ת��ɫ��
	//- QUALIFIED��		�ϸ񣨼�����Ҫ��飩  
	Mat  point_status = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows; ++i) {
		uchar* row_data = src.ptr<uchar>(i);
		uchar* row_status = point_status.ptr<uchar>(i);
		for (int j = 0; j < src.cols; ++j) {
			// �Ƚ�����ֵΪ��ɫ�����ص���ΪQUALIFIED����Щ�㲻Ҫ���м��
			if (row_data[j] == 0) {
				row_status[j] = QUALIFIED;
			}
		}
	}

	//��¼�����λ�� 
	std::vector<Point2i> neighbor_position;
	// ��8���ڽӵ��������
	neighbor_position.push_back(Point2i(-1, 0));
	neighbor_position.push_back(Point2i(1, 0));
	neighbor_position.push_back(Point2i(0, -1));
	neighbor_position.push_back(Point2i(0, 1));
	neighbor_position.push_back(Point2i(-1, -1));
	neighbor_position.push_back(Point2i(-1, 1));
	neighbor_position.push_back(Point2i(1, -1));
	neighbor_position.push_back(Point2i(1, 1));

	int current_x = 0, current_y = 0;

	//��ʼ���  
	for (int i = 0; i < src.rows; ++i) {
		uchar* row_status = point_status.ptr<uchar>(i);
		for (int j = 0; j < src.cols; ++j) {
			//�����ǰ��Ϊ��������ʼ���õ�
			if (row_status[j] == UNCHECKED) {
				std::vector<Point2i> buffer;
				//�����ر�ע�⣺��Ϊ���Ͻ�Ϊ����ԭ�㣬��ǰ���ص��i�е�j��
				//���������֮��һ����ȷ��x���꣬��ȷ��y���꣬����(j,i)������ѹ��buffer���������(j,i)
				buffer.push_back(Point2i(j, i));
				//����ǰ���״̬����Ϊ�Ѽ�⣨CHECKED��
				point_status.at<uchar>(i, j) = CHECKED;

				for (int m = 0; m < buffer.size(); m++) {
					//���8�������  
					for (int n = 0; n < 8; n++) {
						//�õ���ǰ���ص��8���ڽӵ����������ֵ��
						//-	x����Ϊ����ǰ���ص�x + ���λ�� x
						//- y����Ϊ����ǰ���ص�y + ���λ�� y
						current_x = buffer.at(m).x + neighbor_position.at(n).x;
						current_y = buffer.at(m).y + neighbor_position.at(n).y;
						//��ֹԽ�� 
						if (current_x >= 0 && current_x < src.cols && current_y >= 0 && current_y < src.rows) {
							// �����ǰ��Ϊ��⣬����м��
							if (point_status.at<uchar>(current_y, current_x) == UNCHECKED) {
								//���ڽӵ����buffer
								buffer.push_back(Point2i(current_x, current_y));
								//�����ڽӵ�ļ��״̬�������ظ����  
								point_status.at<uchar>(current_y, current_x) = CHECKED;
							}
						}
					}
				}

				//�жϽ�����Ƿ񳬳��޶���area��С��
				//- ����������򽫽����ΪUNIQUALIFED
				//- ���û�г�������˵����ǰ��ļ��ʱ�ϸ��
				int check_result;
				if (buffer.size() > area)
					check_result = UNQUALIFIED;
				else
					check_result = CHECKED;

				//����status��¼  
				for (int z = 0; z < buffer.size(); z++) {
					current_x = buffer.at(z).x;
					current_y = buffer.at(z).y;
					point_status.at<uchar>(current_y, current_x) += check_result;
				}
			}
		}
	}

	//��ʼ��ת�����С������  
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