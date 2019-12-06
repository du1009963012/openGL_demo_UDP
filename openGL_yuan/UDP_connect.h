#ifndef UDP_CONNECT_H
#define UDP_CONNECT_H
#include <iostream>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
/*
	����δ���Ǳ������ݴ���Э��
	�汾���:
		v1.0.0

	�޸�����:
		2019/12/04

	Э������:
		��Ϣ���䷽ʽ:
			UDP
		��Ϣ��������:
			������ϢID����
		��Ϣ����ʽ:
			С����
		��Ϣ֡�ṹ���£�������B����Byte�ֽڣ�
		------------------------------------------------------------------------------
		| ��ʶλ | ��ϢID | ̨��ID |��Ϣ��ˮ��| ��Ϣ�峤�� |     ��Ϣ��     | У��λ |
		|   1B   |   1B   |   1B   |    1B    |     2B     |  ��Ϣ�峤��*B  |   1B   |
		------------------------------------------------------------------------------
		��ʶλ:
			0xCA
		��ϢID:
			0x11		̨���Ǳ�������		����:200ms			��Ϣ�峤��:5B
			0x12  		̨���л��Ǳ���		����:�¼��Դ���		��Ϣ�峤��:0B
		̨��ID:
			1~255Ϊ��Ч��̨��ID
		��Ϣ��ˮ��:
			ÿ֡��Ϣ��1
		��Ϣ�峤��:
			��Ϣ����ռ�ֽ���
		У����:
			����Ϣ�г��˱�ʶλ��У��λ֮��������ֽڽ������У��
		��Ϣ��������:
			1. ̨���Ǳ���������Ϣ
				��Ϣ:0xCA 0x11 0x01 0x64 0x05 0x00 0x01 0x00 0x2C 0x01 0x64 0x39
					����			�����ֶ�					ʵ������
					��ʶλ:			0xCA 						��ʶλ
					��ϢID:			0x11 						̨���Ǳ�������
					ƽ̨ID:			0x01						1��̨��
					��Ϣ��ˮ��:		0x64						��100֡
					��Ϣ�峤��:		0x05 0x00					0x0005 ����Ϊ5�ֽ�
					��Ϣ��:
									0x01 						P��
									0x00 				 		ת��ƹر�
									0x2C 0x01 					���� 0x012C -> 300 * 0.1km/h -> 30km/h
									0x64						���� 0x64->100-> 100%
					У����:			0x39						У��λ

			2. �Ǳ����л���Ϣ
				��Ϣ:0xCA 0x12 0x01 0x64 0x00 0x00 0x4E
					����			�����ֶ�					ʵ������
					��ʶλ:			0xCA 						��ʶλ
					��ϢID:			0x12 						�л��Ǳ���
					ƽ̨ID:			0x01						1��̨��
					��Ϣ��ˮ��:		0x64						��100֡
					��Ϣ�峤��:		0x00 0x00					����Ϊ0�ֽ�
					��Ϣ��:										��
					У����:			0x4E						У��λ
*/

typedef struct {
	unsigned char 	Head; 						//��ʶλ  			ֵ����ֵ����
	unsigned char	packetID;					//��ϢID			ֵ����ֵ����
	unsigned char 	platID;						//̨�ܱ��			1~255
	unsigned char 	frameID;					//��Ϣ��ˮ��		0x00 ~ 0xFF
	unsigned short  length;						//��Ϣ���ݵĳ���(5B)
	/* ��Ϣ���� */
	unsigned char	gear;						//��λ				P��:0x1��R��:0x2��N��:0x3��D��:0x4
	unsigned char	turnsig;					//ת���			�ر�:0x0����ת��:0x1����ת��:0x2 
	unsigned short	speed;						//��ǰ����			��λ:km/h��������:0.1����Χ:0~4600
	unsigned char 	soc;						//����				ʣ������İٷֱȣ�������:1����Χ:0~100
	/* У��λ */
	unsigned char 	xor_verify;  				//���У��			(����֡ͷ����ĳ�Ա�������У��)
}DASHBOARD_INFO;

typedef struct {
	unsigned char 	Head; 						//��ʶλ  			ֵ����ֵ����
	unsigned char	packetID;					//��ϢID			ֵ����ֵ����
	unsigned char 	platID;						//̨�ܱ��			1~255
	unsigned char 	frameID;					//��Ϣ��ˮ��		0x00 ~ 0xFF
	unsigned short  length;						//��Ϣ���ݵĳ���(0B)
	/* ��Ϣ���� */
	/* �� */
	/* У��λ */
	unsigned char 	xor_verify;  				//���У��			(����֡ͷ����ĳ�Ա�������У��)
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