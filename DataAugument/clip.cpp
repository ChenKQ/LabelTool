#include "DataAugument.h"
#include <vector>
#include <iostream>
using namespace std;

//vector<int>& testsss(vector<int>&);

int main(int argc,char** argv)
{
	/*******=================augFrom 500============================*/
	//if(argc != 2)
	//{
	//	cout<<"Usage: aug500 originImage"<<endl;
	//	return -1;
	//}
	//DataAugument::augFrom520(argv[1]);

	/*******=================augFrom 1000============================*/
	//if(argc != 2)
	//{
	//	cout<<"Usage: aug500 originImage"<<endl;
	//	return -1;
	//}
	//DataAugument::augFrom1020(argv[1]);

	/*******=================clip============================================*/
	if(argc != 3 && argc != 4)
	{
		cout<<"Usage: clip.exe image sizeOfCrop (sizeOfStep = sizeOfCrop)"<<endl;
		return -1;
	}

	if(argc == 3)
		DataAugument::clipImage(argv[1],atoi(argv[2]),atoi(argv[2]));
	else 
		DataAugument::clipImage(argv[1],atoi(argv[2]),atoi(argv[3]));
	

	/*******=================recoverFromPatches============================================*/
	//if(argc != 5 && argc!=4 && argc!=2)
	//{
	//	cout<<"Usage: recoverFromPatches files sizeOfCrop(520) sizeOfStep(500) originalSize(100)"<<endl;
	//	return 0;
	//}
	//if(argc == 2)
	//	DataAugument::recoverFromPatches(argv[1]);
	//else if(argc == 4)
	//	DataAugument::recoverFromPatches(argv[1],atoi(argv[2]),atoi(argv[3]));
	//else if(argc == 5)
	//	DataAugument::recoverFromPatches(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
	

	/*******=================compareOriginRecovered============================================*/
	//if(argc != 5)
	//{
	//	cout<<"Usage:compareOriginRecovered originImage recoveredImage sizeOfCrop originalImageSize"<<endl;
	//	return 0;
	//}
	//DataAugument::compareOrignRecovered(argv[1],argv[2],atoi(argv[3]),atoi(argv[4]));

	/*******=================end============================================*/
	cout<<"按任意键结束程序..."<<endl;
	getchar();
	return 0;
}