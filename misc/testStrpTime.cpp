#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define NS_PER_SEC 1000000000ULL

uint32_t tv_sec = 0;
uint32_t tv_nsec = 0;

char* strptime(const char* s, const char* format) {
  time_t now;
#ifdef __linux__
  struct timespec T;
  clock_gettime(CLOCK_REALTIME, &T);
  tv_sec = static_cast<uint32_t>(T.tv_sec);
  tv_nsec = static_cast<uint32_t>(T.tv_nsec);
  now = tv_sec;
#else
  time(&now);
  tv_sec = now;
  tv_nsec = 0;
#endif

  struct tm* ptm;
#if !defined(_WIN32)
  struct tm tmBuf;
  ptm = localtime_r(&now, &tmBuf);
#else
  ptm = localtime(&now);
#endif

  char fmt[strlen(format) + 1];
  strcpy(fmt, format);

  char* ret = const_cast<char*>(s);
  char* cp;
  for (char* f = cp = fmt;; ++cp) {
    if (!*cp) {
      if (f != cp) {
        ret = ::strptime(ret, f, ptm);
      }
      break;
    }
    if (*cp != '%') {
      continue;
    }
    char* e = cp;
    ++e;
//#if (defined(__BIONIC__))
    if (*e == 's') {
      *cp = '\0';
      if (*f) {
        ret = ::strptime(ret, f, ptm);
        if (!ret) {
          break;
        }
      }
      tv_sec = 0;
      while (isdigit(*ret)) {
        tv_sec = tv_sec * 10 + *ret - '0';
        ++ret;
      }
      now = tv_sec;
#if !defined(_WIN32)
      ptm = localtime_r(&now, &tmBuf);
#else
      ptm = localtime(&now);
#endif
    } else
//#endif
    {
      unsigned num = 0;
      while (isdigit(*e)) {
        num = num * 10 + *e - '0';
        ++e;
      }
      if (*e != 'q') {
        continue;
      }
      *cp = '\0';
      if (*f) {
        ret = ::strptime(ret, f, ptm);
        if (!ret) {
          break;
        }
      }
      unsigned long mul = NS_PER_SEC;
      if (num == 0) {
        num = INT_MAX;
      }
      tv_nsec = 0;
      while (isdigit(*ret) && num && (mul > 1)) {
        --num;
        mul /= 10;
        tv_nsec = tv_nsec + (*ret - '0') * mul;
        ++ret;
      }
    }
    f = cp = e;
    ++f;
  }

  if (ret) {
    tv_sec = mktime(ptm);
    return ret;
  }

// Upon error, place a known value into the class, the current time.
  time(&now);
  tv_sec = now;
  tv_nsec = 0;

  return ret;
}


int main(int argc, char *argv[])
{
    char str[] = "[  262.123456 @2] hello world!";  

    char* ret = strptime(str, "[ %s.%q @2]");
    printf("ret:%s, tv_sec:%d, tv_nsec:%d\n", ret, tv_sec, tv_nsec);

    return 0;
}
