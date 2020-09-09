#ifndef MONOCULAR_CALIBRATION_H
#define MONOCULAR_CALIBRATION_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class MonocularCalibration
{
public:
  MonocularCalibration(vector<Mat>& imgs, Size patternSize, Size winSize = Size(5, 5));

  void getReProjError();
  bool startCalibrate();
  Mat cameraMatrix;
  Mat distCoeffs;

private:
  bool getIthChessboardCornerSubPix(int ith, InputOutputArray corners);
  void getChessboardWorldPoints();
  void getChessboardCornerPoints();

private:
  vector<Mat> imgs;
  Size patternSize;
  // Size squareSize;
  Size winSize;

  bool isLog;
  bool isShowImage;

  vector<vector<Point3f>> objectPoints;
  vector<vector<Point2f>> cornerPoints;
  vector<Mat> rvecs;
  vector<Mat> tvecs;

  int n; //
  int m; //

  double reProjError;
};

#endif // MONOCULAR_CALIBRATION_H
