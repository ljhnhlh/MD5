#include "MD5.hpp"
#include <iostream>
#include <bitset>
#include <math.h>
using namespace std;

int index[64] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 1, 6,11, 0, 5,10,15, 4, 9, 14, 3, 8,13, 2, 
7,12,5, 8,11,14, 1, 4, 7,10,13, 0, 3, 6, 9,12,15, 2,0, 7,14, 5,12, 3,10, 1, 8,15, 6,13, 4,11, 2, 9 };
int s[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20 , 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23 , 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
unsigned int T[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
						0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
						0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
						0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
						0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
						0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
						0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
						0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };



MD5::MD5()
{
	A = 0x67452301;
	B = 0xEFCDAB89;
	C = 0x98BADCFE;
	D = 0x10325476;
}

MD5::~MD5()
{
	
}

void output(int a)//小端规则
{
	for (int i = 0; i < 4; i++)
	{
		int temp = a & 0xff;
		if (temp < 16)
			cout << "0";
		printf("%x", temp);
		a = a >> 8;
	}
}



/*
	信息读取函数使用一种比较耗内存，但很容易写的方法来实现：
	1）定义一个非常大的数组
	2）将所有元素按顺序输入数组
	3）读取，填充和加入信息长度这些步骤全部完成后，将数组赋值给M[16]，每16个就push到MessageFlow中（16*32 = 512）
	为何要使用大数组？使用大数组，你就不需要考虑当前的信息是否要push，可以节省代码，也可以减少if语句，降低程序的
	复杂性。
*/
void MD5::read(string str)
{
	unsigned int big_temp[1000] = {0};
	unsigned int sub_index = 0;
	unsigned int temp;
	big_temp[sub_index] = 0;
	int i;
	long long int   count = str.length() * 8;
	for (i = 0; i < str.length(); i++)//transfer message
	{
		temp = str[i];
		big_temp[sub_index] += temp << ((i % 4) * 8);
		if (i % 4 == 3)
			sub_index++;
	}

	temp = 0x80;//padding
	big_temp[sub_index] += temp << ((i % 4) * 8);
	if (i % 4 == 3)
		sub_index++;
	i++;
	temp = 0x00;
	while ((i * 8) % 512 != 448)
	{
		big_temp[sub_index] += temp << ((i % 4) * 8);
		if (i % 4 == 3)
			sub_index++;
		i++;
	}
	big_temp[sub_index++] = (count << 32) >> 32;
	big_temp[sub_index] = count >> 32;
	unsigned int * M;
	for (int i = 0; i <= sub_index; i++)//add message block to MessageFlow
	{
		if (i % 16 == 0)
		{
			M = new unsigned int[16];
		}
		else if (i % 16 == 15)
		{
			MessageFlow.push_back(M);
		}
		M[i % 16] = big_temp[i];
	}
	/*
	此处用于将输入的信息print出来，主要验证“小端存储”操作是否出现错误。
	for (int i = 0; i < MessageFlow.size(); i++)
	{
		for (int j = 0; j < 16; j++)
		{
			printf("%x ", MessageFlow[i][j]);
		}
		cout << endl;
	}
	*/

}



unsigned int g2(int i, unsigned int b, unsigned int c, unsigned int d) {
	switch (i)
	{
	case 0:return (b & c) | (~b & d);
	case 1:return (b & d) | (c & ~d);
	case 2:return b ^ c ^ d;
	default:return c ^ (b | ~d);
	}
}

void MD5::HMD5(int q)
{
	unsigned int a, b, c, d;
	a = A;
	b = B;
	c = C;
	d = D;
	unsigned int temp;
	unsigned int X, sub_index;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			sub_index = i * 16 + j;
			X = MessageFlow[q][index[sub_index]];
			temp = a + g2(i, b, c, d) + X + T[sub_index];
			/*
				此处输出的是官方给的数据的输出，验证数据是否出错，注:T虽然可以临时计算，但我看很多例子
				和官方代码都是使用现成的T值，且我尝试过直接算，一开始结果还是对的，到后面错了很多，因此建议
				将T存入数组，直接使用就好。
				cout << index[sub_index] << " " << T[sub_index] << " " << s[sub_index] << endl;
			*/
			temp = (temp << s[sub_index]) | (temp >> (32 - s[sub_index]));
			a = b + temp;
			/*
				此处用于输出a，若a输出正确，说明以上步骤都是正确的。
				printf("%x\n",a);
			*/
			temp = d;
			d = c;
			c = b;
			b = a;
			a = temp;
		}
	}
	A = A + a;
	B = B + b;
	C = C + c;
	D = D + d;
}



string  MD5::Encryption()
{

	
	A = 0x67452301;
	B = 0xEFCDAB89;  
	C = 0x98BADCFE;
	D = 0x10325476;

	for (int i = 0; i < MessageFlow.size(); i++)
	{
		HMD5(i);
	}
	output(A);
	output(B);
	output(C);
	output(D);
	cout << endl;
	return string();
}




