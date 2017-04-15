#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <map>
using namespace std;

int main(int argc, char const *argv[])
{
	string input = argv[1];
	int len = input.length();
	double entropy = 0;
	map<char,int> freq;

	for (char c : input)
		freq[c]++;
	for(pair<char,int> p : freq)
	{
		double freq = static_cast<double>(p.second) / len;
		entropy += freq * log2(freq);
	}
	entropy *= -1;
	cout << "Entropy is: " <<entropy << endl;

	return 0;
}