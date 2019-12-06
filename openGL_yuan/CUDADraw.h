#ifndef CUDADRAW_H
#define CUDADRAW_H
#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>
#include <Windows.h>
#include "UDP_connect.h"

class CCUDADraw
{
public:
	CCUDADraw(char *pWinName);
	~CCUDADraw();


private:
	int                                         m_nWidth; /* ´°¿ÚÍ¼Æ¬´óÐ¡ */
	int                                         m_nHeight;

	GLuint                                      m_glui_texture[1];
	char                                        m_sWindowName[256]{0};
	cv::Mat									    src_img;
	DASHBOARD_SWITCHDB							m_SWITCHDB_data;
	DASHBOARD_INFO								m_DASHBOARD_data;
private:

	static void CallbackRefresh(void* userdata);
	void Render();

public:

	
	void set_m_SWITCHDB_data(DASHBOARD_SWITCHDB _data);
	void set_m_DASHBOARD_data(DASHBOARD_INFO _data);
	void Init(int Width, int Height,int _length);
	void Refresh();
	int bindTexImg(cv::cuda::GpuMat _GPUMat,int _matLength);
	GLvoid BuildFont(GLvoid);
	GLvoid glPrint(const char *fmt, ...);
	GLvoid KillFont(GLvoid);
	void set_m_glui_texture(GLuint _GLuint[]);
	GLuint* get_m_glui_texture();
};

#endif // !CUDADRAW_H