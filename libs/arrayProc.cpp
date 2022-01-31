#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include "/home/user/job/squire3/routine/extract.hpp"
using namespace std;

vector <double> vectorDiff(vector <double> input) {
	vector <double> result(input.size()-1);
	for(int i=1;i<input.size();i++) {
		result[i-1]=input[i]-input[i-1];
	}
	return result;
}

double extractMinEntries(vector <double> input) {
	double output;
	output=input[0];
	for(int i=1;i<input.size();i++) {
		if(input[i]<output) {
			output=input[i];
		}
	}
	return output;
}

double findMinValue(vector <double> input) {
	double result=input[0];
	for(int i=1;i<input.size();i++) {
		if(input[i]<result) {
			result=input[i];
		}
	}
	return result;
}

double findMaxValue(vector <double> input) {
	double result=input[0];
	for(int i=1;i<input.size();i++) {
		if(input[i]>result) {
			result=input[i];
		}
	}
	return result;
}

//frequently the indices of values are stored in another vector; if vector with values is sorted to be able to correspond value indices later it is needed to perform same operations on the indices vector, which this function does
void coSortVectors(vector <double>* values,vector <int>* indices) {
	/*
	cout<<"coSortVectors::input="<<endl;
	for(int i=0;i<(*values).size();i++) {
		cout<<(*values)[i]<<" "<<(*indices)[i]<<endl;
	}
	cout<<"/////////////////////////"<<endl;
	*/
	for(int i=0;i<(*values).size();i++) {
		for(int j=i;j<(*values).size();j++) {
			double bufferValues;
			int bufferIndices;
			if((*values)[i]>(*values)[j]) {
				bufferValues=(*values)[i];
				(*values)[i]=(*values)[j];
				(*values)[j]=bufferValues;
				bufferIndices=(*indices)[i];
				(*indices)[i]=(*indices)[j];
				(*indices)[j]=bufferIndices;
			}
		}
	}
}

void coSortVectors2(vector <double>* values,vector <double>* indices) {
	/*
	cout<<"coSortVectors::input="<<endl;
	for(int i=0;i<(*values).size();i++) {
		cout<<(*values)[i]<<" "<<(*indices)[i]<<endl;
	}
	cout<<"/////////////////////////"<<endl;
	*/
	for(int i=0;i<(*values).size();i++) {
		for(int j=i;j<(*values).size();j++) {
			double bufferValues;
			double bufferIndices;
			if((*values)[i]>(*values)[j]) {
				bufferValues=(*values)[i];
				(*values)[i]=(*values)[j];
				(*values)[j]=bufferValues;
				bufferIndices=(*indices)[i];
				(*indices)[i]=(*indices)[j];
				(*indices)[j]=bufferIndices;
			}
		}
	}
}

//Sort vector values in ascending order; the vector indices is co-sorted
vector <string> coSortVectors3(vector <double>* values,vector <string> names) {
	cout<<(*values).size()<<endl;
	vector <int> indexVector((*values).size());
	for(int i=0;i<(*values).size();i++) {
		indexVector[i]=i;
	}
	vector <int>* indexVectorPtr=&indexVector;
	coSortVectors(values,indexVectorPtr);
	vector <string> result(names.size());
	for(int i=0;i<names.size();i++) {
		result[i]=names[indexVector[i]];
	}
	return result;
}

void sortVector(vector <double>* values) {
	for(int i=0;i<(*values).size();i++) {
		for(int j=i;j<(*values).size();j++) {
			double bufferValues;
			if((*values)[i]>(*values)[j]) {
				bufferValues=(*values)[i];
				(*values)[i]=(*values)[j];
				(*values)[j]=bufferValues;
			}
		}
	}
}

vector <double> removeNan(vector <double> input) {
	vector <double> result;
	for(int i=0;i<input.size();i++) {
		if(isnan(input[i]) || isinf(input[i])) {
			continue;
		}
		result.push_back(input[i]);
		}
	return result;
}

//erase an element from an array
double* arrayErase(double input[],int size,int indexToErase) {
	double* output=new double[size];
	for(int i=0;i<indexToErase;i++) {
		*(output+sizeof(double)*i)=input[i];
	}
	for(int i=indexToErase+1;i<size;i++) {
		*(output+sizeof(double)*(i-1))=input[i];
	}
	return output;
}

vector <vector <string> > vectorOfVectorsShuffle(vector <vector <string> > input) {
	vector <string> inputElements;
	vector <vector <string> > result(input.size());
	for(int i=0;i<result.size();i++) {
		result[i].resize(input[i].size());
	}
	for(int i=0;i<input.size();i++) {
		for(int j=0;j<input[i].size();j++) {
			inputElements.push_back(input[i][j]);
		}
	}
	random_device rd;
	mt19937 g(rd());
	shuffle(inputElements.begin(),inputElements.end(),g);
	int index=0;
	for(int i=0;i<result.size();i++) {
		for(int j=0;j<result[i].size();j++) {
			result[i][j]=inputElements[index];
			index++;
		}
	}
	return result;
}

