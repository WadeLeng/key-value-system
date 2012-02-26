/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified: 2012-02-08 20:44
# Filename: log.c
# Description: 
============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void log_err(const char* filename, const int line, FILE* fp, const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);

	if (fp) {
		vfprintf (fp, fmt, ap);	
		fprintf (fp, "\t\t%s[%d]\n", filename, line);
		fflush(fp);
	} else {
		vprintf (fmt, ap);
		printf ("\t\t%s[%d]\n", filename, line);
	}

	va_end(ap);
	return;
}

