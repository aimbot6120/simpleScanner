#include<iostream>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>

void showContours(cv::Mat& img,std::vector<cv::Point> contour);

std::vector<cv::Point> scanner(cv::Mat &img, bool showImg = 0)
{
    const int ksize = 5;
    const int sampleWidth = 300;

    cv::Mat resized;
    
    int ratio = 1;

    if(sampleWidth < img.cols)
    {
        int ratio = img.cols/sampleWidth;
        cv::resize(img,resized,cv::Size(sampleWidth,img.rows/ratio));
    }
    else
        resized = img;
    
    cv::Mat gray = resized;
    cv::cvtColor(resized,gray,cv::COLOR_BGR2GRAY); //handle non bgr type images
    cv::medianBlur(gray,gray,ksize);

    cv::Mat &canny = gray;
    cv::Canny(gray,canny,75,200);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> finalContour;
    finalContour.reserve(4);

    cv::findContours(canny,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    
    for(auto contour:contours)
    {
        double perimeter = cv::arcLength(contour,true);
        std::vector<cv::Point> approx; 
        cv::approxPolyDP(contour,approx,0.02*perimeter,true);

        if(approx.size() == 4)
            {
                finalContour = approx;
                break;
            }
    }
    
    for(auto point : finalContour)
    {
        point.x *= ratio;
        point.y *= ratio;
    }
    if(showImg) showContours(img,finalContour);
    return finalContour;
}
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