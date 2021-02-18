/******* include custom libs ******/
#include "file_utils.h"

FILE* utils_fopen(const char *f_name, char *mode) { return fopen(f_name, mode); }

int utils_fclose(FILE *f) { return fclose(f); }

size_t utils_fsize(const char *f_name, int f_close) {
	
	FILE *f;
	size_t f_size;
	
	f = utils_fopen(f_name, "rb"); //open file for reading
	if (f == NULL) { return -1; }
	
	fseek(f, 0L, SEEK_END); //seek to end of file
	f_size = ftell(f); //get size of file
	
	if (f_close) { utils_fclose(f); } //cleanup
	
	return f_size;
}

size_t utils_fread(const char *f_name, FileReadBuffer f_buffer, long int read_at, int endian_flag) {
	
	FILE *f;
	size_t f_read_len = 0;
	size_t f_read_total = 0;
	
	f = utils_fopen(f_name, "rb"); //open file for reading
	if (f == NULL) { return -1; }
	if (read_at > -1) { fseek(f, read_at, SEEK_SET); } //start reading specific location if desired
	
//	printf("debug -- file read start\n");
	f_read_len = fread(f_buffer.buffer, f_buffer.size, f_buffer.nmemb, f); //read file to buffer
	if (f_read_len == f_buffer.nmemb) { f_read_total += f_read_len * f_buffer.size; }
	else { return 0; }
	
	printf("debug -- file read size: [%ld]   file read length: [%ld]   file read total: [%ld]\n", f_buffer.size, f_read_len, f_read_total);
	
	if (endian_flag && f_buffer.size > sizeof(uint8_t)) { //if endianness matters, convert buffer.buffer from big endian to host default
	
		if (f_buffer.size == sizeof(uint16_t)) 		{ *((uint16_t*)f_buffer.buffer) = be16toh(*((uint16_t*)f_buffer.buffer)); }
		else if (f_buffer.size == sizeof(uint32_t)) { *((uint32_t*)f_buffer.buffer) = be32toh(*((uint32_t*)f_buffer.buffer)); }
		else 										{ *((uint64_t*)f_buffer.buffer) = be64toh(*((uint64_t*)f_buffer.buffer)); }
	}
	
	utils_fclose(f); //cleanup
	
	return f_read_total;
}

size_t utils_fread_m(const char *f_name, int num_buffer, int buffer_offset, FileReadBuffer f_buffer[], long int read_at, int endian_flag) {
	
	int i = 0;
	
	FILE *f;
	size_t f_read_len = 0;
	size_t f_read_total = 0;
	
	f = utils_fopen(f_name, "rb"); //open file for reading
	if (f == NULL) { return -1; }
	if (read_at > -1) { fseek(f, read_at, SEEK_SET); } //start reading specific location if desired
	
//	printf("debug -- file read start\n");
	for (i = buffer_offset; i < num_buffer; i++) {
		
		f_read_len = fread(f_buffer[i].buffer, f_buffer[i].size, f_buffer[i].nmemb, f);
		if (f_read_len == f_buffer[i].nmemb) { f_read_total += f_read_len * f_buffer[i].size; }
		else { return 0; }
		
		if (endian_flag && f_buffer[i].size > sizeof(uint8_t)) { //if endianness matters, convert buffer.buffer from big endian to host default
	
			if (f_buffer[i].size == sizeof(uint16_t)) 		{ *((uint16_t*)f_buffer[i].buffer) = be16toh(*((uint16_t*)f_buffer[i].buffer)); }
			else if (f_buffer[i].size == sizeof(uint32_t)) { *((uint32_t*)f_buffer[i].buffer) = be32toh(*((uint32_t*)f_buffer[i].buffer)); }
			else 										{ *((uint64_t*)f_buffer[i].buffer) = be64toh(*((uint64_t*)f_buffer[i].buffer)); }
		}
		
//		printf("debug -- file read size: [%ld]   file read length: [%ld]   file read total: [%ld]\n", f_buffer[i].size, f_read_len, f_read_total);
	}
	
	utils_fclose(f); //cleanup
	
	return f_read_total;
}

size_t utils_fwrite(const char *f_name, FileWriteBuffer f_buffer, long int write_at, int write_clear, int endian_flag) {
	
	FILE *f;
	size_t f_write_len = 0;
	char *f_mode = "";
	
	if (write_clear) { f_mode = "wb+"; } 		//open file for writing, clearing contents if file exists;
	else if (write_at > -1) { f_mode = "rb+"; } //open file for reading and writing
	else { f_mode = "ab"; }
	f = utils_fopen(f_name, f_mode); //open file for reading
	if (f == NULL) { return -1; }
	
	if (write_at > -1) { fseek(f, write_at, SEEK_SET); } //write to specific location if desired
	
//	printf("debug -- file write start\n");	
	if (endian_flag && f_buffer.size > sizeof(uint8_t)) { //if endianness matters, convert buffer.buffer to big endian format
	
		if (f_buffer.size == sizeof(uint16_t)) 		{ *((uint16_t*)f_buffer.buffer) = htobe16(*((uint16_t*)f_buffer.buffer)); }
		else if (f_buffer.size == sizeof(uint32_t)) { *((uint32_t*)f_buffer.buffer) = htobe32(*((uint32_t*)f_buffer.buffer)); }
		else 										{ *((uint64_t*)f_buffer.buffer) = htobe64(*((uint64_t*)f_buffer.buffer)); }
	}
	f_write_len = fwrite(f_buffer.buffer, f_buffer.size, f_buffer.nmemb, f); //write to file
	
	utils_fclose(f); //cleanup
	
	if (f_write_len == f_buffer.nmemb) { return f_buffer.size * f_write_len; }
	return 0;
}

size_t utils_fwrite_m(const char *f_name, int num_buffer, int buffer_offset, FileWriteBuffer f_buffer[], long int write_at, int write_clear, int endian_flag) {
	
	int i = 0;
	
	FILE *f;
	size_t f_write_len = 0;
	size_t f_write_total = 0;
	char *f_mode = "";
	
	if (write_clear) { f_mode = "wb+"; } 		//open file for writing, clearing contents if file exists;
	else if (write_at > -1) { f_mode = "rb+"; } //open file for reading and writing
	else { f_mode = "ab"; }
	f = utils_fopen(f_name, f_mode); //open file for reading
	if (f == NULL) { return -1; }
	
	if (write_at > -1) { fseek(f, write_at, SEEK_SET); } //write to specific location if desired
	
//	printf("debug -- file write start\n");
	for (i = buffer_offset; i < num_buffer; i++) {
		
		if (endian_flag && f_buffer[i].size > sizeof(uint8_t)) { //if endianness matters, convert buffer.buffer to big endian format
		
			if (f_buffer[i].size == sizeof(uint16_t)) 		{ *((uint16_t*)f_buffer[i].buffer) = htobe16(*((uint16_t*)f_buffer[i].buffer)); }
			else if (f_buffer[i].size == sizeof(uint32_t)) { *((uint32_t*)f_buffer[i].buffer) = htobe32(*((uint32_t*)f_buffer[i].buffer)); }
			else 										{ *((uint64_t*)f_buffer[i].buffer) = htobe64(*((uint64_t*)f_buffer[i].buffer)); }
		}
		f_write_len = fwrite(f_buffer[i].buffer, f_buffer[i].size, f_buffer[i].nmemb, f); //write to file
		
		if (f_write_len == f_buffer[i].nmemb) { f_write_total += f_write_len * f_buffer[i].size; }
		else { return 0; }
		
//		printf("debug -- file write size: [%ld]   file write length: [%ld]   file write total: [%ld]\n", f_buffer[i].size, f_write_len, f_write_total);
	}
	
	utils_fclose(f); //cleanup
	
	return f_write_total;
}