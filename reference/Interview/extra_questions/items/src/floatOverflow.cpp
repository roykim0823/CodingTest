#include <iostream>
#include <string>
#include <vector>

using namespace std;


int main()
{
	float f, g;
	f=0;
	unsigned i=0;
	do {
		g=f++;        
		i++;
		//cout << g <<", "<< f << "\n";
	} while ((f-g) != 0);	// It will stop since a float overflows (it is not increased)
	cout << "g=" << g << ", f=" << f << ", i=" << i<< ", f-g=" << f-g << endl;
	return 0;
}
