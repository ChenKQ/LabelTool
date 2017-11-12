#ifndef _DATA_AUGUMENT_

#define _DATA_AUGUMENT_



#include "opencv2\core\core.hpp"
#include <string>

#define RECOVER_LABEL_PATCH

static class DataAugument
{
public:
	///ClipImage
	//static std::string parseClipName(char* originImage,const int columnIndex,const int rowIndex);
	static int clipImage(const char* file2Crop,const int sizeOfCrop=520,const int stepOfCrop=500);
	static int recoverFromPatches(const char* file2Crop,const int sizeOfCrop=520,const int sizeOfStep=500,const int originImageSize=10000);
	static int compareOrignRecovered(const char* origin,const char* recovered,const int sizeOfStep=500,const int originImageSize=10000);

	//Crop Image To Augument

	static std::vector<std::vector<cv::Mat>>& augFrom520(const cv::Mat& patch2Crop,const cv::Mat& labeled,std::vector<std::vector<cv::Mat>>& results,const int cropSize=250,const int cropStep=250,bool returnResized=true);
	static int augFrom520(const char* originImage,const int sizeOfCrop=520,const int stepOfCrop=500,const int originSize=10000);
	static std::vector<std::vector<cv::Mat>>& augFrom500(const cv::Mat& patch2Crop,const cv::Mat& labeled,std::vector<std::vector<cv::Mat>>& results,const int cropSize=250,const int cropStep=250,bool returnResized=true);
	static std::vector<std::vector<cv::Mat>>& augFrom1020(const cv::Mat& patch2Crop,const cv::Mat& labeled,std::vector<std::vector<cv::Mat>>& results,const int cropSize=250,const int cropStep=250);
	static int augFrom1020(const char* originImage,const int sizeOfCrop=1000,const int stepOfCrop=1000,const int originSize=10000);
	static void convert255to1(const cv::Mat& img);
	static int convert255to1(const char* originImage,const int sizeOfCrop,const int stepOfCrop,const int originSize);
	static int convert1to255(const char* originImage,const int sizeOfCrop,const int stepOfCrop,const int originSize);
};

class ImageName
{
private:
	std::string prefix;
	std::string subfix;
	std::string path;
	std::string fileName;
	std::string fileNamePrefix;
	const std::string labelString;
	const std::string augString;
	const std::string augSavePath;
public:

	ImageName(const char* originImage);
	std::string getPrefix();
	std::string getSubfix();
	std::string getPath();
	std::string getFileName();
	std::string getFileNamePrefix();
	std::string parseClipName(const int columnIndex,const int rowIndex);	//clip names
	std::string parseLabelClipName(const int columnIndex,const int rowIndex);  //labeled clip name
	std::string recoveredName();   //recover from patches
	std::string recoveredLabelName();  //recover from labeled patches
	std::string augName(const char* patchFile,const int index,std::string format=".jpg");   //augumented patches name
};

#endif