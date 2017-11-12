#include "DataAugument.h"
#include <vector>
#include <iostream>
using namespace std;

//vector<int>& testsss(vector<int>&);

int main(int argc,char** argv)
{

	

	/*******=================recoverFromPatches============================================*/
	if(argc != 5 && argc!=4 && argc!=2)
	{
		cout<<"Usage: recoverFromPatches files sizeOfCrop(520) sizeOfStep(500) originalSize(100)"<<endl;
		return 0;
	}
	if(argc == 2)
		DataAugument::recoverFromPatches(argv[1]);
	else if(argc == 4)
		DataAugument::recoverFromPatches(argv[1],atoi(argv[2]),atoi(argv[3]));
	else if(argc == 5)
		DataAugument::recoverFromPatches(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
	


	/*******=================end============================================*/
	cout<<"按任意键结束程序..."<<endl;
	getchar();
	return 0;
}