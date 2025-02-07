#pragma once

#include <stddef.h>
#include <stdint.h>

// Memory functions

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, uint8_t val, size_t count);
void *memmove(void *dest, void *src, size_t count);

// String functions

size_t strlen(const char *str);

// String Compare
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t n);
int strcasecmp(const char *str1, const char *str2);
int strncasecmp(const char *str1, const char *str2, size_t n);

// String Copy
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);

// String Concatenation
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);

// String Search
char *strchr(const char *str, int c);
char *strrchr(const char *str, int c);
char *strstr(const char *haystack, const char *needle);

// String Tokenize
char *strtok(char *str, const char *delim);

// String Conversion
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);
double atof(const char *str);

// T to String Conversion
char *itoa(int value, char *str, int base);
char *ltoa(long value, char *str, int base);
char *lltoa(long long value, char *str, int base);
char *ftoa(float value, char *str, int precision);
char *dtoa(double value, char *str, int precision);
char *utoa(unsigned value, char *str, int base);
char *ultoa(unsigned long value, char *str, int base);
char *ulltoa(unsigned long long value, char *str, int base);

// Character functions

int isdigit(char c);
int isalpha(char c);
int isalnum(char c);
int islower(char c);
int isupper(char c);
int isspace(char c);

// string formatting
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
