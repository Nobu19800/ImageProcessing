// -*- C++ -*-
/*!
 * @file  SubStractCaptureImage.h
 * @brief SubStractCaptureImage component
 * @date  $Date$
 *
 * $Id$
 */

#ifndef SUBSTRACTCAPTUREIMAGE_H
#define SUBSTRACTCAPTUREIMAGE_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <opencv2/opencv.hpp>

#define	MASK_THRESHOLD		1       /* backgroundMaskImageやstillObjectMaskImageを生成するための閾値 */
#define THRESHOLD_MAX_VALUE	255   /* 2値化の際に使用する最大値 */

#define	BACKGROUND_ALPHA				0.01      /* 背景更新の際の混合比率 */
#define	BACKGROUND_INITIAL_THRESHOLD	20  /* 背景の初期閾値 */

#define	STILL_OBJECT_ALPHA				0.1     /* 静止物体更新の際の混合比率 */
#define	STILL_OBJECT_INITIAL_THRESHOLD	255 /* 静止物体の初期閾値 */

#define	THRESHOLD_COEFFICIENT	5.0   /* 閾値の値を引く際の使用する閾値にかける数 */

#define	NOT_STILL_DEC_STEP			10	/* 動いた場合のカウンタ減算量 */
#define	STILL_OBJECT_TO_BACKGROUND	100 /* 背景として採用する基準 */


// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class SubStractCaptureImage
 * @brief SubStractCaptureImage component
 *
 */
class SubStractCaptureImage
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  SubStractCaptureImage(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~SubStractCaptureImage();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::CameraImage m_image_orig;
  /*!
   */
  InPort<RTC::CameraImage> m_image_origIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::CameraImage m_image_out;
  /*!
   */
  OutPort<RTC::CameraImage> m_image_outOut;
  RTC::CameraImage m_foreMaskImg;
  /*!
   */
  OutPort<RTC::CameraImage> m_foreMaskImgOut;
  RTC::CameraImage m_stillMaskImg;
  /*!
   */
  OutPort<RTC::CameraImage> m_stillMaskImgOut;
  RTC::CameraImage m_backGroundImg;
  /*!
   */
  OutPort<RTC::CameraImage> m_backGroundImgOut;
  RTC::CameraImage m_stillImg;
  /*!
   */
  OutPort<RTC::CameraImage> m_stillImgOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

	 //cv::Mat inputImage;
	 cv::Mat backgroundAverageImage;     /* 背景の平均値保存用IplImage */
	 cv::Mat backgroundThresholdImage;   /* 背景の閾値保存用IplImage */
	 cv::Mat stillObjectAverageImage;    /* 静止物体の平均値保存用IplImage */
	 cv::Mat stillObjectThresholdImage;  /* 静止物体の閾値保存用IplImage */
	 cv::Mat stillObjectCounterImage;    /* 静止物体のカウンタ用IplImage */
	 cv::Mat backgroundDifferenceImage;  /* 背景差分画像用IplImage */
	 cv::Mat stillObjectDifferenceImage; /* 静止物体差分画像用IplIMage */
	 cv::Mat thresholdImage32;           /* 32bitの閾値画像用IplImage */
	 cv::Mat thresholdImage;             /* 閾値画像用IplImage */
	 cv::Mat resultImage;                /* 結果画像用IplImage */
	 cv::Mat backgroundMaskImage;        /* 背景マスク画像用IplImage */
	 cv::Mat foregroundMaskImage;        /* 前景マスク用IplImage */
	 cv::Mat stillObjectMaskImage;       /* 静止物体マスク用IplImage */
	 cv::Mat movingObjectMask;           /* 動物体マスク用IplImage */
	 cv::Mat backgroundCopyMaskImage;    /* 背景にコピーする際に使用するマスク用IplImage */
	 cv::Mat tmpMaskImage;       /* テンポラリ用IplImage */
	 cv::Mat tmp2MaskImage;      /* テンポラリ用IplImage(その2) */
	 cv::Mat frameImage32;       /* 32bitのキャプチャした画像用IplImage */
	 cv::Mat backgroundImage;    /* 背景画像用IplImage */
	 cv::Mat stillObjectImage;   /* 静止物体画像用IplImage */
	 cv::Mat outputImage;

	 cv::Mat foreGroundMaskBuff;
	 cv::Mat stillObjectMaskBuff;
	 cv::Mat backGroundBuff;
	 cv::Mat stillObjectImageBuff;
	 cv::Mat stillObjectCounterBuff;

  int key;    /* キー入力用の変数 */
};


extern "C"
{
  DLL_EXPORT void SubStractCaptureImageInit(RTC::Manager* manager);
};

#endif // SUBSTRACTCAPTUREIMAGE_H
