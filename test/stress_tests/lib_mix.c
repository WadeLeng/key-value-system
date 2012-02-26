#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <kvs.h>

#define CNT 52500
#define KEY_BUFFER_SIZE 10002
#define VALUE_BUFFER_SIZE (100*1024)

KVS_ENV kvs_env;
char value_buffer[VALUE_BUFFER_SIZE];
char key_buffer[KEY_BUFFER_SIZE];

void get_key(char* key, int i, int* key_size)
{
	*key_size = 0;
	if (i == 0) *key_size = 1;
	while (i > 0)
	{
		key[*key_size] = '0' + i % 10;
		i = i / 10;
		(*key_size)++;
	}
	key[*key_size] = '0';
}

void random_key(char* key, int i, int* key_size)
{
	int num = random() % i;
	get_key(key, num, key_size);
}

int main()
{
	long long start, end, duration, t1, t2;
	struct timeval tv;
	int i, j, key_size, value_buffer_size;

	kvs_env.init_type = INIT_TYPE_CREATE;
	kvs_env.disk_file_path = "disk_file";
	kvs_env.IMAGE_file_path = "IMAGE_file";
	kvs_env.log_file_path = "log_file";
	kvs_env.buffer_sleep_time = 0;
	kvs_env.buffer_horizon_size = 100 * 1024 * 1024;
	kvs_env.buffer_size = 700 * 1024 * 1024;

	for (i = 0; i < VALUE_BUFFER_SIZE; i++)
		value_buffer[i] = 'a';
	srand((unsigned) time(NULL));
	gettimeofday(&tv, NULL);
	start = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

	if (kv_init(&kvs_env) != 0)
	{
		printf("kvs_init fail\n");
		return -1;
	}

	//first write 5GB
	for (i = 0; i < CNT; i++)
	{
		get_key(key_buffer, i, &key_size);
		if (kv_put(key_buffer, key_size, value_buffer, VALUE_BUFFER_SIZE) != 0)
		{
			printf("kv_put fail\n");
			return -1;
		}
	}
	duration = 0;
	j = CNT;
	for (i = 0; i < CNT; i++)
	{
		get_key(key_buffer, j, &key_size);

		if (i % 200 == 0)
		{
			if (kv_put(key_buffer, key_size, value_buffer, VALUE_BUFFER_SIZE) != 0)
			{
				printf("kv_put fail\n");
				return -1;
			}
			j++;
		}

		random_key(key_buffer, j - 1, &key_size);
		gettimeofday(&tv, NULL);
		t1 = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

		if (kv_get(key_buffer, key_size, value_buffer, &value_buffer_size) != 0)
		{
			printf("kv_get fail\n");
			return -1;
		}
	
		gettimeofday(&tv, NULL);
		t2 = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
		duration += (t2 - t1) / 1000;
	}

	kv_exit();

	printf("read time: %lld ms\n", duration);
	gettimeofday(&tv, NULL);
	end = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	duration = (end - start) / 1000;

	printf("total time: %lld ms\n", duration);
	return 0;
}
