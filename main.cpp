#include <iostream>
#include "MD5.hpp"
using namespace std;
void test(string input);
int main()
{
	int t;
	test("");
	cout << "d41d8cd98f00b204e9800998ecf8427e" << endl << endl;

	test("ab");
	cout << "187ef4436122d1cc2f40dc2b92f0eba0" << endl << endl;

	test("abc");
	cout << "900150983cd24fb0d6963f7d28e17f72" << endl << endl;

	test("message digest");
	cout << "f96b697d7cb7938d525a2f31aaf161d0" << endl << endl;

	test("abcdefghijklmnopqrstuvwxyz");
	cout << "c3fcd3d76192e4007dfb496cca67e13b" << endl << endl;

	test("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	cout << "d174ab98d277d9f5a5611c2c9f419d9f" << endl << endl;

	test("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	cout << "57edf4a22be3c955ac49da2e2107b67a" << endl << endl;
	cin >> t;
}

void test(string input)
{
	MD5 a;
	a.read(input);
	a.Encryption();
}
