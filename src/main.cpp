#include <iostream>
#include <time.h>
#include "processer.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(0)));
	Processer processer;
	processer.Run();

	//cout << "Hit enter to quit" << endl;
	//cin.get();
	return 0;
}