!!!The requirement - GSL library.
Folder compileCommand contains compilations commands for parameterEstimation.cpp and initialEstimate.cpp.
Folder 'binaries' contains compiled programs 'parameterEstimation.out' and 'initialEstimate.out'. The compilation: g++, Linux Debian 10.

The program parameterEstimation.out usage: "./parameterEstimation.out dataFile ARrder MAorder". dataFile - data file (time series, one column), ARorder - order of the autoregressive operator, MAorder - order of the moving average operator. For example, "./parameterEstimation.out CYBR.dat 1 2" would fit ARIMA(1,1,2) model. The output contains AR and MA parameter values as well as prediction of 3 future values. The program was compiled to fit the model with difference order 1. To change the difference order the program has to be recompiled.

The implementation of time series analysis algorithms from the book "Time series: forecasting and control", Box, Jenkins, Reinsel, Ljung.
When results of parameter estimation are compared to the output of arima() command in R, remember that the sign of moving average parameters must differ. In R the ARMA(1,1) model, for example, is z_t=phi*z_(t-1) + pheta*a_(t-1). Here the model is z_t=phi*z_(t-1)-pheta*a_(t-1).
Currently the differencing is implemented as vectorDiff() function that is applied to the input data vector. There is no interface for that. If one wants to change the degree of differencing the program has to be recompiled. 
The file CYBR.dat is the stock market data for the company CyberArk (daily close prices, from 09.07.2021 to 27.01.2022). These data can be used for testing. For example, one can compare the output of R arima() command with the output of this program. 
