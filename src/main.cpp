#include <iostream>
#include "processer.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Processer processer;
	processer.Run();

	cout << "Hit enter to quit" << endl;
	cin.get();
	return 0;
}