
#include "CUDADraw.h"


CCUDADraw::CCUDADraw(char *pWinName)
{
	memset(m_sWindowName, 0, sizeof(m_sWindowName));
	memcpy(m_sWindowName, pWinName, strlen(pWinName));
}


CCUDADraw::~CCUDADraw()
{
	cv::destroyAllWindows();
	
}

void CCUDADraw::Refresh()
{
	cv::setOpenGlDrawCallback(m_sWindowName, CallbackRefresh, this);
	cv::updateWindow(m_sWindowName);

}
void CCUDADraw::CallbackRefresh(void * userdata)
{
	CCUDADraw * ptr = static_cast<CCUDADraw*>(userdata);
	ptr->Render();
}

void CCUDADraw::Render()
{
	static double cnt2 = 0;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glTranslatef(0.0f, 0.0f, 0.0f); 
	glColor3f(1, 1, 1);

	/******************显示画面绘制部分*********************************/
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_glui_texture[0]);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src_img.cols, src_img.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, src_img.ptr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);//  绘制正方形
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, m_nHeight);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(m_nWidth, m_nHeight);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(m_nWidth, 0);
	glEnd();// 正方形绘制结束
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);



	/**************绘制示廓线************************/
	//glColor3f(1, 1, 1);

	////glTranslatef(794.0f, 700.0f, 0.0f);
	//glLineWidth(10.0f);
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_LINE_LOOP);// 
	//
	//glColor3f(0.0, 1.0, 0);
	//glVertex2f(794, 853);// 左下
	//glVertex2f(907, 469);// 右下
	//glVertex2f(1331, 469);// 右下
	//glVertex2f(1423, 853);// 右下
	//
	//glEnd();// 
	//glDisable(GL_TEXTURE_2D);

	/************车辆信息绘制部分*************/
	glColor3f(1, 1, 1);
	//glTranslatef(0.0f, -2.0f, 0.0f);
	BuildFont();
	glRasterPos2f(m_nWidth / 2 - 100, m_nHeight - 100);
	glPrint("car_speed:%d", m_DASHBOARD_data.speed);
	KillFont();

	BuildFont();
	glRasterPos2f(m_nWidth / 2 - 300, m_nHeight - 100);
	glPrint("car_platID:%d", m_SWITCHDB_data.platID);
	KillFont();
	//cnt2 += 0.005f;
}

void CCUDADraw::set_m_SWITCHDB_data(DASHBOARD_SWITCHDB _data)
{
	m_SWITCHDB_data = _data;
}

void CCUDADraw::set_m_DASHBOARD_data(DASHBOARD_INFO _data)
{
	m_DASHBOARD_data = _data;
}

void CCUDADraw::Init(int Width, int Height,int _length)
{
	m_nWidth = Width;
	m_nHeight = Height;

	glViewport(0, 0, (GLsizei)m_nWidth, (GLsizei)m_nHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, m_nWidth, m_nHeight, 0);
	gluPerspective(36.0, 1, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

	//glEnable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);      // 启用纹理映射
	glShadeModel(GL_SMOOTH);      // 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);     // 黑色背景
	glClearDepth(1.0f);       // 设置深度缓存
	glEnable(GL_DEPTH_TEST);      // 启用深度测试
	glDepthFunc(GL_LEQUAL);       // 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // 真正精细的透视修正

	for (int i = 0; i < _length; i++)
	{
		
		glGenTextures(1, &m_glui_texture[i]);

	}
	
	//UDP_mannega.create_connect(11400);
}



int CCUDADraw::bindTexImg(cv::cuda::GpuMat _GPUMat,int _matLength=0)
{

	
	_GPUMat.download(src_img);
	glBindTexture(GL_TEXTURE_2D, m_glui_texture[0]);
	//cv::cvtColor(src,src,CV_BGR2RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src_img.cols, src_img.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, src_img.ptr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	src_img.~Mat();
	return 0;
}
static int base;
GLvoid CCUDADraw::BuildFont(GLvoid)
{
	HFONT font;      // 字体句柄
	HFONT oldfont;      // 旧的字体句柄

	base = glGenLists(96);     // 创建96个显示列表
	font = CreateFont(-24,     // 字体高度
		0,		 // 字体宽度
		0,		 // 字体的旋转角度 Angle Of Escapement
		0,		 // 字体底线的旋转角度Orientation Angle
				 //字体重量是一个很重要的参数，你可以设置一个0–1000之间的值或使用一个已定义的值。FW_DONTCARE是0, FW_NORMAL是400, FW_BOLD是700 and FW_BLACK是900。还有许多预先定义的值，但是这四个的效
				 //果比较好。值越大，字体就越粗。
		FW_BOLD,   // 字体的重量
				   //Italic(斜体), Underline(下划线)和Strikeout（删除线）可以是TRUE或FALSE。如果将Underline设置为TRUE，那么字体就会带有下划线，否则就没有，非常简单。
		FALSE,    // 是否使用斜体
		FALSE,    // 是否使用下划线
		FALSE,    // 是否使用删除线
				  //Character Set Identifier（字符集标识符）用来描述你要使用的字符集（内码）类型。有太多需要说明的类型了。CHINESEBIG5_CHARSET，GREEK_CHARSET，RUSSIAN_CHARSET，DEFAULT_CHARSET,

				  //等等。我使用的是ANSI，尽管DEFAULT也是很好用的。
				  //如果你有兴趣使用Webdings或Wingdings等字体，你必须使用SYMBOL_CHARSET而不是ANSI_CHARSET。

		ANSI_CHARSET,   // 设置字符集

						//Output Precision（输出精度）非常重要。它告诉Windows在有多种字符集的情况下使用哪类字符集。OUT_TT_PRECIS告诉Windows如果一个名字对应多种不同的选择字体，那么选择字体的TRUETYPE类型。

						//Truetype字体通常看起来要好些，尤其是你把它们放大的时候。你也可以使用OUT_TT_ONLY_PRECIS，它将会一直尝试使用一种TRUETYPE类型的字体

		OUT_TT_PRECIS,   // 输出精度

						 //裁剪精度是一种当字体落在裁剪范围之外时使用的剪辑类型，不用多说，只要把它设置为DEFAULT就可以了。

		CLIP_DEFAULT_PRECIS,  // 裁剪精度

							  //输出质量非常重要。你可以使用PROOF，DRAFT，NONANTIALIASED，DEFAULT或ANTIALISED。
							  //我们都知道，ANTIALIASED字体看起来很好，将一种字体Antialiasing(反锯齿)可以实现在Windows下打开字体平滑时同样的效果，它使任何东西看起来都要少些锯齿，也就是更平滑。

		ANTIALIASED_QUALITY,  // 输出质量

							  //下面是Family和Pitch设置。Pitch属性有DEFAULT_PITCH，FIXED_PITCH和VARIABLE_PITCH，Family有FF_DECORATIVE, FF_MODERN, FF_ROMAN, FF_SCRIPT, FF_SWISS, FF_DONTCARE.尝试一下这些

							  //值，你就会知道它们到底有什么功能。我把它们都设置为默认值。

		FF_DONTCARE | DEFAULT_PITCH,   // Family And Pitch

									   //最后，是我们需要的字体的确切的名字。打开Microsoft Word或其它什么文字处理软件，点击字体下拉菜单，找一个你喜欢的字体。将‘Courier New’替换为你想用的字体的名字，你就可以使用它了。（中文还不
									   //行，需要别的方法）

		"Microsoft YaHei UI");        // 字体名称

									  //现在，选择我们刚才创建的字体。Oldfont将指向被选择的对象。然后我们从第32个字符（空格）开始建立96个显示列表。如果你愿意，也可以建立所有256个字符，只要确保使用glGenLists建立256个显示列表就

									  //可以了。然后我们将oldfont对象指针选入hDC并且删除font对象。

	oldfont = (HFONT)SelectObject(wglGetCurrentDC(), font);   // 选择我们需要的字体
	wglUseFontBitmaps(wglGetCurrentDC(), 32, 96, base);   // 创建96个显示列表，绘制从ASCII码为32-128的字符
	SelectObject(wglGetCurrentDC(), oldfont);     // 选择原来的字体
	DeleteObject(font);     // 删除字体
}

GLvoid CCUDADraw::glPrint(const char * fmt, ...)
{
	char  text[256];    // 保存文字串
	va_list  ap;     // 指向一个变量列表的指针


	if (fmt == NULL)      // 如果无输入则返回
		return;


	va_start(ap, fmt);      // 分析可变参数
	vsprintf(text, fmt, ap);    // 把参数值写入字符串
	va_end(ap);      // 结束分析

					 //然后我们将GL_LIST_BIT压入属性堆栈，它会防止glListBase影响到我们的程序中的其它显示列表。
					 //GlListBase(base - 32)是一条有些难解释的命令。比如说要写字母‘A’，它的相应编号为65。如果没有glListBase(base - 32)命令，OpenGL就不知道到哪去找这个字母。它会在显示列表中的第65个位置找它，但

					 //是，假如base的值等于1000，那么‘A’的实际存放位置就是1065了。所以通过base设置一个起点，OpenGL就知道到哪去找到正确的显示列表了。减去32是因为我们没有构造过前32个显示列表，那么就跳过它们好了

					 //。于是，我们不得不通过从base的值减去32来让OpenGL知道这一点。我希望这些有意义。

	glPushAttrib(GL_LIST_BIT);     // 把显示列表属性压入属性堆栈
	glListBase(base - 32);     // 设置显示列表的基础值

							   //现在OpenGL知道字母的存放位置了，我们就可以让它在屏幕上显示文字了。GlCallLists是一个很有趣的命令。它可以同时将多个显示列表的内容显示在屏幕上。
							   //下面的代码做后续工作。首先，它告诉OpenGL我们将要在屏幕上显示出显示列表中的内容。Strlen(text)函数用来计算我们将要显示在屏幕上的文字的长度。然后，OpenGL需要知道我们允许发送给它的列表的最大

							   //值。我们不能发送长度大于255的字符串。这个字符列表的参数被当作一个无符号字符数组处理，它们的值都介于0到255之间。最后，我们通过传递text(它指向我们的字符串)来告诉OpenGL显示的内容。

							   //也许你想知道为什么字符不会彼此重叠堆积在一起。那时因为每个字符的显示列表都知道字符的右边缘在那里，在写完一个字符后，OpenGL自动移动到刚写过的字符的右边，在写下一个字或画下一个物体时就会从GL移

							   //动到的最后的位置开始，也就是最后一个字符的右边。

							   //最后，我们将GL_LIST_BIT属性弹出堆栈，将GL恢复到我们使用glListBase(base - 32)设置base那时的状态。

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // 调用显示列表绘制字符串
	glPopAttrib();      // 弹出属性堆栈
}

GLvoid CCUDADraw::KillFont(GLvoid)
{
	glDeleteLists(base, 96);     //删除96个显示列表
}

void CCUDADraw::set_m_glui_texture(GLuint _GLuint[])
{
	m_glui_texture[0] = _GLuint[0];
	m_glui_texture[1] = _GLuint[1];
}

GLuint* CCUDADraw::get_m_glui_texture()
{
	//GLuint tmp[] = &m_glui_texture;
	return m_glui_texture;
}
