
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "FlyCapture2.h"
namespace pgr2cv
{

	using namespace FlyCapture2;
	using namespace std;
	

	void PrintFormat7Capabilities(Format7Info fmt7Info)
{
	cout << "Max image pixels: (" << fmt7Info.maxWidth << ", " << fmt7Info.maxHeight << ")" << endl;
	cout << "Image Unit size: (" << fmt7Info.imageHStepSize << ", " << fmt7Info.imageVStepSize << ")" << endl;
	cout << "Offset Unit size: (" << fmt7Info.offsetHStepSize << ", " << fmt7Info.offsetVStepSize << ")" << endl;
	cout << "Pixel format bitfield: 0x" << fmt7Info.pixelFormatBitField << endl;
}
	
	void PrintBuildInfo()
{
	FC2Version fc2Version;
	Utilities::GetLibraryVersion(&fc2Version);

	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
	cout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	cout << timeStamp.str() << endl << endl;
}

void update_FPS(int frame_time)
{
	static int frame_count=-1;
	static int last_second=0;
	static int start_time=frame_time;

	frame_count=frame_count+1;

	if(last_second<(frame_time-start_time)/1) // new second
	{
	last_second=(frame_time-start_time)/1;
	cout<<"FPS: "<< frame_count<< "\n";
	frame_count=0;
	}
}

void PrintCameraInfo(CameraInfo* pCamInfo)
{
	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << pCamInfo->serialNumber << endl;
	cout << "Camera model - " << pCamInfo->modelName << endl;
	cout << "Camera vendor - " << pCamInfo->vendorName << endl;
	cout << "Sensor - " << pCamInfo->sensorInfo << endl;
	cout << "Resolution - " << pCamInfo->sensorResolution << endl;
	cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
	cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl << endl;

}

void PrintError(FlyCapture2::Error error)
{
	error.PrintErrorTrace();
}
void PrintIfError(FlyCapture2::Error error)
{
	if (error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

		
	class VideoCapture//: public cv::VideoCapture
	{
	   bool isopen;
	   FlyCapture2::Error error;
	   Camera cam;
	   PGRGuid guid;
	   BusManager busMgr;
	   Image rawImage, fImage;
	   public:
	   
	   
	   VideoCapture& operator=(const VideoCapture& a){
	   	;
	   }
	   
	   CV_WRAP VideoCapture(){
	   	isopen=false;
	   };
	   CV_WRAP VideoCapture(int device){
	   	this->open(device);

	   };
	   CV_WRAP virtual bool open(int device){
	   	error = busMgr.GetCameraFromIndex(device, &guid);
	   	PrintIfError(error);
		//connect
		error = cam.Connect(&guid);
		PrintIfError(error);
		
	   	// Start capturing images
		error = cam.StartCapture();
		PrintIfError(error);
		isopen=true;
	   };
	   
	   CV_WRAP virtual void release(){
	   cout<<" entered release"<<endl;
	   	// Stop capturing images
		error = cam.StopCapture();
		PrintIfError(error);

		// Disconnect the camera
		error = cam.Disconnect();
		PrintIfError(error);
		
		isopen=false;
		cout<<" exited release"<<endl;
	   };

	   virtual ~VideoCapture(){
	   if (isopen) 	this->release();
	   };
	   
	   CV_WRAP virtual bool retrieve(CV_OUT cv::Mat& image, int channel=0){
	   	error = cam.RetrieveBuffer(&rawImage);
		PrintIfError(error);
		//error = rawImage.Convert(PIXEL_FORMAT_MONO8, &fImage);
		//PrintIfError(error);
		unsigned int rowBytes = (double)rawImage.GetReceivedDataSize() / (double)rawImage.GetRows();
		image = cv::Mat(rawImage.GetRows(), rawImage.GetCols(), CV_8UC1, rawImage.GetData(), rowBytes);

	   };
	   
	   virtual VideoCapture& operator >> (CV_OUT cv::Mat& image){
	   	this->retrieve(image);
	   };
	   CV_WRAP virtual bool isOpened() const{
	   	return isopen;
	   };   
	   
	};


}



