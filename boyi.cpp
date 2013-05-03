/*=============================================================================
#     FileName: boyi.cpp
#         Desc:
#       Author: Gin
#        Email: sunxiao.gin@gmail.com
#     HomePage: Compiler: gcc&&g++
#      Version: 0.0.1
#   LastChange: 2012-06-24 18:36:55
#      History:
=============================================================================*/
/*
The gain Martix
A/B 	C		D
-------------------------
C  	   |C_C 		C_D|
	   | 		       |
       | 		       |
D 	   |D_C 		D_D|
-------------------------
*/
#define C_C 1.000000
#define D_C 1.900000
double D_C_B;
#define C_D 0.000000
#define D_D 0.000000

#include <iostream>
#include <fstream>
#include<cmath>
#include <ctime>
#include<cstdlib>


#define LengthX 100
#define LengthY 100
#define Nmax 10000
#define MaxAge 32.0000
#define BriAge 8.0000
#define ReproductionAge  8.0000
#define GeneSize 32.0000
#define SUR 2
#define K 0.5000

using namespace std;
ofstream fileBoyi;
ofstream fileSquare;
ofstream fileChange;
ofstream fileAge;
ofstream fileGen;
typedef struct theper
{
    bool Gene[(int)GeneSize];
	float Age;
	float ChangeGeneNum;
	bool Stra;//0 is Cooperatio
	double MoneyCoo;//the money get from the Cooperation
    double MoneyBet;//the money get from the Betray
	double MoneyLast;
	bool LiveOrDie;
}per;
per Agent[LengthX][LengthY];
long Nt= 0;
double Pt;
int CircleTem[2][8];

//get the strategy change or don't
 bool inline StrategyChange_first(per* theagent)
{
	int check = rand()%100;
	if(theagent->Stra == false)
		return	(check<=100*(double)(theagent->ChangeGeneNum/theagent->Age)/(1+exp((theagent->MoneyCoo-theagent->MoneyBet)/K)))?true:false;
	else
		return  (check<=100*(double)((32-theagent->ChangeGeneNum)/theagent->Age)/(1+exp((theagent->MoneyBet-theagent->MoneyCoo)/K)))?true:false;

}

bool inline StrategyChange_second(per* theagent,per* otheragent)
{
	int check = rand()%100;
	//cout<<check<<endl;
	//cout<<100*(double)((theagent->ChangeGeneNum)/theagent->Age)/(1+exp((otheragent->MoneyLast-theagent->MoneyLast)))<<endl;
    return (check<=100*(double)((theagent->ChangeGeneNum)/theagent->Age)/(1+exp((otheragent->MoneyLast-theagent->MoneyLast))))?otheragent->Stra:theagent->Stra;
}

//copy the Martix
//get the live pro
double GetAverage(per BackUp[][LengthY])
{
	long int all=0;
	long int num=0;
	for(int i=0;i!=LengthX;i++)
        for(int j=0;j!=LengthY;j++)
        {
            BackUp[i][j] = Agent[i][j];
            if(Agent[i][j].LiveOrDie==0)
            {
                num++;
                all+=Agent[i][j].MoneyCoo;
                all+=Agent[i][j].MoneyBet;
            }
        }
	//cout<<all<<endl;
	return (double)((double)all/(double)num);
	//return all;
}

//live or die , depend on the gain
bool inline LivePro(per* agent)
{
	int check = rand()%100;
//	Pt = GetAverage();
		return (check<=100*(double)(Nt/((1+exp(Pt-(agent->MoneyCoo+agent->MoneyBet)))*Nmax)))?true:false;
}

bool inline LivePro()
{
	int check = rand()%100;
	return (check <= SUR)?true:false;

}

void FindCircle(int x,int y)
{
    CircleTem[0][0]= (x+100-1)%100  ; CircleTem[1][0] = (y+100-1)%100  ;
    CircleTem[0][1]= (x+100)%100    ; CircleTem[1][1]= (y+100-1)%100   ;
    CircleTem[0][2]= (x+100+1)%100  ; CircleTem[1][2] = (y+100-1)%100  ;
    CircleTem[0][3]= (x+100-1)%100  ; CircleTem[1][3] = y;
    CircleTem[0][4]= (x+100+1)%100  ; CircleTem[1][4] = y;
    CircleTem[0][5]= (x+100-1)%100  ; CircleTem[1][5] = (y+100+1)%100  ;
    CircleTem[0][6]= (x+100)%100    ; CircleTem[1][6] = (y+100+1)%100  ;
    CircleTem[0][7]= (x+100+1)%100  ; CircleTem[1][7] = (y+100+1)%100  ;
}

//intial
per CreatAgent()
{
	per tempper;
    tempper.Age=rand()%(int)MaxAge;
	tempper.ChangeGeneNum =0;
	tempper.MoneyCoo = 0;	
	tempper.MoneyBet = 0;
	tempper.MoneyLast = 0;
	tempper.Stra = (int)rand()%2;
	tempper.LiveOrDie = 0 ;
	for (int k=0;k!=GeneSize;k++)
	{
		//cout<<(int)rand()%2;
		tempper.Gene[k]=(int)rand()%2;
        //cout<<Agent[i].Gene[j]<<"\t" 
	}
	return tempper;
}

void Intial()
{
	for(int i=0;i!=LengthX;i++)
        for(int j=0;j!=LengthY;j++)
        {
			Agent[i][j] = CreatAgent();
			Nt++;
        }
}

//Age ++
void inline Age(int i,int j)
{
	//if (Agent[i].Gene[Agent[i].Age]==1)
	//Agent[i].ChangeGeneNum++;
	Agent[i][j].Age++;
}

void inline OpenGen(int i , int j)
{
	if (Agent[i][j].Gene[(int)Agent[i][j].Age]==true)
		Agent[i][j].ChangeGeneNum++;
}

//die
void inline Die(int i,int j)
{
//	double v;
//	v=(double)rand()/   ;//cout<<v<<"\n";
	//if (LivePro(&Agent[i][j]))
	if (LivePro())
	{
	    Nt--;
        Agent[i][j].LiveOrDie = 1;
	}
//else if (v>((double)Nt/Nmax))
	//{Nt-=1;
//Agent[i].LiveOrDie == 1;}
	else if (Agent[i][j].Age>=GeneSize)
	{
		Nt--;
		Agent[i][j].LiveOrDie = 1;
	}
}

//reproduction
void inline Reproduction(int x,int y,per BackUP[][LengthY])
{
    //FindCircle(x,y);
    unsigned int MaxGainTem = 0 ;
 //  int k = rand()%8;
  // cout<<k<<"\t"<<CircleTem[0][k]<<"\t"<<CircleTem[1][k]<<endl;
    int MaxGainIndex = -1; // The Coordinate of Max Gain Per
    for(int i=0;i!=8;i++)
        if((BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyBet + BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyCoo) > MaxGainTem && BackUP[CircleTem[0][i]][CircleTem[1][i]].LiveOrDie == 0&&BackUP[CircleTem[0][i]][CircleTem[1][i]].Age >= ReproductionAge)
        {
            MaxGainTem = (BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyBet + BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyCoo);
			MaxGainIndex = i; 
        }
	if (MaxGainIndex != -1)
	{
		Agent[x][y] = BackUP[CircleTem[0][MaxGainIndex]][CircleTem[1][MaxGainIndex]];
		Agent[x][y].Age=0;
		Agent[x][y].ChangeGeneNum =0;
		Agent[x][y].MoneyCoo = 0;	
		Agent[x][y].MoneyBet = 0;
		Agent[x][y].MoneyLast = 0;
	//	Agent[x][y].Stra = (int)rand()%2;
		Agent[x][y].LiveOrDie = 0 ;
		int temp = (int)rand()%32 ; 
		Agent[x][y].Gene[temp] = !Agent[x][y].Gene[temp];  
		Nt++;
	//	cout<<"reproduction"<<endl;
	}
	else
	{
		//Agent[x][y] = CreatAgent();
		//cout<<"o my god ! die !"<<endl;
	}
	return;
}

double MoneyGet(per* theagent,per* otheragent)
{
    if(theagent->Stra == false&&otheragent->Stra==false)
        return C_C;
    else if(theagent->Stra == false&&otheragent->Stra==true)
       return C_D;
    else if(theagent->Stra==true&&otheragent->Stra==false)
       return D_C_B;
    else if(theagent->Stra==true&&otheragent->Stra==true)
        return D_D;
        return 0;
}
//Change Stra
void StraChange(int x,int y, per BackUP[][LengthY],double k)
{
   // FindCircle(x,y);
   unsigned int MaxGainTem = BackUP[x][y].MoneyBet + BackUP[x][y].MoneyCoo;//self - gain
 //  int k = rand()%8;
  // cout<<k<<"\t"<<CircleTem[0][k]<<"\t"<<CircleTem[1][k]<<endl;
   int MaxGainIndex = -1; // The Coordinate of Max Gain Per,-1 means itself
   for(int i=0;i!=8;i++)
        if((BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyBet + BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyCoo) > MaxGainTem && BackUP[CircleTem[0][i]][CircleTem[1][i]].LiveOrDie == 0)
        {
            MaxGainTem = (BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyBet + BackUP[CircleTem[0][i]][CircleTem[1][i]].MoneyCoo);
			MaxGainIndex = i; 
        }
		//cout<<MaxGainTem<<endl;
		//fileBoyi<<MaxGainTem<<endl;
		//gen decide change or not

	int check = rand()%100;
	if(MaxGainIndex!=-1 && check > k*100*BackUP[x][y].ChangeGeneNum/BackUP[x][y].Age)
	//if(true)
	{
		Agent[x][y].Stra = BackUP[CircleTem[0][MaxGainIndex]][CircleTem[1][MaxGainIndex]].Stra;
	//	fileChange<<"i am changing"<<endl;
	}
	//else
	//	fileChange<<"i am not changing"<<endl;
    //!!!!!!!!!!!!!CHANGE ABOUT THE ANOTHER!!!!!!!!!!!!!!
	//Agent[x][y].Stra = StrategyChange_second(&BackUP[x][y],&BackUP[MaxGain[0]][MaxGain[1]])?BackUP[x][y].Stra:BackUP[MaxGain[0]][MaxGain[1]].Stra;
}
//trade
void Trad(int x,int y, per BackUP[][LengthY])
{
// FindCircle(x,y);
	int k = 0;
	int n = 0;
    double tempMoneyLast = 0;
	double tempMoneyCoo = 0;
	double tempMoneyBet = 0;
	for(k=0;k!=8;k++)
	{
		if(BackUP[CircleTem[0][k]][CircleTem[1][k]].LiveOrDie == 0)
		{
    		if(BackUP[x][y].Stra == 0)//coo
    		{
    	   	 	tempMoneyLast += MoneyGet(&BackUP[x][y],&BackUP[CircleTem[0][k]][CircleTem[1][k]]);
    	   	 	tempMoneyCoo += tempMoneyLast;
				//cout<<Agent[x][y].MoneyCoo<<endl;
		    }
   		    else
   			 {
   		     	tempMoneyLast += MoneyGet(&BackUP[x][y],&BackUP[CircleTem[0][k]][CircleTem[1][k]]);
   		     	tempMoneyBet += tempMoneyLast;
				//cout<<Agent[x][y].MoneyBet<<endl;
		     }
			 n++;
		}
		//cout<<"n is : "<<n<<endl;	
	}
	Agent[x][y].MoneyLast = tempMoneyLast/n;
    Agent[x][y].MoneyCoo = tempMoneyCoo/n;
    Agent[x][y].MoneyBet = tempMoneyBet/n;
}

int main()
{
	fileBoyi.open("fileBoyi.txt",ofstream::out |  ofstream::trunc);
	fileSquare.open("fileSquare.txt",ofstream::out |  ofstream::trunc);
	fileChange.open("fileChange.txt",ofstream::out |  ofstream::trunc);
	fileAge.open("fileAge.txt",ofstream::out |  ofstream::trunc);
	fileGen.open("fileGen.txt",ofstream::out |  ofstream::trunc);
	int cooage[32]; 		//count the num of Cooperation each age
	int betage[32];
	int coogennum[32];
	int begennum[32];
	unsigned int GenNum;
	double coonum = 0;
	double benum = 0;
	double Bili = 0;
	double k_ = 0.0;
	D_C_B = 1.0;
	while(D_C_B < 2){
		D_C_B += 0.2;
		k_=0;
	while(k_ < 1){
		k_+= 0.05;
	srand((unsigned)time(NULL));
	per AgentBackUp[LengthX][LengthY];
	//bool flag = 0;
	Nt=0;
	Intial();
	Pt = GetAverage(AgentBackUp);//get average + copy to back up
	for(int t=0;t!=1000;t++)
	{
		for(int i=0;i!=32;i++)
		{
			cooage[i] = 0;
			betage[i] = 0;
			coogennum[i] = 0;
			begennum[i] = 0;
		}
		coonum = 0;
		benum = 0;
		GenNum = 0;
		/* 
		cout<<"the step is : "<<t<<endl;
		if(t==0 || t==10 || t==50 || t==100 || t==1000 || t== 1999)
		{
			fileSquare<<"The step is "<<t<<endl;	
			flag = 1;			
		}
		else
			flag = 0;
		fileBoyi<<t<<"\t";
		cout<<Nt<<"\t";
		fileBoyi<<Nt<<"\t";
		cout<<Pt<<endl;
		fileBoyi<<Pt<<endl;
		*/
	
		for (int i=0;i!=LengthX;i++)
            for(int j=0;j!=LengthY;j++)
            {
				/*
				if(flag)
					{
						if(Agent[i][j].LiveOrDie == 0)
							fileSquare<<Agent[i][j].Stra<<"\t";
						else
							fileSquare<<"-1"<<"\t";
						if(j==99)
							fileSquare<<endl;
					}
				*/
				if(Agent[i][j].LiveOrDie == 0)
				{
					if(Agent[i][j].Stra == 0)
					{
						coonum++;
						cooage[(int)Agent[i][j].Age]++;
					}
					else
					{
						benum++;
						betage[(int)Agent[i][j].Age]++;
					}
					
						coogennum[(int)Agent[i][j].Age] += Agent[i][j].ChangeGeneNum;
						begennum[(int)Agent[i][j].Age] += (Agent[i][j].Age - Agent[i][j].ChangeGeneNum);
				   	
				}
			//	cout<<AgentBackUp[i][j].Stra<<"\t"<<Agent[i][j].Stra<<"\n";
				FindCircle(i,j);
                if (Agent[i][j].LiveOrDie==0)
                {
                    //get the money
                    Trad(i,j,AgentBackUp);
					StraChange(i,j,AgentBackUp,k_);
                    Age(i,j);
                    OpenGen(i,j);
                    Die(i,j);
                    //if(Agent[i][j].LiveOrDie==0&&Agent[i][j].Age>=ReproductionAge)
				}
				else
				{
					//ReproductionAge ?
					Reproduction(i,j,AgentBackUp);
				}
		}

			Bili = coonum/(coonum+benum);
			//fileChange<<t<<"\t"<<coonum<<"\t"<<benum<<"\t"<<Bili<<endl;
			Pt = GetAverage(AgentBackUp);//get average + copy to back up
		//	fileChange<<t<<"\t"<<Bili<<"\n";
	}
	fileChange<<k_<<"\t"<<D_C_B<<"\t"<<Bili<<"\t"<<Pt<<endl;
	cout<<k_<<"\t"<<D_C_B<<"\t"<<Bili<<"\t"<<Pt<<endl;
	fileAge<<k_<<"\t"<<D_C_B<<endl;
	cout<<k_<<"\t"<<D_C_B<<endl;
	fileGen<<k_<<"\t"<<D_C_B<<endl;
	for(int i=0;i!=32;i++)
	{
		fileAge<<cooage[i]<<"\t";
		cout<<cooage[i]<<"\t";
		fileGen<<coogennum[i]<<"\t";
	} 
	fileAge<<endl;
	fileGen<<endl;
	cout<<endl;
	for(int i=0;i!=32;i++)
	{
		fileAge<<betage[i]<<"\t";
		cout<<betage[i]<<"\t";
		fileGen<<begennum[i]<<"\t";
	}
	fileAge<<endl;
	fileGen<<endl;
	cout<<endl;

	fileGen<<k_<<"\t"<<D_C_B<<"\t";
	fileGen<<GenNum<<endl;
	}
}
	return 0;
}

