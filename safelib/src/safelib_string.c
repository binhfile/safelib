#ifdef USE_SECURE_LIB
#include "../include/safelib.h"
#include <sys/types.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 * http://www.opensource.apple.com/source/OpenSSH/OpenSSH-7.1/openssh/bsd-strlcpy.c
 */
size_t _strncpy_safe(dst, src, siz)
	char *dst;
	const char *src;
	size_t siz;
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;
	if(!dst || !src) return 0;

	/* Copy as many bytes as will fit */
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}
/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 * http://www.opensource.apple.com/source/OpenSSH/OpenSSH-7.1/openssh/bsd-strlcat.c
 */
size_t _strncat_safe(dst, src, siz)
	char *dst;
	const char *src;
	size_t siz;
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;
	size_t dlen;
	if(!dst || !src) return 0;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (*d != '\0' && n-- != 0)
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen_safe(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}

size_t _strlen_safe(const char *str, size_t max_len)
{
    const char * end = 0;
    if(!str) return 0;
    end = (const char *)memchr(str, '\0', max_len);
    if (end == NULL)
        return 0;
    else
        return end - str;
}
int _snprintf_safe(char * buf, size_t size,
			 const char * format, ...)
{
	va_list arg;
	int rv;
	if(!buf|| !format) return 0;
	va_start(arg, format);
	rv = _vsnprintf_safe(buf, size, format, arg);
	va_end(arg);
	return rv;
}
int _vsnprintf_safe (s, n, format, ap)
     char * s;
     size_t n;
     const char *format;
     va_list ap;
{
  char *buf = 0;
  int result = vasprintf (&buf, format, ap);

  if (!buf)
    return -1;
  if (result < 0)
    {
      free (buf);
      return -1;
    }

  result = strlen_safe (buf);
  if (n > 0)
    {
      if ((long) n > result)
    	  memcpy_safe (s, buf, result+1, n);
      else
        {
    	  memcpy_safe (s, buf, n-1, n);
	  s[n - 1] = 0;
	}
    }
  free (buf);
  return result;
}

void *_memcpy_safe (
		void *__restrict __dest,
		const void *__restrict __src,
		size_t __n,
		size_t __dest_size) {
	int i = 0;
	const unsigned char*__restrict s = (const unsigned char*__restrict)__src;
	unsigned char*__restrict d = (unsigned char*__restrict)__dest;

	if(__n > __dest_size){
		__n = __dest_size;
	}
	for(i = 0; i < __n; i++){
		d[i] = s[i];
	}
	return __dest;
}
#endif

