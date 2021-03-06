/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2011-12-11 20:11
# Filename: index.h
# Description: 
============================================================================*/
#ifndef KVS_INDEX
#define KVS_INDEX

#include "type.h"

typedef struct IDX_VALUE_INFO
{
	int		value_size;
	PTR_BUF		buf_ptr;
	OFFSET_T	disk_offset;
}IDX_VALUE_INFO;

int idx_init(const char* image, const char* index_log, INIT_TYPE init_type);
int idx_insert(const char* key, int key_size, IDX_VALUE_INFO** value_info);
int idx_search(const char* key, int key_size, IDX_VALUE_INFO** value_info);
int idx_delete(const char* key, int key_size, IDX_VALUE_INFO* value_info);
int idx_exit();

#endif
