struct linRegrGSLresult {
	std::vector <double> betas;
	double R2;
	std::vector <double> residuals;
	double chisq;
};

linRegrGSLresult linRegrGSLwrapper1(std::vector <std::vector <double> > X,std::vector <double> Y,double tolerance); 
linRegrGSLresult linRegrGSLwrapper2(std::vector <std::vector <double> > X,std::vector <double> Y,std::vector <double> weights,double tolerance); 
