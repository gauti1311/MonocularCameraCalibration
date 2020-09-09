#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

// cv::Mat takePicture()
//{
//  cv::VideoCapture cam(0);
//  cv::Mat pic;
//  while (!cam.isOpened())
//  {
//    std::cout << "Failed to make connection to cam" << std::endl;
//    cam.open(0, 0);
//  }

//  cam >> pic;
//  return pic;
//}
// int main()
//{
//  cv::waitKey(1000);
//  cv::Mat pic1;
//  pic1 = takePicture();
//  cv::imshow("camera", pic1);
//  cv::waitKey();
//}
