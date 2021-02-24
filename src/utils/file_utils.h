#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>

/******* struct declarations ******/
typedef struct {
	
	void *buffer;
	size_t nmemb;
	size_t size;
} FileReadBuffer;

typedef struct {
	
	const void *buffer;
	size_t nmemb;
	size_t size;
} FileWriteBuffer;

#endif /* FILE_UTILS_H */

/****** function declarations *****/
FILE* utils_fopen(const char *f_name, char *mode);

int utils_fclose(FILE *f);

size_t utils_fsize(const char *f_name, int f_close);

size_t utils_fread(const char *f_name, FileReadBuffer f_buffer, long int read_at, int endian_flag);

size_t utils_fread_m(const char *f_name, int num_buffer, int buffer_offset, FileReadBuffer f_buffer[], long int read_at, int endian_flag);

size_t utils_fwrite(const char *f_name, FileWriteBuffer f_buffer, long int write_at, int write_clear, int endian_flag);

size_t utils_fwrite_m(const char *f_name, int num_buffer, int buffer_offset, FileWriteBuffer f_buffer[], long int write_at, int write_clear, int endian_flag);