#pragma once
#include <iostream>
#include <bitset>
#include <vector>
using namespace std;

class MD5
{  
public:
	MD5();
	~MD5();
	void read(string a);
	void HMD5(int q);
	string Encryption();
private:
	vector<unsigned int *> MessageFlow;
	unsigned int A,B,C,D;
};
