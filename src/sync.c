/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:58
# Filename:		sync.c
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
#include <errno.h>
#include <string.h>

#include "type.h"
#include "layout.h"
#include "sync.h"
#include "log.h"

static int fd = 0;
extern FILE* log_file;

int sync_init(char* pathname)
{
	fd = open(pathname, O_RDWR | O_LARGEFILE | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP); 
	if (fd <= 0) 
	{
		log_err(__FILE__, __LINE__, log_file, "sync_init---open file fail.");
		return -1;
	}

	log_err(__FILE__, __LINE__, log_file, "SYNC INIT SUCCESS.");
	return 0;
}

int sync_read(char* mem, int size, OFFSET_T disk_offset)
{
	off64_t ret;
	ssize_t n = 0, toread;
	char* ptr;

	if (fd <= 0)
	{
		log_err(__FILE__, __LINE__, log_file, "sync_read---fd <= 0.");
		return -1;
	}

	ret = lseek64(fd, (off64_t)disk_offset, SEEK_SET);
	if (ret < 0)
	{
		log_err(__FILE__, __LINE__, log_file, "sync_read---lseek fail.");
		return -1;
	}

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
	{
		log_err(__FILE__, __LINE__, log_file, "sync_write---fd <= 0.");
		return -1;
	}

	ret = lseek64(fd, (off64_t)disk_offset, SEEK_SET);
	if (ret < 0) 
	{
		log_err(__FILE__, __LINE__, log_file, "sync_write---lseek fail.");
		return -1;
	}

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

int sync_image_load(const char* file_name, char* g_image, int size)
{
	off_t ret;
	ssize_t n, toread;
	char* ptr;
	int image_fd;

	image_fd = open(file_name, O_RDWR , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP); 
	if (image_fd <= 0) 
	{
		log_err(__FILE__, __LINE__, log_file, "sync_image_load---fd <= 0.");
		return -1;
	}

	ret = lseek(image_fd, 0, SEEK_SET);
	if (ret < 0)
	{
		log_err(__FILE__, __LINE__, log_file, "sync_image_load---lseek fail.");
		return -1;
	}

	toread = size;
	ptr = g_image;
	while (toread > 0)
	{
		n = read(image_fd, ptr, size);
		toread -= n;
		ptr += n;
	}

	return 0;
}

int sync_image_save(const char* file_name, char* g_image, int size)
{
	off_t ret;
	ssize_t n, towrite;
	char* ptr;
	int image_fd;
	
	image_fd = open(file_name, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP); 
	if (image_fd <= 0) 
	{
		log_err(__FILE__, __LINE__, log_file, "sync_image_save---fd <= 0.");
		return -1;
	}

	ret = lseek(image_fd, 0, SEEK_SET);
	if (ret < 0) 
	{
		log_err(__FILE__, __LINE__, log_file, "sync_image_save---lseek fail.");
		return -1;
	}

	towrite = size;
	ptr = g_image;
	while (towrite > 0)
	{
		n = write(image_fd, ptr, size); 
		towrite -= n;
		ptr += n;
	}

	return 0;
}

int sync_exit()
{
	if (log_file)
		log_err(__FILE__, __LINE__, log_file, "SYNC EXIT.");
	close(fd);

	return 0;
}

