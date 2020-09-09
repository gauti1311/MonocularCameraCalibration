#include <stdio.h>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "MonocularCalibration.h"

using namespace std;

void getImages(const std::vector<string>& images_url, vector<cv::Mat>& images)
{
  for (size_t i = 0; i < images_url.size(); ++i)
  {
    cv::Mat img = imread(images_url[i]);
    images.push_back(img);
  }
}


void saveCalibration(string imgs_directory, cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
  std::cout << "saving calibration parameters into calibration.txt" << std::endl;
  fstream file;
  file.open(imgs_directory + "/calibration.txt");
  file << "\n" << "camera_matrix :" << cameraMatrix << std::endl;
  file << "\n" << "distortion coefficients:" << distCoeffs << std::endl;
  file.close();
}


int main()
{
  int board_width = 7;
  int board_height = 7;
  std::string imgs_directory;
  std::string file_extension;
  std::cout << "enter images folder location, eg. ../imgs/"<<std::endl;
  std::cout <<"imgs_directory:";
  std::cin >> imgs_directory;
  std::cout << "enter images format, eg. jpg"<<std::endl;
  std::cout<<"file_extentsion:";
  std::cin >> file_extension;

  std::vector<cv::Mat> images;
  std::vector<string> images_url;

  string tmp = imgs_directory[imgs_directory.size() - 1] == '/' ? "*." : "/*.";
  string path = imgs_directory + tmp + file_extension;

  cv::glob(path, images_url);

  getImages(images_url, images);

  MonocularCalibration calib(images, cv::Size(board_width, board_height));
  calib.startCalibrate();
  saveCalibration(imgs_directory, calib.cameraMatrix, calib.distCoeffs);

  return 0;
}
