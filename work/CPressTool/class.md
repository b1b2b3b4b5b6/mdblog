# class
```C++
class CPressTool : public CThreadBase
{
public:
    CPressTool(STPressToolParam stPressTool, std::vector<STUserInfo> vecUserGroup, STEntrustInfo& stEntrustInfo);
    virtual ~CPressTool(void);

    // CPressTool线程实例初始化
    //virtual bool InitInstance();
    virtual void Run();
    virtual void ExitInstance();

    // CPressTool对象初始化
    bool Init();

private:
    bool LogInit();
    void DoUserLogin();
    void DoEntrust();
    void DoQry();
    void DoUpdatePassWord();
    void TimeConsumingStatis();

private:
	//每个用户发送一次委托，将原来委托全部推送回
    void DoDummyEntrust();

public:
    CPressTool* m_ptrCPressToolhandle;
    CLogFile m_PressToolLogFile;

    int m_nUserLoginStatus;
    int m_nRtnOrderStatus;
    long long *m_pTimeBegin;
    long long *m_pTimeEnd;
    int m_nLogLevel;

    std::vector< STUserInfoComb> m_vecUserInfoComb;
    std::map<int, int> m_mapReqId2SeqNum;

    
private:
    FTS::FTSTraderAPI* m_ptrFTSTraderAPI;

    unsigned int m_nReqId;
    int m_nOrderSum;

    int m_nSpeed;
    int m_nTime;
    int m_nCaclStep;
    int m_nIndex;
    std::string m_sConfFileName;

    long long m_usec;
    double m_sec;
    double m_avg_pkt;

    std::vector<STUserInfo> m_vecUserLoginInfo;
    std::vector<FTS::ReqSingleOrderInsertField> m_vecOrderReq;
    std::vector<FTS::ReqQrySingleOrderField> m_vecOrderQry;
    std::vector<FTS::ReqQryTradeField> m_vecTradeQry;
    std::vector<FTS::ReqQryInvestorPositionField> m_vecPositionQry;

    int m_nThreadQty;
    std::map<int, int> m_mapLoginedCust;

public:
	//将原来委托全部推送，需要等待推送完成
    bool m_tricky;
};

```