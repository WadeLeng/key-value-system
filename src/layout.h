/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:59
# Filename:		layout.h
# Description: 
============================================================================*/
#ifndef	KVS_LAYOUT
#define	KVS_LAYOUT

/* 1.SuperImage Layout:
 * total 1 KB
 * FIXME: Something may be here in the near future.
 * may be used in function: kv_init() @ interface.c
 * Magic Number? KVS instance version?
 */

/*2.IndexImage Layout:
 * total 360002228 B = 343.3MB
 */
#define	IMAGE_HT_TABLE			1024		//300 * 4	
#define	IMAGE_IDX_NODES			2224		//1000w * 32
#define	IMAGE_FREE_IDX_NODES		320002224	//1000w * 4
#define	IMAGE_FREE_IDX_NODES_HORIZON	360002224	//4

#define	IMAGE_SIZE			360002228

#define	IDX_NODES_MAX			10000000
#define	IDX_HT_TABLE_SIZE		300

#define	DISK_IMAGE_OFFSET		0
#define	DISK_VALUE_OFFSET		0			//320002228

#define	VALUE_MAX_SIZE			(5*1024*1024)	// (5MB)

#define	BUF_PTR_NULL			NULL
#define	DISK_OFFSET_NULL		-1
#define	IDX_NODE_NULL			-1

#endif
