//I have referred code from http://rosettacode.org/wiki/Entropy#C.2B.2B 
//I used this to refer the formula. 

#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;

typedef unsigned char BYTE;

long getFileSize(FILE *file)
{
	long curr, end;
	curr = ftell(file);
	fseek(file, 0, 2);
	end = ftell(file);
	fseek(file, curr, 0);
	return end;
}

int main(int argc, char const *argv[])
{
    const char *filePath = "/home/shantanu/Downloads/ExactFile-Setup.exe";	
	BYTE *fileBuf;			// Pointer to our buffered data
	FILE *file = NULL;		// File pointer
	
	if ((file = fopen(filePath, "rb")) == NULL)
		cout << "Could not open specified file" << endl;
	else
		cout << "File opened successfully" << endl;

	long fileSize = getFileSize(file);

	// Allocate space in the buffer for the whole file
	fileBuf = new BYTE[fileSize];

	// Read the file in to the buffer
	fread(fileBuf, fileSize, 1, file);

	double entropy = 0;
	map<char,int> freq;

	for(int i = 0; i < fileSize; i++)
		freq[fileBuf[i]]++;
	for(pair<char,int> p : freq)
	{
		double freq = static_cast<double>(p.second) / fileSize;
		entropy += freq * log2(freq);
	}
	entropy *= -1;
	cout << "Entropy is: " <<entropy << endl;
	fclose(file);
	return 0;
}