#include <memory>

auto f(std::unique_ptr<int> ptr) {
	*ptr=42;
	return ptr;
}

auto g(const std::unique_ptr<int> &ptr) {
	*ptr=42;
	return *ptr;
}

int main() {
	auto ptr = std::make_unique<int>();
	//ptr = f(ptr);	// compilation error
	ptr = f(std::move(ptr));
	auto ptr2 = g(ptr);
}
