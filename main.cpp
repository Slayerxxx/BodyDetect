#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <vector>
#include <deque>
#include <windows.h>
#include "ImageSegmentation.h"
#include "BodyDetect.h"
#include "Body.h"
#include "TargetArea.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;
using namespace std;

int main() {
	VideoCapture cap;
	namedWindow("frame", CV_WINDOW_NORMAL);
	namedWindow("ske", CV_WINDOW_NORMAL);
	namedWindow("src", CV_WINDOW_NORMAL);
	cap.open("../limbs_1_low.mkv");
	Mat frame;
	while (cap.read(frame)) {
		Mat src = frame.clone();		
		//Mat result(frame.size(), CV_8U, Scalar(0));		
		imshow("frame", frame);
		cvtColor(frame, frame, COLOR_BGR2HSV, 0);
		vector<cv::Mat> channels;
		split(frame,channels);
		frame = channels[0];
		threshold(frame, frame, 50, 255, THRESH_BINARY_INV);
		RemoveSmallRegion2(frame, frame, 20000);
		imageblur(frame,frame, Size(5, 5), 20);
		//vector<vector<Point>> contours;
		//findContours(frame,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		//drawContours(result, contours, -1, Scalar(255), 1);
		imshow("ske", frame);
		Mat src_copy = frame.clone();
		Mat thinSrc = thinImage(src_copy);					//骨架化
		Mat thin_copy = thinSrc.clone();
		vector<cv::Point2f> endpoints = skeletonEndPoints(thinSrc);		//找端点
		showThinImage(thin_copy);
		for (int i = 0; i < endpoints.size()-1; i++) {
			Point opp_point;
			opp_point.x = endpoints[i].x + 3;
			opp_point.y = endpoints[i].y + 3;
			rectangle(src, endpoints[i], opp_point,cv::Scalar(0,0,255));
		}
		imshow("src", src);
		waitKey(1);
	}
	
	
}
//int main() {
//	Mat thinSrc, src_copy,thin_copy;
//	Mat src = imread("../rab_bin.png",CV_8UC3);
//	imshow("src2", src);
//	imageblur(src, src, Size(5, 5), 120);			//光滑边缘
//	src_copy = src.clone();
//	thinSrc = thinImage(src_copy);					//骨架化
//	thin_copy = thinSrc.clone();
//	showThinImage(thin_copy);
////	waitKey(0);
//	vector<cv::Point2f> endpoints = skeletonEndPoints(thinSrc);		//找端点
////	vector<cv::Point2f> branchpoints = skeletonBranchPoints(thinSrc, 4, 8, 4);
//	for (int i = 0; i < endpoints.size()-1; i++) {
//		//line(thinSrc, endpoints[i], endpoints[i+1], Scalar(255, 255, 255), 1);
////		circle(src, endpoints[i], 1, Scalar(0, 0, 255), 2);
//		Point opp_point;
//		opp_point.x = endpoints[i].x + 3;
//		opp_point.y = endpoints[i].y + 3;
//		rectangle(src, endpoints[i], opp_point,cv::Scalar(0,255,0));
//	}
//	cout << endpoints;
//	imshow("src",src);
//	imwrite("ske_endpoints.png",src);
//	waitKey(0);
//}
//int main()
//{
//	CJcCalBody test;
//	int cutTop = 30, cutBottom = 10;
//	VideoCapture video;
//	video.open("../abc3.mp4");//limbs_low.mkv
//
//	Mat videoFrame, videoFrameResize;
//
//	while (video.read(videoFrame))
//	{
//		resize(videoFrame, videoFrameResize, Size(480, 270));
//
//		DWORD startTime = GetCurrentTime();
//
//		Rect cutRect = Rect(0, cutTop, videoFrameResize.size().width, videoFrameResize.size().height - cutTop - cutBottom);
//
//		Mat videoDisplay, videoDisplayResize;
//		videoFrameResize(cutRect).copyTo(videoDisplay);
//
//		Mat cutFrame = cutGreenScreen(videoFrameResize, cutTop, cutBottom);
//
//		test.recognizeImage(cutFrame);
//
//		std::vector<BodyData> BodyArr;
//		test.GetBodyData(BodyArr);
//
//		for (int i = 0; i < BodyArr.size(); i++)
//		{
//
//			for (int j = 0; j < 7; j++)
//			{
//				if (BodyArr[i]._keyBodyDts[j][0] != NULL)
//					circle(videoDisplay, BodyArr[i]._keyBodyDts[j][0]->pos, 4, Scalar(0, 255, 0),-1);
//			
//			}
//
//			stringstream s;
//			s << BodyArr[i]._index;
//
//
//			putText(videoDisplay, s.str(), Point(BodyArr[i]._heart.x + 5, BodyArr[i]._heart.y), CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255));
//			cout << BodyArr[i].m_fTimes << endl;
//		}
//
//		std::vector<TornadoData> TornadoArr;
//		test.GetTornadoData(TornadoArr);
//
//		for (int i = 0; i < TornadoArr.size(); i++)
//		{
//			circle(videoDisplay,TornadoArr[i]._pos, 4, Scalar(0, 0, 255), -1);
//		}
//
//		cv::imshow("test",videoDisplay);
//		waitKey(1);
//
//	}
//
//}