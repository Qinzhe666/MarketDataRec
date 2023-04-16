#include "record.h"

record::record() : td_ptr(nullptr), md_ptr(nullptr)
{
    td_ptr = new Trader();
    while (!td_ptr->bIsReady.load(std::memory_order_acquire)) {}

    md_ptr = new MarketData(td_ptr->inst_vec);

    td_ptr->tdapi->Release(); delete td_ptr;

	start();
}

record::~record()
{
    delete md_ptr;
}

void record::start()
{
	auto& md = md_ptr->md;
	CThostFtdcDepthMarketDataField t{};
	while (true)
	{
		if (md.remove(t))
		{
            static auto day = md_ptr->mdapi->GetTradingDay();
            char fileName[64] = { '\0' };
            sprintf(fileName, "./%s/%s_%s.csv", day, t.InstrumentID, day);
            std::ofstream outFile;
            outFile.open(fileName, std::ios::app); // �ļ�׷��д��
            outFile << t.InstrumentID << ","///����������
                << t.ExchangeID << ","///��Լ����
                << t.TradingDay << ","///������
                << t.UpdateTime << ","///����޸�ʱ��
                << t.UpdateMillisec << ","///����޸ĺ���
                << t.PreSettlementPrice << ","///�ϴν����
                << t.PreClosePrice << ","///������
                << t.PreOpenInterest << ","///��ֲ���
                << t.OpenPrice << ","///����
                << t.UpperLimitPrice << ","///��ͣ���
                << t.LowerLimitPrice << ","///��ͣ���
                << t.HighestPrice << ","///��߼�
                << t.LowestPrice << ","///��ͼ�
                << t.LastPrice << ","///���¼�
                << t.Volume << ","///����
                << t.Turnover << ","///�ɽ����
                << t.OpenInterest << ","///�ֲ���
                << t.BidPrice1 << ","///�����һ
                << t.BidVolume1 << ","///������һ
                << t.AskPrice1 << ","///������һ
                << t.AskVolume1 << ","///������һ
                << t.BidPrice2 << ","///����۶�
                << t.BidVolume2 << ","///��������
                << t.AskPrice2 << ","///�����۶�
                << t.AskVolume2 << ","///��������
                << t.BidPrice3 << ","///�������
                << t.BidVolume3 << ","///��������
                << t.AskPrice3 << ","///��������
                << t.AskVolume3 << ","///��������
                << t.BidPrice4 << ","///�������
                << t.BidVolume4 << ","///��������
                << t.AskPrice4 << ","///��������
                << t.AskVolume4 << ","///��������
                << t.BidPrice5 << ","///�������
                << t.BidVolume5 << ","///��������
                << t.AskPrice5 << ","///��������
                << t.AskVolume5///��������
                << std::endl;
            outFile.close();
        }
	}
}
