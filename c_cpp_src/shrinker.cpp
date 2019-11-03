#include <iostream>
#include <string>
#include <stdio.h>
int main(){
	unsigned long long start;
	unsigned long long end;
	std::cout << "Welcome to the File Shrinker..." << std::endl;
	std::string fileName;
	std::cout << "Please enter filename: ";
	std::cin >> fileName;
	std::cout << "Selected File: " << fileName << std::endl;
	FILE* theFile;
	const char * fileNameP = fileName.c_str();
	theFile = std::fopen(fileNameP,"rb");
	if(theFile == NULL){
		perror("Error reading the file!");
		return EXIT_FAILURE;
	}
	char buffer[1024];
	std::string fileNameShrinked = fileName.append("Shrinked");
	const char * fileNameShrinkedP = fileNameShrinked.c_str();
	FILE* theShrinkedFile;
	theShrinkedFile= std::fopen (fileNameShrinkedP,"wb");
	std::fseek(theFile, 0, SEEK_END);
    	unsigned long long size = (unsigned long long )ftell(theFile);
	std::cout << "File size: " << size << " Bytes" << std::endl;
	std::cout << "Enter start Offset: ";
	std::cin >> start;
	std::cout << "Enter end Offset: ";
	std::cin >> end;
	unsigned long long space = end - start;
	if (std::fseek(theFile, start, SEEK_SET) != 0) {
		perror("Offset out of file length.");
		return EXIT_FAILURE;
	}
	unsigned long long i = start;
	for (start; start < end;start+=1024){
		std::fread(buffer,sizeof(buffer[0]),1024,theFile);
		std::fwrite(buffer,sizeof(buffer[0]),1024,theShrinkedFile);
	}
	fclose(theShrinkedFile);
	fclose(theFile);
	std::cout << "Done!" << std::endl;
	std::cout << "Wrote Data to File: " << fileNameShrinked << std::endl;
	return 0;
}
