#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char key_table[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
int main()
{
	int i, j, key_size, value_size;

	freopen("input_file", "w", stdout);
	srand((unsigned)time(NULL));
	printf("26\n");
	for (i = 0; i < 26; i++)
	{
		key_size = i + 4;	
		printf("%d ", key_size);

		printf("key-");			//like "key-aaaaaaaaa"
		for (j = 0; j < key_size - 4; j++)
			printf("%c", key_table[key_size - 4]);
		printf(" ");

		value_size = i + 6;
		printf("%d ", value_size);

		printf("value-");
		for (j = 0; j < value_size - 6; j++)
			printf("%c", key_table[value_size - 6]);
		printf("\n");
	}
	return 0;
}
