/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2011-12-11 20:11
# Filename: buffer.h
# Description: 
============================================================================*/
#ifndef KVS_BUFFER
#define KVS_BUFFER

#include "index.h"
#include "type.h"

typedef struct buf_word
{
	IDX_VALUE_INFO* value_info_ptr;
	enum state_t state;
}buf_word;

int buffer_init(const char* buffer_mem, const char* buffer_log, const int buffer_size, const int buffer_sleep_time, const int horizon_size);
int buffer_put(const char* value, int value_size, PTR_BUF* buf_value_ptr, IDX_VALUE_INFO* value_info_ptr);
int buffer_get(PTR_BUF buf, int buf_size, PTR_BUF buf_value_ptr);
int buffer_delete(PTR_BUF buf_value_ptr);
int buffer_exit();

#endif
