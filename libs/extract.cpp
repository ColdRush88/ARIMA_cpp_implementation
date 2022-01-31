#include <iostream>
#include <string>
#include <vector>
using namespace std;


string extract(string csv_line,int column_index,string delimiter) {
	string extract;
	int first_tab=csv_line.find(delimiter);
	int second_tab;
	if(column_index==1) {
		extract=csv_line.substr(0,first_tab);
		return extract;
	}
	for(int i=0;i<(column_index-1);i++) {
		if(csv_line.find(delimiter,first_tab+1)!=string::npos) {
			second_tab=csv_line.find(delimiter,first_tab+1);
			if(i==(column_index-2)) {
				extract=csv_line.substr(first_tab+1,second_tab-first_tab-1);
			}
		}
	if(csv_line.find(delimiter,first_tab+1)==string::npos) {
		extract=csv_line.substr(first_tab+1,csv_line.size()-first_tab-1);
		return extract;
	}
	first_tab=second_tab;
	}
	return extract;
}

//Cut line into pieces using specified delimiter, put all pieces into a vector.
vector <string> extract2(string inputLine,string delimiter) {
	vector <string> result;
	if(inputLine.find(delimiter)==string::npos) {
		result.push_back(inputLine);
		return result;
	}
	int firstDelim=inputLine.find(delimiter);
	result.push_back(inputLine.substr(0,firstDelim));
	while(inputLine.find(delimiter,firstDelim+1)!=string::npos) {
		int secondDelim=inputLine.find(delimiter,firstDelim+1);
		string columnValue=inputLine.substr(firstDelim+1,secondDelim-firstDelim-1);
		firstDelim=secondDelim;
		result.push_back(columnValue);
	}
	result.push_back(inputLine.substr(firstDelim+1));
	return result;
}

//Extract first n fields
string extract3(string inputLine,string delimiter,int n) {
	string result;
	int extractionPos=inputLine.find(delimiter);
	for(int i=0;i<(n-1);i++) {
		extractionPos=inputLine.find(delimiter,extractionPos+1);
	}
	result=inputLine.substr(0,extractionPos);
	return result;
}
