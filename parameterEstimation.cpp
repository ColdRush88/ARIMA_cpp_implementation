#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>
#include "libs/jacobianCompute.hpp"
#include "libs/arrayProc.hpp"
#include "libs/fileProc.hpp"
#include "libs/forecasting.hpp"
using namespace std;

//Parameter for model fitting
struct estimateParameters_params {
	unsigned int ARorder;
	unsigned int MAorder;
	vector <double> data;//data to fit
};

//This likelihood function is maximized to evaluate model parameters.
double normalLogLikelihoodFunction(const gsl_vector*x,void*params) {
	estimateParameters_params*p=(estimateParameters_params*)params;
	unsigned int ARorder=p->ARorder;
	unsigned int MAorder=p->MAorder;
	vector <double> z=p->data;
	unsigned int n=z.size();
	//Copy gsl_vector x to vector 
	vector <double> variables(ARorder+MAorder);
	for(int i=0;i<(ARorder+MAorder);i++) {
		variables[i]=gsl_vector_get(x,i);
	}
	vector <double> a(n,0.0);
	/////////////////////////////
	for(int i=MAorder;i<n;i++) {
		a[i]=z[i];
		for(int j=0;j<ARorder;j++) {
			a[i]-=variables[j]*z[i-j-1];
		}
		for(int j=0;j<MAorder;j++) {
			a[i]+=variables[ARorder+j]*a[i-j-1];
		}
	}
	double aSqSum=0.0;
	for(int i=0;i<n;i++) {
		aSqSum+=a[i]*a[i];
	}
	double sigmaA=1.0;
	for(int i=0;i<MAorder;i++) {
		//sigmaA+=gsl_vector_get(x,ARorder+i)*gsl_vector_get(x,ARorder+i);//computing variance of shocks through variables theta (theoretical value)
		sigmaA=aSqSum/(double)n;//computing sample variance of shocks
	}
	double f=((double)(n)/2.0)*log(2.0*3.14*sigmaA)+aSqSum/(2.0*sigmaA);
	return f;
}

//Computes shock values (or in some books they are called innovations)
vector <double> arima_computeShockValues(vector <double> z,vector <double> params,unsigned int ARorder,unsigned int MAorder) {
	unsigned int n=z.size();
	vector <double> a(n,0.0);
	for(int i=MAorder;i<n;i++) {
		a[i]=z[i];
		for(int j=0;j<ARorder;j++) {
			a[i]-=params[j]*z[i-j-1];
		}
		for(int j=0;j<MAorder;j++) {
			a[i]+=params[ARorder+j]*a[i-j-1];
		}
	}
	return a;
}

//Wrapper for GSL function. Minimizes normalLogLikelihood() function
vector <double> gsl_multimin_fminimizer_wrapper(double(func)(const gsl_vector*,void*),estimateParameters_params params,unsigned int problemOrder) {
	const gsl_multimin_fminimizer_type*T=gsl_multimin_fminimizer_nmsimplex2;
	gsl_multimin_fminimizer*s=NULL;
	gsl_vector*ss,*x;
	gsl_multimin_function minex_func;
	size_t iter=0;
	size_t max_iter=1000;
	int status;
	double size;
	x=gsl_vector_alloc(problemOrder);
	ss=gsl_vector_alloc(problemOrder);
	gsl_vector_set_all(x,0.0);
	gsl_vector_set_all(ss,1.0);
	minex_func.n=problemOrder;
	minex_func.f=func;
	minex_func.params=&params;
	s=gsl_multimin_fminimizer_alloc(T,problemOrder);
	gsl_multimin_fminimizer_set(s,&minex_func,x,ss);
	vector <double> result(problemOrder);
	do {
		iter++;
		status=gsl_multimin_fminimizer_iterate(s);
		if(status) {
			break;
		}
		size=gsl_multimin_fminimizer_size(s);
		status=gsl_multimin_test_size(size,1e-2);
		if(status==GSL_SUCCESS) {
			printf("Convergence\n");
			for(int i=0;i<problemOrder;i++) {
				result[i]=gsl_vector_get(s->x,i);
			}
		}
	}
	while(status==GSL_CONTINUE && iter<max_iter);
	gsl_vector_free(x);
	gsl_vector_free(ss);
	gsl_multimin_fminimizer_free(s);
	return result;
}

void estimateARMAparameters(string inputDataFile,string ARorderOpt,string MAorderOpt) {
	unsigned int ARorder=atoi(ARorderOpt.c_str());
	unsigned int MAorder=atoi(MAorderOpt.c_str());
	vector <double> inputData;
	ifstream inputDataRead(inputDataFile.c_str(),ifstream::in);
	string buffer;
	while(inputDataRead.good()) {
		getline(inputDataRead,buffer);
		if(buffer.size()<1) {
			continue;
		}
		inputData.push_back(atof(buffer.c_str()));
	}
	vector <double> data=vectorDiff(inputData);
	//vector <double> data=inputData;
	estimateParameters_params p;
	p.ARorder=ARorder;
	p.MAorder=MAorder;
	p.data=data;
	vector <double> modelParams=gsl_multimin_fminimizer_wrapper(normalLogLikelihoodFunction,p,ARorder+MAorder);
	cout<<"Autoregressive parameters: ";
	for(int i=0;i<ARorder;i++) {
		cout<<modelParams[i]<<"	";
	}
	cout<<endl;
	cout<<"Moving average parameters: ";
	for(int i=ARorder;i<modelParams.size();i++) {
		cout<<modelParams[i]<<"	";
	}
	cout<<endl;
	//Example of forecasting
	vector <double> shockValues=arima_computeShockValues(data,modelParams,ARorder,MAorder);
	vector <double> forecast=forecasting(data,shockValues,modelParams,ARorder,MAorder,ARorder+MAorder);
	cout<<"The forecast: ";
	for(int i=0;i<forecast.size();i++) {
		cout<<forecast[i]<<"	";
	}
	cout<<endl;
}

int main(int argc,char*argv[]) {
	estimateARMAparameters(argv[1],argv[2],argv[3]);
}
