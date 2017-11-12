#include "DataAugument.h"
#include <vector>
#include <iostream>
using namespace std;

//vector<int>& testsss(vector<int>&);

int main(int argc,char** argv)
{


	/*******=================augFrom 1000============================*/
	if(argc != 2)
	{
		cout<<"Usage: aug500 originImage"<<endl;
		return -1;
	}
	DataAugument::augFrom1020(argv[1]);



	/*******=================end============================================*/
	cout<<"按任意键结束程序..."<<endl;
	getchar();
	return 0;
}