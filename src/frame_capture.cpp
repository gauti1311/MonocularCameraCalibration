#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
  std::string file = "video.mp4";
  cv::VideoCapture cap(file); 
  if (!cap.isOpened())       
    std::cout << "camera can't opened" << std::endl;
    return -1;
  else
  {
    std::cout << "camera opened" << std::endl;
  }
  cv::Mat frame;
  int count = 0;
  for (;;)
  {
    // wait for a new frame from camera and store it into 'frame'
    cap.read(frame);
    // check if we succeeded
    if (frame.empty())
    {
      std::cerr << "ERROR! blank frame grabbed\n";
      break;
    }
    // show live and wait for a key with timeout long enough to show images
    std::string name = "./../frame/" + std::to_string(count) + ".jpg";
    cv::imshow("Live", frame);
    cv::imwrite(name, frame);
    count++;
  }
  cap.release();
  cv::destroyAllWindows(); // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
