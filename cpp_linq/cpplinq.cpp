#include "cpplinq.h"
#include <iostream>

using namespace xt;
using namespace std;

int main()
{
	int x[] = { 1,2,3,4,5,6,7,8 };
	int y[] = { 1,2,3,4,5,6,7,8 };
	auto q = from(x).average<double>();

	cout << q;
	/*for (auto p : q)
		cout << p << endl;*/

	system("pause");
	return 0;
}

