#include <stdio.h>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "MonocularCalibration.h"

using namespace std;

void getImages(string configutationFile, vector<cv::Mat>& images)
{
  ifstream fin(configutationFile);
  string filename;
  while (getline(fin, filename))
  {
    cv::Mat img;
    img = imread(filename);
    images.push_back(img);
  }
}

void getImages(const vector<cv::String>& images_url, vector<cv::Mat>& images)
{
  for (size_t i = 0; i < images_url.size(); ++i)
  {
    cv::Mat img = imread(images_url[i]);
    images.push_back(img);
  }
}
void saveCalibration(string imgs_directory, cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
  cout << "saving calibration parameters into calibration.txt" << endl;

  fstream file;
  file.open(imgs_directory + "/calibration.txt");
  file << endl << "camera_matrix :" << cameraMatrix << endl;
  file << endl << "distortion coefficients:" << distCoeffs << endl;
  file.close();
}
int main(int argc, char* argv[])
{
  //  if (argc != 5)
  //  {
  //    printf("usage : ./calib [chessboard_width] [chessboard_height] [imgs_directory] [file_extension]\n");
  //    exit(-1);
  //  }

  //  int board_width = atoi(argv[1]);
  //  int board_height = atoi(argv[2]);
  //  std::string imgs_directory = argv[3];
  //  std::string file_extension = argv[4];

  int board_width = 7;
  int board_height = 7;
  std::string imgs_directory;
  std::string file_extension;
  std::cout << "enter images folder location, eg. for webcam: ../imgs/webcam_genius\n imgs_directory:" << std::endl;
  std::cin >> imgs_directory;
  std::cout << "enter images format, eg. for webcam: jpg and picocam :bmp/ \n file_extentsion:" << std::endl;
  std::cin >> file_extension;

  vector<Mat> images;
  vector<cv::String> images_url;

  string tmp = imgs_directory[imgs_directory.size() - 1] == '/' ? "*." : "/*.";
  string path = imgs_directory + tmp + file_extension;

  cv::glob(path, images_url);

  getImages(images_url, images);

  MonocularCalibration calib(images, Size(board_width, board_height));
  calib.startCalibrate();
  saveCalibration(imgs_directory, calib.cameraMatrix, calib.distCoeffs);

  return 0;
}
