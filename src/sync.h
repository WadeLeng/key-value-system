/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:59
# Filename:		sync.h
# Description: 
============================================================================*/
#ifndef KVS_SYNC
#define KVS_SYNC

#include "type.h"

int sync_init(char* pathname, int file_len);
int sync_read(char* mem, int size, OFFSET_T disk_offset);
int sync_write(const char* mem, int size, OFFSET_T disk_offset);
int sync_image_load(const char* file_name, char* g_image, int size);
int sync_image_save(const char* file_name, char* g_image, int size);
int sync_exit();

#endif
