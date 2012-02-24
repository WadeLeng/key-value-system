/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:59
# Filename:		interface.c
# Description: 
============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "layout.h"
#include "type.h"
#include "kvs.h"
#include "index.h"
#include "buffer.h"
#include "sync.h"
#include "log.h"

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
			log_err(__FILE__, __LINE__, log_file, "kvs_init---open log file fail.");
			return -1;
		}
	} else
		log_file = NULL;

	if (sync_init(kvs->disk_file_path) != 0)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_init---sync_init fail.");
		return -1;
	}

	IMAGE_file_name = kvs->IMAGE_file_path;
	if (kvs->init_type == INIT_TYPE_LOAD)
	{
		if (sync_image_load(IMAGE_file_name, g_image, IMAGE_SIZE) != 0)
		{
			log_err(__FILE__, __LINE__, log_file, "kv_init---sync_image_load fail.");
			return -1;
		}
	}

	if (idx_init(g_image, kvs->init_type) != 0)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_init---idx_init fail.");
		return -1;
	}
	if (kvs->buffer_size <= MINIMUM_BUFFER_SIZE)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_init---buffer_size smaller than MINIMUM_BUFFER_SIZE.");
		return -1;
	}
	if (buffer_init(buffer_mem, kvs->buffer_size, kvs->buffer_sleep_time, kvs->buffer_horizon_size) != 0)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_init---buffer_init fail.");
		return -1;
	}
	log_err(__FILE__, __LINE__, log_file, "KVS INIT SUCCESS.");

	return 0;
}

int kv_exit(int sync_image)
{
	if (buffer_exit() != 0)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_exit---buffer_exit fail.");
		return -1;
	}
	if (idx_exit() != 0)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_exit---idx_exit fail.");
		return -1;
	}
	if (sync_exit() != 0)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_exit---sync_exit fail.");
		return -1;
	}
	if (sync_image == 1)
		if (sync_image_save(IMAGE_file_name, g_image, IMAGE_SIZE) != 0)
		{
			log_err(__FILE__, __LINE__, log_file, "kv_exit---sync_image_save fail.");
			return -1;
		}
	log_err(__FILE__, __LINE__, log_file, "KVS EXIT.");
	if (log_file)
		fclose(log_file);

	return 0;
}

int kv_put(const char* key, int key_size, const char* value, int value_size)
{
	IDX_VALUE_INFO value_info_entity;
	IDX_VALUE_INFO* value_info = &value_info_entity;
	PTR_BUF buf_ptr;

	if (value_size > VALUE_MAX_SIZE || value_size < 0)
	{
		log_err(__FILE__, __LINE__, log_file, "kv_put---value_size illegal.");
		return -1;
	}
	
	if (idx_insert(key, key_size, &value_info))
	{
		log_err(__FILE__, __LINE__, log_file, "kv_put---idx_insert fail.");
		return -1;
	}

	if (buffer_put(value, value_size, &buf_ptr, value_info))
	{
		log_err(__FILE__, __LINE__, log_file, "kv_put---buffer_put fail.");
		return -1;
	}

	return 0;
}

int kv_get(const char* key, int key_size, char* buf, int* buf_size)
{
	IDX_VALUE_INFO value_info_entity;
	IDX_VALUE_INFO* value_info = &value_info_entity;

	if (idx_search(key, key_size, &value_info)) 
	{
		log_err(__FILE__, __LINE__, log_file, "kv_get---idx_search fail.");
		return -1;
	}

	if (value_info->value_size > *buf_size) 
	{
		log_err(__FILE__, __LINE__, log_file, "kv_get---buf_size too small.");
		return -1;
	}

	*buf_size = value_info->value_size;
	if (value_info->buf_ptr != BUF_PTR_NULL)					//value @ buffer
	{
		if (buffer_get(buf, *buf_size, value_info->buf_ptr))
		{
			log_err(__FILE__, __LINE__, log_file, "kv_get---buffer_get fail.");
			return -1;
		}
		else return 0;
	}
	else if (value_info->disk_offset != DISK_OFFSET_NULL)		//value @ disk
	{
		if (sync_read(buf, value_info->value_size, value_info->disk_offset))
		{
			log_err(__FILE__, __LINE__, log_file, "kv_get---sync_read fail.");
			return -1;
		}
		else return 0;
	}
	else 
	{
		log_err(__FILE__, __LINE__, log_file, "kv_get---can not find value.");
		return -1;
	}

	return 0;
}

int kv_delete(const char* key, int key_size)
{
	IDX_VALUE_INFO value_info;

	if (idx_delete(key, key_size, &value_info))		//copy value_info,delete idx_node
	{
		log_err(__FILE__, __LINE__, log_file, "kv_delete---idx_delete fail.");
		return -1;
	}

	if (value_info.buf_ptr != BUF_PTR_NULL)
		buffer_delete(value_info.buf_ptr);

	return 0;
}
