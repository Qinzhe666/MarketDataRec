#include "Trader.h"

Trader::Trader() : tdapi(nullptr), bIsReady(false)
{
    //loadBrokerInfo("./broker/simnow.txt", bi);
    //loadBrokerInfo("./broker/simnow_24.txt", bi);
    loadBrokerInfo("./broker/mk.txt", bi);
    
    createFolder(pTradeFlowPath);

    tdapi = CThostFtdcTraderApi::CreateFtdcTraderApi(pTradeFlowPath);
    tdapi->RegisterSpi(this);
    tdapi->RegisterFront(bi.TradeFront);
    tdapi->SubscribePublicTopic(THOST_TERT_QUICK);
    tdapi->SubscribePrivateTopic(THOST_TERT_QUICK);
    tdapi->Init();
}

Trader::~Trader() {}

///�ͻ�����֤����
void Trader::reqAuthenticate()
{
    CThostFtdcReqAuthenticateField t{};
    strcpy(t.BrokerID, bi.BrokerID);
    strcpy(t.UserID, bi.UserID);
    strcpy(t.AuthCode, bi.AuthCode);
    strcpy(t.AppID, bi.AppID);
    
    tdapi->ReqAuthenticate(&t, ++nReqID);
}

///�û���¼����
void Trader::reqUserLogin()
{
    CThostFtdcReqUserLoginField t{};
    strcpy(t.BrokerID, bi.BrokerID);
    strcpy(t.UserID, bi.UserID);
    strcpy(t.Password, bi.Password);
    
    tdapi->ReqUserLogin(&t, ++nReqID);
}

///�����ѯ��Լ
void Trader::reqQryInstrument()
{
    CThostFtdcQryInstrumentField t{};
    
    tdapi->ReqQryInstrument(&t, ++nReqID);
}


///���ͻ����뽻�׺�̨������ͨ������ʱ(��δ��¼ǰ)���÷��������á�
void Trader::OnFrontConnected()
{
    reqAuthenticate();
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void Trader::OnFrontDisconnected(int nReason)
{
    printf("Trade Front Disconnected\n");
    printf("nReason = %d %s\n", nReason, nReason2str(nReason));
}

///�ͻ�����֤��Ӧ
void Trader::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
    {
        printf("Failed to Authenticate\n");
        printf("ErrorID = %d ErrorMsg = %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        return;
    }
    
    reqUserLogin();
}

///��¼������Ӧ
void Trader::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
    {
        printf("Failed to User Login\n");
        printf("ErrorID = %d, ErrorMsg : %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        return;
    }
    
    reqQryInstrument();
}

///�����ѯ��Լ��Ӧ
void Trader::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
    {
        printf("Failed to Qry Instrument\n");
        printf("ErrorID = %d, ErrorMsg : %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        return;
    }
    if (pInstrument->ProductClass == THOST_FTDC_PC_Futures)
    {
        inst_vec.push_back(pInstrument->InstrumentID);
    }

    if (bIsLast)
    {
        send2WeCom("Qry Instrument Done");
        bIsReady.store(true, std::memory_order_release);
    }
}
