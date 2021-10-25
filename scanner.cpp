#include<iostream>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>

#include "temp.cpp"

cv::Mat scanner(std::string path)
{
    const int ksize = 5;
    const int sampleWidth = 300;

    cv::Mat img = cv::imread(path); //check read
    cv::Mat resized;
    
    int ratio = img.cols/sampleWidth;
    cv::resize(img,resized,cv::Size(sampleWidth,img.rows/ratio));

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
    
    //cv::Mat temp = resiz
    showContours(resized,finalContour);
    return cv::Mat();

}

int main(int argc,char** argv)
{
    std::string path = "doc.jpeg";
    if(argc > 1 ) path = argv[1];
    scanner(path);
    return 0;
}