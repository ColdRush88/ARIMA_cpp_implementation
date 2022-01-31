#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include "./arrayProc.hpp"
using namespace std;

vector <double> stat(vector <double> data) {
	vector <double> result;
	double sum=0.0;
	for(int i=0;i<data.size();i++) {
		sum+=data[i];
	}
	double meanVal=sum/(double)data.size();
	sum=0.0;
	for(int i=0;i<data.size();i++) {
		sum+=pow(data[i]-meanVal,2);
	}
	double varVal=sum/((double)data.size()-1.0);
	result.push_back(meanVal);
	result.push_back(varVal);
	return result;
}
double median(vector <double> data) {
	sort(data.begin(),data.end());
	int n=data.size();
	int medianValueIndex;
	double medianValue;
	if(n%2!=0) {
		medianValueIndex=((n-1)/2)+1;
		medianValue=data[medianValueIndex];
	}
	if(n%2==0) {
		medianValue=(data[(n-1)/2]+data[(n+1)/2])/2.0;
	}
	return medianValue;
}

vector <double> statInt(vector <int> dataInput) {
	vector <double> data(dataInput.size());
	for(int i=0;i<dataInput.size();i++) {
		data[i]=(double)dataInput[i];
	}
	vector <double> result;
	double sum=0.0;
	for(int i=0;i<data.size();i++) {
		sum+=data[i];
	}
	double meanVal=sum/(double)data.size();
	sum=0.0;
	for(int i=0;i<data.size();i++) {
		sum+=pow(data[i]-meanVal,2);
	}
	double varVal=sum/((double)data.size()-1.0);
	result.push_back(meanVal);
	result.push_back(varVal);
	return result;
}
double statSkewness(vector <double> data, double mean, double stDev) {
	int N=data.size();
	double skew=0.0;
	for(int i=0;i<N;i++) {
		skew+=pow((data[i]-mean)/stDev,3);
	}
	skew=skew/(double)N;
	return skew;
}

double statKurtosis(vector <double> data, double mean, double stDev) {
	int N=data.size();
	double kurt=0.0;
	for(int i=0;i<N;i++) {
		kurt+=pow((data[i]-mean)/stDev,4);
	}
	kurt=(kurt/(double)N)-3.0;
	return kurt;
}



double corrCoeff(vector <double> data1, vector <double> data2) {
double corrCoeff=0.0;
double dataSize=(double)data1.size();
vector <double> stat1=stat(data1);
vector <double> stat2=stat(data2);
double mean1=stat1[0];
double mean2=stat2[0];
double var1=stat1[1];
double var2=stat2[1];
for(int i=0;i<data1.size();i++) {
corrCoeff+=(data1[i]-mean1)*(data2[i]-mean2);
}
corrCoeff=corrCoeff/(dataSize*sqrt(var1*var2));
return corrCoeff;
}

double corrCoeff2(vector <vector <double> > data) {
vector <double> data1;
vector <double> data2;
for(int i=0;i<data.size();i++) {
data1.push_back(data[i][0]);
data2.push_back(data[i][1]);
}
double corrCoeff=0.0;
double dataSize=(double)data1.size();
vector <double> stat1=stat(data1);
vector <double> stat2=stat(data2);
double mean1=stat1[0];
double mean2=stat2[0];
double var1=stat1[1];
double var2=stat2[1];
for(int i=0;i<data1.size();i++) {
corrCoeff+=(data1[i]-mean1)*(data2[i]-mean2);
}
corrCoeff=corrCoeff/(dataSize*sqrt(var1*var2));
return corrCoeff;
}

double corrCoeffSpearman(vector <double> data1,vector <double> data2) {
	vector <int> ranking1(data1.size());
	vector <int> ranking2(data2.size());
	for(int i=0;i<data1.size();i++) {
		ranking1[i]=i+1;
		ranking2[i]=i+1;
	}
	vector <double>* data1Ptr=&data1;
	vector <double>* data2Ptr=&data2;
	vector <int>* ranking1Ptr=&ranking1;
	vector <int>* ranking2Ptr=&ranking2;
	coSortVectors(data1Ptr,ranking1Ptr);
	coSortVectors(data2Ptr,ranking2Ptr);
	double squaredRankingDiffsSum=0.0;
	for(int i=0;i<ranking1.size();i++) {
		int rank1,rank2;
		int currentIndex=i+1;
		for(int j=0;j<ranking1.size();j++) {
			if(ranking1[j]==currentIndex) {
				rank1=j;
			}
			if(ranking2[j]==currentIndex) {
				rank2=j;
			}
		}
		squaredRankingDiffsSum+=(double)((rank1-rank2)*(rank1-rank2));
	}
	double n=(double)data1.size();
	double spearmanCorrCoeff=1.0-((6.0*squaredRankingDiffsSum)/(n*(n*n-1.0)));
	return spearmanCorrCoeff;
}

//Rank data according to the sum of ranks of predictors.
//Supposedly data is certain quantity that can be predicted by measures in the predictorData matrix, where one vector correspond to one such measure. Data points are ranked according to each predictor and rank of each data point according to each predictor is summed together. Thus data points which were put high by all predictors tend to score high overall.
struct rankSumResult {
	double corrCoeffSpearman;
	vector <double> rankSums;
};

rankSumResult corrCoeffSpearman_rankSum(vector <double> data,vector <vector <double> > predictorData) {
	vector <vector <int> > predictorRankings(predictorData.size());
	for(int i=0;i<predictorData.size();i++) {
		for(int j=0;j<predictorData[i].size();j++) {
			predictorRankings[i].push_back(j+1);
		}
	}
	vector <double> dataRanking(data.size());
	for(int i=0;i<data.size();i++) {
		dataRanking[i]=(double)(i+1);
	}
	vector <double>* dataPtr=&data;
	vector <double>* dataRankingPtr=&dataRanking;
	vector <vector <double>* > predictorDataPtrs(predictorData.size());
	vector <vector <int>* > predictorRankingPtrs(predictorRankings.size());
	for(int i=0;i<predictorData.size();i++) {
		predictorDataPtrs[i]=&predictorData[i];
		predictorRankingPtrs[i]=&predictorRankings[i];
		coSortVectors(predictorDataPtrs[i],predictorRankingPtrs[i]);
	}
	coSortVectors2(dataPtr,dataRankingPtr);
	vector <double> rankSums(data.size(),0.0);
	for(int i=0;i<data.size();i++) {
		int currentIndex=i+1;
		for(int j=0;j<predictorData.size();j++) {
			for(int k=0;k<predictorData[j].size();k++) {
				if(predictorRankings[j][k]==currentIndex) {
					rankSums[i]+=(double)(k+1);
					break;
				}
			}
		}
	}
	rankSumResult result;
	double spearmanCorrCoeff=corrCoeffSpearman(dataRanking,rankSums);
	result.corrCoeffSpearman=spearmanCorrCoeff;
	result.rankSums=rankSums;
	return result;
}


vector <int> histCreate(double minVal,double maxVal,double step,vector <double> data) {
vector <int> hist;
int pointNum=abs(maxVal-minVal)/step;
hist.resize(pointNum);
for(int i=0;i<hist.size();i++) {
hist[i]=0;
}
for(int i=0;i<data.size();i++) {
double point=abs(data[i]-minVal)/step;
if(isnan(point)) {
continue;
}
hist[(int)point]++;
}
return hist;
}

vector <int> histCreate2(double minVal,double maxVal,double step,vector <double> data,vector <int> counters) {
vector <int> hist;
int pointNum=abs(maxVal-minVal)/step;
hist.resize(pointNum+1);
for(int i=0;i<hist.size();i++) {
hist[i]=0;
}
for(int i=0;i<data.size();i++) {
double point=abs(data[i]-minVal)/step;
if(isnan(point)) {
continue;
}
hist[(int)point]+=counters[i];
}
return hist;
}

double tTest1(vector <double> data1,vector <double> data2) {
	double n1=(double)data1.size();
	double n2=(double)data2.size();
	vector <double> stats1=stat(data1);
	vector <double> stats2=stat(data2);
	double mean1=stats1[0];
	double mean2=stats2[0];
	double variance1=stats1[1];
	double variance2=stats2[1];
	double pooledVariance=sqrt(variance1/n1+variance2/n2);
	double tStatistic=abs(mean1-mean2)/pooledVariance;
	return tStatistic;
}

double tTest2(vector <double> data) {
	double N=(double)data.size();
	vector <double> stats=stat(data);
	double mean=stats[0];
	double variance=stats[1];
	double tStatistic=mean/(sqrt(variance/N));
	return tStatistic;
}

double autocorrCoeffCompute(vector <double> input,int lag) {
	vector <double> inputStats=stat(input);
	double mean=inputStats[0];
	double sum1=0.0;
	double sum2=0.0;
	for(int i=lag;i<input.size();i++) {
		sum1+=(input[i]-mean)*(input[i-lag]-mean);
		sum2+=(input[i]-mean)*(input[i]-mean);
	}
	double autocorrCoeff=sum1/sum2;
	return autocorrCoeff;
}

vector <vector <double> > autocorrFunctionCompute(vector <double> input,int minLag,int maxLag) {
	vector <double> autocorrValues(maxLag-minLag);
	vector <double> lagValues(maxLag-minLag);
	for(int i=0;i<(maxLag-minLag);i++) {
		autocorrValues[i]=autocorrCoeffCompute(input,minLag+i);
		lagValues[i]=(double)(minLag+i);
	}
	vector <vector <double> > result(2);
	result[0]=lagValues;
	result[1]=autocorrValues;
	return result;
}
