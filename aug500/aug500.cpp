#include "DataAugument.h"
#include <vector>
#include <iostream>
using namespace std;

//vector<int>& testsss(vector<int>&);

int main(int argc,char** argv)
{
	/*******=================augFrom 500============================*/
	if(argc != 2)
	{
		cout<<"Usage: aug500 originImage"<<endl;
		return -1;
	}
	DataAugument::augFrom520(argv[1]);



	/*******=================end============================================*/
	cout<<"���������������..."<<endl;
	getchar();
	return 0;
}