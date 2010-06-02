// -*- C++ -*-
/*!
 * @file  PGRCameraCapture.cpp
 * @brief pgrcameracapture
 * @date $Date$
 *
 * $Id$
 */

#include "PGRCameraCapture.h"
#include <iostream>
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* pgrcameracapture_spec[] =
  {
    "implementation_id", "PGRCameraCapture",
    "type_name",         "PGRCameraCapture",
    "description",       "pgrcameracapture",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    "exec_cxt.periodic.rate", "1000.0",
	"conf.default.index", "0",
    ""
  };

int PGRCameraCapture::s_number = 0;
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */

PGRCameraCapture::PGRCameraCapture(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outputImageOut("outputImage", m_outputImage)
	//m_intsnOut("s/n", m_intsn)

    // </rtc-template>
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  registerOutPort("outputImage", m_outputImageOut);
  //registerOutPort("s/n", m_intsnOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

}

/*!
 * @brief destructor
 */
PGRCameraCapture::~PGRCameraCapture()
{
}



RTC::ReturnCode_t PGRCameraCapture::onInitialize()
{	
	bindParameter("index", m_index, "0");
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t CameraCapture::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

RTC::ReturnCode_t PGRCameraCapture::onActivated(RTC::UniqueId ec_id)
{	
	
	Error error;
	FlyCapture2::BusManager busMgr;

	m_camera = new Camera();
	
	//Index����ID���擾����
	error = busMgr.GetCameraFromIndex(m_index, &guid);
	if ( error != PGRERROR_OK ){ 
		//cout << "�J����������܂���B" << endl;
		return RTC::RTC_ERROR;	
	}
	
	//ID����Connect����
	error = m_camera->Connect(&guid);
	if ( error != PGRERROR_OK ){ 
		//cout << "Connection ���s�B" << endl;
		return RTC::RTC_ERROR;	
	}
	
	//�J���������擾����B
	error = m_camera->GetCameraInfo(&m_camInfo);
	if ( error != PGRERROR_OK ){ 
		//cout << "���擾�@���s" << endl;
		return RTC::RTC_ERROR;	
	}
	
	//�f����Capture���n�߂�B
	error = m_camera->StartCapture();
	if ( error != PGRERROR_OK ){ 
		//cout << "Capture ���s�B" << endl;
		return RTC::RTC_ERROR;	
	}
	
	return RTC::RTC_OK;
	
}


RTC::ReturnCode_t PGRCameraCapture::onDeactivated(RTC::UniqueId ec_id)
{
	
	Error error;
	
	//�f���擾���I������B
	if(m_camera != NULL){
		error = m_camera->StopCapture();
		if ( error != PGRERROR_OK ){ 
			delete &m_camera;
			return RTC::RTC_ERROR;	
		}
	}

	//connection����������B
	if(m_camera != NULL){
		error = m_camera->Disconnect();
		if ( error != PGRERROR_OK ){ 
			delete &m_camera;
			return RTC::RTC_ERROR;	
		}
	}

	delete &m_camera;
	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t PGRCameraCapture::onExecute(RTC::UniqueId ec_id)
{
	Error error;

	// �f����Image
    error = m_camera->RetrieveBuffer( &rawImage );
    if (error != PGRERROR_OK)
    {
		//cout << "�f����RawImage�̗̈��Retrieve���ł��܂���B" << endl;
        
		return RTC::RTC_ERROR;
    }

	// RawImage��RGB8Bit��Convert����B
    error = rawImage.Convert( PIXEL_FORMAT_RGB8, &convertedImage );
    if (error != PGRERROR_OK)
    {
        //cout << "RGBFormat�Ƃ��ĕϊ��ł��܂���B" << endl;
        
		return RTC::RTC_ERROR;
    }  
		
	int len = convertedImage.GetCols() * convertedImage.GetRows() * 3;			 //�������͈̔͌Œ�
	
	m_outputImage.pixels.length(len);
	
	//�擾����ConvertImage��MomoryCopy����B
	memcpy((void *)&(m_outputImage.pixels[0]),convertedImage.GetData(), len);
	
	//BGR�f����RGB�Ƃ��ĕϊ����邽�߂�Image�̐錾
	IplImage* frame = cvCreateImage(cvSize(convertedImage.GetRows(), convertedImage.GetCols()), 8, 3);
	
	//BGR��RGB�Ƃ��ĕϊ�����B
	memcpy(frame->imageData, (void *)&(m_outputImage.pixels[0]), len);
	cvCvtColor(frame, frame, CV_BGR2RGB);
	
	//�ϊ�����ImageData��Outport�̃������R�s�[����B
	memcpy((void *)&(m_outputImage.pixels[0]), frame->imageData, len);
	//m_intsn.data = m_camInfo.serialNumber;
	cvReleaseImage(&frame);
	m_outputImageOut.write();
	//m_intsnOut.write();

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraCapture::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void PGRCameraCaptureInit(RTC::Manager* manager)
  {
    coil::Properties profile(pgrcameracapture_spec);
    manager->registerFactory(profile,
                             RTC::Create<PGRCameraCapture>,
                             RTC::Delete<PGRCameraCapture>);
  }
  
};


