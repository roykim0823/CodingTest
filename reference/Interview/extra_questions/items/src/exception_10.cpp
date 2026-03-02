#include <iostream>
class A {
public:
	A() {}
	~A() {
		throw 42; 	// throw 42 during the exception handling (stack unwinding) 
					// which causes program to crash	
	}
};

int main() {
	try {
		A a;	// without it, cache(int a) catches throw 32 and print out 32   
		throw 32;
	} 
	catch(int a) {
		std::cout << a << std::endl;;
	}

	return 0;
}
