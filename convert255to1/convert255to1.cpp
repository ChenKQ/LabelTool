#include "DataAugument.h"
#include <iostream>

using namespace std;
int main(int argc,char** argv)
{
	if(argc!=5)
	{
		cout<<"Usage: convert255to1.exe originImage sizeOfCrop stepOfCrop originImageSize"<<endl;
		getchar();
		return -1;
	}
	
	return DataAugument::convert255to1(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
	cout<<"ת����ϣ���ִ�����ݷŴ������"<<endl;
	cout<<"���������������!"<<endl;
	getchar();
	return 0;
}