#include<iostream>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>

void showContours(cv::Mat& img,std::vector<cv::Point> contour);

std::vector<cv::Point> scanner(cv::Mat &img, bool showImg = 1)
{
    const int ksize = 5;
    const int sampleWidth = 300;

    cv::Mat resized;
    
    float ratio = 1.0f;

    if(sampleWidth < img.cols)
    {
        ratio = (float)img.cols/sampleWidth;
        cv::resize(img,resized,cv::Size(sampleWidth,img.rows/ratio));
    }
    else
        resized = img;
    
    std::cout<<"downsampled to "<< resized.size()<<" ratio "<<ratio<<"\n";

    cv::Mat gray (resized);
    cv::cvtColor(resized,gray,cv::COLOR_BGR2GRAY); //handle non bgr type images

    cv::medianBlur(gray,gray,ksize);

    cv::Mat &canny = gray;
    cv::Canny(gray,canny,75,200);

    if(showImg)
    {
        cv::imshow("canny",canny);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    cv::Mat morphed;
    //cv::dilate(canny,dilated,cv::getStructuringElement())
    cv::morphologyEx(canny,morphed,cv::MORPH_CLOSE,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5)));

    if(showImg)
    {
        cv::imshow("morphed",morphed);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> finalContour;
    finalContour.reserve(4);
    cv::findContours(morphed,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2){
    return cv::contourArea(c1, false) > cv::contourArea(c2, false);});
    for(int i = 0; i !=contours.size(); ++i)
    {
        double perimeter = cv::arcLength(contours[i],true);
        std::vector<cv::Point> approx; 
        cv::approxPolyDP(contours[i],approx,0.02*perimeter,true);

        if(approx.size() == 4)
            {
                finalContour = approx;
                break;
            }
    }

    std::cout<<contours.size()<<" contours\n";
    // std::cout<<"downsampled contour"<<finalContour<<"\n";
    if(showImg) showContours(resized,finalContour);
    for(cv::Point &point : finalContour)
    {
        point.x *= 3.2f;
        point.y *= 3.2f;
    }
    // std::cout<<"original contour"<<finalContour<<"\n"; 
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