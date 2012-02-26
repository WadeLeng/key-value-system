/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2012-01-05 17:01
# Filename: test_buffer.c
# Description: 
============================================================================*/
#include "buffer.h"
#include "layout.h"

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE	120 
struct value_type {
	char value[1024];
	int value_size;
	PTR_BUF buf_ptr;
};
char buffer[BUFFER_SIZE];//200MB
IDX_VALUE_INFO* value_ptr = NULL;
IDX_VALUE_INFO	delete_info[300];
IDX_VALUE_INFO	put_info[300];
struct value_type value_info[300];
char value[1024];
int state, i, max;

int main()
{
	freopen("input_file", "r", stdin);
	scanf("%d", &max);
	state = sync_init("out.txt", BUFFER_SIZE, "sync_log.txt");
	if (state != 0)
	{
		printf("sync init fail\n");
		return -1;
	}

	state = buffer_init(buffer, "log.txt", BUFFER_SIZE, 4, 10);
	if (state != 0)
	{
		printf("buffer init fail\n");
		return -1;
	}

	for (i = 0; i < 10; i++)
	{
		scanf("%d %s\n", &value_info[i].value_size, value_info[i].value);
		state = buffer_put(value_info[i].value, value_info[i].value_size, &value_info[i].buf_ptr, &put_info[i]);
		if (state != 0)
		{
			printf("buffer put fail\n");
			return -1;
		}
		printf("PUT value_size: %d value: %s buf_ptr: %p\n", value_info[i].value_size, value_info[i].value, value_info[i].buf_ptr);
		sleep(1);
	}
	for (i = 0; i < 10; i++)
	{
		if (put_info[i].buf_ptr != BUF_PTR_NULL)
		{
			state = buffer_get((PTR_BUF)&value, 1024, value_info[i].buf_ptr);
			if (state != 0)
			{
				printf("buffer_get fail\n");
				return -1;
			}
			printf("GET value_size: %d value: %s buf_ptr: %p\n", value_info[i].value_size, value, value_info[i].buf_ptr);
		}
		else
			printf("the value @buffer has delete, sync_read~\n");
	}

	for (i = 0; i < 10; i += 3)
	{
		state = buffer_delete(value_info[i].buf_ptr);
		if (state != 0)
		{
			printf("buffer delete fail\n");
			return -1;
		}
		printf("DELETE i: %d value_size: %d buf_ptr: %p\n", i, value_info[i].value_size, value_info[i].buf_ptr);
	}

	for (i = 0; i < 10; i++)
	{
		if (put_info[i].buf_ptr != BUF_PTR_NULL)
		{
			state = buffer_get((PTR_BUF)&value, 1024, value_info[i].buf_ptr);
			if (state != 0)
			{
				printf("buffer_get fail %d\n", i);
				return -1;
			}
			printf("GET value_size: %d value: %s buf_ptr: %p\n", value_info[i].value_size, value, value_info[i].buf_ptr);
		}
		else
			printf("the value @buffer has delete, sync_read~\n");
	}

	sleep(3);
	for (i = 10; i < max; i++)
	{
		scanf("%d %s\n", &value_info[i].value_size, value_info[i].value);
		state = buffer_put(value_info[i].value, value_info[i].value_size, &value_info[i].buf_ptr, &put_info[i]);
		if (state != 0)
		{
			printf("buffer put fail\n");
			return -1;
		}
		printf("PUT value_size: %d value: %s buf_ptr: %p\n", value_info[i].value_size, value_info[i].value, value_info[i].buf_ptr);
	}

	state = buffer_exit();
	if (state != 0)
	{
		printf("buffer exit fail\n");
		return -1;
	}
	state = sync_exit();
	if (state != 0)
	{
		printf("sync exit fail\n");
		return -1;
	}

	return 0;
}
