#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "/home/user/job/squire3/routine/stat.hpp"
#include "/home/user/job/squire3/routine/linRegr.hpp"
#include "/home/user/job/squire3/routine/linRegrGSLwrappers.hpp"
#include "/home/user/job/squire3/routine/matrixProc.hpp"
//#include "/home/user/education/stat/linRegr.hpp"
#include <gsl/gsl_cdf.h>
using namespace std;

bool signalIdentifyGaussian(vector <double> input,double criticalPvalue) {
	bool hasSignal=false;
	vector <double> stats=stat(input);
	double meanValue=stats[0];
	double variance=stats[1];
	cout<<meanValue<<" "<<variance<<endl;
	double inputMaxValue=(*max_element(input.begin(),input.end())-meanValue)/sqrt(variance);//the maximum value of the input transformed to std gaussian distribution
	double pValue=gsl_cdf_ugaussian_Q(inputMaxValue);//calculate the probability that larger value is observed
	cout<<"p-val="<<pValue<<endl;
	if(pValue>=criticalPvalue) {
		hasSignal=true;
	}
	return hasSignal;
}

double signalIdentifyGamma(vector <double> input) {
	vector <double> stats=stat(input);
	double mean=stats[0];
	double variance=stats[1];
	double pheta=variance/mean;
	double k=(mean*mean)/variance;
	double inputMaxValue=*max_element(input.begin(),input.end());//the maximum value of the input
	double pValue=gsl_cdf_gamma_Q(inputMaxValue,k,pheta);
	return pValue;
}

double signalProc_mean(vector <double> input) {
	double inputSize=(double)input.size();
	double inputSum=0.0;
	for(int i=0;i<input.size();i++) {
		inputSum+=input[i];
	}
	double result=inputSum/inputSize;
	return result;
}

double signalProc_variance(vector <double> input) {
	double inputSize=(double)input.size();
	double inputSum=0.0;
	for(int i=0;i<input.size();i++) {
		inputSum+=input[i];
	}
	double mean=inputSum/inputSize;
	inputSum=0.0;
	for(int i=0;i<input.size();i++) {
		inputSum+=(input[i]-mean)*(input[i]-mean);
	}
	double result=inputSum/inputSize;
	return result;
}

double** signalProc_autocovarianceMatrix(vector <double> input) {
	double** result=new double* [input.size()];
	for(int i=0;i<input.size();i++) {
		result[i]=new double [input.size()];
	}
	double mean=signalProc_mean(input);
	double n=(double)input.size();
	for(int i=0;i<input.size();i++) {
		for(int j=0;(i+j)<input.size();j++) {
			result[i][j]=(input[j]-mean)*(input[i+j]-mean);
			result[j][i]=result[i][j];
		}
	}

	return result;
}

double** signalProc_autocorrelationMatrix(vector <double> input) {
	double** result=new double* [input.size()];
	for(int i=0;i<input.size();i++) {
		result[i]=new double [input.size()];
	}
	double mean=signalProc_mean(input);
	double variance=signalProc_variance(input);
	for(int i=0;i<input.size();i++) {
		for(int j=i;j<input.size();j++) {
			result[i][j]=((input[i]-mean)*(input[j]-mean))/variance;
			result[j][i]=result[i][j];
		}
	}
	return result;
}

double* signalProc_autocovarianceFunction(vector <double> input) {
	double* result=new double [input.size()];
	for(int i=0;i<input.size();i++) {
		result[i]=0.0;
	}
	double mean=signalProc_mean(input);
	for(int i=0;i<input.size();i++) {
		for(int j=0;(i+j)<input.size();j++) {
			result[i]+=(input[j]-mean)*(input[i+j]-mean);
		}
	}
	double n=(double)input.size();
	for(int i=0;i<input.size();i++) {
		result[i]=result[i]/n;
	}
	return result;
}

double* signalProc_autocorrelationFunction(vector <double> input) {
	double* result=new double [input.size()];
	for(int i=0;i<input.size();i++) {
		result[i]=0.0;
	}
	double* normFactors=new double [input.size()];
	for(int i=0;i<input.size();i++) {
		normFactors[i]=0;
	}
	double mean=signalProc_mean(input);
	double variance=signalProc_variance(input);
	for(int i=0;i<input.size();i++) {
		for(int j=0;(i+j)<input.size();j++) {
			result[i]+=(input[j]-mean)*(input[i+j]-mean);
		}
	}
	double n=(double)input.size();
	for(int i=0;i<input.size();i++) {
		result[i]=(result[i]/variance)/n;
	}
	return result;
}

double* signalProc_partialAutocorrelationFunction(vector <double> inputDirect,int order) {
	vector <double> input(inputDirect.size());
	for(int i=0;i<input.size();i++) {
		input[input.size()-1-i]=inputDirect[i];
	}
	double* result=new double [order-2];//partial autocorrelation function
	if(order==0) {
		order=input.size()-1;
	}
	vector <vector <double> > X(input.size()-1);//this is the input for multiple regression; the design matrix is to be produced from this
	for(int i=1;i<input.size();i++) {
		X[i-1].push_back(1.0);
		X[i-1].push_back(input[i]);
	}
	vector <double> Y;//input for multiple regression
	vector <vector <double> > residualsMatrix(order-1);//residuals
	vector <double> lastCoefficients;
	for(int i=1;i<order;i++) {
		X.pop_back();//at every iteration one less data point is used for computations; thus one vector is removed from matrix X so all vectors have always equal size
		for(int j=i+1;j<input.size();j++) {//note that design matrix must be supplied in the form 'one vector per variable'; 
			X[j-i-1].push_back(input[j]);
		}
		Y=vector <double>(input.begin(),input.end()-i-1);//certain number of values is removed
		//cout<<"Y: "<<Y.size()<<"\n";
		//cout<<"X: "<<X.size()<<"\n";
		//vector <double> OLSresult=linRegrOLS(X2,Y);
		linRegrGSLresult linRegrResult;
		linRegrResult=linRegrGSLwrapper1(X,Y,0.01);
		lastCoefficients.push_back(linRegrResult.betas[linRegrResult.betas.size()-1]);
		if(i==1) {
			for(int k=0;k<linRegrResult.betas.size();k++) {
				cout<<linRegrResult.betas[k]<<"	";
			}
			cout<<endl;
			for(int l=0;l<10;l++) {
				cout<<X[l][0]<<"	"<<X[l][1]<<"	"<<X[l][2]<<"	"<<Y[l]<<endl;
			}
		}
		residualsMatrix[i-1]=linRegrResult.residuals;
		//cout<<linRegrResult.residuals.size()<<endl;
	}
	ofstream residualsWrite("residuals.dat",ofstream::out);
	for(int i=0;i<residualsMatrix.size();i++) {
		for(int j=0;j<residualsMatrix[i].size();j++) {
			residualsWrite<<residualsMatrix[i][j]<<"	";
		}
		residualsWrite<<endl;
	}
	cout<<"MultiFit done"<<endl;
	for(int i=0;i<(residualsMatrix.size()-2);i++) {
		residualsMatrix[i].pop_back();
		double PearsonCorrCoeff=corrCoeff(residualsMatrix[i],residualsMatrix[i+1]);
		result[i]=PearsonCorrCoeff;
	}
	ofstream lastCoefficientsWrite("lastCoeffs.dat",ofstream::out);
	for(int i=0;i<lastCoefficients.size();i++) {
	       lastCoefficientsWrite<<lastCoefficients[i]<<"\n";
	}	       
	return result;
}
