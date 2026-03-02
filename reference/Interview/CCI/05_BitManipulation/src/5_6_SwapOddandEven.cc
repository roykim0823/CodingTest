// Q. Swap odd and even bits in an interger with as few instructions as possible

int swapOddEvenBits(int x) {
	int odd = (x & 0xaaaaaaaa) >> 1;	// 0xa = 1010
	int even = (x & 0x55555555) << 1;	// 0x5 = 0101
	return odd | even;
}

int main() {
	return 0;
}
