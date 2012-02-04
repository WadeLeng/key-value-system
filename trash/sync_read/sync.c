/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2011-12-16 15:19
# Filename: sync.c
# Description: 
============================================================================*/
#define	_FILE_OFFSET_BITS	64
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "type.h"
#include "layout.h"
#include "sync.h"

static int fd = 0;
static FILE* log_file = NULL;

int sync_init(char* pathname, int file_len, char* sync_log)
{
	if (sync_log) {
		log_file = fopen(sync_log, "w");/*XXX: append write?? */
		if (log_file == NULL) 
			return -1;
	} 

	fd = open(pathname, O_RDWR | O_LARGEFILE | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP); 
	if (fd <= 0) 
		return -1;

	return 0;
}

int sync_read(char* mem, int size, OFFSET_T disk_offset)
{
	off64_t ret;
	ssize_t n = 0, toread;
	char* ptr;

	if (fd <= 0)
		return -1;

	ret=lseek64(fd, (off64_t)disk_offset, SEEK_SET);
	if (ret < 0)
		return -1;

	toread = size;
	ptr = mem;
	while (toread > 0)
	{
		n = read(fd, ptr, size);
		toread -= n;
		ptr += n;
	}

	return 0;
}

int sync_write(const char* mem, int size, OFFSET_T disk_offset)
{
	off64_t ret;
	ssize_t n, towrite;
	const char* ptr;

	if (fd <= 0) 
		return -1;

	ret = lseek64(fd, (off64_t)disk_offset, SEEK_SET);
	if (ret < 0) 
		return -1;

	towrite = size;
	ptr = mem;
	while (towrite > 0)
	{
		n = write(fd, ptr, size); 
		towrite -= n;
		ptr += n;
	}

	return 0;
}

int sync_flush()
{
	return 0;
}

int sync_exit()
{
	if (log_file) {
		fprintf (log_file, "EXIT.\n");
		fclose(log_file);
	}
	log_file=NULL;
	close(fd);
	if (!sync_flush())
		return -1;

	return 0;
}

