#ifndef TRADER_H
#define TRADER_H

#include "ThostFtdcTraderApi.h"
#include "common.h"

#include <atomic>

class Trader : public CThostFtdcTraderSpi
{
public:
	Trader();
	~Trader();

public:
	void reqAuthenticate();//�ͻ�����֤����
	void reqUserLogin();//�û���¼����
	void reqQryInstrument();//�����ѯ��Լ

public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ(��δ��¼ǰ)���÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///�ͻ�����֤��Ӧ
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

public:
	std::vector<std::string> inst_vec;
	std::atomic<bool> bIsReady{ false };

public:
	BrokerInfo bi;
	CThostFtdcTraderApi* tdapi;
	int nReqID = 0;
};

#endif
