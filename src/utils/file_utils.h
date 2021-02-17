int utils_little_endian();

size_t utils_fsize(const char *f_name, int f_close);

size_t utils_fwrite(const char *f_name, const void *f_write, size_t write_size, size_t write_len, long int write_at, int write_clear);

size_t utils_fread(const char *f_name, void *f_buffer, size_t read_size, size_t buffer_len, long int read_at);