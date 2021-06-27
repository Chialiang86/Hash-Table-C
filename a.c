#include <stdio.h>

int main() {
	unsigned int a = 0xFFFFFFFF;
	int num = 0x0000FF00;
	printf("a = %d\n", a);
	int s = 10;
	a = a << s;
	printf("a = %d\n", a);
	return 0;
}
