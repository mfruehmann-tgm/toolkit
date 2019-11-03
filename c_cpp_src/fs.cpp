#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
int main(int argc, char** argv){
	if(argv[1] == NULL || argv[2] == NULL){
		cout << "Usage: fs <file> <b | kb | mb | gb>" << endl;
		return EXIT_FAILURE;
	}
	FILE* fp = fopen(argv[1],"r");
	fseek(fp,0,SEEK_END);
	long double size = (long double) ftell(fp);
	string unit = argv[2];
	if(unit == "kb") size /= 1024;
	if(unit == "mb") size = size / 1024 / 1024;
	if(unit == "gb") size = size /1024 / 1024 / 1024;
	cout << "Size: " << size << endl;
	return 0;
}
