#include <iostream>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <vector>
#include <math.h>
using namespace std;
using namespace cv;


Mat src;
Mat src_gray;
Mat crop_clone;
Mat img_segmented;
Mat img_croped;
Scalar postive (255,255,255);
Scalar negative(0,0,0);
bool isCroped = false;
int thresh=100;
int max_thresh=255;
int choice=0;
int max_choices=6;
int positiveInContour = 0;
int maxStatus = 2;
vector<Point> pts ;	 
vector<vector<Point>> contours;
vector<Vec4i> hieracky;
RNG rng(12345);
void thresh_callback(int ,void*);
void choice_callback(int,void*);
void status_callback(int,void*);
void onMouse(int event, int x, int y, int flags, void* userdata);
void onCropMouse(int event,int x,int y,int flags,void* userdata);
//void bt_DrawLine(int ,void*);

int main(int argc,char** argv)
{
	if(argc !=2)
	{
		cout<<"Uasge: Main ImageToLoad"<<endl;
		return -1;
	}

	src = imread(argv[1],-1);
	//src_clone= src.clone();
	blur(src,img_segmented,Size(3,3));
	//img_segmented = src.clone();
	cvtColor(src,src_gray,CV_RGB2BGRA);


	namedWindow("ControlPanel",CV_WINDOW_FREERATIO);
	namedWindow("Segment",CV_WINDOW_AUTOSIZE);
	createTrackbar("Threshold","ControlPanel",&thresh,max_thresh,thresh_callback);
	createTrackbar("operations","ControlPanel",&choice,max_choices,choice_callback);
	createTrackbar("Postive","ControlPanel",&positiveInContour,maxStatus,status_callback);
	setMouseCallback("Segment",onMouse);
	imshow("Segment",img_segmented);

	
	waitKey(0);
	Mat* spilted = new Mat[3];
	split(img_segmented,spilted);
	for(int i=0;i<3;++i)
	{
		threshold(spilted[i],spilted[i],254,255,CV_THRESH_BINARY);
	}
	Mat result = spilted[0] & spilted[1];
	result &=spilted[2];
	string name(argv[1]);
	int pos = 0;
	pos = name.find(".",0);
	string saveName = name.substr(0,pos) + "_label" + name.substr(pos,name.length()-1);
	//string savePath = "D:\\";
	//string saveFile = savePath + saveName;
	imwrite(saveName,result);

	//imshow("r",spilted[0]);
	//imshow("g",spilted[1]);
	//imshow("b",spilted[2]);


	waitKey(0);

	return 0;
}

	//Load Source image and convert it to gray
	//src = imread("D:\\a.jpg");
	//src_clone = src.clone();
	////Convert image to gray and blur it
	//cvtColor(src,src_gray,CV_BGR2GRAY);
	////blur(src_gray,src_gray,Size(3,3));

	////char* source_window="Source";
	////namedWindow(source_window,CV_WINDOW_AUTOSIZE);
	////imshow(source_window,src);
	//namedWindow("contours",CV_WINDOW_AUTOSIZE);
	//createTrackbar("Canny Thresh","contours",&thresh,max_thresh,thresh_callback);
	//setMouseCallback("contours",onMouse);
	//Mat drawing(src.size(),src.type());
	//thresh_callback(0,0);
	//
	////namedWindow("contours",CV_WINDOW_AUTOSIZE);
	////imshow("contours",src);
	////createTrackbar("Thresh","contours",&thresh,max_thresh,thresh_callback);

	//waitKey(0);
	//return 0;


	//Mat img = imread("D:\\a.jpg");
	//Mat gray(img.size(),CV_8UC1);
	//cvtColor(img,gray,CV_BGR2GRAY);

	//Mat binary(img.size(),CV_8UC1);
	//threshold(gray,binary,200,255,CV_THRESH_BINARY);

	//
	//vector<vector<Point>> contours;
	//vector<Vec4i> hieracky ;
	//findContours(binary,contours,hieracky,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);

	//drawContours(img,contours,-1,(0,0,255),3);

	//namedWindow("contour");
	//imshow("contour",img);

	//waitKey();

	//return 0;

void thresh_callback(int,void*)
{
	if(choice == 3 && isCroped == true)
	{
		crop_clone = img_croped.clone();
		Mat crop_gray;
		Mat crop_binary;
		cvtColor(crop_clone,crop_gray,CV_RGB2GRAY);
		threshold(crop_gray,crop_binary,thresh,255,CV_THRESH_BINARY);
		findContours(crop_binary,contours,hieracky,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
		drawContours(crop_clone,contours,-1,(0,0,255),3);
		imshow("Croped",crop_clone);

	}
	else if(choice == 4 && isCroped == true)
	{
		crop_clone = img_croped.clone();
		//Mat crop_gray;
		Mat crop_binary;
		Mat crop_hsv;
		Mat* crop_split = new Mat[3];
		cvtColor(crop_clone,crop_hsv,CV_RGB2HSV);
		split(crop_hsv,crop_split);
		Mat crop_gray(crop_split[0]);
		threshold(crop_gray,crop_binary,thresh,255,CV_THRESH_BINARY);
		findContours(crop_binary,contours,hieracky,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
		drawContours(crop_clone,contours,-1,(0,0,255),3);
		imshow("Croped",crop_clone);

	}




	//Mat thresh_output;

	//

	//src_clone=src.clone();
	////Detect edges using canny
	//threshold(src_gray,thresh_output,thresh,max_thresh,CV_THRESH_BINARY);
	////Canny(src_gray,canny_output,thresh,thresh*2,3);
	/////Find Contours
	//findContours(thresh_output,contours,hieracky,CV_RETR_TREE,CV_CHAIN_APPROX_NONE,Point(0,0));
	////Mat drawing = src.clone();
	////drawContours(drawing,contours,0,Scalar(0,255,0),2,8,hieracky,0,Point());
	////画出所有的轮廓
	//for(int i=0;i<contours.size();++i)
	//{
	//	Scalar color(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
	//	drawContours(src_clone,contours,i,color,2,8,hieracky,1,Point());
	//}


	////填充单个区域
	//vector<vector<Point>> singleContour;
	//if(!contours.empty())
	//{
	//	cout<<"Threshold:"<<thresh<<";the number of contours:"<<contours.size()<<endl;
	//	singleContour.push_back(contours[0]);
	//	fillPoly(src_clone,singleContour,Scalar(255,255,255),8);
	//	cout<<"The location of target is "<<singleContour[0]<<endl;
	//	imshow("contours",src_clone);
	//}
	//else
	//{
	//	cout<<"No contours!"<<endl;
	//}
	//

	
}

void onMouse(int event, int x, int y, int flags, void* userdata)
{

	imshow("Segment",img_segmented);
	///点鼠标右键清空vector内容
	if(event == CV_EVENT_RBUTTONDOWN)
	{
		pts.clear();
		cout<<"Points in vector are cleared!"<<endl;
	}
	//Draw Line
	if(choice == 1)
	{
		if(event == CV_EVENT_LBUTTONDOWN)
		{
			if(pts.size() >= 2)
			{
				pts.clear();
				cout<<"Points in vector are cleared!"<<endl;
				pts.push_back(Point(x,y));
				cout<<"Added New Point to vector"<<pts[0]<<endl;
			}
			else
			{
				pts.push_back(Point(x,y));
				cout<<"Added New Point to vector"<<pts[pts.size()-1]<<endl;
				if(pts.size() == 2)
				{
					line(img_segmented,pts[0],pts[1],postive,3);
					imshow("Segment",img_segmented);
				}

			}
		}
	}
	///Draw Polygon
	else if(choice == 2)		
	{
		if(event == CV_EVENT_LBUTTONDOWN)
		{
			pts.push_back(Point(x,y));
			//polylines(img_segmented,pts,);
			cout<<"Added New Point to vector(Polygon):"<<pts[pts.size()-1]<<endl;
		}
		if(event == CV_EVENT_LBUTTONDBLCLK)
		{
			if(pts.size()<=3)
			{
				cout<<"Too little Points!The points in vector will be cleared!"<<endl;
				pts.clear();
			}
			else
			{
				vector<vector<Point>> temp_contour ;
				temp_contour.push_back(pts);
				fillPoly(img_segmented,temp_contour,(positiveInContour == 0)?postive:negative,8);
				cout<<"Finish fill Polygon"<<endl;
				imshow("Segment",img_segmented);
				pts.clear();
			}
		}
	}
	//Crop Image
	else if(choice == 3 || choice==4)
	{
		if(event == CV_EVENT_LBUTTONDOWN)
		{
			if(pts.size() >= 2)
			{
				pts.clear();
				cout<<"Points in vector are cleared!"<<endl;
				pts.push_back(Point(x,y));
				cout<<"Added New Point to vector"<<pts[0]<<endl;
			}
			else
			{
				pts.push_back(Point(x,y));
				cout<<"Added New Point to vector"<<pts[pts.size()-1]<<endl;
				if(pts.size() == 2)
				{
					//if(pts[0].x<pts[1].x && pts[0].y<pts[1].y)
					//	img_croped = Mat(img_segmented,Rect(pts[0].x,pts[0].y,abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
					//else if (pts[0].x > pts[1].x && pts[0].y<pts[1].y)
					//	img_croped = Mat(img_segmented,Rect(pts[1].x,pts[0].y,abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
					//else if (pts[0].x<pts[1].x && pts[0].y>pts[1].y)
					//	img_croped = Mat(img_segmented,Rect(pts[0].x,pts[1].y,abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
					if(pts[0].x == pts[1].x || pts[0].y==pts[1].y)
					{
						cout<<pts[0]<<" and "<<pts[1]<<"can not consist the rectangle for the width or the heith is zero"<<endl;
						return;
					}
					img_croped = Mat(img_segmented,Rect(min(pts[0].x,pts[1].x),min(pts[0].y,pts[1].y),abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
					namedWindow("Croped",CV_WINDOW_AUTOSIZE);
					imshow("Croped",img_croped);
					isCroped = true;
					cout<<"Finish crop from the original image!"<<endl;
					setMouseCallback("Croped",onCropMouse);
					waitKey(0);
					isCroped = false;
					destroyWindow("Croped");
				}

			}
		}
	}
	//HSV
	//else if(choice == 4)
	//{
	//			if(event == CV_EVENT_LBUTTONDOWN)
	//	{
	//		if(pts.size() >= 2)
	//		{
	//			pts.clear();
	//			cout<<"Points in vector are cleared!"<<endl;
	//			pts.push_back(Point(x,y));
	//			cout<<"Added New Point to vector"<<pts[0]<<endl;
	//		}
	//		else
	//		{
	//			pts.push_back(Point(x,y));
	//			cout<<"Added New Point to vector"<<pts[pts.size()-1]<<endl;
	//			if(pts.size() == 2)
	//			{
	//				//if(pts[0].x<pts[1].x && pts[0].y<pts[1].y)
	//				//	img_croped = Mat(img_segmented,Rect(pts[0].x,pts[0].y,abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
	//				//else if (pts[0].x > pts[1].x && pts[0].y<pts[1].y)
	//				//	img_croped = Mat(img_segmented,Rect(pts[1].x,pts[0].y,abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
	//				//else if (pts[0].x<pts[1].x && pts[0].y>pts[1].y)
	//				//	img_croped = Mat(img_segmented,Rect(pts[0].x,pts[1].y,abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
	//				if(pts[0].x == pts[1].x || pts[0].y==pts[1].y)
	//				{
	//					cout<<pts[0]<<" and "<<pts[1]<<"can not consist the rectangle for the width or the heith is zero"<<endl;
	//					return;
	//				}
	//				img_croped = Mat(img_segmented,Rect(min(pts[0].x,pts[1].x),min(pts[0].y,pts[1].y),abs(pts[1].x-pts[0].x),abs(pts[1].y-pts[0].y)));
	//				namedWindow("Croped",CV_WINDOW_AUTOSIZE);
	//				imshow("Croped",img_croped);
	//				isCroped = true;
	//				cout<<"Finish crop from the original image!"<<endl;
	//				setMouseCallback("Croped",onCropMouse);
	//				waitKey(0);
	//				isCroped = false;
	//				destroyWindow("Croped");
	//			}

	//		}
	//	}
	//	
	//}
	//Point pt(x,y);
	//
	//if(event == EVENT_RBUTTONDOWN)
	//{
	//	floodFill(src,pt,Scalar(255,255,255));
	//	src_clone=src.clone();
	//	thresh_callback(0,0);
	//	//imshow("origin",src);
	//	imshow("contours",src_clone);
	//	cout<<"The location of mouse is ";
	//	cout<<":("<<x<<","<<y<<")"<<endl;
	//	
	//}

	//if(event == CV_EVENT_MOUSEMOVE)
	//{
	//	string location = "(";
	//	location += x;
	//	location +=",";
	//	location +=y;
	//	location +=")";
	//	putText(src_clone,location,pt,FONT_HERSHEY_SIMPLEX,1,Scalar(255,0,0));
	//}

	//if(event == CV_EVENT_LBUTTONDOWN)
	//{
	//	for(int i=0;i<contours.size();++i)
	//	{
	//		vector<vector<Point>> singelContours;
	//		singelContours.push_back(contours[i]);
	//		if(pointPolygonTest(contours[i],Point(x,y),false) == 1)
	//		{
	//			cout<<pt<<"is in the polygon "<<i<<endl;	
	//			if(hieracky[i][3] == -1)
	//			{
	//				fillPoly(src_clone,singelContours,Scalar(255,255,255),8);
	//				imshow("contours",src_clone);
	//			}
	//			else
	//			{
	//				int parent=hieracky[i][3];
	//				cout<<parent<<endl;
	//			}


	//		}
	//	}
	//}
}


void choice_callback(int,void*)
{
		
}

void onCropMouse(int event,int x,int y,int flags,void* userdata)
{
	Point pt(x,y);
	if(event == CV_EVENT_LBUTTONDOWN)
	{
		for(int i=0;i<contours.size();++i)
		{
			vector<vector<Point>> singelContours;
			singelContours.push_back(contours[i]);
			if(pointPolygonTest(contours[i],Point(x,y),false) == 1)
			{
				cout<<pt<<"is in the polygon "<<i<<endl;	
				if(hieracky[i][3] == -1 && positiveInContour == 0)
				{
					fillPoly(img_croped,singelContours,postive,16);
					imshow("Croped",img_croped);
					imshow("Segment",img_segmented);
				}
				else if(hieracky[i][3] == -1 && positiveInContour == 1)
				{
					fillPoly(img_croped,singelContours,negative,16);
					imshow("Croped",img_croped);
					imshow("Segment",img_segmented);
				}
				else
				{
					int parent=hieracky[i][3];
					cout<<parent<<endl;
				}
			}
		}
	}
	if(positiveInContour == 2)
	{
		MatIterator_<Vec3b> it,end;
		for(it=img_croped.begin<Vec3b>(),end=img_croped.end<Vec3b>();it!=end;++it)
		{
			if((*it)[0]==0 && (*it)[1]==0 && (*it)[2]==0)
				continue;
			(*it)[0] = 255;
			(*it)[1] = 255;
			(*it)[2] = 255;						
		}
		positiveInContour = 0;
	}

}

void status_callback(int ,void*)
{
	
}