#include "MonocularCalibration.h"

MonocularCalibration::MonocularCalibration(std::vector<cv::Mat>& imgs, cv::Size patternSize, cv::Size winSize)
{
  this->imgs = imgs;
  this->patternSize = patternSize;
  this->winSize = winSize;
  isLog = true;
  isShowImage = true;
  n = imgs.size();
  m = 0;
}

bool MonocularCalibration::getIthChessboardCornerSubPix(int i, cv::InputOutputArray corners)
{
  if (isLog)
  {
    std::cout << "getting " << i << "th chessboardCornerSubpix" <<std::endl;
  }

  bool found = cv::findChessboardCorners(imgs[i], patternSize, corners);
  if (found)
  {
    cv::Mat gray;
    cv::cvtColor(imgs[i], gray, cv::COLOR_BGR2GRAY);
    cv::find4QuadCornerSubpix(gray, corners, winSize);

    if (isShowImage)
    {
      cv::drawChessboardCorners(imgs[i], patternSize, corners, found);
      cv::namedWindow("camera", CV_WINDOW_NORMAL);
      cv::imshow("camera", imgs[i]);
      cv::imwrite(std::to_string(i) + ".jpg", imgs[i]);
      cv::waitKey(100);
    }
  }
  return found;
}


void MonocularCalibration::getChessboardWorldPoints()
{
  int count = n - m;
  std::vector<cv::Point3f> tmpPoints;

  for (int i = 0; i < patternSize.height; ++i)
  {
    for (int j = 0; j < patternSize.width; ++j)
    {
      tmpPoints.push_back(cv::Point3f(j, i, 0));
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
    std::cout << "get Chessboard Corner Points " << std::endl;
  }
  for (int i = 0; i < n; ++i)
  {
    std::vector<cv::Point2f> corners;
    bool found = getIthChessboardCornerSubPix(i, corners);
    if (!found)
    {
      m++;
      std::cout << " Not all corner detected" << std::endl;
    }
    else
    {
      std::cout << " corner detection successfull" << std::endl;
      cornerPoints.push_back(corners);
    }
  }
  if (isLog)
  {
    std::cout<< "result : total " << imgs.size() << " images, " << n - m << " succeed " << m << " failed " << std::endl;
  }
}
bool MonocularCalibration::startCalibrate()
{
  getChessboardCornerPoints();

  if (n - m < 5) return false;

  getChessboardWorldPoints();

  cv::calibrateCamera(objectPoints, cornerPoints, imgs[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);

  getReProjError();

  std::cout << " \n Camera Matrix: " << std::endl;
  std::cout << cameraMatrix << std::endl;
  std::cout << "\n Distortion Matrix : " << std::endl;
  std::cout << distCoeffs << std::endl;
}

void MonocularCalibration::getReProjError()
{
  reProjError = 0;
  double err = 0;
  for (int i = 0; i < n - m; ++i)
  {
    std::vector<cv::Point3f>& tmpObjPoints = objectPoints[i];
    std::vector<cv::Point2f>& tmpCornerPoints = cornerPoints[i];
    std::vector<cv::Point2f> reProPoints;
    cv::projectPoints(tmpObjPoints, rvecs[i], tvecs[i], cameraMatrix, distCoeffs, reProPoints);

    cv::Mat cornerPointMat(tmpCornerPoints);
    cv::Mat reProPointMat(reProPoints);
    err = cv::norm(cornerPointMat, reProPointMat, cv::NORM_L2);
    err /= (patternSize.width * patternSize.height);
    
    std::cout << i << "th images avg. error in pixel: " << err << std::endl;
    reProjError += err;
  }
  reProjError /= (n - m);
  std::cout << " \n avg. re-projection error: " << reProjError << std::endl;
}
