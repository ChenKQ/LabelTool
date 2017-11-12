#include "DataAugument.h"
#include <vector>
#include <iostream>
using namespace std;

//vector<int>& testsss(vector<int>&);

int main(int argc,char** argv)
{

	/*******=================compareOriginRecovered============================================*/
	if(argc != 5)
	{
		cout<<"Usage:compareOriginRecovered originImage recoveredImage sizeOfCrop originalImageSize"<<endl;
		return 0;
	}
	DataAugument::compareOrignRecovered(argv[1],argv[2],atoi(argv[3]),atoi(argv[4]));

	/*******=================end============================================*/
	cout<<"按任意键结束程序..."<<endl;
	getchar();
	return 0;
}