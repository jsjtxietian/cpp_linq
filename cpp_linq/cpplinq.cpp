#include "cpplinq.h"
#include <iostream>
#include <cassert>

using namespace xt;
using namespace std;

int main()
{
	{
		int xs[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		vector<int> ys = { 0, 1 };


		vector<int> ys2 = { 6, 8 };
		auto g2 = from(xs)
			.group_by([](int x) { return x % 2; })
			.select([](std::pair<int, linq<int>> p) { return p.second; })
			.first()
			.where([](int x) { return x > 5; });
		for (auto t : g2)
			cout << t << endl;
	}
	
	system("pause");
	return 0;
}

