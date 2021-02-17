/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
/******* include custom libs ******/
#include "file_utils.h"

int utils_little_endian() {
	
	short int test_word = 0x0001;
	char *byte = (char *)&test_word;
	
	return byte[0];
}

size_t utils_fsize(const char *f_name, int f_close) {
	
	FILE *f;
	size_t f_size;
	
	f = fopen(f_name, "r"); //open file for reading
	if (f == NULL) { return -1; }
	
	fseek(f, 0L, SEEK_END); //seek to end of file
	f_size = ftell(f); //get size of file
	
	if (f_close) { fclose(f); } //cleanup
	
	return f_size;
}

size_t utils_fwrite(const char *f_name, const void *f_write, size_t write_size, size_t write_len, long int write_at, int write_clear) {
	
	FILE *f;
	size_t f_write_len;
	
	if (write_clear) { f = fopen(f_name, "wb+"); } //open file for writing, clearing contents if file exists;
	else if (write_at > -1) { f = fopen(f_name, "rb+"); }	//open file for reading and writing
	else { f = fopen(f_name, "ab"); }
	if (f == NULL) { return 0; }
	
	if (write_at > -1) { fseek(f, write_at, SEEK_SET); } //write to specific location if desired
	
	f_write_len = fwrite(f_write, write_size, write_len, f); //write to file
	
	fclose(f); //cleanup
	
	if (f_write_len == write_len) { return write_size * f_write_len; }
	return 0;
}

size_t utils_fread(const char *f_name, void *f_buffer, size_t read_size, size_t buffer_len, long int read_at) {
	
	FILE *f;
	size_t f_read_len;

	f = fopen(f_name, "rb"); //open file for reading
	if (f == NULL) { return 0; }
	
	if (read_at > -1) { fseek(f, read_at, SEEK_SET); } //start reading specific location if desired
	f_read_len = fread(f_buffer, read_size, buffer_len, f); //read file to buffer
	
	//utils_endian_convert(f_buffer) //assumes input is big endian
	
	fclose(f); //cleanup
	
	if (f_read_len == buffer_len) { return read_size * f_read_len; }
	return 0;
}

void utils_endian_convert(void *word, size_t word_len) {
	
//	int i = 0;
	
//	for (i = 0; i < (word_len * sizeof(word))-2; i+=2) {
//		
//		uint8_t temp = word[i];
//		word[i] = word[i + sizeof(uint8_t)];
//		word[i + sizeof(uint8_t)] = temp;
//	}
//	
	return;
}