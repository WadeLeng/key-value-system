/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2012-02-08 20:43
# Filename: log.h
# Description: 
============================================================================*/
#ifndef KVS_LOG
#define KVS_LOG

void log_err(const char* filename, const int line, FILE* fp, const char* fmt, ...);

#endif
