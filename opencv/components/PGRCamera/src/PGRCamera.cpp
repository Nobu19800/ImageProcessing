// -*- C++ -*-
/*!
 * @file  PGRCamera.cpp
 * @brief PGRCamera
 * @date $Date$
 *
 * $Id$
 */

#include "PGRCamera.h"
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* pgrcamera_spec[] =
  {
    "implementation_id", "PGRCamera",
    "type_name",         "PGRCamera",
    "description",       "PGRCamera",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.index", "0",
    // Widget
    "conf.__widget__.index", "text",
    // Constraints
    ""
  };

int PGRCamera::s_number = 0;
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
PGRCamera::PGRCamera(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outputImageOut("outputImage", m_outputImage)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
PGRCamera::~PGRCamera()
{
}



RTC::ReturnCode_t PGRCamera::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("outputImage", m_outputImageOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("index", m_index, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t PGRCamera::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCamera::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCamera::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t PGRCamera::onActivated(RTC::UniqueId ec_id)
{	
	
	Error error;
	FlyCapture2::BusManager busMgr;

	m_camera = new Camera();
	
	//Index����ID���擾����
	error = busMgr.GetCameraFromIndex(m_index, &guid);
	if ( error != PGRERROR_OK ){ 
		cout << "�J����������܂���B" << endl;
		return RTC::RTC_ERROR;	
	}
	
	//ID����Connect����
	error = m_camera->Connect(&guid);
	if ( error != PGRERROR_OK ){ 
		cout << "Connection ���s�B" << endl;
		return RTC::RTC_ERROR;	
	}
	
	//�J���������擾����B
	error = m_camera->GetCameraInfo(&m_camInfo);
	if ( error != PGRERROR_OK ){ 
		cout << "���擾�@���s" << endl;
		return RTC::RTC_ERROR;	
	}
	
	//�f����Capture���n�߂�B
	error = m_camera->StartCapture();
	if ( error != PGRERROR_OK ){ 
		cout << "Capture ���s�B" << endl;
		return RTC::RTC_ERROR;	
	}
	
	return RTC::RTC_OK;
	
}


RTC::ReturnCode_t PGRCamera::onDeactivated(RTC::UniqueId ec_id)
{
	
	Error error;
	
	//�f���擾���I������B
	if(m_camera != NULL){
		error = m_camera->StopCapture();
		if ( error != PGRERROR_OK ){ 
			delete m_camera;
			return RTC::RTC_ERROR;	
		}
	}

	//connection����������B
	if(m_camera != NULL){
		error = m_camera->Disconnect();
		if ( error != PGRERROR_OK ){ 
			delete m_camera;
			return RTC::RTC_ERROR;	
		}
	}

	delete m_camera;
	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t PGRCamera::onExecute(RTC::UniqueId ec_id)
{
	Error error;
	static coil::TimeValue tm_pre;
	static int count = 0;

	// �f����Image
    error = m_camera->RetrieveBuffer( &rawImage );
    if (error != PGRERROR_OK)
    {
		cout << "�f����RawImage�̗̈��Retrieve���ł��܂���B" << endl;
        
		return RTC::RTC_ERROR;
    }

	// RawImage��RGB8Bit��Convert����B
    error = rawImage.Convert( PIXEL_FORMAT_RGB8, &convertedImage );
    if (error != PGRERROR_OK)
    {
        cout << "RGBFormat�Ƃ��ĕϊ��ł��܂���B" << endl;
        
		return RTC::RTC_ERROR;
    }
		
	int len = convertedImage.GetCols() * convertedImage.GetRows() * 3;			 //�������͈̔͌Œ�
	
	// ��ʂ̃T�C�Y��������
	m_outputImage.pixels.length(len);
	m_outputImage.width = convertedImage.GetCols();
	m_outputImage.height = convertedImage.GetRows();
	
	//�擾����ConvertImage��MomoryCopy����B
	memcpy((void *)&(m_outputImage.pixels[0]),convertedImage.GetData(), len);
	
	//BGR�f����RGB�Ƃ��ĕϊ����邽�߂�Image�̐錾
	IplImage* frame = cvCreateImage(cvSize(convertedImage.GetRows(), convertedImage.GetCols()), 8, 3);
	
	//BGR��RGB�Ƃ��ĕϊ�����B
	memcpy(frame->imageData, (void *)&(m_outputImage.pixels[0]), len);
	cvCvtColor(frame, frame, CV_BGR2RGB);
	
	//�ϊ�����ImageData��Outport�̃������R�s�[����B
	memcpy((void *)&(m_outputImage.pixels[0]), frame->imageData, len);
	cvReleaseImage(&frame);

	m_outputImageOut.write();

	if (count > 100)
	{
		count = 0;
		coil::TimeValue tm;
		tm = coil::gettimeofday();

		double sec(tm - tm_pre);
		
		if (sec > 1.0 && sec < 1000.0)
		{
		    std::cout << 100/sec << " [FPS]" << std::endl;
		}

		tm_pre = tm;
	}
	++count;

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t PGRCamera::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCamera::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCamera::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCamera::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PGRCamera::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void PGRCameraInit(RTC::Manager* manager)
  {
    coil::Properties profile(pgrcamera_spec);
    manager->registerFactory(profile,
                             RTC::Create<PGRCamera>,
                             RTC::Delete<PGRCamera>);
  }
  
};


