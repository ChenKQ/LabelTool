#include "DataAugument.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;


//string DataAugument::parseClipName(char* originImage,const int columnIndex,const int rowIndex)
//{
//	string originName(originImage);
//	int dotPos = originName.find(".",0);
//	string prefixName = originName.substr(0,dotPos);
//	string subfix = originName.substr(dotPos,originName.length()-1);
//
//	string saveName (prefixName);	
//	saveName += (columnIndex<10)?(string("0") + to_string(columnIndex)):""+to_string(columnIndex);
//	saveName += (rowIndex<10)?(string("0")+to_string(rowIndex)):""+to_string(rowIndex);
//	saveName += subfix;
//
//	return saveName;
//
//}
#ifdef RECOVER_LABEL_PATCH
	
#endif




int DataAugument::clipImage(const char* file2Crop,const int sizeOfCrop,const int stepOfCrop)
{
	Mat img_src;
	cout<<"正在打开图像："<<file2Crop<<endl;
	img_src = imread(file2Crop,-1);
	cout<<"打开图片成功！"<<endl;
	//imshow("original image",img_src);
	//string fileName (file2Crop);
	//int dotPos = fileName.find(".",0);
	//string prefixName = fileName.substr(0,dotPos);
	//string subfix = fileName.substr(dotPos,fileName.length()-1);
	ImageName _imgName (file2Crop);
	int horizon_count = 1;
	int vertical_count = 1;
	for(int i = 0;i<=(img_src.rows - sizeOfCrop);i += stepOfCrop)
	{
		for(int j=0;j<=(img_src.cols - sizeOfCrop); j += stepOfCrop)
		{
			//string saveName (prefixName);
			Mat crop(img_src,Rect(i,j,sizeOfCrop,sizeOfCrop));
			//saveName += (vertical_count<10)?(string("0") + to_string(vertical_count)):""+to_string(vertical_count);
			//saveName += (horizon_count<10)?(string("0")+to_string(horizon_count)):""+to_string(horizon_count);
			//saveName += subfix;
			string saveName = _imgName.parseClipName(vertical_count,horizon_count);
			cout<<"正在保存图片"<<Point(i,j)<<endl;
			imwrite(saveName,crop);
			++horizon_count;
		}
		++vertical_count;
		horizon_count = 1;
	}

	cout<<"回车结束程序！"<<endl;
	getchar();
	return 0;
}

int DataAugument::recoverFromPatches(const char* file2Crop,const int sizeOfCrop,const int sizeOfStep,const int originImageSize)
{
	Mat recoverImage;
	if(sizeOfCrop==1000)
		recoverImage = Mat::zeros(originImageSize,originImageSize,CV_8UC3);
	else
		recoverImage = Mat::zeros(originImageSize-sizeOfStep,originImageSize-sizeOfStep,CV_8UC3);
#ifdef RECOVER_LABEL_PATCH
	Mat recoverLabeled;
	if(sizeOfCrop==1000)
		recoverLabeled = Mat::zeros(originImageSize,originImageSize,CV_8UC1);
	else
		recoverLabeled = Mat::zeros(originImageSize-sizeOfStep,originImageSize-sizeOfStep,CV_8UC1);
#endif
	//string fileName (file2Crop);
	//int dotPos = fileName.find(".",0);
	//string prefixName = fileName.substr(0,dotPos);
	//string subfix = fileName.substr(dotPos,fileName.length()-1);
	ImageName _imgName(file2Crop);

	int horizon_count = 1;
	int vertical_count = 1;
	for(int i = 0;i<=(originImageSize - sizeOfCrop);i += sizeOfStep)
	{
		for(int j=0;j<=(originImageSize - sizeOfCrop); j += sizeOfStep)
		{
			//to make sure the name of the image being attached
			//string readName (prefixName);
			//readName += (vertical_count<10)?(string("0") + to_string(vertical_count)):""+to_string(vertical_count);
			//readName += (horizon_count<10)?(string("0")+to_string(horizon_count)):""+to_string(horizon_count);
			//readName += subfix;

			string readName = _imgName.parseClipName(vertical_count,horizon_count);
			cout<<"正在写入区域"<<Point(i,j)<<endl;
			Mat readFile = imread(readName,-1);
			Mat crop = Mat(recoverImage,Rect(i,j,sizeOfStep,sizeOfStep));
			if(sizeOfCrop==1000)
				crop += Mat(readFile,Rect(0,0,sizeOfStep,sizeOfStep));
			else
				crop += Mat(readFile,Rect(10,10,sizeOfStep,sizeOfStep));
			cout<<"区域"<<Point(i,j)<<"写入完毕"<<endl;	

#ifdef RECOVER_LABEL_PATCH
			string readLabelName = _imgName.parseLabelClipName(vertical_count,horizon_count);
			cout<<"正在写入已标注的图像区域"<<Point(i,j)<<endl;
			Mat readLabelFile = imread(readLabelName,-1);
			Mat cropLabel = Mat(recoverLabeled,Rect(i,j,sizeOfStep,sizeOfStep));
			if(sizeOfCrop==1000)
				cropLabel += Mat(readLabelFile,Rect(0,0,sizeOfStep,sizeOfStep)); 
			else
				cropLabel += Mat(readLabelFile,Rect(10,10,sizeOfStep,sizeOfStep));
			cout<<"区域"<<Point(i,j)<<"写入完毕"<<endl;		
#endif
			++horizon_count;
		}
		++vertical_count;
		horizon_count = 1;
	}

	cout<<"回车保存图片！"<<endl;
	string saveName =_imgName.recoveredName();
	imwrite(saveName,recoverImage);
#ifdef RECOVER_LABEL_PATCH
	string saveLabelName =_imgName.recoveredLabelName();
	imwrite(saveLabelName,recoverLabeled);
#endif
	cout<<"回车结束程序！"<<endl;
	getchar();
	return 0;
}

int DataAugument::compareOrignRecovered(const char* origin,const char* recovered,const int sizeOfStep,const int originImageSize)
{
	Mat origin_img = imread(origin,-1);
	Mat recover_img = imread(recovered,-1);
	Mat result = Mat(origin_img,Rect(10,10,originImageSize-sizeOfStep,originImageSize-sizeOfStep)) - recover_img;
	imwrite("compare.tiff",result);
	return 0;
}

vector<vector<Mat>>& DataAugument::augFrom520(const cv::Mat& patch2Crop,const cv::Mat& labeled,std::vector<std::vector<cv::Mat>>& results,const int cropSize,const int cropStep,bool returnResized)
{
	int count =1;
	const int edge=9;
	//vector<vector<Mat>> results;  //resykts[0] stores original crops, results[1] stores mask crops
	vector<Mat> origin;
#ifdef RECOVER_LABEL_PATCH
	vector<Mat> mask;
#endif	
	for(int i=edge;i<=patch2Crop.cols-cropSize-edge;i+=cropStep)
	{
		for(int j=edge;j<patch2Crop.rows-cropSize-edge;j+=cropStep)
		{
			origin.push_back(Mat(patch2Crop,Rect(i,j,cropSize,cropSize)));
#ifdef RECOVER_LABEL_PATCH
			//Mat cp(labeled,Rect(i,j,cropSize,cropSize));
			//cout<<cp<<endl;
			mask.push_back(Mat(labeled,Rect(i,j,cropSize,cropSize)));
#endif			
		}
	}

	if(returnResized)
	{
		//Resize part
		Mat origin_clone = patch2Crop.clone();
		resize(origin_clone,origin_clone,Size(cropSize,cropSize),0,0,0);
		origin.push_back(origin_clone);

#ifdef RECOVER_LABEL_PATCH
		Mat mask_clone = labeled.clone();
		resize(mask_clone,mask_clone,Size(cropSize,cropSize),0,0,0);
		mask.push_back(mask_clone);
#endif
	}

	results.push_back(origin);
	results.push_back(mask);
	return results;
}

int DataAugument::augFrom520(const char* originImage,const int sizeOfCrop,const int stepOfCrop,const int originSize)
{
	ImageName _imgName(originImage);
	int horizon_count = 1;
	int vertical_count = 1;
	for(int i = 0;i<=(originSize - sizeOfCrop);i += stepOfCrop)
	{
		for(int j=0;j<=(originSize - sizeOfCrop); j += stepOfCrop)
		{
			string readPatch = _imgName.parseClipName(vertical_count,horizon_count);
			string labedPatch = _imgName.parseLabelClipName(vertical_count,horizon_count);
			Mat originImage = imread(readPatch.c_str(),-1);
			Mat maskImage = imread(labedPatch.c_str(),-1);
			if(originImage.empty() || maskImage.empty())
			{
				cout<<"第"<<Point(vertical_count,horizon_count)<<"位置出错"<<endl;
				return -1;
			}
				
			vector<vector<Mat>> augs ;
			augFrom520(originImage,maskImage,augs);   //pay attention
			int count =1;
			for(int i=0;i<augs[0].size();++i)
			{
				string saveOriginAugName = _imgName.augName(readPatch.c_str(),count);
				string saveMaskAugName = _imgName.augName(labedPatch.c_str(),count,".png");
				cout<<"正在写第"<<Point(vertical_count,horizon_count)<<"幅图的第"<<count<<"个path"<<endl;
				imwrite(saveOriginAugName,augs[0][i]);
				imwrite(saveMaskAugName,augs[1][i]);
				++count;
			}
			++horizon_count;
		}
		++vertical_count;
		horizon_count = 1;
	}
	return 0;
}

vector<vector<Mat>>& DataAugument::augFrom500(const cv::Mat& patch2Crop,const cv::Mat& labeled,std::vector<std::vector<cv::Mat>>& results,const int cropSize,const int cropStep,bool returnResized)
{
	int count =1;
	//vector<vector<Mat>> results;  //resykts[0] stores original crops, results[1] stores mask crops
	vector<Mat> origin;
#ifdef RECOVER_LABEL_PATCH
	vector<Mat> mask;
#endif	
	for(int i=0;i<=patch2Crop.cols-cropSize;i+=cropStep)
	{
		for(int j=0;j<=patch2Crop.rows-cropSize;j+=cropStep)
		{
			origin.push_back(Mat(patch2Crop,Rect(i,j,cropSize,cropSize)));
#ifdef RECOVER_LABEL_PATCH
			mask.push_back(Mat(labeled,Rect(i,j,cropSize,cropSize)));
#endif			
		}
	}

	if(returnResized)
	{
		Mat origin_clone = patch2Crop.clone();
		resize(origin_clone,origin_clone,Size(cropSize,cropSize),0,0,0);
		origin.push_back(origin_clone);
#ifdef RECOVER_LABEL_PATCH
		Mat mask_clone = labeled.clone();
		resize(mask_clone,mask_clone,Size(cropSize,cropSize),0,0,0);
		mask.push_back(mask_clone);
#endif
	}

	results.push_back(origin);
	results.push_back(mask);
	return results;
}

vector<vector<Mat>>& DataAugument::augFrom1020(const cv::Mat& patch2Crop,const cv::Mat& labeled,std::vector<std::vector<cv::Mat>>& results,const int cropSize,const int cropStep)
{
	//crop directly from 1020x1020 to 250x250x4x4(16)
	//vector<vector<Mat>> results;
	Mat patch2CropUpSample = patch2Crop.clone();
	//resize(patch2CropUpSample,patch2CropUpSample,Size(1020,1020),0,0,0);
	Mat labeledUpSample = labeled.clone();
	//resize(labeledUpSample,labeledUpSample,Size(1020,1020),0,0,0);
	results = augFrom500(patch2CropUpSample,labeledUpSample,results,250,250,false);   //pay attention
	// crop 500x500->250x250(9)
	for(int i=0;i<3;++i)
	{
		for(int j=0;j<3;++j)
		{
			Mat tmp_img=Mat(patch2CropUpSample,Rect(i*250,j*250,500,500)).clone();
			Mat tmp_lab=Mat(labeledUpSample,Rect(i*250,j*250,500,500)).clone();
			resize(tmp_img,tmp_img,Size(250,250),0,0,0);
			resize(tmp_lab,tmp_lab,Size(250,250),0,0,0);
			results[0].push_back(tmp_img);
			results[1].push_back(tmp_lab);
		}
	}
	//crop 750x750->250x250(4)
	for(int i=0;i<2;++i)
	{
		for(int j=0;j<2;++j)
		{
			Mat tmp_img=Mat(patch2CropUpSample,Rect(i*250,j*250,750,750)).clone();
			Mat tmp_lab=Mat(labeledUpSample,Rect(i*250,j*250,750,750)).clone();
			resize(tmp_img,tmp_img,Size(250,250),0,0,0);
			resize(tmp_lab,tmp_lab,Size(250,250),0,0,0);
			results[0].push_back(tmp_img);
			results[1].push_back(tmp_lab);
		}
	}
	//directly resize to 250x250
	Mat tmp_img=patch2CropUpSample.clone();
	Mat tmp_lab=labeledUpSample.clone();
	resize(tmp_img,tmp_img,Size(250,250),0,0,0);
	resize(tmp_lab,tmp_lab,Size(250,250),0,0,0);
	results[0].push_back(tmp_img);
	results[1].push_back(tmp_lab);
	//reshape from 1000x1000 to 500x500
	//Mat origin_clone =Mat(patch2CropUpSample,Rect(10,10,1000,1000)).clone();		//origin_clone = 1000x1000
	//Mat mask_clone = Mat(labeledUpSample,Rect(10,10,1000,1000)).clone();			//mask_clone = 1000x1000
	//resize(origin_clone,origin_clone,Size(500,500),0,0,0);
	//resize(mask_clone,mask_clone,Size(500,500),0,0,0);
	//1.crop resized 500x500 to 250x250x4 2. add to results(4)
	//vector<vector<Mat>> tempResult;
	//augFrom500(origin_clone,mask_clone,tempResult,250,250,false);   //pay attention
	//for(int i=0;i<tempResult[0].size();++i)
	//{
	//	results[0].push_back(tempResult[0][i]);
	//	results[1].push_back(tempResult[1][i]);
	//}
	//tempResult.clear();
	////1.resize from resized 500x500 to 250x250 2.add to results(1)
	//resize(origin_clone,origin_clone,Size(250,250),0,0,0);
	//results[0].push_back(origin_clone);
	//resize(mask_clone,mask_clone,Size(250,250),0,0,0);
	//results[1].push_back(mask_clone);
	//1. Crop from 1020x1020 to 500x500 2. reshape
	//augFrom520(patch2CropUpSample,labeledUpSample,tempResult,500,500,false);		//pay attention
	//for(int i=0;i<tempResult[0].size();++i)
	//{
	//	resize(tempResult[0][i],tempResult[0][i],Size(250,250),0,0,0);
	//	resize(tempResult[1][i],tempResult[1][i],Size(250,250),0,0,0);
	//	results[0].push_back(tempResult[0][i]);
	//	results[1].push_back(tempResult[1][i]);
	//}
	return results;
}

int DataAugument::augFrom1020(const char* originImage,const int sizeOfCrop,const int stepOfCrop,const int originSize)
{
	ImageName _imgName(originImage);
	int horizon_count = 1;
	int vertical_count = 1;
	for(int i = 0;i<=(originSize - sizeOfCrop);i += stepOfCrop)
	{
		for(int j=0;j<=(originSize - sizeOfCrop); j += stepOfCrop)
		{
			string readPatch = _imgName.parseClipName(vertical_count,horizon_count);
			string labedPatch = _imgName.parseLabelClipName(vertical_count,horizon_count);
			Mat originImage = imread(readPatch.c_str(),-1);
			Mat maskImage = imread(labedPatch.c_str(),-1);
			if(originImage.empty() || maskImage.empty())
			{
				cout<<"第"<<Point(vertical_count,horizon_count)<<"位置出错"<<endl;
				return -1;
			}
			vector<vector<Mat>> augs ;
			augFrom1020(originImage,maskImage,augs,250,250);   //pay attention
			int count =1;
			for(int i=0;i<augs[0].size();++i)
			{
				string saveOriginAugName = _imgName.augName(readPatch.c_str(),count);
				string saveMaskAugName = _imgName.augName(labedPatch.c_str(),count,".png");
				cout<<"正在写第"<<Point(vertical_count,horizon_count)<<"幅图的第"<<count<<"个path"<<endl;
				imwrite(saveOriginAugName,augs[0][i]);
				imwrite(saveMaskAugName,augs[1][i]);
				++count;
			}
			++horizon_count;
		}
		++vertical_count;
		horizon_count = 1;
	}
	return 0;
}


void DataAugument::convert255to1(const Mat& img)
{
	img /= 255;
}

int DataAugument::convert255to1(const char* originImage,const int sizeOfCrop,const int stepOfCrop,const int originSize)
{
	ImageName _imgName(originImage);
	int horizon_count = 1;
	int vertical_count = 1;
	for(int i = 0;i<=(originSize - sizeOfCrop);i += stepOfCrop)
	{
		for(int j=0;j<=(originSize - sizeOfCrop); j += stepOfCrop)
		{
			//string readPatch = _imgName.parseClipName(vertical_count,horizon_count);
			string labedPatch = _imgName.parseLabelClipName(vertical_count,horizon_count);
			//Mat originImage = imread(readPatch.c_str());
			Mat maskImage = imread(labedPatch.c_str(),-1);
			if(maskImage.empty() || maskImage.empty())
			{
				cout<<"第"<<Point(vertical_count,horizon_count)<<"位置出错"<<endl;
				return -1;
			}
			DataAugument::convert255to1(maskImage);	
			//string saveOriginAugName = _imgName.augName(readPatch.c_str(),count);
			//string saveMaskAugName = _imgName.augName(labedPatch.c_str(),count);
			cout<<"正在写第"<<Point(vertical_count,horizon_count)<<"幅图"<<endl;
			imwrite(labedPatch,maskImage);
			++horizon_count;
		}
		++vertical_count;
		horizon_count = 1;
	}
	return 0;
}

int DataAugument::convert1to255(const char* originImage,const int sizeOfCrop,const int stepOfCrop,const int originSize)
{
	ImageName _imgName(originImage);
	int horizon_count = 1;
	int vertical_count = 1;
	for(int i = 0;i<=(originSize - sizeOfCrop);i += stepOfCrop)
	{
		for(int j=0;j<=(originSize - sizeOfCrop); j += stepOfCrop)
		{
			//string readPatch = _imgName.parseClipName(vertical_count,horizon_count);
			string labedPatch = _imgName.parseLabelClipName(vertical_count,horizon_count);
			//Mat originImage = imread(readPatch.c_str());
			Mat maskImage = imread(labedPatch.c_str(),-1);
			if(maskImage.empty() || maskImage.empty())
			{
				cout<<"第"<<Point(vertical_count,horizon_count)<<"位置出错"<<endl;
				return -1;
			}
			maskImage *=255;
			//string saveOriginAugName = _imgName.augName(readPatch.c_str(),count);
			//string saveMaskAugName = _imgName.augName(labedPatch.c_str(),count);
			cout<<"正在写第"<<Point(vertical_count,horizon_count)<<"幅图"<<endl;
			imwrite(labedPatch,maskImage);
			++horizon_count;
		}
		++vertical_count;
		horizon_count = 1;
	}
	return 0;
}

///***************************The ImageName Class Implementation********************************************///

ImageName::ImageName(const char* originImage):labelString("_label"),augString("_aug_"),augSavePath("augument\\")
{
	string originName(originImage);
	int dotPos = originName.find(".",0);
	int lastPos = originName.rfind("\\");
	this->path = originName.substr(0,lastPos+1);
	this->fileNamePrefix = originName.substr(lastPos+1,dotPos-lastPos-1);
	this->fileName = originName.substr(lastPos+1,originName.size()-lastPos);

	this->prefix = originName.substr(0,dotPos);
	this->subfix = originName.substr(dotPos,originName.size()-1);

}

std::string ImageName::getPrefix()
{
	return this->prefix;
}

std::string ImageName::getSubfix()
{
	return this->subfix;
}

std::string ImageName::getPath()
{
	return this->path;
}

std::string ImageName::getFileName()
{
	return this->fileName;
}

std::string ImageName::getFileNamePrefix()
{
	return this->fileNamePrefix;
}

std::string ImageName::parseClipName(const int columnIndex,const int rowIndex)
{
	string saveName (this->prefix);	
	saveName += (columnIndex<10)?(string("0") + to_string((long double)columnIndex)):""+to_string((long double)columnIndex);
	saveName += (rowIndex<10)?(string("0")+to_string((long double)rowIndex)):""+to_string((long double)rowIndex);
	saveName += this->subfix;
	return saveName;
}

std::string ImageName::parseLabelClipName(const int columnIndex,const int rowIndex)
{
	string saveName (this->prefix);	
	saveName += (columnIndex<10)?(string("0") + to_string((long double)columnIndex)):""+to_string((long double)columnIndex);
	saveName += (rowIndex<10)?(string("0")+to_string((long double)rowIndex)):""+to_string((long double)rowIndex);
	saveName += this->labelString;
	saveName += this->subfix;
	return saveName;
}

std::string ImageName::recoveredName()
{
	string saveName = this->prefix + "_recovered" + this->subfix;
	return saveName;
}

std::string ImageName::recoveredLabelName()
{
	string saveName = this->prefix +  this->labelString  + "_recovered" + this->subfix;
	return saveName;
}

std::string ImageName::augName(const char* patchName,int index,std::string format)
{
	ImageName _patchName(patchName);
	string _augName (_patchName.getPath());
	_augName += this->augSavePath + _patchName.getFileNamePrefix() + this->augString + string((index<10)?(string("0") + to_string((long double)index)):""+to_string((long double)index)) + format;
	return _augName;
}