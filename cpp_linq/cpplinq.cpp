#include "cpplinq.h"
#include <iostream>

using namespace xt;
using namespace std;

int main()
{
	int src[] = { 1,2,3,4,5,6,7,8 };
	vector<int> v(begin(src),end(src));
	
	auto query = from(v)
		.select([](int x) {return x; }).take(4);
	;
	
	for (auto x : query)
	{	
		cout << x << endl;
	}
	

	system("pause");
	return 0;
}

