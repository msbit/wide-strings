#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void printAlternate(const char *, int, int);
void findWideStrings(const char *, int, int);

int main(int argc, char **argv) {
  while (*(++argv) != NULL) {
    printf("%s:\n", *argv);
    FILE *f = fopen(*argv, "r");
    if (f == NULL) {
      perror("fopen");
      continue;
    }

    if (fseek(f, 0, SEEK_END) == -1) {
      perror("fseek");
      goto cleanup_close;
    }

    long size = ftell(f);

    if (fseek(f, 0, SEEK_SET) == -1) {
      perror("fseek");
      goto cleanup_close;
    }

    char *buffer = malloc(size * sizeof(char));
    if (buffer == NULL) {
      perror("malloc");
      goto cleanup_close;
    }

    if (fread(buffer, 1, size, f) != size) {
      perror("fread");
      goto cleanup_free;
    }

    findWideStrings(buffer, size, 4);

    printf("\n");

  cleanup_free:
    free(buffer);

  cleanup_close:
    fclose(f);
  }
}

void printAlternate(const char *buffer, int from, int to) {
  for (; from < to; from += 2) {
    printf("%c", buffer[from]);
  }
  printf("\n");
}

void findWideStrings(const char *buffer, int size, int threshold) {
  int head = 0;
  int tail = 0;

  while (head < size && tail < size) {
    while (head < size && !isprint(buffer[head])) {
      head++;
    }

    if (head >= size) {
      break;
    }

    tail = head;
    while (tail < (size - 1)) {
      if (!isprint(buffer[tail])) {
        break;
      }

      if (buffer[tail + 1] != '\0') {
        break;
      }

      tail += 2;
    }

    if ((tail - head) > (threshold * 2)) {
      printAlternate(buffer, head, tail);
    }

    head = tail + 1;
  }
}
