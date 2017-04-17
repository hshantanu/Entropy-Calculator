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

vector<string> listFiles(const char *searchpath) {

	vector<string> names;
	WIN32_FIND_DATAA search_data;
	LPWIN32_FIND_DATAA sd = &search_data;
	memset(sd, 0, sizeof(WIN32_FIND_DATAA));
	HANDLE handle = FindFirstFileA(searchpath, sd);
	
	while (handle != INVALID_HANDLE_VALUE)
	{
		//cout << "\n" << search_data.cFileName;
		names.push_back(search_data.cFileName);
		if (FindNextFileA(handle, sd) == FALSE)
			break;
	}

	FindClose(handle);
	return names;
}

int main(int argc, char const *argv[])
{
	int i;
	BYTE *fileBuf;			// Pointer to our buffered data
	FILE *file = NULL;		// File pointer
	long fileSize = 0;
	const char *filepath;	
	
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
	map<string, float> track;
	float entropy_sum = 0;
	float max = 0;
	const char *exe = "/*.exe";
	const char *dll = "/*.dll";
	char result1[100];
	char result2[100];
	strcpy(result1, filepath);
	strcat(result1, exe);
	strcpy(result2, filepath);
	strcat(result2, dll);
	vector<string> names = listFiles(result1);
	int exe_size = names.size();
	vector<string> names2 = listFiles(result2);
	int dll_size = names2.size();
	names.insert(names.end(), names2.begin(), names2.end());
	int total_size = exe_size + dll_size;

	cout << "Number of .exe files are: " << exe_size << endl;
	cout << "Number of .dll files are: " << dll_size << endl;
	cout << "Total files are: " << total_size << endl;

	if (SetCurrentDirectoryA(filepath))
		cout << "Going to the filepath." << endl;
	else
		cout << "Can't go to the filepath." << endl;
	
	for(string s : names){
		
		if ((file = fopen(s.c_str(), "rb")) == NULL) {
			cout << endl << "Could not open specified file" << endl;
			exit(1);
		}
		else
			cout << endl << "File opened successfully" << endl;

		if (file)
		{
			int fileNum = _fileno(file);
			HANDLE hFile = (HANDLE)_get_osfhandle(fileNum);
			fileSize = GetFileSize(hFile, NULL);
		}

		// Allocate space in the buffer for the whole file
		fileBuf = new BYTE[fileSize];

		// Read the file in to the buffer
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
		entropy_sum += entropy;
		if (entropy > max)
			max = entropy;
		track[s] = entropy;
		fclose(file);
	}
	float avg = entropy_sum / total_size;
	float cut_off = max * 0.9;
	cout << "Cut off is: " << cut_off << endl;
	for (auto val : track) {
		if (val.second >= cut_off)
			cout << val.first << ": " << val.second << endl;
	}
	cout << "The average entropy of all the files is: " << avg << endl;
	cin >> i;
	return 0;
}