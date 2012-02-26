#include <stdio.h>
#include <stdlib.h>

#include "sync.h"
#include "layout.h"
#include "type.h"

int state;
char ans[5*1024];
OFFSET_T disk_offset;

int main()
{
	state = sync_init("file.txt", 4096*2, "log.txt");
	if (state != 0)
	{
		printf("sync_init fail\n");
		return -1;
	}

	state = sync_read(ans, 10, 4095);
	if (state != 0)
	{	
		printf("sync_read fail\n");
		return -1;
	}
	ans[10] = '\0';
	printf("read answer: %s\n", ans);

	state = sync_write(ans, 10, 1);
	if (state != 0)
	{	
		printf("sync_write fail\n");
		return -1;
	}
	state = sync_read(ans, 10, 1);
	if (state != 0)
	{	
		printf("sync_read fail\n");
		return -1;
	}
	ans[10] = '\0';
	printf("read answer: %s\n", ans);

	state = sync_exit();
	if (state != 0)
	{	
		printf("sync_exit fail\n");
		return -1;
	}

	return 0;
}
