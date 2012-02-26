/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2012-02-25 15:56
# Filename: best_speed.c
# Description: test best write speed of disk (1000MB/9.69s)
============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE (1024*100)
char value_buffer[SIZE + 1];

int main()
{
	int i, fd = 0, size;
	ssize_t n, towrite;
	char* ptr;
	off_t ret;
	long long start, end, duration;
	struct timeval tv;

	for (i = 0; i < SIZE; i++)
		value_buffer[i] = 'a';

	fd = open("write_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

	ret = lseek(fd, 0, SEEK_SET);
	duration = 0;
	for (i = 0; i < 10240; i++)
	{
		towrite = SIZE;
		gettimeofday(&tv, NULL);
		start = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
		ptr = value_buffer;

		while (towrite > 0)
		{
			n = write(fd, ptr, SIZE);
			towrite -= n;
			ptr += n;
		}

		gettimeofday(&tv, NULL);
		end = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
		duration += (end - start) / 1000;
	}

	printf("over: %lld ms\n", duration);
	close(fd);

	return 0;
}

