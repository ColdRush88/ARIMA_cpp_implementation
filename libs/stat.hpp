using namespace std; 
vector <double> stat(vector <double> data);
vector <double> statInt(vector <int> dataInput); 
double median(std::vector <double> data); 
double corrCoeff(vector <double> data1, vector <double> data2);
double corrCoeff2(vector <vector <double> > data);
vector <int> histCreate(double minVal,double maxVal,double step,vector <double> data);
vector <int> histCreate2(double minVal,double maxVal,double step,vector <double> data,vector <int> counters);
double statSkewness(vector <double> data, double mean, double stDev);
double statKurtosis(vector <double> data, double mean, double stDev);
double tTest1(vector <double> data1,vector <double> data2);
double tTest2(vector <double> data);
double corrCoeffSpearman(vector <double> data1,vector <double> data2); 
struct rankSumResult {
	double corrCoeffSpearman;
	vector <double> rankSums;
};
//Rank data according to the sum of ranks of predictors.
//Supposedly data is certain quantity that can be predicted by measures in the predictorData matrix, where one vector correspond to one such measure. Data points are ranked according to each predictor and rank of each data point according to each predictor is summed together. Thus data points which were put high by all predictors tend to score high overall.
rankSumResult corrCoeffSpearman_rankSum(vector <double> data,vector <vector <double> > predictorData); 
double autocorrCoeffCompute(vector <double> input,int lag); 
vector <vector <double> > autocorrFunctionCompute(vector <double> input,int minLag,int maxLag); 
double* linRegr_squaredError(std::vector <double> y,std::vector <double> x, double a, double b);
