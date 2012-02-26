/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:57
# Filename:		test_interface.c
# Description: 
============================================================================*/
#include <kvs.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

struct kv_t {
	int key_size, value_size;
	char key[1024];
	char value[1024*5+1];
};
KVS_ENV kvs_env;
struct kv_t in[MAX_SIZE], out[MAX_SIZE];
int num;

void fill_kvs_env()
{
	kvs_env.init_type = INIT_TYPE_LOAD;
	kvs_env.disk_file_path = "disk_file";
	kvs_env.IMAGE_file_path = "IMAGE_file";
	kvs_env.log_file_path = "log_file";
	kvs_env.buffer_sleep_time = 1;
	kvs_env.buffer_horizon_size = 10;
	kvs_env.buffer_size = 100*1024*1024;
}

int main()
{
	int i, state;

	freopen("input_file", "r", stdin);
	scanf("%d", &num);
	for (i = 0; i < num; i++)
	{
		scanf("%d %s %d %s", &in[i].key_size, in[i].key, &in[i].value_size, in[i].value);
		out[i].value_size = 1025;
	}

	//----------------KV_INIT-------------
	fill_kvs_env();
	if (kv_init(&kvs_env) != 0)
	{
		printf("kvs_init fail\n");
		return -1;
	}

	//----------------KV_PUT-------------
	for (i = 0; i < num / 2; i++)
	{
		state = kv_put(in[i].key, in[i].key_size, in[i].value, in[i].value_size);
		if (state != 0)
		{
			printf("kv_put %d fail\n", i);
		}
		else
		{
			printf("put: key_size:%d\t\tkey:%s\t\tvalue_size:%d\t\tvalue:%s\n", in[i].key_size, in[i].key, in[i].value_size, in[i].value);
		}
	}
	fflush(stdout);

	//----------------KV_GET-------------
	for (i = 0; i < num / 2; i++)
	{
		state = kv_get(in[i].key, in[i].key_size, out[i].value, &out[i].value_size);
		if (state != 0)
		{
			printf("kv_get %d fail\n", i);
		}
		else
		{
			printf("get: key_size:%d\t\tkey:%s\t\tvalue_size:%d\t\tvalue:%s\n", in[i].key_size, in[i].key, out[i].value_size, out[i].value);
		}
	}
	fflush(stdout);

	//----------------KV_DELETE-------------
	for (i = 0; i < num / 4; i += 2)
	{
		state = kv_delete(in[i].key, in[i].key_size);
		if (state != 0)
		{
			printf("kv_delete %d fail\n", i);
		}
		else
		{
			printf("delete: key_size:%d\t\tkey:%s\t\tvalue_size:%d\t\tvalue:%s\n", in[i].key_size, in[i].key, out[i].value_size, out[i].value);
		}
	}
	fflush(stdout);
	
	//----------------KV_GET-------------
	for (i = 0; i < num / 4; i += 2)
	{
		state = kv_get(in[i].key, in[i].key_size, out[i].value, &out[i].value_size);
		if (state != 0)
		{
			printf("kv_get %d fail\n", i);
		}
		else
		{
			printf("get: key_size:%d\t\tkey:%s\t\tvalue_size:%d\t\tvalue:%s\n", in[i].key_size, in[i].key, out[i].value_size, out[i].value);
		}
	}
	fflush(stdout);
	
	//----------------KV_PUT-------------
	for (i = num / 2; i < num; i++)
	{
		state = kv_put(in[i].key, in[i].key_size, in[i].value, in[i].value_size);
		if (state != 0)
		{
			printf("kv_put %d fail\n", i);
		}
		else
		{
			printf("put: key_size:%d\t\tkey:%s\t\tvalue_size:%d\t\tvalue:%s\n", in[i].key_size, in[i].key, in[i].value_size, in[i].value);
		}
	}
	fflush(stdout);

	//----------------KV_GET-------------
	for (i = num / 2; i < num; i++)
	{
		state = kv_get(in[i].key, in[i].key_size, out[i].value, &out[i].value_size);
		if (state != 0)
		{
			printf("kv_get %d fail\n", i);
		}
		else
		{
			printf("get: key_size:%d\t\tkey:%s\t\tvalue_size:%d\t\tvalue:%s\n", in[i].key_size, in[i].key, out[i].value_size, out[i].value);
		}
	}
	fflush(stdout);

	//----------------KV_EXIT-------------
	if (kv_exit() != 0)
	{
		printf("kvs_exit fail\n");
		return -1;
	}
	return 0;
}
