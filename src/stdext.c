#include "stdext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HAVE_STRNLEN
size_t strnlen(const char *str, size_t n) {
    const char * stop = (char *)memchr(str, '\0', n);
    return stop ? stop - str : n;
}
#endif

void strrev(char *arr, int start, int end) {
    char temp;

    if (start >= end)
        return;

    temp = *(arr + start);
    *(arr + start) = *(arr + end);
    *(arr + end) = temp;

    start++;
    end--;
    strrev(arr, start, end);
}

char *itoa(int number, char *arr, int base) {
    int i = 0, r, negative = 0;

    if (number == 0) {
        arr[i] = '0';
        arr[i + 1] = '\0';
        return arr;
    }

    if (number < 0 && base == 10) {
        number *= -1;
        negative = 1;
    }

    while (number != 0) {
        r = number % base;
        arr[i] = (r > 9) ? (r - 10) + 'a' : r + '0';
        i++;
        number /= base;
    }

    if (negative) {
        arr[i] = '-';
        i++;
    }

    strrev(arr, 0, i - 1);

    arr[i] = '\0';

    return arr;
}

const char *pidname(int pid) {
	char *name = calloc(1024, sizeof(char));
	if(!name)
		return "unknown";
	sprintf(name, "/proc/%d/cmdline",pid);
	FILE *file = fopen(name, "r");
	if (!file)
		return "unknown";
	size_t size;
        size = fread(name, sizeof(char), 1024, file);
	if(size>0) {
		if(name[size-1] == '\n')
			name[size-1] = '\0';
	}
	fclose(file);
	return name;
}

