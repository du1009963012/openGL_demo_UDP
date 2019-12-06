
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

	/******************��ʾ������Ʋ���*********************************/
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_glui_texture[0]);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src_img.cols, src_img.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, src_img.ptr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);//  ����������
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, m_nHeight);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(m_nWidth, m_nHeight);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(m_nWidth, 0);
	glEnd();// �����λ��ƽ���
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);



	/**************����ʾ����************************/
	//glColor3f(1, 1, 1);

	////glTranslatef(794.0f, 700.0f, 0.0f);
	//glLineWidth(10.0f);
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_LINE_LOOP);// 
	//
	//glColor3f(0.0, 1.0, 0);
	//glVertex2f(794, 853);// ����
	//glVertex2f(907, 469);// ����
	//glVertex2f(1331, 469);// ����
	//glVertex2f(1423, 853);// ����
	//
	//glEnd();// 
	//glDisable(GL_TEXTURE_2D);

	/************������Ϣ���Ʋ���*************/
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

	glEnable(GL_TEXTURE_2D);      // ��������ӳ��
	glShadeModel(GL_SMOOTH);      // ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);     // ��ɫ����
	glClearDepth(1.0f);       // ������Ȼ���
	glEnable(GL_DEPTH_TEST);      // ������Ȳ���
	glDepthFunc(GL_LEQUAL);       // ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // ������ϸ��͸������

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
	HFONT font;      // ������
	HFONT oldfont;      // �ɵ�������

	base = glGenLists(96);     // ����96����ʾ�б�
	font = CreateFont(-24,     // ����߶�
		0,		 // ������
		0,		 // �������ת�Ƕ� Angle Of Escapement
		0,		 // ������ߵ���ת�Ƕ�Orientation Angle
				 //����������һ������Ҫ�Ĳ��������������һ��0�C1000֮���ֵ��ʹ��һ���Ѷ����ֵ��FW_DONTCARE��0, FW_NORMAL��400, FW_BOLD��700 and FW_BLACK��900���������Ԥ�ȶ����ֵ���������ĸ���Ч
				 //���ȽϺá�ֵԽ�������Խ�֡�
		FW_BOLD,   // ���������
				   //Italic(б��), Underline(�»���)��Strikeout��ɾ���ߣ�������TRUE��FALSE�������Underline����ΪTRUE����ô����ͻ�����»��ߣ������û�У��ǳ��򵥡�
		FALSE,    // �Ƿ�ʹ��б��
		FALSE,    // �Ƿ�ʹ���»���
		FALSE,    // �Ƿ�ʹ��ɾ����
				  //Character Set Identifier���ַ�����ʶ��������������Ҫʹ�õ��ַ��������룩���͡���̫����Ҫ˵���������ˡ�CHINESEBIG5_CHARSET��GREEK_CHARSET��RUSSIAN_CHARSET��DEFAULT_CHARSET,

				  //�ȵȡ���ʹ�õ���ANSI������DEFAULTҲ�Ǻܺ��õġ�
				  //���������Ȥʹ��Webdings��Wingdings�����壬�����ʹ��SYMBOL_CHARSET������ANSI_CHARSET��

		ANSI_CHARSET,   // �����ַ���

						//Output Precision��������ȣ��ǳ���Ҫ��������Windows���ж����ַ����������ʹ�������ַ�����OUT_TT_PRECIS����Windows���һ�����ֶ�Ӧ���ֲ�ͬ��ѡ�����壬��ôѡ�������TRUETYPE���͡�

						//Truetype����ͨ��������Ҫ��Щ��������������ǷŴ��ʱ����Ҳ����ʹ��OUT_TT_ONLY_PRECIS��������һֱ����ʹ��һ��TRUETYPE���͵�����

		OUT_TT_PRECIS,   // �������

						 //�ü�������һ�ֵ��������ڲü���Χ֮��ʱʹ�õļ������ͣ����ö�˵��ֻҪ��������ΪDEFAULT�Ϳ����ˡ�

		CLIP_DEFAULT_PRECIS,  // �ü�����

							  //��������ǳ���Ҫ�������ʹ��PROOF��DRAFT��NONANTIALIASED��DEFAULT��ANTIALISED��
							  //���Ƕ�֪����ANTIALIASED���忴�����ܺã���һ������Antialiasing(�����)����ʵ����Windows�´�����ƽ��ʱͬ����Ч������ʹ�κζ�����������Ҫ��Щ��ݣ�Ҳ���Ǹ�ƽ����

		ANTIALIASED_QUALITY,  // �������

							  //������Family��Pitch���á�Pitch������DEFAULT_PITCH��FIXED_PITCH��VARIABLE_PITCH��Family��FF_DECORATIVE, FF_MODERN, FF_ROMAN, FF_SCRIPT, FF_SWISS, FF_DONTCARE.����һ����Щ

							  //ֵ����ͻ�֪�����ǵ�����ʲô���ܡ��Ұ����Ƕ�����ΪĬ��ֵ��

		FF_DONTCARE | DEFAULT_PITCH,   // Family And Pitch

									   //�����������Ҫ�������ȷ�е����֡���Microsoft Word������ʲô���ִ��������������������˵�����һ����ϲ�������塣����Courier New���滻Ϊ�����õ���������֣���Ϳ���ʹ�����ˡ������Ļ���
									   //�У���Ҫ��ķ�����

		"Microsoft YaHei UI");        // ��������

									  //���ڣ�ѡ�����ǸղŴ��������塣Oldfont��ָ��ѡ��Ķ���Ȼ�����Ǵӵ�32���ַ����ո񣩿�ʼ����96����ʾ�б������Ը�⣬Ҳ���Խ�������256���ַ���ֻҪȷ��ʹ��glGenLists����256����ʾ�б��

									  //�����ˡ�Ȼ�����ǽ�oldfont����ָ��ѡ��hDC����ɾ��font����

	oldfont = (HFONT)SelectObject(wglGetCurrentDC(), font);   // ѡ��������Ҫ������
	wglUseFontBitmaps(wglGetCurrentDC(), 32, 96, base);   // ����96����ʾ�б����ƴ�ASCII��Ϊ32-128���ַ�
	SelectObject(wglGetCurrentDC(), oldfont);     // ѡ��ԭ��������
	DeleteObject(font);     // ɾ������
}

GLvoid CCUDADraw::glPrint(const char * fmt, ...)
{
	char  text[256];    // �������ִ�
	va_list  ap;     // ָ��һ�������б��ָ��


	if (fmt == NULL)      // ����������򷵻�
		return;


	va_start(ap, fmt);      // �����ɱ����
	vsprintf(text, fmt, ap);    // �Ѳ���ֵд���ַ���
	va_end(ap);      // ��������

					 //Ȼ�����ǽ�GL_LIST_BITѹ�����Զ�ջ�������ֹglListBaseӰ�쵽���ǵĳ����е�������ʾ�б�
					 //GlListBase(base - 32)��һ����Щ�ѽ��͵��������˵Ҫд��ĸ��A����������Ӧ���Ϊ65�����û��glListBase(base - 32)���OpenGL�Ͳ�֪������ȥ�������ĸ����������ʾ�б��еĵ�65��λ����������

					 //�ǣ�����base��ֵ����1000����ô��A����ʵ�ʴ��λ�þ���1065�ˡ�����ͨ��base����һ����㣬OpenGL��֪������ȥ�ҵ���ȷ����ʾ�б��ˡ���ȥ32����Ϊ����û�й����ǰ32����ʾ�б���ô���������Ǻ���

					 //�����ǣ����ǲ��ò�ͨ����base��ֵ��ȥ32����OpenGL֪����һ�㡣��ϣ����Щ�����塣

	glPushAttrib(GL_LIST_BIT);     // ����ʾ�б�����ѹ�����Զ�ջ
	glListBase(base - 32);     // ������ʾ�б�Ļ���ֵ

							   //����OpenGL֪����ĸ�Ĵ��λ���ˣ����ǾͿ�����������Ļ����ʾ�����ˡ�GlCallLists��һ������Ȥ�����������ͬʱ�������ʾ�б��������ʾ����Ļ�ϡ�
							   //����Ĵ������������������ȣ�������OpenGL���ǽ�Ҫ����Ļ����ʾ����ʾ�б��е����ݡ�Strlen(text)���������������ǽ�Ҫ��ʾ����Ļ�ϵ����ֵĳ��ȡ�Ȼ��OpenGL��Ҫ֪�����������͸������б�����

							   //ֵ�����ǲ��ܷ��ͳ��ȴ���255���ַ���������ַ��б�Ĳ���������һ���޷����ַ����鴦�����ǵ�ֵ������0��255֮�䡣�������ͨ������text(��ָ�����ǵ��ַ���)������OpenGL��ʾ�����ݡ�

							   //Ҳ������֪��Ϊʲô�ַ�����˴��ص��ѻ���һ����ʱ��Ϊÿ���ַ�����ʾ�б�֪���ַ����ұ�Ե�������д��һ���ַ���OpenGL�Զ��ƶ�����д�����ַ����ұߣ���д��һ���ֻ���һ������ʱ�ͻ��GL��

							   //����������λ�ÿ�ʼ��Ҳ�������һ���ַ����ұߡ�

							   //������ǽ�GL_LIST_BIT���Ե�����ջ����GL�ָ�������ʹ��glListBase(base - 32)����base��ʱ��״̬��

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // ������ʾ�б�����ַ���
	glPopAttrib();      // �������Զ�ջ
}

GLvoid CCUDADraw::KillFont(GLvoid)
{
	glDeleteLists(base, 96);     //ɾ��96����ʾ�б�
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
