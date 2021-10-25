#include<iostream>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>

#include "temp.cpp"

std::vector<cv::Point> scanner(std::string path)
{
    const int ksize = 5;
    const int sampleWidth = 300;

    cv::Mat img = cv::imread(path); //check read
    if(img.empty())
        {
            std::cout<<"Cannot read or find image\n";
            exit(0);
        }

    std::cout<<img.size()<<" "<<img.channels()<<"\n";
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
    showContours(img,finalContour);
    return finalContour;
}

int main(int argc,char** argv)
{
    std::string path = "test.png";
    if(argc > 1 ) path = argv[1];
    scanner(path);
    return 0;
}