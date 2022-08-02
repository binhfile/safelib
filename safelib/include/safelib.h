#ifndef INCLUDE_SAFELIB_H_
#define INCLUDE_SAFELIB_H_
/**
 * http://www.dwheeler.com/secure-programs/Secure-Programs-HOWTO/library-c.html
 * https://www.securecoding.cert.org/confluence/pages/viewpage.action?pageId=2130132
 */
#include <sys/types.h>
#include <string.h>
#include <stdio.h>


#ifdef USE_SECURE_LIB
#define SAFELIB_MAX_STRING_LENGTH			4096	/*one page*/
/*string function*/
#define strncpy_safe		_strncpy_safe
#define strncat_safe		_strncat_safe
#define strlen_safe(x)		_strlen_safe(x, SAFELIB_MAX_STRING_LENGTH)
#define snprintf_safe		_snprintf_safe
#define memcpy_safe			_memcpy_safe
/*system function*/
#define popen_safe			_popen_safe	// ? thread-safe
#define usleep_safe			_usleep_safe
#else
/*string function*/
#define strncpy_safe		strncpy
#define strncat_safe		strncat
#define strlen_safe			strlen
#define snprintf_safe		snprintf
#define memcpy_safe(d, s, cnt, d_n)			memcpy(d, s, cnt)
/*system function*/
#define popen_safe			popen
#define usleep_safe			usleep
#endif

#ifdef USE_SECURE_LIB
/**
 * copies up to n-1 characters from the NUL-terminated string src to dst,
 * NIL-terminating the result
 */
size_t _strncpy_safe(char* dst, const char* src, size_t n);
/**
 * appends the NIL-terminated string src to the end of dst.
 * It will append at most size - strlen(dst) - 1 bytes,
 * NIL-terminating the result.
 */
size_t _strncat_safe(char* dst, const char* src, size_t n);
/**
 * retval = 0 if search > max_len
 */
size_t _strlen_safe(const char* src, size_t max_len);
int _snprintf_safe(char * buf, size_t size, const char * format, ...);
int _vsnprintf_safe (char* s, size_t n, const char* format, va_list ap);
void *_memcpy_safe (
		void *__restrict __dest,
		const void *__restrict __src,
		size_t __n,
		size_t __dest_size);

/*system functions*/
FILE *_popen_safe(const char *cmd, const char *type);
int _usleep_safe (__useconds_t usec);
#endif //#ifdef USE_SECURE_LIB

#endif /* INCLUDE_SAFELIB_H_ */
