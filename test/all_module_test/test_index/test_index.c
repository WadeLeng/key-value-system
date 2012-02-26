/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2011-12-23 13:31
# Filename: test_index.c
# Description: 
============================================================================*/
#include "index.h"
#include "layout.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_KEY_SIZE	256
char* image = NULL;
INIT_TYPE init = INIT_TYPE_CREATE;
int line_num = 0;
struct key_type {
	char key[MAX_KEY_SIZE];
	int key_size;
}key[201];
IDX_VALUE_INFO value_info;
IDX_VALUE_INFO* value_info_ptr = NULL;
IDX_VALUE_INFO* value_info_ptr2 = &value_info;

int state;

void print(IDX_VALUE_INFO* value_info)
{
	printf("value_info:\n");
	printf("value_info.value_size: %d\n", value_info->value_size);
	printf("value_info.buf_ptr: %p\n", value_info->buf_ptr);
	printf("value_info.disk_offset %d\n", value_info->disk_offset);
	printf("----------------------------------------------------\n");
}

int main()
{
	int i;

	freopen("key.txt", "r", stdin);
	image = malloc(IMAGE_SIZE);
	if (idx_init(image, "log.txt", init) != 0)
	{
		printf("idx_init errno\n");
		return -1;
	}

	scanf("%d\n", &line_num);
	printf("IDX_INSERT:\n");
	for (i = 0; i < line_num; i++)
	{
		scanf("%d %s\n", &(key[i].key_size), key[i].key);
		printf("%d %s\n", key[i].key_size, key[i].key);
		state = idx_insert(key[i].key, key[i].key_size, &value_info_ptr);
		if (state != 0)
			printf("idx_insert error\n");
		value_info_ptr->value_size = key[i].key_size + 100;
		value_info_ptr->buf_ptr = NULL;
		value_info_ptr->disk_offset = DISK_OFFSET_NULL;
		print(value_info_ptr);
		if (i == 0)
		{
			printf("IDX_DELETE FIRST:\n");
			state = idx_delete(key[i].key, key[i].key_size, &value_info);
			if (state != 0)
				printf("idx_delete error\n");
			print(&value_info);
		}
	}

	printf("IDX_SEARCH\n");
	printf("line_num: %d\n", line_num);
	for (i = 0; i < line_num; i++)
	{
		state = idx_search(key[i].key, key[i].key_size, &value_info_ptr2);
		if (state != 0)
			printf("idx_search error\n");
		else
			print(value_info_ptr2);
	}

	printf("IDX_DELETE\n");
	state = idx_delete(key[3].key, key[3].key_size, &value_info);
	if (state != 0)
		printf("idx_delete error\n");
	print(&value_info);

	state = idx_delete(key[6].key, key[6].key_size, &value_info);
	if (state != 0)
		printf("idx_delete error\n");
	print(&value_info);

	state = idx_delete(key[2].key, key[2].key_size, &value_info);
	if (state != 0)
		printf("idx_delete error\n");
	print(&value_info);

	printf("IDX_SEARCH\n");
	for (i = 0; i < line_num; i++)
	{
		state = idx_search(key[i].key, key[i].key_size, &value_info_ptr2);
		if (state != 0)
			printf("idx_search error\n");
		print(value_info_ptr2);
	}
	idx_exit();

	return 0;
}
