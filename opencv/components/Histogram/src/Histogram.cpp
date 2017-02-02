// -*- C++ -*-
/*!
 * @file  Histogram.cpp
 * @brief Histogram image component
 * @date $Date$
 *
 * $Id$
 */

#include "Histogram.h"
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* histogram_spec[] =
  {
    "implementation_id", "Histogram",
    "type_name",         "Histogram",
    "description",       "Histogram image component",
    "version",           "1.1.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.brightness", "100",
    "conf.default.contrast", "100",
    // Widget
    "conf.__widget__.brightness", "slider.1",
    "conf.__widget__.contrast", "slider.1",
    // Constraints
    "conf.__constraints__.brightness", "0<=x<=200",
    "conf.__constraints__.contrast", "0<=x<=200",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Histogram::Histogram(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_image_origIn("original_image", m_image_orig),
    m_image_histogramImageOut("histogram_image", m_image_histogramImage),
    m_image_histogramOut("histogram", m_image_histogram)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Histogram::~Histogram()
{
}



RTC::ReturnCode_t Histogram::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("original_image", m_image_origIn);
  
  // Set OutPort buffer
  addOutPort("histogram_image", m_image_histogramImageOut);
  addOutPort("histogram", m_image_histogramOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("brightness", m_brightness, "100");
  bindParameter("contrast", m_contrast, "100");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Histogram::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Histogram::onActivated(RTC::UniqueId ec_id)
{


  /* OutPort画面サイズの初期化 */
  m_image_histogram.width = 0;
  m_image_histogram.height = 0;
  m_image_histogramImage.width = 0;
  m_image_histogramImage.height = 0;

  /* ヒストグラムに描画される縦棒の数 */
  //histogramSize = 128;
  /* ヒストグラムの範囲 */
  //range_0[0] = 0;
  //range_0[1] = 256;

  /* ヒストグラム各次元の範囲を示す配列のポインタ */
  //ranges[0] = range_0 ;

  /* ヒストグラムに描画される縦棒の数 */
  histogramSize = 128;
  /* ヒストグラムの範囲 */
  range_0[0] = 0;
  range_0[1] = 256;

  /* ヒストグラムを生成 */
  //histogram = cvCreateHist( DIMENSIONS, &histogramSize, CV_HIST_ARRAY, ranges, UNIFORM );

  /* 行列を生成 */
  lookUpTableMatrix.create(cv::Size(1, 256), CV_8UC1);
  //lookUpTableMatrix = cvCreateMatHeader( 1, 256, CV_8UC1 );

  /* 濃度対応行列に濃度対応表をセット */
  //lookUpTableMatrix.setTo(lookUpTable);

  //cv::setData( lookUpTableMatrix, lookUpTable, 0 );
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Histogram::onDeactivated(RTC::UniqueId ec_id)
{
  
  

  if (!imageBuff.empty())
  {
	  imageBuff.release();
  }
  if (!grayImage.empty())
  {
	  grayImage.release();
  }
  if (!destinationImage.empty())
  {
	  destinationImage.release();
  }
  if (!histogramImage.empty())
  {
	  histogramImage.release();
  }
  if (!histogramBarImage.empty())
  {
	  histogramBarImage.release();
  }


  return RTC::RTC_OK;
}


RTC::ReturnCode_t Histogram::onExecute(RTC::UniqueId ec_id)
{
  /* 新しいデータのチェック */
  if(m_image_origIn.isNew())
  {
    /* InPortデータの読み込み */
    m_image_origIn.read();

    /* InPortとOutPortの画面サイズ処理およびイメージ用メモリ確保 */
    if(m_image_orig.width != m_image_histogram.width || m_image_orig.height != m_image_histogram.height)
    {
      m_image_histogram.width = m_image_histogramImage.width = m_image_orig.width;
      m_image_histogram.height = m_image_histogramImage.height = m_image_orig.height;



      /* イメージ用メモリの確保 */
	  imageBuff.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3);
	  grayImage.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC1);
	  destinationImage.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC1);
	  histogramBarImage.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3);
	  imageBuff.create(cv::Size(m_image_orig.width, m_image_orig.height), CV_8UC3);
      /* ヒストグラムの縦棒の横幅を計算する */
      bin_w = cvRound( ( double )histogramBarImage.size().width / histogramSize );
	  
    }

    /* InPortの画面データをコピー */
    memcpy(imageBuff.data,(void *)&(m_image_orig.pixels[0]),m_image_orig.pixels.length());

    /* RGBからグレースケールに変換 */
    cv::cvtColor( imageBuff, grayImage, CV_RGB2GRAY);

    int brightness = m_brightness - TRACKBAR_MAX_VALUE / 2;	//	輝度値
    int contrast = m_contrast - TRACKBAR_MAX_VALUE / 2;		//	コントラスト

    if ( contrast > 0 ) 
    {
      double delta = 127.0 * contrast / 100.0;
      double a = 255.0 / ( 255.0 - delta * 2 );
      double b = a * ( brightness - delta );
      for (int i = 0; i < 256; i++ )
      {
        /* 変換後の階調を求める */
        int v = cvRound( a * i + b );
			  if( v < 0 ){
          v = 0;
        }
        if( v > 255 ){
          v = 255;
        }
		lookUpTableMatrix.at<uchar>(i) = (unsigned char)v;
      }
    } else {
      double delta = -128.0 * contrast / 100.0;
      double a = (256.0 - delta * 2.0) / 255.0;
      double b = a * brightness + delta;
      for(int i = 0; i < 256; i++ )
      {
        int v = cvRound( a * i + b);
        if( v < 0 ){
          v = 0;
        }
        if( v > 255 ){
          v = 255;
        }
		lookUpTableMatrix.at<uchar>(i) = (unsigned char)v;
      }
    }
	
    /* 濃度対応行列を用いた濃度階調変換を行う */
	cv::LUT(grayImage, lookUpTableMatrix, destinationImage);

    /* グレースケールからRGBに変換 */
    cv::cvtColor( destinationImage, histogramImage, CV_GRAY2RGB );

    /* 画像データのサイズ取得 */
	int len = histogramImage.channels() * histogramImage.size().width * histogramImage.size().height;
    m_image_histogramImage.pixels.length(len);

    /* 変転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_histogramImage.pixels[0]), histogramImage.data,len);

    /* 変転した画像データをOutPortから出力 */
    m_image_histogramImageOut.write();

    /* 画像のヒストグラムを計算する */
	int image_num = 1;
	int channels[] = { 0 };
	
	
	
	const float *ranges[] = { range_0 };

	
	
	

	/* ヒストグラム各次元の範囲を示す配列のポインタ */
	
	
	
	cv::calcHist(&destinationImage, image_num, channels, cv::Mat(), histogram, DIMENSIONS, &histogramSize, ranges);
	
    float max_value = 0;
    /* ヒストグラム値の最大値を得る */
    //cvGetMinMaxHistValue( histogram, NULL, &max_value, NULL, NULL );

    /* ヒストグラムを最大値によって正規化する */
	//histogram.convertTo(histogram, CV_32F, (double)histogramBarImage.size().height);
	cv::normalize(histogram, histogram, (double)histogramBarImage.size().height);
    //cvConvertScale( histogram->bins, histogram->bins, 
    //( ( double )histogramBarImage->height ) / max_value, SCALE_SHIFT );

    /* ヒストグラム画像を白で初期化する */
	histogramBarImage.setTo(cv::Scalar::all(255));
	
	
    /* ヒストグラムの縦棒を描画する */
    for ( int i = 0; i < histogramSize; i++ ) {
      cv::rectangle(
        histogramBarImage,
        cvPoint( i * bin_w, histogramBarImage.size().height ),
		cvPoint((i + 1) * bin_w, histogramBarImage.size().height - cvRound(histogram.at<float>(i))),
		cv::Scalar::all(0),
        LINE_THICKNESS,
        LINE_TYPE,
        SHIFT
      );
    }

    /* 画像データのサイズ取得 */
	len = histogramBarImage.channels() * histogramBarImage.size().width * histogramBarImage.size().height;
    m_image_histogram.pixels.length(len);

    /* 変転した画像データをOutPortにコピー */
    memcpy((void *)&(m_image_histogram.pixels[0]), histogramBarImage.data,len);

    /* 変転した画像データをOutPortから出力 */
    m_image_histogramOut.write();

  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Histogram::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Histogram::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void HistogramInit(RTC::Manager* manager)
  {
    coil::Properties profile(histogram_spec);
    manager->registerFactory(profile,
                             RTC::Create<Histogram>,
                             RTC::Delete<Histogram>);
  }
  
};


