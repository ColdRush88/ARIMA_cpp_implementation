#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

void spiral() {
	string outputFile="spiral.dat";
	ofstream outputWrite(outputFile.c_str(),ofstream::out);
	double r=0.4;
	double a=0.1;
	double step=0.1;
	unsigned int N=1000;
	for(int i=0;i<N;i++) {
		double t=(double)i*step;
		double x=r*cos(t);
		double y=r*sin(t);
		double z=5.0*sin(a*t);
		outputWrite<<x<<"	"<<y<<"	"<<z<<endl;
	}
}

void spiral2() {
	string outputFile="spiral2.dat";
	ofstream outputWrite(outputFile.c_str(),ofstream::out);
	double r=0.4;
	double a=0.1;
	double step=0.1;
	unsigned int N=1000;
	for(int i=0;i<N;i++) {
		double t=(double)i*step;
		double pheta=(double)i*3.14/(4000.0);
		double x=r*cos(t);
		double y=r*sin(t);
		double z=a*t;
		double x2=r*sin(t);
		double y2=r*cos(t);
		double z2=a*t;
		/*
		double xRotated=x*cos(pheta)-y*sin(pheta);
		double yRotated=x*sin(pheta)+y*cos(pheta);
		double zRotated=z;
		*/
		double xRotated=x;
		double yRotated=y*cos(pheta)-z*sin(pheta);
		double zRotated=y*sin(pheta)+z*cos(pheta);
		double x2Rotated=x2;
		double y2Rotated=y2*cos(pheta)-z2*sin(pheta);
		double z2Rotated=y2*sin(pheta)+z2*cos(pheta);
		outputWrite<<xRotated<<"	"<<yRotated<<"	"<<zRotated<<"	"<<x2Rotated<<"	"<<y2Rotated<<"	"<<z2Rotated<<endl;
	}
}



int main(int argc,char*argv[]) {
	spiral();
	spiral2();
}
