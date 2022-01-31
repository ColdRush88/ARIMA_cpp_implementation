bool signalIdentify(std::vector <double> input,double criticalPvalue); 
double signalIdentifyGamma(std::vector <double> input); 
double signalProc_mean(std::vector <double> input); 
double signalProc_variance(std::vector <double> input);
double** signalProc_autocovarianceMatrix(std::vector <double> input);
double** signalProc_autocorrelationMatrix(std::vector <double> input);
double* signalProc_autocovarianceFunction(std::vector <double> input);
double* signalProc_autocorrelationFunction(std::vector <double> input);
double* signalProc_partialAutocorrelationFunction(std::vector <double> input,int order);
