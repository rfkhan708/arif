#ifndef STRATEGYCLASS_H
#define STRATEGYCLASS_H
#include "Enums.h"



using namespace Enums;

class StrategyClass
{
    public:
        int UID;

        StrategyClass();
        virtual ~StrategyClass();
        NearMonthPacket _NMPack;
    void OnSubscriptionEventHandler(strFOPAIRIOC* _FOpairObj);
    void OnUnSubscriptionEventHandler(strFOPAIRIOC* _FOpairObj);
    void OnDataEventHandler(FinalPrice _fp);
    void OnDifferenceEventHandler(FOPAIRDIFFIOC* _INpairDiff);
    void BOARD_LOT_IN_TRSell(int FarTokenNo,BUYSELL BS, int qty, int price, PFHolder _PF);
    void BOARD_LOT_IN_TRBuy(int FarTokenNo,BUYSELL BS ,int qty, int price, PFHolder _PF);

    void ORDER_MOD_IN_TR(int TokenNo, int price,BUYSELL buySell);
    void ORDER_CANCEL_IN_TR(int TokenNo, BUYSELL buySell) ;
    int ClientID;

    PFHolder _pfBuy;
    PFHolder _pfSell;

    PFHolder _pfMKTBuy;
    PFHolder _pfMKTSell;

    PFHolder _pfMKTBuy2;
    PFHolder _pfMKTSell2;

    OrderStatus BuyFarStatus;
    OrderStatus SellFarStatus;

bool CancelBuyOrder;
bool CancelSellOrder;
int BLQ;

void ORDER_CONFIRMATION_TR (char *buffer) ;
void ORDER_CXL_CONFIRMATION_TR (char *buffer) ;
void ORDER_MOD_CONFIRMATION_TR (char *buffer) ;
void TRADE_CONFIRMATION_TR (char *buffer) ;

 void ORDER_CXL_REJ_OUT (char *buffer);

 void ORDER_MOD_REJ_OUT (char *buffer);
 void ORDER_ERROR_OUT (char *buffer);
    int StrategyClass::GetExpectedProdPrice(  BUYSELL BS, FinalPrice FP, int Ratio,bool reverse = false);
    int StrategyClass::GetExpectedBidPrice(BUYSELL BS,FinalPrice FP,int Ratio,bool Create= false);
    int GetBestMktPrime(BUYSELL BS);
    protected:
    FinalPrice FarFP;
    FinalPrice NearFP;
    FinalPrice ThirdFP;

    TokenPartnerDetails FarDetails;
    TokenPartnerDetails NearDetails;

    short BuyTradeCounter;
    short SellTradeCounter;


    short BuyTradeCounterleg2;
    short SellTradeCounterleg2;

    short BuyTradeCounterleg3;
    short SellTradeCounterleg3;

    short PFNumber;

    int _BNSFMNQ ;
	int _BFSNMNQ ;
	int _BNSFMXQ ;
	int _BFSNMXQ ;
	int BFSNDIFF ;
	int BNSFDIFF ;
    int InstanceGroup;


    short  CancelCode;
	short  ModificationCode;

    long long concat(long long x, long long y);


    strFOPAIRIOC FOpairObj;

    BUYSELL CreateSide;
    BUYSELL ReverseSide;
    BUYSELL _TradeCreateSide;
    BUYSELL _TradeReverseSide;

    pthread_mutex_t count_mutex;
pthread_mutex_t initbuy_mutex;
pthread_mutex_t initsell_mutex;


   int GetBestData(BUYSELL BS);
    int GetMKTData(BUYSELL BS);
    int GetMKTDataLeg3(BUYSELL BS);
    int GetMKTDataLeg2(BUYSELL BS);
    int GetDepthPrice(BUYSELL BS, int DIFF,BUYSELL FirstLeg);
    int GetMKTDataPrime(BUYSELL BS);
    int MultipleValue(BUYSELL BS);
    private:


StructHash _shBuy;
StructHash _shSell;

StructHash _shMKTBuy;
StructHash _shMKTSell;

StructHash _shMKTBuy2;
StructHash _shMKTSell2;

int Token1Ratio;
int Token2Ratio;
int Token3Ratio;

int Token1MinCreateRatio;
int Token2MinCreateRatio;
int Token3MinCreateRatio;

int Token1MaxCreateRatio;
int Token2MaxCreateRatio;
int Token3MaxCreateRatio;

int Token1MinReverseRatio;
int Token2MinReverseRatio;
int Token3MinReverseRatio;

int Token1MaxReverseRatio;
int Token2MaxReverseRatio;
int Token3MaxReverseRatio;

int _CreateRemainingQty;
int _ReverseRemainingQty;

float Leg2HitRatio;
float Leg3HitRatio;


};

#endif // STRATEGYCLASS_H
