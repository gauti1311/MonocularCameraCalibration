#pragma once
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>


class MonocularCalibration
{
public:
  MonocularCalibration(std::vector<cv::Mat>& imgs, cv::Size patternSize, cv::Size winSize = cv::Size(5, 5));
  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  
  void getReProjError();
  bool startCalibrate();
  bool getIthChessboardCornerSubPix(int ith, cv::InputOutputArray corners);
  void getChessboardWorldPoints();
  void getChessboardCornerPoints();

private:
  std::vector<cv::Mat> imgs;
  cv::Size patternSize;
  cv::Size winSize;

  bool isLog;
  bool isShowImage;

  std::vector<std::vector<cv::Point3f>> objectPoints;
  std::vector<std::vector<cv::Point2f>> cornerPoints;
  std::vector<cv::Mat> rvecs;
  std::vector<cv::Mat> tvecs;

  int n; // no. of images
  int m; // no. of failed img to detect corner

  double reProjError;
};
