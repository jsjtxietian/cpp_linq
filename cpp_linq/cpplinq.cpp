#include "cpplinq.h"
#include <cassert>


int main()
{
	{
		vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto q = from(v)
			.where([](int x) {return x%2 == 1; })
			.select([](int x) {return x*x; });


		for (auto t : q)
			cout << t << endl;

	} 

	system("pause");
	return 0;
}
