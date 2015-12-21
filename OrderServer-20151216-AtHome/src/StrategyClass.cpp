#include "StrategyClass.h"
#include "string.h"

#include "StaticClass.h"
#include <arpa/inet.h>
#include "../PFMap.h"
//#include <boost/thread/mutex.hpp>
//#include<boost/thread/condition_variable.hpp>

#include <pthread.h>
#include <math.h>

using namespace std;




StrategyClass::StrategyClass()
{
    //ctor
            BuyFarStatus= (OrderStatus)NONE;
            SellFarStatus = (OrderStatus)NONE;

            SellTradeCounter=0;
            BuyTradeCounter=0;

            SellTradeCounterleg2 =0;
            SellTradeCounterleg3 =0;

            BuyTradeCounterleg2 =0;
            BuyTradeCounterleg3 =0;




            memset(&FarFP,0,sizeof(FinalPrice));
            memset(&NearFP,0,sizeof(FinalPrice));
            memset(&ThirdFP,0,sizeof(FinalPrice));

            CancelCode=ntohs(20070);
            ModificationCode=ntohs(20040);

            memset(&_pfBuy,0,sizeof(PFHolder));
            memset(&_pfSell,0,sizeof(PFHolder));
            memset(&_shBuy,0,sizeof(StructHash));
            memset(&_shSell,0,sizeof(StructHash));

            memset(&_pfMKTBuy,0,sizeof(PFHolder));
            memset(&_pfMKTSell,0,sizeof(PFHolder));
            memset(&_shMKTBuy,0,sizeof(StructHash));
            memset(&_shMKTSell,0,sizeof(StructHash));

            memset(&_pfMKTBuy2,0,sizeof(PFHolder));
            memset(&_pfMKTSell2,0,sizeof(PFHolder));
            memset(&_shMKTBuy2,0,sizeof(StructHash));
            memset(&_shMKTSell2,0,sizeof(StructHash));

            _BNSFMNQ=0;
            _BFSNMNQ =0;
            _BNSFMXQ =0;
            _BFSNMXQ =0;
            BFSNDIFF =0;
            BNSFDIFF =0;
            InstanceGroup=0;

        count_mutex = PTHREAD_MUTEX_INITIALIZER;
        initbuy_mutex = PTHREAD_MUTEX_INITIALIZER;
        initsell_mutex = PTHREAD_MUTEX_INITIALIZER;

        memset(&FarFP,0,sizeof(FinalPrice));
        memset(&NearFP,0,sizeof(FinalPrice));
        memset(&ThirdFP,0,sizeof(FinalPrice));
        BLQ=0;

        Token1MinCreateRatio = 0;
                Token2MinCreateRatio = 0;
                Token3MinCreateRatio = 0;

                Token1MinReverseRatio = 0;
                Token2MinReverseRatio = 0;
                Token3MinReverseRatio = 0;

                Token1MaxCreateRatio = 0;
                Token2MaxCreateRatio = 0;
                Token3MaxCreateRatio = 0;

                Token1MaxReverseRatio = 0;
                Token2MaxReverseRatio = 0;
                Token3MaxReverseRatio = 0;
                _CreateRemainingQty=0;
                _ReverseRemainingQty =0 ;
}

StrategyClass::~StrategyClass()
{
    //dtor
}

long long StrategyClass::concat(long long x, long long y)
{
    long long temp = y;
    while (y != 0) {
        x *= 10;
        y /= 10;
    }
    return x + temp;
}

 void StrategyClass::OnSubscriptionEventHandler(strFOPAIRIOC* _FOpairObj)
        {
            FOpairObj = *_FOpairObj;
           // strFOPAIR _FOpairObj;
           // memcpy(&_FOpairObj,buffer,sizeof(_FOpairObj));
            FarFP.Token = _FOpairObj->TokenFar;
            NearFP.Token = _FOpairObj->TokenNear;
            ThirdFP.Token = _FOpairObj->TokenThree;
            InstanceGroup = _FOpairObj->INSTANCEGROUP;

            Token1Ratio = _FOpairObj->TokenFarRatio;
            Token2Ratio = _FOpairObj->TokenNearRatio;
            Token3Ratio = _FOpairObj->TokenThreeRatio;

            Leg2HitRatio = (float)Token2Ratio / (float)Token1Ratio;
            Leg3HitRatio =(float) Token3Ratio / (float) Token1Ratio;

            PFNumber = _FOpairObj->PORTFOLIONAME;

            if(BuyTradeCounter <=0)
                BuyTradeCounter = _FOpairObj->CreateQTY;
            if(SellTradeCounter <=0)
                    SellTradeCounter = _FOpairObj->ReverseQTY;
           // cout << "PFNumber "<< PFNumber <<" New class OnSub PF " << PFNumber << " Far " << FarFP.Token << " Near "<< NearFP.Token<<endl<<endl;

            _shBuy.Token1 = FarFP.Token;
            _shBuy.side1 = BUY;

            _shSell.Token1 = FarFP.Token;
            _shSell.side1 = SELL;

             _pfBuy.PF = PFNumber;
            _pfBuy.CID = ClientID;

             _pfSell.PF = PFNumber;
            _pfSell.CID = ClientID;

            _shMKTBuy.Token1 = NearFP.Token;
            _shMKTBuy.side1 = BUY;

            _shMKTSell.Token1 = NearFP.Token;
            _shMKTSell.side1 = SELL;

             _pfMKTBuy.PF = PFNumber;
            _pfMKTBuy.CID = ClientID;

             _pfMKTSell.PF = PFNumber;
            _pfMKTSell.CID = ClientID;


            _shMKTBuy2.Token1 = ThirdFP.Token;
            _shMKTBuy2.side1 = BUY;

            _shMKTSell2.Token1 = ThirdFP.Token;
            _shMKTSell2.side1 = SELL;

             _pfMKTBuy2.PF = PFNumber;
            _pfMKTBuy2.CID = ClientID;

             _pfMKTSell2.PF = PFNumber;
            _pfMKTSell2.CID = ClientID;


            long lng=concat((short)(MessageType)eORDER,_pfSell.CID);
            lng = concat(lng,(short)_pfSell.PF);

            _pfBuy.SubscriptionTag = lng;
            _pfSell.SubscriptionTag = lng;

            _pfMKTBuy.SubscriptionTag = lng;
            _pfMKTSell.SubscriptionTag = lng;

            _pfMKTBuy2.SubscriptionTag = lng;
            _pfMKTSell2.SubscriptionTag = lng;

            _BNSFMNQ=0;
            _BFSNMNQ =0;
            _BNSFMXQ =0;
            _BFSNMXQ =0;
            BFSNDIFF =0;
            BNSFDIFF =0;

            CancelBuyOrder = false;
	 	 	CancelSellOrder= false;

        }

        void StrategyClass::OnUnSubscriptionEventHandler(strFOPAIRIOC* _FOpairObj)
        {
            //strFOPAIR _FOpairObj;

	 	 	// memcpy(&_FOpairObj,buffer,sizeof(_FOpairObj));

            if(BuyFarStatus==OPEN || BuyFarStatus== REPLACED || BuyFarStatus == PENDING)
            {
                BuyFarStatus = PENDING;

                cout<<"StrategyClass OnUnSubscriptionEventHandler BUY "<<(BUYSELL)BUY<<" FarFP.Token ="<<FarFP.Token<<endl<<endl;
                ORDER_CANCEL_IN_TR ( FarFP.Token, CreateSide);
            }
            if(SellFarStatus == OPEN || SellFarStatus == REPLACED|| SellFarStatus == PENDING)
            {
                SellFarStatus = PENDING;


                cout<<"StrategyClass OnUnSubscriptionEventHandler SELL \t"<<(BUYSELL)SELL<<" FarFP.Token "<<FarFP.Token<<endl<<endl;
                ORDER_CANCEL_IN_TR ( FarFP.Token, ReverseSide);

            }


            _BNSFMNQ=0;
            _BFSNMNQ =0;
            _BNSFMXQ =0;
            _BFSNMXQ =0;
            BFSNDIFF =0;
            BNSFDIFF =0;

            BuyFarStatus=NONE;
            SellFarStatus=NONE;
            CancelBuyOrder = true;
            CancelSellOrder= true;

            cout << " Unsubscription recieved in Strategy for CID " << ClientID << " PF "<< PFNumber<<endl;
       // cout << "PFNumber "<< PFNumber <<" New class OnUnSub PF " << PFNumber << " Far " << _FOpairObj->TokenFar << " Near "<< _FOpairObj->TokenNear<<endl<<endl;
        }

    int StrategyClass::GetExpectedProdPrice(BUYSELL BS, FinalPrice FP, int Ratio,bool reverse = false)
	{

			int RetVal = 0;

			if (!reverse)
			{
				// THis case calculates the price to generate buy spread
				RetVal = BS== (BUYSELL)BUY ? (FP.MINASK*Ratio* -1) : (FP.MAXBID*Ratio);
			}
			else
			{
				// Here in case of sale actual stg with buy mode token will be sold just to make a complete trade
				RetVal = BS == (BUYSELL)BUY ? (FP.MAXBID*Ratio ) : (FP.MINASK*Ratio * -1);
			}
			return RetVal;

	}



    int StrategyClass::GetExpectedBidPrice(BUYSELL BS,FinalPrice FP,int Ratio,bool Create= false)
    {
        int RetVal=0;

        if(Create)
        {
            RetVal = BS == (BUYSELL)BUY ? (FP.MAXBID * Ratio * -1) : (FP.MINASK * Ratio );

        }
        else
        {
            RetVal = BS == (BUYSELL)BUY ? (FP.MINASK * Ratio ) :  (FP.MAXBID * Ratio * -1 );

        }

    }


    int StrategyClass::MultipleValue(BUYSELL BS)
    {
        return BS == BUY ? -1 : 1;
    }

    int StrategyClass::GetBestData(BUYSELL BS)
    {
        return BS== BUY ? FarFP.MAXBID + 5: FarFP.MINASK -5 ;
    }

    int StrategyClass::GetDepthPrice(BUYSELL BS, int DIFF,BUYSELL FirstLeg)
    {
        int x=0;///FirstLeg Price
        int y=GetMKTData(BS);///Second Leg Price
        int z = DIFF;///DIFF
        if(BS==SELL && FirstLeg ==BUY)
            x = y + abs(z);
        else if(BS==BUY && FirstLeg ==SELL)
            x = y + abs(z);
        else if(BS==BUY && FirstLeg ==BUY)
            x =  y * -1 + abs(z);
        else if(BS==SELL && FirstLeg ==SELL)
            x = y - z;
       // return  abs(GetMKTData(BS) + DIFF) ;
       return abs(x);
    }

    int StrategyClass::GetMKTData(BUYSELL BS)
    {
        return BS == BUY ? NearFP.MINASK  : NearFP.MAXBID ;
    }

    int StrategyClass::GetMKTDataLeg2(BUYSELL BS)
    {
        return BS == BUY ? NearFP.MINASK  : NearFP.MAXBID * -1 ;
    }


    int StrategyClass::GetMKTDataLeg3(BUYSELL BS)
    {
        return BS == BUY ? ThirdFP.MINASK  : ThirdFP.MAXBID * -1 ;
    }

    int StrategyClass::GetMKTDataPrime(BUYSELL BS)
    {
        return BS == BUY ? FarFP.MINASK  : FarFP.MAXBID ;
    }

    int StrategyClass::GetBestMktPrime(BUYSELL BS)
    {
        return BS == BUY ? -5  : 5 ;
    }

        void StrategyClass::OnDataEventHandler(FinalPrice _fp)
        {

          //  boost::mutex::scoped_lock lock(the_inmutex);
          //  cout << " New class OnDataArrived PF "<< PFNumber <<" Token "<< _fp.Token << " Bid "<< _fp.MAXBID << " Ask "<< _fp.MINASK<<endl<<endl;
          //  cout << " FarFP Token "<< FarFP.Token << " NearFP Token "<< NearFP.Token<<endl<<endl;
            if(FarFP.Token==0 && NearFP.Token==0)
            return;

             pthread_mutex_trylock(&count_mutex);

            if(_fp.MAXBID > 0 && _fp.MINASK >0)
            {
                if(FarFP.Token== _fp.Token)
                {
                    FarFP = _fp;
                }
                else if(NearFP.Token == _fp.Token)
                {
                   NearFP = _fp;
                }
                else if(ThirdFP.Token == _fp.Token)
                {

                   ThirdFP = _fp;
                }
            }
            else
            {
                cout << " Error in rate "<<endl;
            }
           // cout <<" FarFP.MINASK "<<FarFP.MINASK << " FarFP.MAXBID "<<FarFP.MAXBID << " NearFP.MAXBID " << NearFP.MAXBID << " NearFP.MINASK " <<NearFP.MINASK <<endl<<endl;

            if(FarFP.MINASK > 0 && FarFP.MAXBID >0 && NearFP.MAXBID >0 && NearFP.MINASK >0 && BLQ>0 )//&&  FarFP.MAXBID < FarFP.MINASK && NearFP.MAXBID < NearFP.MINASK)
            {
                if(ThirdFP.Token >0 && ThirdFP.MINASK <= 0 && ThirdFP.MAXBID<=0)
                return;

                int secondbuyProd = GetExpectedProdPrice((BUYSELL)FOpairObj.TokenNearSide, NearFP, FOpairObj.TokenNearRatio); // Market rate for Leg2

                int firstbuyProd = InstanceGroup == Best ? GetExpectedBidPrice((BUYSELL)FOpairObj.TokenFarSide, FarFP, FOpairObj.TokenFarRatio,true)
                 : GetExpectedProdPrice((BUYSELL)FOpairObj.TokenFarSide, FarFP, FOpairObj.TokenFarRatio) ; //Best rate for Leg1

                //int firstbuyProd = GetExpectedBidPrice((BUYSELL)FOpairObj.TokenFarSide, FarFP, FOpairObj.TokenFarRatio,true);

                int thirdbuyprod =ThirdFP.Token>0?GetExpectedProdPrice((BUYSELL)FOpairObj.TokenThreeSide, ThirdFP, FOpairObj.TokenThreeRatio):0;


    //int secondbuyProd = GetExpectedBidPrice((BUYSELL)_FOPair.Token2Side, FarFP, _FOPair.Token2Ratio,true);

   // int thirdbuyProd = ThirdToken > 0 ? GetExpectedProdPrice((BUYSELL)_NFToken._OptTokens.Token3side, ThirdFP, _NFToken._OptTokenDets.Token3Ratio)  : 0;
   // int fourthbuyProd = FourthToken > 0 ? GetExpectedProdPrice((BUYSELL)_NFToken._OptTokens.Token4side, FourthFP, _NFToken._OptTokenDets.Token4Ratio)  : 0;



            //cout << "firstbuyProd "<< firstbuyProd << " secondbuyProd "<<secondbuyProd << " thirdbuyprod "<<thirdbuyprod<<endl;


    //int firstsellProd = GetExpectedProdPrice((BUYSELL)_FOPair.Token1Side, NearFP, _FOPair.Token1Ratio,true);
    int secondsellProd = GetExpectedProdPrice((BUYSELL)FOpairObj.TokenNearSide,NearFP , FOpairObj.TokenNearRatio,true); //Market rate for leg2

    int firstsellProd = InstanceGroup == Best ? GetExpectedBidPrice((BUYSELL)FOpairObj.TokenFarSide, FarFP, FOpairObj.TokenFarRatio,false)
                        :  GetExpectedProdPrice((BUYSELL)FOpairObj.TokenFarSide,FarFP , FOpairObj.TokenFarRatio,true); // Best Rate for Leg1

   // int firstsellProd = GetExpectedBidPrice((BUYSELL)FOpairObj.TokenFarSide, FarFP, FOpairObj.TokenFarRatio,false);

    int thirdsellprod = ThirdFP.Token>0?GetExpectedProdPrice((BUYSELL)FOpairObj.TokenThreeSide,ThirdFP , FOpairObj.TokenThreeRatio,true):0;


 //cout << "firstsellProd "<< firstsellProd << " secondsellProd "<<secondsellProd << " thirdsellprod "<<thirdsellprod<<endl;


    int dAsksDifference = firstsellProd+secondsellProd +thirdsellprod;// + thirdsellProd + fourthsellProd;
    int dBidsDifference = firstbuyProd + secondbuyProd +thirdbuyprod;// + fourthbuyProd;


          //  cout <<"PFNumber "<< PFNumber <<" dAsksDifference " << dAsksDifference<< " dBidsDifference "<< dBidsDifference <<endl; //" SellFarStatus " << SellFarStatus << " SellTradeCounter "<<SellTradeCounter<<endl<<endl;

            CreateSide = FOpairObj.TokenFarSide;
            ReverseSide = CreateSide == BUY ? SELL:BUY;

if(!CancelSellOrder)
{

   // cout << " PF "<<PFNumber << "Reverse secondsellProd "<< secondsellProd<< "  UD "<< BNSFDIFF << " Side "<< ReverseSide <<endl;


            if((SellFarStatus==(OrderStatus)TRADE || SellFarStatus==(OrderStatus)CANCEL || SellFarStatus==(OrderStatus)REJECTED || SellFarStatus==(OrderStatus)NONE) &&
               BNSFDIFF !=0  && _BNSFMNQ > 0 && _BNSFMXQ > 0 && SellTradeCounter < (Token1MaxReverseRatio - 1 )  && !CancelSellOrder )// abs(BNSFDIFF) <= abs(dAsksDifference) &&
            {
                int dFarMonthSellRate = 0;

                if(InstanceGroup == Best)
                {
                    if (abs(BNSFDIFF) < abs(dAsksDifference))
                    {
                        dFarMonthSellRate =  GetBestData(ReverseSide) ;
                    }
                    else if (abs(BNSFDIFF) == abs(dAsksDifference ))
                    {
                        dFarMonthSellRate = GetMKTData(FOpairObj.TokenNearSide == BUY ? SELL :BUY) + abs(BNSFDIFF);
                    }
                }
                else if(InstanceGroup == Depth )
                {
                   /* int diff = BNSFDIFF ;// (BNSFDIFF)  -(dAsksDifference) ;
                    int mktprice =    //  GetMKTDataPrime(ReverseSide);

                    cout << " PF "<< PFNumber<<" MD " << dAsksDifference << " UD "<< BNSFDIFF  <<
                     " Reversediff "<< diff<< " MktPrice "<<mktprice<< " Bid "<< FarFP.MAXBID <<
                     " Ask "<< FarFP.MINASK<< " secondsellProd "<< ((float)secondsellProd / (float)Token2Ratio)<< endl;
                */
                 /*   if( ReverseSide == BUY &&  abs(diff + mktprice) <= GetBestData(ReverseSide) ||
                     ReverseSide == SELL &&  abs(diff + mktprice) >= GetBestData(ReverseSide))
                        {*/
                            dFarMonthSellRate =abs( GetMKTDataLeg2(FOpairObj.TokenNearSide == BUY ? SELL :BUY)
                            + GetMKTDataLeg3(FOpairObj.TokenThreeSide == BUY ? SELL :BUY)
                            + BNSFDIFF);//abs( diff  + mktprice);   //GetDepthPric(FOpairObj.TokenNearSide == BUY ? SELL :BUY,BNSFDIFF,ReverseSide) ;
                        //}
                    //cout << " diff " << diff<< " mktprice "<< mktprice<<endl;
                }
             //  cout << "PF "<< PFNumber<< " ClientID "<< ClientID <<" dFarMonthSellRate " << dFarMonthSellRate << " FarFP.MINASK "  <<FarFP.MINASK<<endl<<endl;

             /*  if(dFarMonthSellRate == FarFP.MINASK)
               {
                cout << " Error "<<endl;
               }*/

                if(dFarMonthSellRate > GetBestData(CreateSide) )//FarFP.MAXBID)
                {

                    pthread_mutex_lock(&initsell_mutex);


                   SellFarStatus = (OrderStatus)PENDING;
                   SellTradeCounterleg2=0;
                   SellTradeCounterleg3=0;
                   //_FillData.producer(new FinalPrice());
                   // _FillData.producer((char*)new FinalPrice());

                  /*  if(BLQ <= 0)
                    {
                        cout << " error "<<endl;
                    }
                    if(_BNSFMNQ * BLQ<=0)
                    {
                        cout << " Error "<<endl;
                    }*/

                    _shSell.Qty1= min(Token1MinReverseRatio, ( Token1MaxReverseRatio - SellTradeCounter )) * BLQ ;   //Token1Ratio * _BNSFMNQ * BLQ;
                    _shSell.Price1=dFarMonthSellRate;
                    _shSell.side1=ReverseSide;
                    //_pfSell.HashKey = MyHash<StructHash>()(_shSell);
                    _pfSell.side =(short)ReverseSide;
                    BOARD_LOT_IN_TRSell (FarFP.Token,_pfSell.side,
                                	ntohl(_shSell.Qty1),
                                    ntohl(dFarMonthSellRate),_pfSell);

                   // cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA PFnumber " <<  PFNumber << " Sell bid @ "<< dFarMonthSellRate<<  " Token "<< _shSell.Token1<< " Side "<< _shSell.side1
                   // << " Qty "<< _shSell.Qty1 << " Price "<< _shSell.Price1 <<  " Hashkey "<< _pfSell.HashKey<<" AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<< endl<<endl;

                   /* cout << " New PF "<< PFNumber << " InstanceGroup "<< InstanceGroup<< " Token " <<  FarFP.Token << " Reverse UD "<< BNSFDIFF << " MD "
                    << dAsksDifference << " Price generated "<< dFarMonthSellRate << " firstsellProd "
                    <<firstsellProd << " secondsellProd "<< secondsellProd << " thirdsellprod "<<thirdsellprod<< " Bid "<< FarFP.MAXBID<< " ASK "<< FarFP.MINASK << " GetMKTDataPrime(ReverseSide) " <<GetMKTDataPrime(ReverseSide)<<endl;
*/
                    pthread_mutex_unlock(&initsell_mutex);



                }
                else
                {
                    //cout <<"PFNumber \t"<< PFNumber << "\t BNSF \t"<< "New Order Sell price calculated less than Bid Calculated Price :- "<<dFarMonthSellRate<<endl<<endl;
                }

            }

            else if((SellFarStatus == (OrderStatus)OPEN || SellFarStatus == (OrderStatus)REPLACED ))//&& abs(dAsksDifference) >= abs(BNSFDIFF) && !CancelSellOrder)
			{
                int dQuoteRate;

                if(InstanceGroup == Best )
                {
                    if (abs(BNSFDIFF) < abs(dAsksDifference) && !CancelSellOrder)
                    {
                        dQuoteRate =  GetBestData(ReverseSide);
                    }
                    else if (abs(BNSFDIFF) == abs(dAsksDifference ) && !CancelSellOrder)
                    {
                        dQuoteRate = GetMKTData(FOpairObj.TokenNearSide == BUY ? SELL :BUY) + abs(BNSFDIFF);
                    }
                    else if(abs(dAsksDifference) < abs(BNSFDIFF) || CancelSellOrder)
                    {
                        SellFarStatus = (OrderStatus)PENDING;
                        ORDER_CANCEL_IN_TR ( FarFP.Token, ReverseSide);
                    }
                }
                else if(InstanceGroup == Depth  )
                {
                   /* int diff =  (BNSFDIFF)  -(dAsksDifference);
                    int mktprice = GetMKTDataPrime(ReverseSide);// secondsellProd * -1;//GetMKTDataPrime(ReverseSide);
                    cout << " PF "<< PFNumber<<" MD " << dAsksDifference << " UD "<< BNSFDIFF  <<
                    " Reverse diff "<< diff<< " MktPrice "<<mktprice<<" Bid "<< FarFP.MAXBID <<
                    " Ask "<< FarFP.MINASK<< " secondsellProd "<< ((float)secondsellProd/(float) Token2Ratio) << endl;*/
                   // if( ReverseSide == BUY &&  (diff + mktprice) > GetBestData(ReverseSide) ||
                    //    ReverseSide == SELL &&  (diff + mktprice) < GetBestData(ReverseSide)
                    //    || CancelSellOrder)

                  /*  if( ReverseSide == BUY &&  abs(diff + mktprice) >= GetBestData(CreateSide) ||
                        ReverseSide == SELL &&  abs(diff + mktprice) <= GetBestData(CreateSide)
                        || CancelSellOrder)*/
                   /* if(abs(dAsksDifference) < abs(BNSFDIFF))// || CancelSellOrder)
                    {
                       // SellFarStatus = (OrderStatus)PENDING;
                       // ORDER_CANCEL_IN_TR ( FarFP.Token, ReverseSide);

                       dQuoteRate = GetBestData(ReverseSide);

                   // cout << " PF "<< PFNumber << " Cancellation in Depth Reverse Price "<< abs(diff + mktprice) <<endl;
                    }

                   else */
                   if(!CancelSellOrder )
                    {

                        dQuoteRate =abs(GetMKTDataLeg2(FOpairObj.TokenNearSide == BUY ? SELL :BUY)
                        +   GetMKTDataLeg3(FOpairObj.TokenThreeSide == BUY ? SELL :BUY)+ BNSFDIFF);// abs (diff + mktprice);

                        if(dQuoteRate <= GetMKTDataPrime(ReverseSide))
                            cout << "PF " << PFNumber <<"  Reverse TradeHit Leg 1 @  " << dQuoteRate << " Leg2 @ "  << GetMKTData(FOpairObj.TokenNearSide == BUY ? SELL :BUY)
                            <<     " Leg3 @ " << abs(GetMKTDataLeg3(FOpairObj.TokenThreeSide == BUY ? SELL :BUY))
                              << " Trade Diff " << dQuoteRate - (GetMKTDataLeg2(FOpairObj.TokenNearSide == BUY ? SELL :BUY) +
                            GetMKTDataLeg3(FOpairObj.TokenThreeSide == BUY ? SELL :BUY))  <<endl;

                       // cout << " Reverse  dQuoteRate "<< dQuoteRate<<endl;
                        //cout << " diff " << diff<< " mktprice "<< mktprice<<endl;
                    }
                    else if(CancelSellOrder)
                    {
                        SellFarStatus = (OrderStatus)PENDING;
                        ORDER_CANCEL_IN_TR ( FarFP.Token, ReverseSide);
                    }
                 //   dQuoteRate =((BNSFDIFF) -(dAsksDifference)  ) + GetMKTDataPrime(ReverseSide); //GetDepthPrice(FOpairObj.TokenNearSide == BUY ? SELL :BUY,BNSFDIFF,ReverseSide) ;
                }

                if(dQuoteRate > GetBestData(CreateSide) && SellFarStatus!= PENDING && dQuoteRate > 0)
                {
                    SellFarStatus = (OrderStatus)PENDING;
                    ORDER_MOD_IN_TR (FarFP.Token,
									          //      ntohl( OrdDetailSell.Qty *BLQ),
									 ntohl(dQuoteRate),ReverseSide
									);

							/*		cout << "Mod PF "<< PFNumber << " InstanceGroup "<< InstanceGroup<< " Token " <<  FarFP.Token << " Reverse UD "<< BNSFDIFF << " MD "
                    << dAsksDifference << " Price generated "<< dQuoteRate << " firstsellProd "
                    <<firstsellProd << " secondsellProd "<< secondsellProd << " thirdsellprod "<<thirdsellprod<< " Bid "<< FarFP.MAXBID<< " ASK "<< FarFP.MINASK <<  " GetMKTDataPrime(ReverseSide) " <<GetMKTDataPrime(ReverseSide) <<endl;*/
                   // cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA Modified Sell PF "<< PFNumber << " AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<endl<<endl;
                }

            }

        }

         //   cout <<"PFNumber"<< PFNumber << " dAsksDifference " << dAsksDifference<< " dBidsDifference "<< dBidsDifference << " BuyFarStatus " << BuyFarStatus << " BuyTradeCounter "<<BuyTradeCounter<<endl<<endl;
if(!CancelBuyOrder)
{

//cout << " PF "<<PFNumber << "Create secondbuyProd "<< secondbuyProd<< "  UD "<< BFSNDIFF << " Side "<< CreateSide <<endl;

             if((BuyFarStatus==(OrderStatus)TRADE || BuyFarStatus==(OrderStatus)CANCEL || BuyFarStatus==(OrderStatus)REJECTED || BuyFarStatus==(OrderStatus)NONE) &&
               BFSNDIFF !=0  && _BFSNMNQ > 0 && _BFSNMXQ > 0 && BuyTradeCounter < (Token1MaxCreateRatio -1)    && !CancelBuyOrder) //&& abs(BFSNDIFF) >= abs(dBidsDifference)
            {
                int dFarMonthBuyRate = 0;

                if(InstanceGroup == Best )
                {

                    if (abs(BFSNDIFF) > abs(dBidsDifference))
                    {
                        dFarMonthBuyRate = GetBestData(CreateSide) ;
                    }
                    else if (abs(BFSNDIFF) == abs(dBidsDifference ))
                    {
                        dFarMonthBuyRate = GetMKTData(FOpairObj.TokenNearSide) + abs(BFSNDIFF);
                    }
                }
                else if(InstanceGroup == Depth )
                {
                    /*int diff = (dBidsDifference) -  (BFSNDIFF);
                    int mktprice = GetMKTDataPrime(CreateSide);// GetMKTDataPrime(CreateSide);
                    cout << " PF "<< PFNumber<<" MD " << dBidsDifference << " UD "<< BFSNDIFF
                     << " Create diff "<< diff<< " MktPrice "<<mktprice<<" Bid "<< FarFP.MAXBID
                     << " Ask "<< FarFP.MINASK<< " secondbuyProd " <<  <<  endl;*/

                 /*  if( CreateSide == BUY && abs(diff + mktprice) <= GetBestData(CreateSide) ||
                      CreateSide == SELL && abs(diff + mktprice) >= GetBestData(CreateSide))
                        {*/
                            dFarMonthBuyRate =  abs(GetMKTDataLeg2(FOpairObj.TokenNearSide)
                            + GetMKTDataLeg3(FOpairObj.TokenThreeSide)
                            + BFSNDIFF);//abs ( diff +mktprice );//GetDepthPrice(FOpairObj.TokenNearSide,BFSNDIFF,CreateSide)  ;
                        //}
                    //cout << " diff " << diff<< " mktprice "<< mktprice<<endl;
                }

                if(dFarMonthBuyRate < GetBestData(ReverseSide)  && dFarMonthBuyRate > 0)
                {

                    pthread_mutex_lock(&initbuy_mutex);
                    BuyFarStatus = (OrderStatus)PENDING;

                    BuyTradeCounterleg2=0;
                    BuyTradeCounterleg3=0;

                   /* if(BLQ <= 0)
                    {
                        cout << " error "<<endl;
                    }
                    if(_BFSNMNQ * BLQ<=0)
                    {
                        cout << " Error "<<endl;
                    }*/
                    _shBuy.Qty1= min(Token1MinCreateRatio, ( Token1MaxCreateRatio - BuyTradeCounter )) * BLQ;// Token1Ratio * _BFSNMNQ * BLQ;
                    _shBuy.side1=CreateSide;
                    _shBuy.Price1=dFarMonthBuyRate;
                    //_pfBuy.HashKey = MyHash<StructHash>()(_shBuy);
                    _pfBuy.side = (short)CreateSide ;

                    BOARD_LOT_IN_TRBuy (FarFP.Token,_pfBuy.side,
                                    ntohl( _shBuy.Qty1),
                                    ntohl(dFarMonthBuyRate),_pfBuy
                                );
                   // cout << " QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ PFnumber " <<  PFNumber << " buy bid @ "<< dFarMonthBuyRate<<  " Token "<< _shBuy.Token1<< " Side "<< _shBuy.side1
                  //  << " Qty "<< _shBuy.Qty1 << " Price "<< _shBuy.Price1 <<  " Hashkey "<< _pfBuy.HashKey<<" QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"<< endl<<endl;

               /*   cout << "New PF "<< PFNumber << " InstanceGroup "<< InstanceGroup<< " Token " <<  FarFP.Token << " Create UD "<< BFSNDIFF << " MD "
                    << dBidsDifference << " Price generated "<< dFarMonthBuyRate << " firstbuyProd "
                    <<firstbuyProd << " secondbuyProd "<< secondbuyProd << " thirdbuyprod "<<thirdbuyprod<< " Bid "<< FarFP.MAXBID<< " ASK "<< FarFP.MINASK << " GetMKTDataPrime(CreateSide) "<<GetMKTDataPrime(CreateSide) << endl;*/

                    pthread_mutex_unlock(&initbuy_mutex);


                }
                else
                {
                   // cout <<"PFNumber \t"<< PFNumber << "\t BFSN \t"<< "New Order Buy price calculated greater than Ask dFarMonthBuyRate "<<dFarMonthBuyRate<<endl<<endl;
                }

            }

            else if((BuyFarStatus == (OrderStatus)OPEN || BuyFarStatus == (OrderStatus)REPLACED ))//&& abs(dBidsDifference) <= abs(BFSNDIFF) && !CancelBuyOrder)
			{
                int dQuoteRate;

                if(InstanceGroup == Best )
                {

                    if (abs(BFSNDIFF) > abs(dBidsDifference) && !CancelBuyOrder)
                    {
                        dQuoteRate = GetBestData(CreateSide) ;
                    }
                    else if (abs(BFSNDIFF) == abs(dBidsDifference ) && !CancelBuyOrder)
                    {
                        dQuoteRate = GetMKTData(FOpairObj.TokenNearSide) + abs(BFSNDIFF);
                    }
                    else if(abs(dBidsDifference) > abs(BFSNDIFF)  || CancelBuyOrder)
                    {
                        BuyFarStatus = (OrderStatus)PENDING;
                        ORDER_CANCEL_IN_TR ( FarFP.Token, CreateSide);
                    }
                }
                else if(InstanceGroup == Depth )
                {

                 /*   int diff = (dBidsDifference) -  (BFSNDIFF) ;
                    int mktprice =  GetMKTDataPrime(CreateSide) ;//secondbuyProd * -1;// GetMKTDataPrime(CreateSide);
                    cout << " PF "<< PFNumber<< " MD " << dBidsDifference << " UD "<< BFSNDIFF  <<
                    " Create diff "<< diff<< " MktPrice "<<mktprice<<" Bid "<< FarFP.MAXBID <<
                    " Ask "<< FarFP.MINASK<< " secondbuyProd "<<mktprice<<  endl;*/

                   /* if( CreateSide == BUY && abs(diff + mktprice) >= GetBestData(ReverseSide) ||
                       CreateSide == SELL && abs(diff + mktprice) <= GetBestData(ReverseSide)
                        || CancelBuyOrder)*/
                  /*  if( abs(dBidsDifference) > abs(BFSNDIFF))//  || CancelBuyOrder)
                    {

                    //cout << " PF "<< PFNumber << " Cancellation in Depth Create Price "<< abs(diff + mktprice) <<endl;
                      //  BuyFarStatus = (OrderStatus)PENDING;
                       // ORDER_CANCEL_IN_TR ( FarFP.Token, CreateSide);

                       dQuoteRate = GetBestData(CreateSide);
                    }

                    else*/
                    if(!CancelBuyOrder)
                    {

                        dQuoteRate =abs( GetMKTDataLeg2(FOpairObj.TokenNearSide)
                        + GetMKTDataLeg3(FOpairObj.TokenThreeSide)
                         + BFSNDIFF);//abs(diff +mktprice );

                        if(dQuoteRate >= GetMKTDataPrime(CreateSide) )
                            cout << "PF " << PFNumber <<" Create TradeHit Leg 1 @  " << dQuoteRate << " Leg2 @ "  << GetMKTData(FOpairObj.TokenNearSide) <<
                            " Leg3 @ "  << abs(GetMKTDataLeg3(FOpairObj.TokenThreeSide))
                            << " Trade Diff " << dQuoteRate + (GetMKTDataLeg2(FOpairObj.TokenNearSide)  +
                            GetMKTDataLeg3(FOpairObj.TokenThreeSide))  <<endl;
                        //cout << " Create  dQuoteRate "<< dQuoteRate<<endl;
                        // cout << " diff " << diff<< " mktprice "<< mktprice<<endl;
                    }
                    else if(CancelBuyOrder)
                    {
                        BuyFarStatus = (OrderStatus)PENDING;
                        ORDER_CANCEL_IN_TR ( FarFP.Token, CreateSide);
                    }

                    //dQuoteRate =((dBidsDifference) -  (BFSNDIFF) ) + GetMKTDataPrime(CreateSide); //GetDepthPrice(FOpairObj.TokenNearSide,BFSNDIFF,CreateSide)  ;
                }

                if(dQuoteRate < GetBestData(ReverseSide) && dQuoteRate > 0 && BuyFarStatus!=PENDING)
                {
                    BuyFarStatus = (OrderStatus)PENDING;
                    ORDER_MOD_IN_TR (FarFP.Token,
						         //ntohl(OrdDetailBuy.Qty *BLQ),
                                ntohl(dQuoteRate),CreateSide
						);

					/*	cout << "Mod PF "<< PFNumber << " InstanceGroup "<< InstanceGroup<< " Token " <<  FarFP.Token << " Create UD "<< BFSNDIFF << " MD "
                    << dBidsDifference << " Price generated "<< dQuoteRate << " firstbuyProd "
                    <<firstbuyProd << " secondbuyProd "<< secondbuyProd << " thirdbuyprod "<<thirdbuyprod<< " Bid "<< FarFP.MAXBID<< " ASK "<< FarFP.MINASK << " GetMKTDataPrime(CreateSide) "  << GetMKTDataPrime(CreateSide)<<endl;*/
					//	cout << "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ Modified Buy PF "<< PFNumber <<" QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"<< endl<<endl;
                }
                else
                {
                   // cout <<"PFNumber \t"<< PFNumber << "\t BFSN \t"<< "Modify Order Buy price calculated greater than Ask dQuoteRate "<< dQuoteRate<<endl<<endl;
                }
            }

            }

        }
            pthread_mutex_unlock(&count_mutex);
       // lock.unlock();
      //  the_incondition_variable.notify_one();

        }



        void StrategyClass::OnDifferenceEventHandler(FOPAIRDIFFIOC* _INpairDiff)
        {
			//struct FOPAIRDIFF _INpairDiff;

           // memcpy(&_INpairDiff,buffer,sizeof(_INpairDiff));

            	_BNSFMNQ = _INpairDiff->BNSFMNQ;
                _BFSNMNQ = _INpairDiff->BFSNMNQ;
                _BNSFMXQ = _INpairDiff->BNSFMXQ;
                _BFSNMXQ = _INpairDiff->BFSNMXQ;
            //	DEPTHORBEST = _FOPAIRDIFF [PFNumber].Depth_Best ==Depth ? true  :false ;

                BFSNDIFF = (int)_INpairDiff->BFSNDIFF;
                BNSFDIFF = (int)_INpairDiff->BNSFDIFF;

                Token1MinCreateRatio = Token1Ratio * _BFSNMNQ;
                Token2MinCreateRatio = Token2Ratio * _BFSNMNQ;
                Token3MinCreateRatio = Token3Ratio * _BFSNMNQ;

                Token1MinReverseRatio = Token1Ratio * _BNSFMNQ;
                Token2MinReverseRatio = Token2Ratio * _BNSFMNQ;
                Token3MinReverseRatio = Token3Ratio * _BNSFMNQ;

                Token1MaxCreateRatio = Token1Ratio * _BFSNMXQ;
                Token2MaxCreateRatio = Token2Ratio * _BFSNMXQ;
                Token3MaxCreateRatio = Token3Ratio * _BFSNMXQ;

                Token1MaxReverseRatio = Token1Ratio * _BNSFMXQ;
                Token2MaxReverseRatio = Token2Ratio * _BNSFMXQ;
                Token3MaxReverseRatio = Token3Ratio * _BNSFMXQ;


               // cout << " New class OnDiff PF " << PFNumber << " BNSFDIFF " << _INpairDiff->BNSFDIFF << " BFSN "<< _INpairDiff->BFSNDIFF<<endl<<endl;
        }


void StrategyClass::BOARD_LOT_IN_TRBuy(int FarTokenNo,BUYSELL BS,int qty, int price, PFHolder _PF)  //-- 20000
		{// MS_OE_REQUEST_TR 110+26

          //  cout<<"Buy: "<<FarTokenNo<<" price "<<htonl(price)<<" size "<<sizeof (_NMPACK [FarTokenNo].FARMONTHBUY) <<endl;
                MS_OE_REQUEST_TR obj;
                  memset(&obj,0,138);
                // cout <<"PFNumber "<< PFNumber <<" New Buy Order location 1 from STG  Price : "<< htonl(price) <<" Volume :"<<htonl(qty)<<endl;

                    //cout<<"BOARD_LOT_IN_TRBuy  : 1 ";

                   // cout<<"BOARD_LOT_IN_TRBuy  : 2 ";

                    //cout<<"BOARD_LOT_IN_TRBuy  : 3 ";
                    obj=_NMPack.FARMONTH;
                    obj.Buy_SellIndicator =ntohs((short)BS);

                   // cout<<"BOARD_LOT_IN_TRBuy  : 4 ";
                    obj.DisclosedVolume=obj.Volume=qty;

                   // if(ntohl(qty) > BLQ)
                   // cout << " Check "<<endl;

	 				obj.Price=price;
	 				//cout<<"BOARD_LOT_IN_TRBuy  : 5 ";
	 				//ProcessToEnqueue((char*)&obj,136);
	 				//cout<<"BOARD_LOT_IN_TRBuy  : 6 ";

	 				_PF._oetr = obj;
                    _PF._size = 136;

                    //_PFHashMap.AddRecord(_PF,false);

                   _FillData.BidQueue(_PF);

                   cout <<"PFNumber "<< PFNumber <<" New Buy Order sent from STG "<<  " Symbol "<< obj.Symbol<< " Instrument Name "<< obj.InstrumentName<<" Price : "<< htonl(price) <<" Volume :"<<htonl(obj.Volume)<<endl;

        }


 void StrategyClass::BOARD_LOT_IN_TRSell(int FarTokenNo,BUYSELL BS,int qty, int price, PFHolder _PF)  //-- 20000
		{// MS_OE_REQUEST_TR 110+26

//cout<<"Sell: "<<FarTokenNo<<" price "<<htonl(price)<<" size "<<sizeof (_NMPACK [FarTokenNo].FARMONTHSELL) <<endl;
 //cout << "PFNumber "<< PFNumber <<" New Sell Order location 1 from STG  Price : "<< htonl(price) <<" Volume :"<<htonl(qty)<<endl;
                   // cout<<"BOARD_LOT_IN_TRSell  : 1 ";
                    MS_OE_REQUEST_TR obj;
                  // cout<<"BOARD_LOT_IN_TRSell  : 2 ";
                    memset(&obj,0,136);
                  // cout<<"BOARD_LOT_IN_TRSell  : 3 ";
                    obj=_NMPack.FARMONTH;
                    obj.Buy_SellIndicator =ntohs((short)BS);
	 				obj.DisclosedVolume=obj.Volume=qty;

	 				// if(ntohl(qty)> BLQ)
                   // cout << " Check "<<endl;

	 				//cout<<"BOARD_LOT_IN_TRSell  : 4 ";
	 				obj.Price=price;
	 				//cout<<"BOARD_LOT_IN_TRSell  : 5 ";
	 				//ProcessToEnqueue((char*)&obj,136);
	 				//cout<<"BOARD_LOT_IN_TRSell  : 6 ";
                    _PF._oetr = obj;
                    _PF._size=136;

                     //cout<<"\nLength= "<<ntohl(obj.Length) <<" tcode "<<ntohl(obj.TransactionCode);

                    //_PFHashMap.AddRecord(_PF,false);

	 				_FillData.BidQueue(_PF);

                    cout << "PFNumber "<< PFNumber <<" New Sell Order sent from STG "<<  " Symbol "<< obj.Symbol<< " Instrument Name "<< obj.InstrumentName<<" Price : "<< htonl(price) <<" Volume :"<<htonl(obj.Volume)<<endl;
		}


 void StrategyClass::ORDER_MOD_IN_TR(int TokenNo, int price,BUYSELL buySell)// 20040
		{//MS_OM_REQUEST_TR 138+26=164

                    //cout<<"ORDER_MOD_IN_TR  : 1 ";
                    MS_OM_REQUEST_TR obj;
                  //  cout<<"ORDER_MOD_IN_TR  : 2 ";
                    memset(&obj,0,164);
                  //  cout<<"ORDER_MOD_IN_TR  : 3 ";
  	 				if(buySell==CreateSide)
  	 				{
	 					obj=_NMPack.FARMONTHMODBUY;
	 				//	cout<<"ORDER_MOD_IN_TR  : 4 ";
                        //cout << " Mod Buy Order sent from STG "<<endl;
	 				}
	 				else if(buySell==ReverseSide)
	 				{
	 					obj=_NMPack.FARMONTHMODSELL;
	 					//cout << " Mod Sell Order sent from STG "<<endl;
                      //  cout<<"ORDER_MOD_IN_TR  : 5 ";
	 				}
	 				char TransCodeBytes[0];
                    //cout<<"ORDER_MOD_IN_TR  : 6 ";
	 				obj.TransactionCode=ModificationCode;
	 			//	obj.DisclosedVolume=obj.Volume=volume;
	 				//cout<<"ORDER_MOD_IN_TR  : 7 ";
	 				obj.Price=price;
	 				//cout<<"ORDER_MOD_IN_TR  : 8 ";

                   // ProcessToEnqueue((char*)&obj,164);
                  //  cout<<"ORDER_MOD_IN_TR  : 9 ";
                    _pfBuy._size = 164;
                    _pfBuy._omtr= obj;
                  _FillData.BidQueue(_pfBuy);

		}


 void StrategyClass::ORDER_CANCEL_IN_TR(int TokenNo, BUYSELL buySell)  //-- 20070
		{// MS_OM_REQUEST_TR 138+26

        //cout<<"ORDER_CANCEL_IN_TR  : 1 ";
		MS_OM_REQUEST_TR obj;
		//cout<<"ORDER_CANCEL_IN_TR  : 2 ";
        memset(&obj,0,164);
        //cout<<"ORDER_CANCEL_IN_TR  : 3 ";
  	 				if(buySell==CreateSide)
  	 				{
	 					obj=_NMPack.FARMONTHMODBUY;
                //    cout << " Cancel Buy Order sent Price"<<ntohl(obj.Price)<<" Volume :"<<ntohl(obj.Volume)<<endl;
	 				//	cout<<"ORDER_CANCEL_IN_TR  : 4 "<<" OrderNumber ="<<_NMPack.FARMONTHMODBUY.OrderNumber<<endl;
	 				}
	 				else if(buySell==ReverseSide)
	 				{

	 					obj=_NMPack.FARMONTHMODSELL;

	 				//	 cout << " Cancel Sell Order sent Price"<<ntohl(obj.Price)<<" Volume :"<<ntohl(obj.Volume)<<endl;
	 				//	cout << " Cancel Sell Order sent from STG  5"<<endl;
                     //   cout<<"ORDER_CANCEL_IN_TR  : 5 "<<" OrderNumber ="<<_NMPack.FARMONTHMODBUY.OrderNumber<<endl;
	 				}
	 			//	cout<<"ORDER_CANCEL_IN_TR  : 6 ";
	 				char TransCodeBytes[0];
	 				obj.TransactionCode=CancelCode;
	 				//cout<<"ORDER_CANCEL_IN_TR  : 7 ";
	 				if(obj.OrderNumber==0 || obj.OrderNumber== -1)
	 				{
                        cout << " OrderNumber invalid. Cancellation not sent " << endl;
                        return;
	 				}
                   // ProcessToEnqueue((char*)&obj,164);
                   // cout<<"ORDER_CANCEL_IN_TR  : 8\n ";

                   _pfBuy._size = 164;
                    _pfBuy._omtr= obj;
                  _FillData.BidQueue(_pfBuy);
		}


 void StrategyClass::ORDER_CONFIRMATION_TR (char *buffer) //-- 20073
		{

			 MS_OE_RESPONSE_TR obj;//156
			// cout<<"ORDER_CONFIRMATION_TR  : 1 ";
            memset(&obj,0,156);
           // cout<<"ORDER_CONFIRMATION_TR  : 2 ";
            memcpy(&obj,buffer,156);

       //cout<<"ORDER_CONFIRMATION_TR  : 3 ";
		short _BS = htons(obj.Buy_SellIndicator);
		int _TKN =htonl(obj.TokenNo);

       // cout << "Token " << _TKN << " _OrderNo " << _OrderNo << " ORDER_CONFIRMATION_TR In" << endl;
        if(_TKN== FarFP.Token)
        {
            if(_BS == CreateSide )
            {
                 MS_OM_REQUEST_TR _obj;
                memset(&_obj,0,164);
                _obj=_NMPack.FARMONTHMODBUY;
                _obj.OrderNumber=obj.OrderNumber;
                _obj.DisclosedVolume =obj.DisclosedVolume ;
                _obj.DisclosedVolumeRemaining =obj.DisclosedVolumeRemaining ;
                _obj.TotalVolumeRemaining =obj.TotalVolumeRemaining ;
                _obj.Volume =obj.Volume ;
                _obj.VolumeFilledToday =obj.VolumeFilledToday ;
                _obj.Price =obj.Price ;
                _obj.EntryDateTime =obj.EntryDateTime ;
                _obj.LastModified =obj.LastModified ;
                _obj.filler =obj.filler ;
                _obj.Buy_SellIndicator = obj.Buy_SellIndicator;
    //     _NMPACK [_TKN].FARMONTHMODBUY=_obj;

                _NMPack.FARMONTHMODBUY=_obj;
                BuyFarStatus = (OrderStatus)OPEN;

              /*  if(CancelBuyOrder)
                {
                    if(BuyFarStatus== PENDING || BuyFarStatus == OPEN || BuyFarStatus== REPLACED )
                    ORDER_CANCEL_IN_TR ( FarFP.Token, CreateSide);
                }*/
            }
            else
            {
                  MS_OM_REQUEST_TR _obj;
                    memset(&_obj,0,164);
                    _obj=_NMPack.FARMONTHMODSELL;
                    _obj.OrderNumber=obj.OrderNumber;
                    _obj.DisclosedVolume =obj.DisclosedVolume ;
                    _obj.DisclosedVolumeRemaining =obj.DisclosedVolumeRemaining ;
                    _obj.TotalVolumeRemaining =obj.TotalVolumeRemaining ;
                    _obj.Volume =obj.Volume ;
                    _obj.VolumeFilledToday =obj.VolumeFilledToday ;
                     _obj.Price =obj.Price ;
                    _obj.EntryDateTime =obj.EntryDateTime ;
                    _obj.LastModified =obj.LastModified ;
                    _obj.filler =obj.filler ;
                    _obj.Buy_SellIndicator = obj.Buy_SellIndicator;
//cout<<"ORDER_CONFIRMATION_TR  : 11 ";


                    _NMPack.FARMONTHMODSELL=_obj;
                    SellFarStatus = (OrderStatus)OPEN;

                  /*  if(CancelSellOrder)
                    {
                          if(SellFarStatus== PENDING || SellFarStatus == OPEN || SellFarStatus== REPLACED )
                        ORDER_CANCEL_IN_TR ( FarFP.Token, ReverseSide);
                    }*/
            }

		}
      //  cout <<"ORDER_CONFIRMATION_TR 6  "<<endl;
     //   ORDER_MOD_IN_TR(_TKN, 50,ntohl( _price+100),(BUYSELL)_BS);
	}

		 void StrategyClass::ORDER_CXL_CONFIRMATION_TR (char *buffer) //-- 20075
	{

			 MS_OE_RESPONSE_TR obj;//156
			//  cout<<"ORDER_CONFIRMATION_TR  : 1 ";
			 memset(&obj,0,156);
			 memcpy(&obj,buffer,156);
			//  cout<<"ORDER_CONFIRMATION_TR  : 2 ";
        short _BS = htons(obj.Buy_SellIndicator);
        int _TKN =htonl(obj.TokenNo);

		// cout<<"ORDER_CONFIRMATION_TR  : 3 ";
		if(_TKN == FarFP.Token)
		{
            if(_BS==CreateSide )
            {
               BuyFarStatus = (OrderStatus)CANCEL;
            }
            else if(_BS == ReverseSide)
            {
                SellFarStatus = (OrderStatus)CANCEL;
            }

        }
        else
        {
            short ErrorCode = htons(obj.ErrorCode);
            if(ErrorCode == 17070)
            {
               cout <<" Self Trade Prevention occured  ErrorCode "<< ErrorCode <<endl;

                MS_OE_REQUEST_TR obj_New;

                if(_TKN == NearFP.Token)
                {
                    obj_New = _NMPack.NEARMONTHMARKET;
                }
                else if(_TKN == ThirdFP.Token)
                {
                    obj_New = _NMPack.NEARMONTHMARKET2;
                }

                obj_New.Buy_SellIndicator = obj.Buy_SellIndicator ;
                obj_New.DisclosedVolume=obj_New.Volume=obj.Volume;

                _shMKTSell.Price1=0;
                _shMKTSell.side1 = ntohs(FOpairObj.TokenNearSide);

                        //_pfMKTSell.HashKey = MyHash<StructHash>()(_shMKTSell);
                _pfMKTSell.side =_shMKTSell.side1;
                _pfMKTSell._size = 136;
                _pfMKTSell._oetr= obj_New;

                        //_PFHashMap.AddRecord(_pfMKTSell,false);

                _FillData.MktQueue(_pfMKTSell);
                cout <<" Order placed for Market after SFT "<< ErrorCode <<endl;

            }
        }

	}
//***********************

		 void StrategyClass::ORDER_MOD_CONFIRMATION_TR (char *buffer) //-- 20074
		{
//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step1"<<endl;

       MS_OE_RESPONSE_TR obj;//156
//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step2"<<endl;
	   memset(&obj,0,156);
	//   cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step3"<<endl;
		memcpy(&obj,buffer,156);
		//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step4"<<endl;
		short _BS = htons(obj.Buy_SellIndicator);
		//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step5"<<endl;
		int _TKN = htonl(obj.TokenNo);
		//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step6"<<endl;
		double _OrderNo =obj.OrderNumber ;
		//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step7"<<endl;
		int _price = htonl(obj.Price);
		//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step8"<<endl;
        if(_TKN == FarFP.Token)
        {
            if(_BS == CreateSide)
			{
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step9"<<endl;
            MS_OM_REQUEST_TR _obj;
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step10"<<endl;
            memset(&_obj,0,164);
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step11"<<endl;
            _obj=_NMPack.FARMONTHMODBUY;
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step12"<<endl;
            _obj.OrderNumber=obj.OrderNumber;
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step13"<<endl;
            _obj.DisclosedVolume =obj.DisclosedVolume ;
			// cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step14"<<endl;
             _obj.DisclosedVolumeRemaining =obj.DisclosedVolumeRemaining ;
			// cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step15"<<endl;
              _obj.TotalVolumeRemaining =obj.TotalVolumeRemaining ;
			  //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step16"<<endl;
               _obj.Volume =obj.Volume ;
			   //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step17"<<endl;
                _obj.VolumeFilledToday =obj.VolumeFilledToday ;
				//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step18"<<endl;
                 _obj.Price =obj.Price ;
				 //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step19"<<endl;
                  _obj.EntryDateTime =obj.EntryDateTime ;
				  //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step20"<<endl;
                   _obj.LastModified =obj.LastModified ;
				   //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step21"<<endl;
                    _obj.filler =obj.filler ;
					//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step22"<<endl;
                _obj.Buy_SellIndicator = obj.Buy_SellIndicator;
            //_NMPACK [_TKN].FARMONTHMODBUY=_obj;

            _NMPack.FARMONTHMODBUY=_obj;
            BuyFarStatus = (OrderStatus)REPLACED;

           /* if(CancelBuyOrder)
            {
                if(BuyFarStatus== PENDING || BuyFarStatus == OPEN || BuyFarStatus== REPLACED )
               ORDER_CANCEL_IN_TR ( FarFP.Token, CreateSide);
            }*/

//cout << " ORDER_MOD_CONFIRMATION_TR Buy Order recieved in STG "<<endl;
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step25"<<endl;

			}
            else if(_BS == ReverseSide)
			{
			    MS_OM_REQUEST_TR _obj;
			//	cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step26"<<endl;
            memset(&_obj,0,164);
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step27"<<endl;
            _obj=_NMPack.FARMONTHMODSELL;
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step28"<<endl;
            _obj.OrderNumber=obj.OrderNumber;
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step29"<<endl;
            _obj.DisclosedVolume =obj.DisclosedVolume ;
			//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step30"<<endl;
             _obj.DisclosedVolumeRemaining =obj.DisclosedVolumeRemaining ;
			 //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step31"<<endl;
              _obj.TotalVolumeRemaining =obj.TotalVolumeRemaining ;
			  //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step32"<<endl;
               _obj.Volume =obj.Volume ;
			   //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step33"<<endl;
                _obj.VolumeFilledToday =obj.VolumeFilledToday ;
				//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step34"<<endl;
                 _obj.Price =obj.Price ;
				 //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step35"<<endl;
                  _obj.EntryDateTime =obj.EntryDateTime ;
				  //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step36"<<endl;
                   _obj.LastModified =obj.LastModified ;
				   //cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step37"<<endl;
                    _obj.filler =obj.filler ;
					//cout<<"ORDER_MOD_CONFIRMATION_TR =>"<<"step38"<<endl;
                _obj.Buy_SellIndicator = obj.Buy_SellIndicator;

				 _NMPack.FARMONTHMODSELL=_obj;
                  SellFarStatus = (OrderStatus)REPLACED;


                  /*  if(CancelSellOrder)
                    {
                         if(SellFarStatus== PENDING || SellFarStatus == OPEN || SellFarStatus== REPLACED )
                        ORDER_CANCEL_IN_TR ( FarFP.Token, ReverseSide);
                    }*/
                }
//cout << " ORDER_MOD_CONFIRMATION_TR Sell Order recieved in STG "<<endl;

			}
		}


	//	ORDER_CANCEL_IN_TR(_TKN,(BUYSELL)_BS);




		void StrategyClass::TRADE_CONFIRMATION_TR (char *buffer) //-- 20222
		{

          MS_TRADE_CONFIRM_TR obj_Trade;

          memset(&obj_Trade,0,153);

          memcpy(&obj_Trade,buffer,153);

				int _TKN =htonl(obj_Trade.Token);

				short _BS = htons(obj_Trade.Buy_SellIndicator);
				int Qty = htonl(obj_Trade.FillQuantity)/BLQ;
				int RemainingQty = htonl(obj_Trade.RemainingVolume)/BLQ;


          if(FarFP.Token ==_TKN)
          {

                int Leg2HitQty = floor(Qty*Leg2HitRatio) * BLQ;
                int Leg3HitQty = floor(Qty* Leg3HitRatio) * BLQ;

                if(Leg2HitQty>0)
                {
                    MS_OE_REQUEST_TR obj_New;

                    obj_New = _NMPack.NEARMONTHMARKET;

                    short Leg1side = FOpairObj.TokenFarSide == _BS ? (short)FOpairObj.TokenNearSide : (short)(FOpairObj.TokenNearSide == BUY ? SELL :BUY);

                    obj_New.Buy_SellIndicator =ntohs(Leg1side);
                    obj_New.DisclosedVolume=obj_New.Volume= htonl(Leg2HitQty) ;

                    _pfMKTSell._size = 136;
                    _pfMKTSell._oetr= obj_New;
                    _FillData.MktQueue(_pfMKTSell);

                }
                if(Leg3HitQty > 0)
                {
                    MS_OE_REQUEST_TR obj_New2;
                    obj_New2 = _NMPack.NEARMONTHMARKET2;
                    short Leg2side = FOpairObj.TokenFarSide == _BS ? (short)FOpairObj.TokenThreeSide : (short)(FOpairObj.TokenThreeSide == BUY ? SELL :BUY);

                    obj_New2.Buy_SellIndicator =ntohs(Leg2side);

                    obj_New2.DisclosedVolume=obj_New2.Volume=htonl(Leg3HitQty);

                    _pfMKTSell2._size = 136;
                    _pfMKTSell2._oetr= obj_New2;
                    _FillData.MktQueue(_pfMKTSell2);

                }



				if(_BS==(short)FOpairObj.TokenFarSide)
				{
                   BuyTradeCounter +=Qty ;
                   _CreateRemainingQty = RemainingQty;
                }
				else
				{
                   SellTradeCounter += Qty ;
                   _ReverseRemainingQty = RemainingQty;
                }

			}
			 else if(_TKN == NearFP.Token)
			 {

                if(ThirdFP.Token ==0 && _BS ==FOpairObj.TokenNearSide && _CreateRemainingQty ==0)
                {
                     if(BuyTradeCounter >= (Token1MaxCreateRatio-1))
                    {
                        CancelBuyOrder = true;
                    }

                    BuyFarStatus =(OrderStatus)TRADE;
                    cout <<" PFNumber "<< PFNumber << " BuyFarStatus set to Trade"<<endl;
                }
                else if(ThirdFP.Token ==0 && _BS !=FOpairObj.TokenNearSide && _ReverseRemainingQty ==0)
                {
                    if(SellTradeCounter >= (Token1MaxReverseRatio-1))
                    {
                        CancelSellOrder = true;
                    }
                    SellFarStatus =(OrderStatus)TRADE;
                    cout <<" PFNumber "<< PFNumber << " SellFarStatus set to Trade"<<endl;
                }

			  cout<<"PF " << PFNumber <<" Trade Token 2  _TKN: "<<_TKN<<" _BS: "<<_BS<<endl;

            }
            else if(_TKN == ThirdFP.Token)
            {
                 if(_BS ==FOpairObj.TokenThreeSide && _CreateRemainingQty ==0)
                {

                    if(BuyTradeCounter >= (Token1MaxCreateRatio-1))
                    {
                        CancelBuyOrder = true;
                    }

                    BuyFarStatus =(OrderStatus)TRADE;


                    cout <<" PFNumber "<< PFNumber << " BuyFarStatus set to Trade"<<endl;
                }
                else if(_BS !=FOpairObj.TokenThreeSide && _ReverseRemainingQty ==0)
                {
                    if(SellTradeCounter >= (Token1MaxReverseRatio-1))
                    {
                        CancelSellOrder = true;
                    }

                    SellFarStatus =(OrderStatus)TRADE;
                    cout <<" PFNumber "<< PFNumber << " SellFarStatus set to Trade"<<endl;
                }
                cout<<"PF " <<  PFNumber << " Trade Token 3  _TKN: "<<_TKN<<" _BS: "<<_BS<<endl;
            }


		}




		 void StrategyClass::ORDER_CXL_REJ_OUT (char *buffer) //-- 2072
		{//MS_OE_REQUEST 240
//cout<<"ORDER_CXL_REJ_OUT =>"<<"step1"<<endl;
			 MS_OE_REQUEST obj;
	//		 cout<<"ORDER_CXL_REJ_OUT =>"<<"step2"<<endl;
			 memset(&obj,0,240);
		//	 cout<<"ORDER_CXL_REJ_OUT =>"<<"step3"<<endl;
             memcpy(&obj,buffer,240);
			// cout<<"ORDER_CXL_REJ_OUT =>"<<"step4"<<endl;
						    short _BS =htons(obj.Buy_SellIndicator);
				//					cout<<"ORDER_CXL_REJ_OUT =>"<<"step5"<<endl;
							int _TKN =htonl(obj.TokenNo);
					//		cout<<"ORDER_CXL_REJ_OUT =>"<<"step6"<<endl;
							short _Error = htons(obj.ErrorCode);
						//	cout<<"ORDER_CXL_REJ_OUT =>"<<"step7"<<endl;
							if(_BS == CreateSide)
							{
							//cout<<"ORDER_CXL_REJ_OUT =>"<<"step8"<<endl;
							if (_Error == 16273)

							BuyFarStatus = (OrderStatus)CANCEL;

							else if(BuyFarStatus != (OrderStatus)TRADE)
							BuyFarStatus = (OrderStatus)REPLACED;
							}
							else if(_BS == ReverseSide)
							{

							if (_Error == 16273)
							SellFarStatus = (OrderStatus)CANCEL;
							else if(SellFarStatus != (OrderStatus)TRADE)
							SellFarStatus = (OrderStatus)REPLACED;


							}
						//	cout<<"ORDER_CXL_REJ_OUT =>"<<"step9"<<endl;
		}


		void StrategyClass::ORDER_MOD_REJ_OUT (char *buffer) //-- 2042
		{//MS_OE_REQUEST 240
//cout<<"ORDER_MOD_REJ_OUT =>"<<"step1"<<endl;
			 MS_OE_REQUEST obj;
	//		 cout<<"ORDER_MOD_REJ_OUT =>"<<"step2"<<endl;
			 memset(&obj,0,240);
		//	 cout<<"ORDER_MOD_REJ_OUT =>"<<"step3"<<endl;
			  memcpy(&obj,buffer,240);
			//  cout<<"ORDER_MOD_REJ_OUT =>"<<"step4"<<endl;

                short _BS =htons(obj.Buy_SellIndicator);
				//cout<<"ORDER_MOD_REJ_OUT =>"<<"step5"<<endl;
				int _TKN =htonl(obj.TokenNo);
				//cout<<"ORDER_MOD_REJ_OUT =>"<<"step6"<<endl;
				short _Error = htons(obj.ErrorCode);
				//cout<<"ORDER_MOD_REJ_OUT =>"<<"step7"<<endl;


				if(_TKN == FarFP.Token)
                {
                if(_BS==CreateSide)
                {
				if (_Error == 16273)
				BuyFarStatus= (OrderStatus)CANCEL;
				else if(BuyFarStatus != (OrderStatus)TRADE)
				BuyFarStatus = (OrderStatus)REPLACED;
				}
				else
				{
				if (_Error == 16273)
				SellFarStatus = (OrderStatus)CANCEL;
				else if(SellFarStatus != (OrderStatus)TRADE)
				SellFarStatus = (OrderStatus)REPLACED;

				}
				}
				//cout<<"ORDER_MOD_REJ_OUT =>"<<"step8"<<endl;
	}

    void StrategyClass::ORDER_ERROR_OUT (char *buffer) //-- 2231
		{//MS_OE_REQUEST 240
//cout<<"ORDER_ERROR_OUT =>"<<"step1"<<endl;
		MS_OE_REQUEST obj;
	//	cout<<"ORDER_ERROR_OUT =>"<<"step2"<<endl;
		memset(&obj,0,240);
		//cout<<"ORDER_ERROR_OUT =>"<<"step3"<<endl;
        memcpy(&obj,buffer,240);
//cout<<"ORDER_ERROR_OUT =>"<<"step4"<<endl;
        short _BS =htons(obj.Buy_SellIndicator);
	//	cout<<"ORDER_ERROR_OUT =>"<<"step5"<<endl;
		int _TKN =htonl(obj.TokenNo);
		//cout<<"ORDER_ERROR_OUT =>"<<"step6"<<endl;

		if(_TKN == FarFP.Token )
		{
		if(_BS== CreateSide)
		BuyFarStatus = (OrderStatus)REJECTED;
        else
		SellFarStatus = (OrderStatus)REJECTED;


		}

cout<<"ORDER_ERROR_OUT =>"<<"step7"<<endl;
		}


