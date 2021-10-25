#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

void showContours(cv::Mat& img,std::vector<cv::Point> contour)
{
    std::vector<std::vector<cv::Point>> tempContours;
    tempContours.push_back(contour);
    cv::Mat temp = img.clone();
    cv::drawContours(temp,tempContours,-1,cv::Scalar(0,0,255));
    cv::imshow("image",temp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}