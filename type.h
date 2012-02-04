/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:59
# Filename:		type.h
# Description: 
============================================================================*/
#ifndef KVS_TYPE
#define KVS_TYPE

#define	_FILE_OFFSET_BITS	64
#define	_LARGEFILE_SOURCE
#define	_LARGEFILE64_SOURCE

typedef int			PTR_KW;
typedef char*		PTR_BUF;
typedef	int	 		OFFSET_T;
typedef int			INIT_TYPE;
typedef int			HASH;
enum init_enum {INIT_TYPE_CREATE, INIT_TYPE_LOAD};
enum state_t { p_unavail, p_flushed, p_not_flush };

#endif
