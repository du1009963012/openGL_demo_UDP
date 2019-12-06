// openGL_yuan.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

#include "CUDADraw.h"





std::string g_sWindowsName = "opengl";
static CCUDADraw						*g_pCUDADraw = NULL;
#define VIDEO_WIDTH                 1920
#define VIDEO_HEIGHT                1080
#define g_nImageWidth               1920
#define g_nImageHeight              1080
static cv::cuda::GpuMat gpuResultC3 = cv::cuda::GpuMat(1080, 1920, CV_8UC3);
static cv::cuda::GpuMat gpuResultC4 = cv::cuda::GpuMat(1080, 1920, CV_8UC4);
UDP_connect UDP_mannega;
int InitCUDADraw(int nWidth, int nHeight, CCUDADraw *pCUDADraw) 
{
	pCUDADraw->Init(nWidth, nHeight,1);
	
	return 0;
}

int main()
{
	std::string filePath = "main_2.mp4";
	cv::namedWindow(g_sWindowsName,cv::WINDOW_OPENGL);
	cv::resizeWindow(g_sWindowsName, VIDEO_WIDTH, VIDEO_HEIGHT);
	cv::setWindowProperty(g_sWindowsName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	UDP_mannega.create_connect(1234);
	g_pCUDADraw = new CCUDADraw((char*)g_sWindowsName.data());
	InitCUDADraw(gpuResultC3.cols, gpuResultC3.rows, g_pCUDADraw);
	cv::setOpenGlContext(g_sWindowsName);
	cv::cuda::cvtColor(gpuResultC4, gpuResultC3, CV_BGRA2BGR);
	cv::VideoCapture cap;
restart:
	if (!cap.open(filePath)) {
		std::cout << "open file fild!"<<std::endl;
		return 0;
	}
    std::cout << "Hello World!\n";
	cv::Mat src_p;
	for (;;)
	{
		cap>>src_p;
		if (!src_p.data)goto restart;
		//cv::imshow("src",src_p);
		gpuResultC3.upload(src_p);
		g_pCUDADraw->bindTexImg(gpuResultC3,1);
		g_pCUDADraw->set_m_DASHBOARD_data(UDP_mannega.get_DASHBOARD_INFO());
		g_pCUDADraw->set_m_SWITCHDB_data(UDP_mannega.get_DASHBOARD_SWITCHDB());
		g_pCUDADraw->Refresh();
		//cv::waitKey(1000/cap.get(CV_CAP_PROP_FPS));
		if(27==cv::waitKey(1))break;
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
