#include <iostream>

#include <opencv2/opencv.hpp>

#include "scanner.cpp"
#include "transform.cpp"

int main(int argc,char** argv)
{
    std::string path = "test.png";
    std::string resultPath = "result.png"; //add overwrite safety 
    if(argc > 1 ) 
        path = argv[1];
    else 
        std::cout<<"Usage "<<argv[0]<<" [image path] [result path {default result.png (overwrites)}]\n";

    cv::Mat img = cv::imread(path); //check read
    if(img.empty())
        {
            std::cout<<"Cannot read or find image\n";
            exit(0);
        }

    std::cout<<img.size()<<" "<<img.channels()<<"\n";
    cv::Mat org(img);

    auto points = scanner(img);
    cv::Mat result = perspectiveTransform(org,points);
    cv::imwrite(resultPath,result);
    std::cout<<"saved "<<resultPath<<" "<<result.size()<<std::endl;

    return 0;
}