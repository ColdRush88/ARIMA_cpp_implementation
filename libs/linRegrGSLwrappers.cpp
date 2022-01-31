#include <iostream>
#include <vector>
#include <gsl/gsl_multifit.h>
#include "/home/user/job/squire3/routine/stat.hpp"
using namespace std;

struct linRegrGSLresult {
	vector <double> betas;
	double R2;
	vector <double> residuals;
	double chisq;
};

linRegrGSLresult linRegrGSLwrapper1(vector <vector <double> > X,vector <double> Y,double tolerance) {
	double xi,yi,ei,chisq;
	gsl_matrix*Xgsl,*cov;
	gsl_vector*Ygsl,*c,*w;
	Xgsl=gsl_matrix_alloc(X.size(),X[0].size());
	Ygsl=gsl_vector_alloc(Y.size());
	w=gsl_vector_alloc(X.size());
	c=gsl_vector_alloc(X[0].size());
	cov=gsl_matrix_alloc(X[0].size(),X[0].size());
	for(int i=0;i<X.size();i++) {
		gsl_vector_set(w,i,1.0);
		for(int j=0;j<X[i].size();j++) {
			gsl_matrix_set(Xgsl,i,j,X[i][j]);
		}
	}
	for(int i=0;i<Y.size();i++) {
		gsl_vector_set(Ygsl,i,Y[i]);
	}
	const double tol=tolerance;
	size_t rank;
	gsl_multifit_linear_workspace*work=gsl_multifit_linear_alloc(X.size(),X[0].size());
	gsl_multifit_linear(Xgsl,Ygsl,c,cov,&chisq,work);
	//gsl_multifit_linear_tsvd(Xgsl,Ygsl,tol,c,cov,&chisq,&rank,work);
	gsl_multifit_linear_free(work);
	//Compute efficiency evaluations
	gsl_vector*r;
	r=gsl_vector_alloc(X.size());
	gsl_multifit_linear_residuals(Xgsl,Ygsl,c,r);
	vector <double> residuals;
	double residualSqSum=0.0;
	for(int i=0;i<X.size();i++) {
		double residual=gsl_vector_get(r,i);
		residualSqSum+=residual*residual;
		residuals.push_back(residual);
	}
	//Compute total sum of squares
	double yMean=stat(Y)[0];
	double ySStot=0.0;
	for(int i=0;i<Y.size();i++) {
		ySStot+=(Y[i]-yMean)*(Y[i]-yMean);
	}
	////////////////////////
	double R2=1.0-residualSqSum/ySStot;
	//Output betas
	vector <double> betas;
	for(int i=0;i<X[0].size();i++) {
		betas.push_back(gsl_vector_get(c,i));
	}
	///////////////////////
	linRegrGSLresult result;
	result.betas=betas;
	result.R2=R2;
	result.residuals=residuals;
	result.chisq=chisq;
	return result;
}
//Weighted regression
linRegrGSLresult linRegrGSLwrapper2(vector <vector <double> > X,vector <double> Y,vector <double> weights,double tolerance) {
	double xi,yi,ei,chisq;
	gsl_matrix*Xgsl,*cov;
	gsl_vector*Ygsl,*c,*w;
	Xgsl=gsl_matrix_alloc(X.size(),X[0].size());
	Ygsl=gsl_vector_alloc(Y.size());
	w=gsl_vector_alloc(X.size());
	c=gsl_vector_alloc(X[0].size());
	cov=gsl_matrix_alloc(X[0].size(),X[0].size());
	for(int i=0;i<X.size();i++) {
		gsl_vector_set(w,i,weights[i]);
		for(int j=0;j<X[i].size();j++) {
			gsl_matrix_set(Xgsl,i,j,X[i][j]);
		}
	}
	for(int i=0;i<Y.size();i++) {
		gsl_vector_set(Ygsl,i,Y[i]);
	}
	const double tol=tolerance;
	size_t rank;
	gsl_multifit_linear_workspace*work=gsl_multifit_linear_alloc(X.size(),X[0].size());
	//gsl_multifit_linear(Xgsl,Ygsl,c,cov,&chisq,work);
	gsl_multifit_wlinear_tsvd(Xgsl,w,Ygsl,tol,c,cov,&chisq,&rank,work);
	gsl_multifit_linear_free(work);
	//Compute efficiency evaluations
	gsl_vector*r;
	r=gsl_vector_alloc(X.size());
	gsl_multifit_linear_residuals(Xgsl,Ygsl,c,r);
	vector <double> residuals;
	double residualSqSum=0.0;
	for(int i=0;i<X.size();i++) {
		double residual=gsl_vector_get(r,i);
		residualSqSum+=residual*residual;
		residuals.push_back(residual);
	}
	//Compute total sum of squares
	double yMean=stat(Y)[0];
	double ySStot=0.0;
	for(int i=0;i<Y.size();i++) {
		ySStot+=(Y[i]-yMean)*(Y[i]-yMean);
	}
	////////////////////////
	double R2=1.0-residualSqSum/ySStot;
	//Output betas
	vector <double> betas;
	for(int i=0;i<X[0].size();i++) {
		betas.push_back(gsl_vector_get(c,i));
	}
	///////////////////////
	linRegrGSLresult result;
	result.betas=betas;
	result.R2=R2;
	result.residuals=residuals;
	result.chisq=chisq;
	return result;
}
