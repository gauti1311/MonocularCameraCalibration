#include "MonocularCalibration.h"

MonocularCalibration::MonocularCalibration(vector<Mat>& imgs, Size patternSize, Size winSize)
{
  this->imgs = imgs;
  this->patternSize = patternSize;
  // this->squareSize = squareSize;
  this->winSize = winSize;
  isLog = true;
  isShowImage = true;
  n = imgs.size();
  m = 0;
}

bool MonocularCalibration::getIthChessboardCornerSubPix(int ith, InputOutputArray corners)
{
  if (isLog)
  {
    cout << "getting " << ith << "th chessboardCornerSubPix...";
  }

  bool found = cv::findChessboardCorners(imgs[ith], patternSize, corners);
  if (found)
  {
    Mat grayImg = imgs[ith];
    if (grayImg.type() != CV_8UC1) cvtColor(imgs[ith], grayImg, COLOR_BGR2GRAY);

    cv::find4QuadCornerSubpix(grayImg, corners, winSize);

    if (isShowImage)
    {
      cv::drawChessboardCorners(imgs[ith], patternSize, corners, found);
      cv::namedWindow("camera", CV_WINDOW_NORMAL); // CV_WINDOW_NORMAL就是0
      cv::imshow("camera", imgs[ith]);
      cv::imwrite(std::to_string(ith) + ".jpg", imgs[ith]);
      cv::waitKey(1000);
    }
  }
  cv::imwrite(std::to_string(ith) + ".jpg", imgs[ith]);
  return found;
}

void MonocularCalibration::getChessboardWorldPoints()
{
  int count = n - m;
  vector<Point3f> tmpPoints;

  for (int i = 0; i < patternSize.height; ++i)
  {
    for (int j = 0; j < patternSize.width; ++j)
    {
      tmpPoints.push_back(Point3f(j, i, 0));
    }
  }
  for (int k = 0; k < count; ++k)
  {
    objectPoints.push_back(tmpPoints);
  }
}
void MonocularCalibration::getChessboardCornerPoints()
{
  if (isLog)
  {
    cout << "start getChessboardCornerPoints..." << endl << endl;
  }
  for (int i = 0; i < n; ++i)
  {
    vector<Point2f> corners;
    bool found = getIthChessboardCornerSubPix(i, corners);
    if (!found)
    {
      m++;
      cout << " failed" << endl;
    }
    else
    {
      cout << " successfull" << endl;
      cornerPoints.push_back(corners);
    }
  }
  if (isLog)
  {
    cout << endl << "result : total " << imgs.size() << " images, " << n - m << " succeed " << m << " fail " << endl;
  }
}
bool MonocularCalibration::startCalibrate()
{
  getChessboardCornerPoints();
  if (n - m < 3) return false;

  getChessboardWorldPoints();
  cv::calibrateCamera(objectPoints, cornerPoints, imgs[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);
  cout << endl;
  getReProjError();

  cout << endl;
  cout << "Intrinsic parameters : " << endl;
  cout << cameraMatrix << endl;
  cout << "Distortion parameter : " << endl;
  cout << distCoeffs << endl;
}

void MonocularCalibration::getReProjError()
{
  reProjError = 0;
  double err = 0;
  for (int i = 0; i < n - m; ++i)
  {
    const vector<Point3f>& tmpObjPoints = objectPoints[i];
    const vector<Point2f>& tmpCornerPoints = cornerPoints[i];
    vector<Point2f> reProPoints;
    cv::projectPoints(tmpObjPoints, rvecs[i], tvecs[i], cameraMatrix, distCoeffs, reProPoints);

    Mat cornerPointMat(tmpCornerPoints);
    Mat reProPointMat(reProPoints);
    /* Mat cornerPointMat(1, reProPoints.size(), CV_32FC2); */
    /* Mat reProPointMat(1, reProPoints.size(), CV_32FC2); */
    /* for(int j = 0; j < reProPoints.size(); ++j) */
    /* { */
    /* 	cornerPointMat.at<Vec2f>(0, j) = Vec2f(tmpCornerPoints[j].x, tmpCornerPoints[j].y); */
    /* 	reProPointMat.at<Vec2f>(0, j) = Vec2f(reProPoints[j].x, reProPoints[j].y); */
    /* } */
    err = norm(cornerPointMat, reProPointMat, NORM_L2);
    err /= (patternSize.width * patternSize.height);
    cout << i << "th images avg. error in pixel: " << err << endl;
    reProjError += err;
  }
  reProjError /= (n - m);
  cout << endl << "avg. re-projection error: " << reProjError << endl;
}
