
//Shantanu Hirlekar
//Malware Analysis Mini Project Spring(2017)

//I have referred code from http://rosettacode.org/wiki/Entropy#C.2B.2B 
//I used this to refer the formula. 

#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include <map>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <WinBase.h>
#include <FileAPI.h>
#include <io.h>

using namespace std;
typedef unsigned char BYTE;

//Lists the .exe and .dll files present in a directory. 
vector<string> listFiles(const char *searchpath) {

	vector<string> names;
	WIN32_FIND_DATAA search_data;
	LPWIN32_FIND_DATAA sd = &search_data;
	memset(sd, 0, sizeof(WIN32_FIND_DATAA));
	HANDLE handle = FindFirstFileA(searchpath, sd);
	
	while (handle != INVALID_HANDLE_VALUE)
	{
		names.push_back(search_data.cFileName);
		if (FindNextFileA(handle, sd) == FALSE)
			break;
	}

	FindClose(handle);
	return names;
}

//Prints and returns entropy of a file. 
double getEntropy(FILE *file) {

	long fileSize = 0;
	BYTE *fileBuf;
	if (file)
	{
		int fileNum = _fileno(file);
		HANDLE hFile = (HANDLE)_get_osfhandle(fileNum);
		fileSize = GetFileSize(hFile, NULL);
	}

	fileBuf = new BYTE[fileSize];
	fread(fileBuf, fileSize, 1, file);

	double entropy = 0;
	map<char, int> freq;
	cout << "Getting the entropy of the file, please wait....." << endl;
	for (int i = 0; i < fileSize; i++) {
		freq[fileBuf[i]]++;
	}
	for (pair<char, int> p : freq)
	{
		double freq = static_cast<double>(p.second) / fileSize;
		entropy += freq * log2(freq);
	}
	entropy *= -1;
	cout << "Entropy is: " << entropy << endl;
	return entropy;
}

//A function which returs the files having entropy in top 10%
void getTen(float cut_off, map<string, float> track)
{
	cout <<endl<< "Cut off for the 10% is: " << cut_off << endl;
	cout << "The files having entropy in the top 10% are: "<<endl;
	for (auto val : track) {
		if (val.second >= cut_off)
			cout << val.first << ": " << val.second << endl;
	}
}

int main(int argc, char const *argv[])
{
	int i;			
	FILE *file = NULL;		
	const char *filepath;
	map<string, float> track;
	float entropy_sum = 0;
	float max = 0;
	const char *exe = "/*.exe";
	const char *dll = "/*.dll";
	char result1[100];
	char result2[100];
	
	//Open the file provided in the command line argument. Else go o the default path. 
	if (argc != 2) {
		filepath = "C:/WINDOWS/System32/";
	}
	else if (argc == 2)
	{
		filepath = argv[1];
	}
	else {
		cout << endl << "Something has gone wrong opening the directory!" << endl;
		exit(1);
	}
	
	//call the listFiles function for the .exe and .dll files respectively. 
	strcpy(result1, filepath);
	strcat(result1, exe);
	strcpy(result2, filepath);
	strcat(result2, dll);
	vector<string> names = listFiles(result1);
	int exe_size = names.size();
	vector<string> names2 = listFiles(result2);
	int dll_size = names2.size();

	//combining the vectors of .exe and .dll
	names.insert(names.end(), names2.begin(), names2.end());
	int total_size = exe_size + dll_size;

	cout << "Number of .exe files are: " << exe_size << endl;
	cout << "Number of .dll files are: " << dll_size << endl;
	cout << "Total files are: " << total_size << endl;

	//Go to the specified path of the directory.
	if (SetCurrentDirectoryA(filepath))
		cout <<endl<< "Going to the filepath." << endl;
	else {
		cout <<endl<< "Can't go to the filepath." << endl;
		exit(1);
	}

	//Open each file and calculate it's entropy.
	for(string s : names){
		
		if ((file = fopen(s.c_str(), "rb")) == NULL) {
			cout << endl << "Could not open specified file" << endl;
			exit(1);
		}
		else
			cout << endl << "File opened successfully" << endl;

		double entropy = getEntropy(file);

		entropy_sum += entropy;
		if (entropy > max)
			max = entropy;
		track[s] = entropy;
		fclose(file);
	}

	//Printing the average entropy of the files. 
	float avg = entropy_sum / total_size;
	cout <<endl<< "The average entropy of all the files is: " << avg << endl;

	//Getting the files with entropy in top 10%
	float cut_off = max * 0.9;
	getTen(cut_off, track);	
	
	cin >> i;
	return 0;
}