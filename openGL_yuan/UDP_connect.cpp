
#include "UDP_connect.h"


UDP_connect::UDP_connect()
{
	/*memset(&m_switchbs.data.frameID,0,sizeof(DASHBOARD_SWITCHDBS));
	memset(&m_infos.data.frameID,0,sizeof(DASHBOARD_INFO));*/
}


UDP_connect::~UDP_connect()
{
	closesocket( UDP_serSocket);
}

void UDP_connect::create_connect(u_short _port)
{
	WSADATA WSAData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &WSAData) != 0)
		return;

	UDP_serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//创建服务器socket
	if (INVALID_SOCKET == UDP_serSocket)
	{
		std::cout << "socket error!";
		return;
	}

	//设置传输协议、端口以及目的地址 
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(_port);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(UDP_serSocket, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)	 //将socket绑定地址 
	{
		std::cout << "bind error";
		closesocket(UDP_serSocket);
		return;
	}
	sockaddr_in clientAddr;
	int iAddrlen = sizeof(clientAddr);
	m_Crete =  std::thread([=](_In_ SOCKET s, _In_ int flags, sockaddr_in clientAddr, int iAddrlen) {
		int fromlen = sizeof(iAddrlen);
		int buffer_size = sizeof(DASHBOARD_SWITCHDB) > sizeof(DASHBOARD_INFO) ? sizeof(DASHBOARD_SWITCHDB) : sizeof(DASHBOARD_INFO);
		char cech_data[12];
		while (1) {
			int len = recvfrom(s, cech_data, buffer_size, 0, (SOCKADDR *)&clientAddr, &iAddrlen);
			if (len > 0)
			{
				switch (len) {
					case 7:
					{
						if ((cech_data[0] == (char)0xca) && (cech_data[1] == (char)0x12) && m_switchbs.clock)//DASHBOARD_SWITCHDB
						{
							m_switchbs.data.Head = 0xca;
							m_switchbs.data.packetID = cech_data[1];
							m_switchbs.data.platID = cech_data[2];
							m_switchbs.data.frameID = cech_data[3];
							m_switchbs.data.length = ((0x00FF & cech_data[5]) << 8) | (0x00FF & cech_data[4]);
							m_switchbs.data.xor_verify = cech_data[6];
							m_switchbs.clock = 0;
						}
				
						break;
					}

					case 12: 
					{

						if ((cech_data[0] == (char)0xca) && (cech_data[1] == (char)0x11) && m_infos.clock) {//DASHBOARD_INFO
						/*if (cech_data[1]^ cech_data[2]^ cech_data[3]^ cech_data[4]^ cech_data[5]^ cech_data[6]^ cech_data)
						{*/
							m_infos.data.Head = 0xca;
							m_infos.data.packetID = cech_data[1];
							m_infos.data.platID = cech_data[2];
							m_infos.data.frameID = cech_data[3];
							m_infos.data.length = ((0x00FF & cech_data[5]) << 8) | (0x00FF & cech_data[4]);
							m_infos.data.gear = cech_data[6];
							m_infos.data.turnsig = cech_data[7];
							m_infos.data.speed = ((0x00FF & cech_data[9]) << 8) | (0x00FF & cech_data[8]);
							m_infos.data.soc = cech_data[10];
							m_infos.data.xor_verify = cech_data[11];

							m_infos.clock = 0;

							//}

						}
						break;
					}
				}
			}	
		}
	}, UDP_serSocket, 0, clientAddr, iAddrlen);
	m_Crete.detach();
	//return ;
}

DASHBOARD_SWITCHDB UDP_connect::get_DASHBOARD_SWITCHDB()
{
	
		DASHBOARD_SWITCHDB temp = m_switchbs.data;
		m_switchbs.clock = 1;
		return temp;
	
}

DASHBOARD_INFO UDP_connect::get_DASHBOARD_INFO()
{
	
		DASHBOARD_INFO temp = m_infos.data;
		m_infos.clock = 1;
		return temp;
	
}
