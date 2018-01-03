#include "../global/helper.h"
#include<sys/time.h>
#include <cstdlib>
#include "../hpsocket/TcpClient.h"
#include <pthread.h>

#define NUM_THREADS 1000

client_statistics_info s_stat;
class CListenerImpl : public CTcpClientListener
{

public:
	virtual EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override
	{
		return HR_OK;
	}

	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override
	{
#ifdef _DEBUG
		::PostOnConnect3(dwConnID);
#endif
		
		return HR_OK;
	}

	virtual EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override
	{
		return HR_OK;
	}

	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
#ifdef _DEBUG2
		::PostOnReceive(dwConnID, pData, iLength);
#endif
		s_stat.AddTotalRecv(iLength);

		return HR_OK;
	}

	virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
#ifdef _DEBUG2
		::PostOnSend(dwConnID, pData, iLength);
#endif
		s_stat.AddTotalSend(iLength);

		return HR_OK;
	}

	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
	{
		if(iErrorCode == SE_OK)
		{
#ifdef _DEBUG
			::PostOnClose(dwConnID);
#endif
		}
		else
		{
			::PostOnError(dwConnID, enOperation, iErrorCode);
		}

		return HR_OK;
	}

};


CListenerImpl s_listener;
//vector<ITcpClient*> s_vtClients;
// 线程的运行函数
void* say_hello(void *args)
{
	
	CBufferPtr s_sendBuffer;
	int i=(int)(*((int*)args));
	s_stat.Reset();
	unique_ptr<ITcpClient> pSocket = make_unique<CTcpClient>(&s_listener);

		pSocket->SetKeepAliveTime(g_app_arg.keep_alive ? TCP_KEEPALIVE_TIME : 0);

		if(pSocket->Start(g_app_arg.remote_addr, g_app_arg.port, FALSE))
			;
		//	s_vtClients.push_back(pSocket.release());
		else
		{
			::LogClientStartFail(pSocket->GetLastError(), pSocket->GetLastErrorDesc());
		//	ClearPtrSet(s_vtClients);
			return 0;
		}
	

	::LogClientStart(g_app_arg.remote_addr, g_app_arg.port);


 
    for(DWORD j = 0;j<1000 ; j++    )
		{
			sleep(1);
			timeval tBegin;
		   
		        gettimeofday(&tBegin, 0);  
			long long start = ((long)tBegin.tv_sec)*1000+(long)tBegin.tv_usec/1000;
			
			//char* Str=":";
			int size=sizeof(i)+sizeof(j) +sizeof(start)+ 2*sizeof(',')+1;
			char *buf = new char[size];
			sprintf(buf, "%d,%d,%lld\0", i, j,start);
			PRINTLN("OnSend:%s", buf);
			s_sendBuffer.Malloc(strlen(buf)+1, true);
			s_sendBuffer.Copy((const unsigned char*)buf,size);
			if(!pSocket->Send(s_sendBuffer, strlen(buf)+1))
			{
				PRINTLN("Onsend failed\n");
				s_sendBuffer.Free();
				//::LogClientSendFail(i + 1, j + 1, ::GetLastError(), ::GetSocketErrorDesc(SE_DATA_SEND));
				break;
			}
			s_sendBuffer.Free();
		}


	
    return 0;
}
 

void OnCmdStart(CCommandParser* pParser)
{
	pthread_t tids[NUM_THREADS];
    	for(int i = 0; i < NUM_THREADS; ++i)
    	{
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        	int ret = pthread_create(&tids[i], NULL, say_hello, &i);
        	if (ret != 0)
        	{
        	   PRINTLN("pthread%d faild!",i);
        	}
    	}
    	//等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
	while(1);    	
	pthread_exit(NULL);


/*	if(!s_vtClients.empty())
	{
		::LogClientStartFail(SE_ILLEGAL_STATE, ::GetSocketErrorDesc(SE_ILLEGAL_STATE));
		return;
	}

	s_stat.Reset();

	for(DWORD i = 0; i < g_app_arg.conn_count; i++)
	{
		unique_ptr<ITcpClient> pSocket = make_unique<CTcpClient>(&s_listener);

		pSocket->SetKeepAliveTime(g_app_arg.keep_alive ? TCP_KEEPALIVE_TIME : 0);

		if(pSocket->Start(g_app_arg.remote_addr, g_app_arg.port, FALSE))
			s_vtClients.push_back(pSocket.release());
		else
		{
			::LogClientStartFail(pSocket->GetLastError(), pSocket->GetLastErrorDesc());
			ClearPtrSet(s_vtClients);
			return;
		}
	}

	::LogClientStart(g_app_arg.remote_addr, g_app_arg.port);

	DWORD dwSendDelay = 3;
	CString strMsg;

	strMsg.Format(_T("*** willing to send data after %d seconds ***"), dwSendDelay);
	::LogMsg(strMsg);

	::WaitFor(dwSendDelay * 1000);

	::LogMsg(_T("*** Go Now ! ***"));

s_stat.StartTest();                                           

	BOOL bTerminated = FALSE;
	for(DWORD i = 0; i < g_app_arg.test_times; i++)
	{
		for(DWORD j = 0; j < 1000; j++    )
		{
			 sleep(1);
			timeval tBegin;
		   
		        gettimeofday(&tBegin, 0);  
			long long start = ((long)tBegin.tv_sec)*1000+(long)tBegin.tv_usec/1000;
			
			//char* Str=":";
			int size=sizeof(i)+sizeof(j) +sizeof(start)+ 2*sizeof(',')+1;
			char *buf = new char[size];
			sprintf(buf, "%d,%d,%lld\0", i, j,start);
			PRINTLN("OnSend:%s", buf);
			s_sendBuffer.Malloc(strlen(buf)+1, true);
			s_sendBuffer.Copy((const unsigned char*)buf,size);
			ITcpClient* pSocket = s_vtClients[j];
			if(!pSocket->Send(s_sendBuffer, strlen(buf)+1))
			{
				::LogClientSendFail(i + 1, j + 1, ::GetLastError(), ::GetSocketErrorDesc(SE_DATA_SEND));
				bTerminated = TRUE;
				break;
			}
		}

		if(bTerminated)
			break;

		if(g_app_arg.test_interval > 0 && i + 1 < g_app_arg.test_times)
			::WaitFor(g_app_arg.test_interval);
	}

	if(bTerminated)
		::ClearPtrSet(s_vtClients);

	s_sendBuffer.Free();
*/
}

void OnCmdStop(CCommandParser* pParser)
{
	//if(s_vtClients.empty())
	//{
//		::LogClientStopFail(SE_ILLEGAL_STATE, ::GetSocketErrorDesc(SE_ILLEGAL_STATE));
	//	return;
	//}

	//::LogClientStopping(s_vtClients.size());

	//::ClearPtrSet(s_vtClients);

	s_stat.CheckStatistics();
}

void OnCmdStatus(CCommandParser* pParser)
{
	//pParser->PrintStatus(s_vtClients.empty() ? SS_STOPPED : SS_STARTED);
}

int main(int argc, char* const argv[])
{
	CTermAttrInitializer term_attr;
	CAppSignalHandler s_signal_handler({SIGTTOU, SIGINT});

	g_app_arg.ParseArgs(argc, argv);

	g_app_arg.async = FALSE;
	g_app_arg.ShowPFMTestArgs(FALSE);

	CCommandParser::CMD_FUNC fnCmds[CCommandParser::CT_MAX] = {0};

	fnCmds[CCommandParser::CT_START]  = OnCmdStart;
	fnCmds[CCommandParser::CT_STOP]	  = OnCmdStop;
	fnCmds[CCommandParser::CT_STATUS] = OnCmdStatus;

	CCommandParser s_cmd_parser(CCommandParser::AT_CLIENT, fnCmds);
	s_cmd_parser.Run();

	//if(!s_vtClients.empty())
	//	OnCmdStop(&s_cmd_parser);

	return EXIT_CODE_OK;
}
