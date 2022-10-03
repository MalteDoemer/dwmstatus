#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdint.h>

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))

/** print an error message
 * 
 * Additonally calls perror(NULL) if fmt ends with a colon ':'
 * 
 * @param[in] fmt the printf style format string.
 */
void warn(const char* fmt, ...);

/** print an error message and exit(1).
 * 
 * Additonally calls perror(NULL) if fmt ends with a colon ':'
 * 
 * @param[in] fmt the printf style format string.
 */
void die(const char* fmt, ...);

/* fscanf directly from a path */
int pscanf(const char* path, const char* fmt, ...);

int fmt_power_of_ten(uintmax_t num_bytes, int base, char* buffer, size_t buffer_size);

/** read from file into a buffer and remove trailing whitespace (including newlines)
 * 
 * @param[in] file the file to read from
 * 
 * @param[out] buffer the buffer to write into
 * 
 * @param[in] buffer_size the maximum size of the buffer
 * 
 * @return the number of characters read
*/
size_t fread_trim(FILE* file, char* buffer, size_t buffer_size);

typedef enum result_t {
    RESULT_SUCCESS = 0,
    RESULT_ERROR = -1,
} result_t;

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE,
} str2int_result_t;


/** convert a string to an int.
 * 
 * @param[out] out the converted int, cannot be NULL
 * 
 * @param[in] s the string to be converted
 * 
 * @param[in] base the base to interpret the string
 * 
 * @return whether the conversion succeded or not
*/
str2int_result_t str2int(int* out, char *s, int base);

#endif // #ifndef UTIL_H
