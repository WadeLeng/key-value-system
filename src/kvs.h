/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:59
# Filename:		kvs.h
# Description: 
============================================================================*/
#ifndef KVS_INTERFACE
#define KVS_INTERFACE

#include "type.h"

typedef struct KVS_ENV
{
	enum init_enum	init_type;
	char*		disk_file_path;
	char*		IMAGE_file_path;
	char*		log_file_path;
	int		buffer_sleep_time;
	int		buffer_horizon_size;
	int		buffer_size;
}KVS_ENV;

int kv_init(const KVS_ENV* kvs);
int kv_exit(int sync_image);
int kv_put(const char* key, int key_size, const char* value, int value_size);
int kv_get(const char* key, int key_size, char* buf, int* buf_size);
int kv_delete(const char* key, int key_size);

#endif
