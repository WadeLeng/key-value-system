/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2011-12-11 20:11
# Filename: interface.c
# Description: 
============================================================================*/
#include "layout.h"
#include "type.h"
#include "kvs.h"
#include "index.h"
#include "buffer.h"
#include "sync.h"

#include <stdio.h>
#include <stdlib.h>

static	char* g_image = NULL;
static	char* IMAGE_file_name = NULL;
FILE*	log_file = NULL;

int kv_init(const KVS_ENV* kvs)
{
	char* buffer_mem = NULL;

	g_image = (char*)malloc(IMAGE_SIZE + kvs->buffer_size);	//malloc all modules memory
	buffer_mem = (PTR_BUF)(g_image + IMAGE_SIZE);

	if (kvs->log_file_path)
	{
		log_file = fopen(kvs->log_file_path, "w");
		if (log_file == NULL)
		{
			fprintf(log_file, "ERROR: open log file fail.\n");
			return -1;
		}
	}

	if (sync_init(kvs->disk_file_path, kvs->bigfile_len) != 0)
	{
		fprintf(log_file, "ERROR: sync init fail.\n");
		return -1;
	}

	IMAGE_file_name = kvs->IMAGE_file_path;
	if (kvs->init_type == INIT_TYPE_LOAD)
	{
		if (sync_image_load(IMAGE_file_name, g_image, IMAGE_SIZE) != 0)
		{
			fprintf(log_file, "ERROR: sync image load fail.\n");
			return -1;
		}
	}

	if (idx_init(g_image, kvs->init_type) != 0)
	{
		fprintf(log_file, "ERROR: index init fail.\n");
		return -1;
	}
	if (buffer_init(buffer_mem, kvs->buffer_size, kvs->buffer_sleep_time, kvs->buffer_horizon_size) != 0)
	{
		fprintf(log_file, "ERROR: buffer init fail.\n");
		return -1;
	}
	fprintf(log_file, "KVS INIT SUCCESS.\n");

	return 0;
}

int kv_exit(int sync_image)
{
	if (buffer_exit() != 0)
	{
		fprintf(log_file, "ERROR: buffer exit fail.\n");
		return -1;
	}
	if (idx_exit() != 0)
	{
		fprintf(log_file, "ERROR: index exit fail.\n");
		return -1;
	}
	if (sync_exit() != 0)
	{
		fprintf(log_file, "ERROR: sync exit fail.\n");
		return -1;
	}
	if (sync_image == 1)
		if (sync_image_save(IMAGE_file_name, g_image, IMAGE_SIZE) != 0)
		{
			fprintf(log_file, "ERROR: sync image fail.\n");
			return -1;
		}
	fprintf(log_file, "KVS EXIT.\n");
	fclose(log_file);

	return 0;
}

int kv_put(const char* key, int key_size, const char* value, int value_size)
{
	IDX_VALUE_INFO value_info_entity;
	IDX_VALUE_INFO* value_info = &value_info_entity;
	PTR_BUF buf_ptr;

	if (value_size > VALUE_MAX_SIZE || value_size < 0)
		return -1;
	
	if (idx_insert(key, key_size, &value_info))
		return -1;

	if (buffer_put(value, value_size, &buf_ptr, value_info))
		return -1;

	return 0;
}

int kv_get(const char* key, int key_size, char* buf, int* buf_size)
{
	IDX_VALUE_INFO value_info_entity;
	IDX_VALUE_INFO* value_info = &value_info_entity;

	if (idx_search(key, key_size, &value_info)) 
		return -1;

	if (value_info->value_size > *buf_size) 
		return -1;

	*buf_size = value_info->value_size;
	if (value_info->buf_ptr != BUF_PTR_NULL)					//value @ buffer
	{
		if (buffer_get(buf, *buf_size, value_info->buf_ptr))
			return -1;
		else return 0;
	}
	else if (value_info->disk_offset != DISK_OFFSET_NULL)		//value @ disk
	{
		if (sync_read(buf, value_info->value_size, value_info->disk_offset))
			return -1;
		else return 0;
	}
	else return -1;

	return 0;
}

int kv_delete(const char* key, int key_size)
{
	IDX_VALUE_INFO value_info;

	if (idx_delete(key, key_size, &value_info))		//copy value_info,delete idx_node
		return -1;

	if (value_info.buf_ptr != BUF_PTR_NULL)
		buffer_delete(value_info.buf_ptr);

	return 0;
}
