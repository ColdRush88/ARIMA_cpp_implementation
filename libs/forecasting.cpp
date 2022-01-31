#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

//Computes psi and pi weights as they are defined in the book "Time series analysis: forecasting and control", Box, Jenkins, Reinsel, Ljung.
//z_t= sum (from 0 to Inf) pi_i * a_(t-i)
//a_i = sum(from 0 to Inf) psi_i * z_(t-i)
//Currently 10 weights are computed. This must be enough to see whether weights converge or not. If weights converge the series is said to be invertible. Another requirement for inveribility is that absolute value of AR and MA parameters must be less than 1.
vector <vector <double> > arima_computeWeights(vector <double> params,unsigned int ARorder,unsigned int MAorder) {
	//Compute psi weights
	vector <double> psiWeights;
	psiWeights.push_back(1.0);
	if(ARorder==0) {
		for(int i=0;i<params.size();i++) {
			psiWeights.push_back(-params[i]);
		}
	}
	if(ARorder!=0) {
		if(MAorder!=0) {
			psiWeights.push_back(params[0]-params[ARorder]);
		}
		if(MAorder==0) {
			psiWeights.push_back(params[0]);
		}
		for(int i=2;i<10;i++) {
			double value=0.0;
			for(int j=0;j<ARorder;j++) {
				if((psiWeights.size()-j-1)>=0) {
					value+=params[j]*psiWeights[psiWeights.size()-j-1];
				}
			}
			if(MAorder!=0) {
				if((ARorder+i-1)<(params.size())) {
					value-=params[ARorder+i-1];
				}
			}
			psiWeights.push_back(value);
		}
	}
	//Compute pi weights
	vector <double> piWeights;
	piWeights.push_back(1.0);
	if(MAorder==0) {
		for(int i=0;i<params.size();i++) {
			piWeights.push_back(-params[i]);
		}
	}
	if(MAorder!=0) {
		if(ARorder!=0) {
			piWeights.push_back(params[ARorder]-params[0]);
		}
		if(ARorder==0) {
			piWeights.push_back(-params[0]);
		}
		for(int i=2;i<10;i++) {
			double value=0.0;
			for(int j=i;j>=0;j--) {
				value+=params[ARorder+i-j]*piWeights[j-1];
			}
			if(ARorder!=0) {
				value-=params[i-1];
			}
			piWeights.push_back(value);
		}
	}
	vector <vector <double> > result(2);
	result[0]=psiWeights;
	result[1]=piWeights;
	////////////////////////////////////
	return result;
}

//The main function for forecasting. The last values of vectors z (input data) and a (called shock values or innovations) are used for forecasting.
vector <double> forecasting(vector <double> z,vector <double> a,vector <double> params,unsigned int ARorder,unsigned int MAorder,unsigned int forecastLength) {
	//Forecasting; difference form
	vector <double> forecasts(forecastLength,0.0);
	for(int i=0;i<forecastLength;i++) {
		for(int j=0;j<ARorder;j++) {
			forecasts[i]+=params[j]*z[z.size()-j-1];
		}
		for(int j=0;j<MAorder;j++) {
			forecasts[i]+=params[ARorder+j]*a[a.size()-j-1];//Note that weighted shock values are added (not subtracted), because moving average parameters are already signed.
		}
		z.push_back(forecasts[i]);
	}
	///////////////////////////////////
	return forecasts; 
}

//The type of the result of the function forecast_testModel
struct forecast_testModel_result {
	std::vector <std::vector <double> > forecastMatrix;
	std::vector <std::vector <double> > residuals;
	std::vector <std::vector <double> > residualsSquared;
	std::vector <std::vector <double> > residualsAbs;
	std::vector <double> residualsSum;
	std::vector <double> residualsSquaredSum;
	std::vector <double> residualsAbsSum;
};

//Accepts input data (vector z) and it's model parameters. Using every data point forecasts future points (how many is defined by forecastLength). Then compares the result with real data. Output various statistics, like sum of residuals, sum of squared residuals etc.
forecast_testModel_result forecast_testModel(vector <double> z,vector <double> a,vector <double> params,unsigned int ARorder,unsigned int MAorder,unsigned int forecastLength) {
	vector <vector <double> > forecastValues(z.size());
	for(int i=0;i<forecastValues.size();i++) {
		forecastValues[i].resize(forecastLength);
	}
	//!!!Note that forecastValues[] indices are +1 relative to indices of z[]. For example, forecastValues[0] is the prediction of z[1].
	vector <double> z_copy=z;
	vector <double> a_copy=a;
	z_copy.pop_back();
	a_copy.pop_back();
	for(int i=0;i<(forecastValues.size()-1);i++) {
		vector <double> forecasts=forecasting(z_copy,a_copy,params,ARorder,MAorder,forecastLength);
		for(int j=0;j<forecasts.size();j++) {
			if((z_copy.size()+j)<forecastValues.size()) {
				forecastValues[z_copy.size()+j][j]=forecasts[j];
			}
		}
		z_copy.pop_back();
		a_copy.pop_back();
	}
	//Compute residuals
	vector <vector <double> > residuals(forecastValues.size()-1);
	vector <vector <double> > residualsSquared(forecastValues.size()-1);
	vector <vector <double> > residualsAbs(forecastValues.size()-1);
	vector <double> residualsSum(forecastLength,0.0);
	vector <double> residualsSquaredSum(forecastLength,0.0);
	vector <double> residualsAbsSum(forecastLength,0.0);
	for(int i=0;i<(forecastValues.size()-1);i++) {
		residuals[i].resize(forecastLength);
		residualsSquared[i].resize(forecastLength);
		residualsAbs[i].resize(forecastLength);
	}
	for(int i=0;i<(forecastValues.size()-1);i++) {
		for(int j=0;j<forecastValues[i].size();j++) {
			residuals[i][j]=z[i+1]-forecastValues[i][j];
			residualsSquared[i][j]=residuals[i][j]*residuals[i][j];
			residualsAbs[i][j]=abs(residuals[i][j]);
			residualsSum[j]+=residuals[i][j];
			residualsSquaredSum[j]+=residualsSquared[i][j];
			residualsAbsSum[j]+=residualsAbs[i][j];
		}
	}
	string residualsFile="residuals.dat";
	string residualsSquaredFile="residualsSquared.dat";
	string residualsAbsFile="residualsAbs.dat";
	ofstream residualsWrite(residualsFile.c_str(),ofstream::out);
	ofstream residualsSquaredWrite(residualsSquaredFile.c_str(),ofstream::out);
	ofstream residualsAbsWrite(residualsAbsFile.c_str(),ofstream::out);
	for(int i=0;i<(forecastValues.size()-1);i++) {
		for(int j=0;j<residuals[i].size();j++) {
			residualsWrite<<residuals[i][j];
			residualsSquaredWrite<<residualsSquared[i][j];
			residualsAbsWrite<<residualsAbs[i][j];
			if(j!=(residuals[i].size()-1)) {
				residualsWrite<<"	";
				residualsSquaredWrite<<"	";
				residualsAbsWrite<<"	";
			}
		}
		residualsWrite<<"\n";
		residualsSquaredWrite<<"\n";
		residualsAbsWrite<<"\n";
	}
	forecast_testModel_result result;
	result.forecastMatrix=forecastValues;
	result.residualsSum=residualsSum;
	result.residualsSquaredSum=residualsSquaredSum;
	result.residualsAbsSum=residualsAbsSum;
	result.residuals=residuals;
	result.residualsSquared=residualsSquared;
	result.residualsAbs=residualsAbs;
	return result;
}

/*
int main() {
}
*/
