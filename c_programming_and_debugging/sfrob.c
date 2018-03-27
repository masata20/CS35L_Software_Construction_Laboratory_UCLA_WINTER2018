/*
 *
 * Name: Masataka Mori
 */

// report error
// reads the frobnicated textlines from standard input
// write a sorted version to standard output in frobnicated form.
// frobnicated text lines conisit of a seires of non-space bytes followed by a single space
// the spacces represent newlines in the original text.
// all the sorting work itself, by calling frobcmp
// if standard input ends in a partial record that doesn't have a trailing space, your program
// should behave as if a space were appended to the input

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void error_report(const char *message) {
  fprintf(stderr, "Error in %s: %s\n", message, strerror(errno));
  exit(1);
}

char unfrobnicate(const char frob) {
  return frob ^ 42;
}

// return negative, zero, or positive depending on whether
// a is less than, equal to, or greater than b.
int frobcmp(char const *a, char const *b) {
  
  for (;; a++, b++) {
    
    if (*a == ' ' && *b == ' ')
      return 0;
    if (*a == ' ')
      return -1;
    if (*b == ' ')
      return 1;
    if ((unfrobnicate(*a) == (unfrobnicate(*b))))
      continue;
    return ((unfrobnicate(*a) > (unfrobnicate(*b))) ? 1 : -1);
  }
  
}

int wrapfrobcmp(void const *a, void const *b) {
  return frobcmp(*(const char**)a, *(const char**)b);
}

int main (void) {

  char *one_line;
  char **multiple_lines;
  char current_char;
  char previous_char;
  int one_line_size = 0, multiple_lines_size = 0;

  current_char = getchar();
  // if the input is empty
  if (feof(stdin)) {
    //printf("file is empty");
    //return 0;
  }
  if (ferror(stdin))
    error_report("getchar()");

  one_line = (char*)malloc(sizeof(char));
  if (one_line == NULL) 
    error_report("malloc");
  multiple_lines = (char**)malloc(sizeof(char*));
  if (multiple_lines == NULL)
    error_report("malloc");
  
  
  while (current_char != EOF) {

    // ignore the leading space
    // consider the consecutive spaces as just one
    if ((one_line_size != 0) && (current_char == ' ')) {
      one_line[one_line_size] = current_char;
      multiple_lines[multiple_lines_size] = one_line;
      multiple_lines_size++;
      //multiple_lines = (char**)realloc(multiple_lines, (multiple_lines_size+2) * 500);
       multiple_lines = (char**)realloc(multiple_lines, (multiple_lines_size+2) * sizeof(char*));
      if (multiple_lines == NULL)
		error_report("realloc");
      one_line_size = 0;
      one_line = NULL; // otherwise it will points to the same address
      one_line = (char*)malloc(sizeof(char));
    } else {
      if (current_char != ' ') {
		one_line[one_line_size] = current_char;
		one_line_size++;
		one_line = (char*)realloc(one_line, (one_line_size+1)*sizeof(char));
		if (one_line == NULL)
		  error_report("realloc");
      }
    }
    previous_char = current_char;
    current_char = getchar();
    if (ferror(stdin))
      error_report("getchar()");
    // if there is no space in the end, we think as there is space
    // and store the one_line to multiple_lines
    if (current_char == EOF && previous_char != ' ') {
      one_line[one_line_size] = ' ';
      multiple_lines[multiple_lines_size] = one_line;
      multiple_lines_size++;
    }

  }
  
  qsort(multiple_lines, multiple_lines_size, sizeof(char*), wrapfrobcmp);
  
  for (int i = 0; i < multiple_lines_size; i++) {
    for (int j = 0;; j++) {
      putchar(multiple_lines[i][j]);
      if (ferror(stdout))
		error_report("putchar()");
      if (multiple_lines[i][j] == ' ')
		break;
    }
  }

  for (int i = 0; i < multiple_lines_size; i++)
    free(multiple_lines[i]);
  
  free(multiple_lines);
  return 0;
}
