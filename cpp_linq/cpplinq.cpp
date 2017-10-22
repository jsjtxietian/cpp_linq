#include "cpplinq.h"
#include <iostream>

using namespace xt;
using namespace std;

int main()
{
	int src[] = { 1,2,3,4,5,6,7,8 };
	
	auto query = from(src)
		.where([](int x) {return x % 2 == 1; })
		.select([](int x) {return x + 1; }).count()
	;
	cout << query << endl;
	/*for (auto x : query)
	{	
		cout << x << endl;
	}*/
	

	system("pause");
	return 0;
}

