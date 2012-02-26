#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i;

	freopen("file.txt", "w", stdout);
	for (i = 0; i < 4096; i++)
		printf("a");
	for (i = 0; i < 4096; i++)
		printf("%d", i % 10);
	return 0;
}
