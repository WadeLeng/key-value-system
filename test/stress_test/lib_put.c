#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <kvs.h>

#define CNT 1000
#define KEY_BUFFER_SIZE 10002
#define VALUE_BUFFER_SIZE (100*1024)

KVS_ENV kvs_env;
char value_buffer[VALUE_BUFFER_SIZE];
char key_buffer[KEY_BUFFER_SIZE];

void get_key(char* key, int i, int* key_size)
{
	*key_size = 0;
	while (i > 0)
	{
		key[*key_size] = '0' + i % 10;
		i = i / 10;
		(*key_size)++;
	}
	key[*key_size] = '0';
}

int main()
{
	clock_t start, finish, s1, s2;
	double duration = 0;
	int i, key_size;

	kvs_env.init_type = INIT_TYPE_CREATE;
	kvs_env.disk_file_path = "disk_file";
	kvs_env.IMAGE_file_path = "IMAGE_file";
	kvs_env.log_file_path = "log_file";
	kvs_env.buffer_sleep_time = 1;
	kvs_env.buffer_horizon_size = 5 * 1024 * 1024;
	kvs_env.buffer_size = 100 * 1024 * 1024;

	for (i = 0; i < VALUE_BUFFER_SIZE; i++)
		value_buffer[i] = 'a';

	s1 = clock();

	if (kv_init(&kvs_env) != 0)
	{
		printf("kvs_init fail\n");
		return -1;
	}

	for (i = 0; i < CNT; i++)
	{
		get_key(key_buffer, i, &key_size);
		start = clock();
		if (kv_put(key_buffer, key_size, value_buffer, VALUE_BUFFER_SIZE) != 0)
		{
			printf("kv_put fail\n");
			return -1;
		}
		finish = clock();
		duration += (double) (finish - start) / CLOCKS_PER_SEC;
	}

	printf("%lf seconds\n", duration);

	kv_exit(1);
	s2 = clock();
	printf("%ld seconds\n", (s2 - s1) / CLOCKS_PER_SEC);
	return 0;
}
