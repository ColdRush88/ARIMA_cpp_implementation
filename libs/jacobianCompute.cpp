#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

//This is auxiliary function that is called from parameterEstimation().
//Computes Jacobian matrix for ARMA model. 
void arima_computeJacobian(vector <double> z,vector <double> a,unsigned int ARorder,unsigned int MAorder) {
	unsigned int startIndex;
	if(ARorder>=MAorder) {
		startIndex=ARorder;
	}
	if(MAorder>ARorder) {
		startIndex=MAorder;
	}
	vector <double> firstDerivatives(ARorder+MAorder);
	vector <vector <double> > secondDerivatives(ARorder+MAorder);
	for(int i=0;i<secondDerivatives.size();i++) {
		secondDerivatives[i].resize(ARorder+MAorder,0.0);
	}
	for(int j=0;j<firstDerivatives.size();j++) {
		double derivativeValue=0.0;
		for(int i=startIndex;i<a.size();i++) {
			if(j<ARorder) {
				derivativeValue+=a[i]*z[i-j-1];
			}
			if(j>=ARorder) {
				derivativeValue+=a[i]*a[i-j+ARorder-1];
			}
		}
		firstDerivatives[j]=-2.0*derivativeValue;
	}
	for(int k=0;k<secondDerivatives.size();k++) {
		for(int j=0;j<secondDerivatives.size();j++) {
			double derivativeValue=0.0;
			for(int i=(startIndex*2);i<a.size();i++) {
				if(k<ARorder && j>=ARorder) {
					derivativeValue+=z[i-k-1]*a[i-j+ARorder-1]+z[i-j+ARorder-k-2]*a[i-1];
				}
				if(k>=ARorder && j<ARorder) {
					derivativeValue+=a[i-k+ARorder-1]*z[i-j-1]+a[i-j-k+ARorder-2]*z[i-1];
				}
				if(k<ARorder && j<ARorder) {
					derivativeValue+=z[i-k-1]*z[i-j-1]+z[i-j-k-2]*z[i-1];
				}
				if(k>=ARorder && j>=ARorder) {
					derivativeValue+=a[i-k+ARorder-1]*a[i-j+ARorder-1]+a[i-j-k+2*ARorder-2]*a[i-1];
				}
			}
			secondDerivatives[k][j]=2.0*derivativeValue;
		}
	}
	vector <vector <double> > informationMatrix(ARorder+MAorder);
	for(int i=0;i<informationMatrix.size();i++) {
		informationMatrix[i].resize(ARorder+MAorder);
	}
	double n=(double)a.size();
	double pi=3.14;
	double aSqSum=0.0;
	for(int i=0;i<a.size();i++) {
		aSqSum+=a[i]*a[i];
	}
	for(int i=0;i<informationMatrix.size();i++) {
		for(int j=0;j<informationMatrix[i].size();j++) {
			informationMatrix[i][j]=((n*n)/(4.0*pi))*(firstDerivatives[i]*firstDerivatives[j]*(1.0/(aSqSum*aSqSum))+secondDerivatives[i][j]*(1.0/aSqSum));
		}
	}
	string outputFile="jacobian.dat";
	ofstream outputWrite(outputFile.c_str(),ofstream::out);
	for(int i=0;i<informationMatrix.size();i++) {
		for(int j=0;j<informationMatrix[i].size();j++) {
			outputWrite<<informationMatrix[i][j]<<"	";
		}
		outputWrite<<endl;
	}
}

