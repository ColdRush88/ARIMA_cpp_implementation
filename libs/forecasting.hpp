struct forecast_testModel_result {
	std::vector <std::vector <double> > forecastMatrix;
	std::vector <std::vector <double> > residuals;
	std::vector <std::vector <double> > residualsSquared;
	std::vector <std::vector <double> > residualsAbs;
	std::vector <double> residualsSum;
	std::vector <double> residualsSquaredSum;
	std::vector <double> residualsAbsSum;
};
std::vector <double> forecasting(std::vector <double> z,std::vector <double> a,std::vector <double> params,unsigned int ARorder,unsigned int MAorder,unsigned int forecastLength); 
std::vector <std::vector <double> > arima_computeWeights(std::vector <double> params,unsigned int ARorder,unsigned int MAorder); 
forecast_testModel_result forecast_testModel(std::vector <double> z,std::vector <double> a,std::vector <double> params,unsigned int ARorder,unsigned int MAorder,unsigned int forecastLength); 
