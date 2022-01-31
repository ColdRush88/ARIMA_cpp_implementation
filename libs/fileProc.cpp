#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "/home/user/job/squire3/routine/extract.hpp"
using namespace std;

int lineCount(string fileName) {
	ifstream fileRead(fileName.c_str(),ifstream::in);
	string buffer;
	int lineCount=0;
	while(fileRead.good()) {
		getline(fileRead,buffer);
		if(buffer.size()<1) {
			continue;
		}
		lineCount++;
	}
	return lineCount;
}

vector <string> fileProc_createList(string inputFile) {
	ifstream inputRead(inputFile.c_str(),ifstream::in);
	string buffer;
	vector <string> result;
	while(inputRead.good()) {
		getline(inputRead,buffer);
		if(buffer.size()<1) {
			continue;
		}
		result.push_back(buffer);
	}
	inputRead.close();
	return result;
}

//Searches inputFile for expressions listed in the filterFile; only columns equal to fieldIndex (1-based) are compared. If the field (whole field) is equal to at least one expression from the filterFile, the line is written to the outputFile.
string fileProc_grepField(string inputFile,string filterFile,int fieldIndex,string delimiter,string outputFile) {
	ifstream filterRead(filterFile.c_str(),ifstream::in);
	string buffer;
	vector <string> filter;
	while(filterRead.good()) {
		getline(filterRead,buffer);
		if(buffer.size()<1) {
			continue;
		}
		filter.push_back(buffer);
	}
	ofstream outputWrite(outputFile.c_str(),ofstream::out);
	ifstream inputRead(inputFile.c_str(),ifstream::in);
	while(inputRead.good()) {
		getline(inputRead,buffer);
		if(buffer.size()<1) {
			continue;
		}
		string field2Compare=extract(buffer,fieldIndex,delimiter);
		for(int i=0;i<filter.size();i++) {
			if(field2Compare==filter[i]) {
				outputWrite<<buffer<<endl;
				break;
			}
		}
	}
	return outputFile;
}



