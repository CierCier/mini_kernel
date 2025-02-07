#include <stddef.h>
#include <stdint.h>

#include <kernel/klibc/kstring.h>

const char *hex = "0123456789ABCDEF";

// Memory functions

void *memcpy(void *dest, const void *src, size_t count) {
  uint8_t *d = (uint8_t *)dest;
  const uint8_t *s = (const uint8_t *)src;

  for (size_t i = 0; i < count; i++) {
    d[i] = s[i];
  }

  return dest;
}

void *memset(void *dest, uint8_t val, size_t count) {
  uint8_t *d = (uint8_t *)dest;

  for (size_t i = 0; i < count; i++) {
    d[i] = val;
  }

  return dest;
}

void *memmove(void *dest, void *src, size_t count) {
  uint8_t *d = (uint8_t *)dest;
  uint8_t *s = (uint8_t *)src;
  for (size_t i = 0; i < count; i++) {
    d[i] = s[i];
    s[i] = 0;
  }
  return dest;
}

// String functions

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

// String Compare
int strcmp(const char *str1, const char *str2) {
  while (*str1 && *str2 && *str1 == *str2) {
    str1++;
    str2++;
  }

  return *str1 - *str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {

  while (n > 0 && (*str1 && *str2 && *str1 == *str2)) {
    str1++;
    str2++;
    n--;
  }

  return *str1 - *str2;
}

int strcasecmp(const char *str1, const char *str2) { return 0; }

int strncasecmp(const char *str1, const char *str2, size_t n) { return 0; }

// String Copy
char *strcpy(char *dest, const char *src) {
  char *d = dest;
  while (*src) {
    *d = *src;
    d++;
    src++;
  }
  *d = '\0';
  return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
  char *d = dest;
  while (*src && n > 0) {
    *d = *src;
    d++;
    src++;
    n--;
  }
  *d = '\0';
  return dest;
}

// String Concatenation
char *strcat(char *dest, const char *src) {
  char *d = dest;
  while (*d) {
    d++;
  }
  while (*src) {
    *d = *src;
    d++;
    src++;
  }
  *d = '\0';
  return dest;
}

char *strncat(char *dest, const char *src, size_t n) {
  char *d = dest;
  while (*d) {
    d++;
  }
  while (*src && n > 0) {
    *d = *src;
    d++;
    src++;
    n--;
  }
  *d = '\0';
  return dest;
}

// String Search
char *strchr(const char *str, int c) {
  while (*str) {
    if (*str == c) {
      return (char *)str;
    }
    str++;
  }
  return NULL;
}

char *strrchr(const char *str, int c) {
  char *last = NULL;

  while (*str) {
    if (*str == c) {
      last = (char *)str;
    }
    str++;
  }
  return last;
}

char *strstr(const char *haystack, const char *needle) {

  static char *start = NULL;
  static char *c_needle = NULL;

  c_needle = (needle != NULL) ? (char *)needle : c_needle;
  start = (haystack != NULL) ? (char *)haystack : start;

  if (c_needle == NULL || start == NULL) {
    return NULL;
  }

  int needle_len = strlen(c_needle);
  int haystack_len = strlen(start);

  if (needle_len == 0) {
    return start;
  }

  if (haystack_len == 0) {
    return NULL;
  }

  for (int i = 0; i < haystack_len; i++) {
    if (start[i] == c_needle[0]) {
      int j = 0;
      for (j = 0; j < needle_len; j++) {
        if (start[i + j] != c_needle[j]) {
          break;
        }
      }
      if (j == needle_len) {
        return start + i;
      }
    }
  }

  return NULL;
}

// String Tokenize
char *strtok(char *str, const char *delim) {

  // TODO: Implement strtok
  return NULL;
}

// String Conversion
int atoi(const char *str) { return (int)atol(str); }

long atol(const char *str) { return (long)atoll(str); }

long long atoll(const char *str) {
  long long num = 0;

  while (*str) {
    num = num * 10 + (*str - '0');
    str++;
  }

  return num;
}

double atof(const char *str) {
  double num = 0;
  double dec = 0;
  int dec_flag = 0;
  int dec_count = 0;

  while (*str) {
    if (*str == '.') {
      dec_flag = 1;
      str++;
      continue;
    }

    if (dec_flag) {
      dec = dec * 10 + (*str - '0');
      dec_count++;
    } else {
      num = num * 10 + (*str - '0');
    }
    str++;
  }

  while (dec_count > 0) {
    dec = dec / 10;
    dec_count--;
  }

  return num + dec;
}

// T to String Conversion
char *itoa(int value, char *str, int base) { return ltoa(value, str, base); }

char *ltoa(long value, char *str, int base) { return lltoa(value, str, base); }

char *lltoa(long long value, char *str, int base) {
  int si = 0;

  if (value < 0) {
    str[si++] = '-';
    value = -value;
  }

  char tmp[65] = {0};
  int ti = 0;
  while (value) {
    int rem = value % base;
    tmp[ti++] = hex[rem];
    value = value / base;
  }

  while (ti > 0) {
    str[si++] = tmp[--ti];
  }

  str[si] = '\0';

  return str;
}

char *ftoa(float value, char *str, int precision) {
  return dtoa(value, str, precision);
}

char *dtoa(double value, char *str, int precision) {
  long long int_part = (long long)value;
  double dec_part = value - int_part;

  ltoa(int_part, str, 10);

  int i = strlen(str);
  str[i++] = '.';

  while (precision--) {
    dec_part *= 10;
    int rem = dec_part;
    str[i++] = rem + '0';
    dec_part -= rem;
  }

  str[i] = '\0';

  return str;
}

char *utoa(unsigned value, char *str, int base) {
  return ultoa(value, str, base);
}

char *ultoa(unsigned long value, char *str, int base) {
  return ulltoa(value, str, base);
}

char *ulltoa(unsigned long long value, char *str, int base) {
  int si = 0;

  char tmp[65] = {0};
  int ti = 0;
  while (value) {
    int rem = value % base;
    tmp[ti++] = hex[rem];
    value = value / base;
  }

  while (ti > 0) {
    str[si++] = tmp[--ti];
  }

  str[si] = '\0';

  return str;
}

// Character functions

int isdigit(char c) { return (c >= '0' && c <= '9'); }

int isalpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

int isalnum(char c) { return isdigit(c) || isalpha(c); }

int islower(char c) { return (c >= 'a' && c <= 'z'); }
int isupper(char c) { return (c >= 'A' && c <= 'Z'); }

int isspace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
         c == '\r';
}