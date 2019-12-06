#ifndef UDP_CONNECT_H
#define UDP_CONNECT_H
#include <iostream>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
/*
	智行未来仪表盘数据传输协议
	版本编号:
		v1.0.0

	修改日期:
		2019/12/04

	协议内容:
		消息传输方式:
			UDP
		消息传输周期:
			根据消息ID决定
		消息排序方式:
			小端序
		消息帧结构如下：（表中B代表Byte字节）
		------------------------------------------------------------------------------
		| 标识位 | 消息ID | 台架ID |消息流水号| 消息体长度 |     消息体     | 校验位 |
		|   1B   |   1B   |   1B   |    1B    |     2B     |  消息体长度*B  |   1B   |
		------------------------------------------------------------------------------
		标识位:
			0xCA
		消息ID:
			0x11		台架仪表盘数据		周期:200ms			消息体长度:5B
			0x12  		台架切换仪表盘		周期:事件性触发		消息体长度:0B
		台架ID:
			1~255为有效的台架ID
		消息流水号:
			每帧消息加1
		消息体长度:
			消息体所占字节数
		校验码:
			对消息中除了标识位和校验位之外的所有字节进行异或校验
		消息解析范例:
			1. 台架仪表盘数据消息
				消息:0xCA 0x11 0x01 0x64 0x05 0x00 0x01 0x00 0x2C 0x01 0x64 0x39
					类型			报文字段					实际意义
					标识位:			0xCA 						标识位
					消息ID:			0x11 						台架仪表盘数据
					平台ID:			0x01						1号台架
					消息流水号:		0x64						第100帧
					消息体长度:		0x05 0x00					0x0005 长度为5字节
					消息体:
									0x01 						P档
									0x00 				 		转向灯关闭
									0x2C 0x01 					车速 0x012C -> 300 * 0.1km/h -> 30km/h
									0x64						电量 0x64->100-> 100%
					校验码:			0x39						校验位

			2. 仪表盘切换消息
				消息:0xCA 0x12 0x01 0x64 0x00 0x00 0x4E
					类型			报文字段					实际意义
					标识位:			0xCA 						标识位
					消息ID:			0x12 						切换仪表盘
					平台ID:			0x01						1号台架
					消息流水号:		0x64						第100帧
					消息体长度:		0x00 0x00					长度为0字节
					消息体:										空
					校验码:			0x4E						校验位
*/

typedef struct {
	unsigned char 	Head; 						//标识位  			值见宏值定义
	unsigned char	packetID;					//消息ID			值见宏值定义
	unsigned char 	platID;						//台架编号			1~255
	unsigned char 	frameID;					//消息流水号		0x00 ~ 0xFF
	unsigned short  length;						//消息内容的长度(5B)
	/* 消息内容 */
	unsigned char	gear;						//档位				P档:0x1，R档:0x2，N档:0x3，D档:0x4
	unsigned char	turnsig;					//转向灯			关闭:0x0，左转向:0x1，右转向:0x2 
	unsigned short	speed;						//当前车速			单位:km/h，比例尺:0.1，范围:0~4600
	unsigned char 	soc;						//电量				剩余电量的百分比，比例尺:1，范围:0~100
	/* 校验位 */
	unsigned char 	xor_verify;  				//异或校验			(除了帧头以外的成员进行异或校验)
}DASHBOARD_INFO;

typedef struct {
	unsigned char 	Head; 						//标识位  			值见宏值定义
	unsigned char	packetID;					//消息ID			值见宏值定义
	unsigned char 	platID;						//台架编号			1~255
	unsigned char 	frameID;					//消息流水号		0x00 ~ 0xFF
	unsigned short  length;						//消息内容的长度(0B)
	/* 消息内容 */
	/* 空 */
	/* 校验位 */
	unsigned char 	xor_verify;  				//异或校验			(除了帧头以外的成员进行异或校验)
}DASHBOARD_SWITCHDB;

typedef struct {
	DASHBOARD_INFO data;
	int clock{1};
}DASHBOARD_INFOS;
typedef struct {
	DASHBOARD_SWITCHDB data;
	int clock{1};
}DASHBOARD_SWITCHDBS;
class UDP_connect
{
public:
	UDP_connect();
	~UDP_connect();
private:
	SOCKET UDP_serSocket;
	std::thread m_Crete;
	DASHBOARD_INFOS					m_infos;
	DASHBOARD_SWITCHDBS				m_switchbs;
	
public:
	void create_connect(u_short _port);
	DASHBOARD_SWITCHDB get_DASHBOARD_SWITCHDB();
	DASHBOARD_INFO get_DASHBOARD_INFO();
};

#endif // !UDP_CONNECT_H