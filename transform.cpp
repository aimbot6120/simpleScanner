#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

cv::Mat perspectiveTransform(cv::Mat image, std::vector <cv::Point> pts,cv::Size size)
{
	for (int i = 0; i != pts.size(); ++i) std::cout << pts[i] << " "; std::cout<<std::endl;

	std::vector<cv::Point> rect = order_points(pts);
	for (int i = 0; i != rect.size(); ++i) std::cout << rect[i] << " "; std::cout << std::endl;
	float points[8], tl[2], tr[2], br[2], bl[2];
	points[0] = tl[0] = rect.at(0).x;
	points[1] = tl[1] = rect.at(0).y;
	points[2] = tr[0] = rect.at(1).x;
	points[3] = tr[1] = rect.at(1).y;
	points[4] = br[0] = rect.at(2).x;
	points[5] = br[1] = rect.at(2).y;
	points[6] = bl[0] = rect.at(3).x;
	points[7] = bl[1] = rect.at(3).y;
	cv::Mat src = cv::Mat(4, 2, CV_32F, &points);
	std::cout << "image done";
	src.reshape(4, 2);
	float a, b, maxWidth, maxHeight;
	a = sqrt(pow((br[0] - bl[0]), 2) + pow((br[1] - bl[1]), 2));
	b = sqrt(pow((tr[0] - tl[0]), 2) + pow((tr[1] - tl[1]), 2));
	maxWidth = std::max(int(a), int(b));

	a = sqrt(pow((tr[0] - br[0]), 2) + pow((tr[1] - br[1]), 2));
	b = sqrt(pow((tl[0] - bl[0]), 2) + pow((tl[1] - bl[1]), 2));
	maxHeight = std::max(int(a), int(b));

	float dstdata[8] = { 0, 0,maxWidth - 1, 0   , maxWidth - 1, maxHeight - 1,0, maxHeight - 1 };
	cv::Mat dst = cv::Mat(4, 2, CV_32F, &dstdata);
	dst.reshape(4, 2);
	//   compute the perspective transform cv::Matrix and then apply it
	cv::Mat M, warped;
	M = getPerspectiveTransform(src, dst);
	warpPerspective(image, warped, M, cv::Size(maxWidth, maxHeight));
	//    return the warped image
	std::cout << cv::Size(maxWidth, maxHeight);
	return warped;
}

std::vector<cv::Point> order_points(std::vector<cv::Point> pts)
{
	std::vector<cv::Point> rect;
	//cout << "order_points_start";
	sort(pts.begin(), pts.end(), [](cv::Point &a, cv::Point &b){return ((a.x + a.y) < (b.x + b.y));});
	//cout << "sort done";
	rect.push_back(pts.at(0));
	//cout << "push back 0";
	if (pts.at(1).x > pts.at(2).x)
	{
		rect.push_back(pts.at(1));
		rect.push_back(pts.at(3));
		rect.push_back(pts.at(2));
	}
	else
	{
		rect.push_back(pts.at(2));
		rect.push_back(pts.at(3));
		rect.push_back(pts.at(1));
	}
	//cout << "order_points_end";
	return rect;
}
