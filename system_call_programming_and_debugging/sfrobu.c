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
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
// GLOBAL VARIABLE F-FLAG
int F_FLAG;


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

// frobcmp for -f flag case.
int frobcmp_f(char const *a, char const *b){
  
  for (;; a++, b++) {
    
    if (*a == ' ' && *b == ' ')
      return 0;
    if (*a == ' ')
      return -1;
    if (*b == ' ')
      return 1;
    if ((toupper(unfrobnicate(*a)) == toupper((unfrobnicate(*b)))))
      continue;
    return ((toupper(unfrobnicate(*a)) > toupper((unfrobnicate(*b)))) ? 1 : -1);
  }
  
}
int wrapfrobcmp(void const *a, void const *b) {
	if (F_FLAG)
		return frobcmp_f(*(const char**)a, *(const char**)b);
	return frobcmp(*(const char**)a, *(const char**)b);
}

int main (int argc, char *argv[]) {
	// flag check
	F_FLAG = 0;
	if (argc == 2)
		if (!(strcmp(argv[1],"-f")))
			F_FLAG = 1;

  char *one_line;
  char **multiple_lines;
  //char current_char;
  
  char previous_char;
  int one_line_size = 0, multiple_lines_size = 0;

  char current_char[1];
  int ret_value = 0;
  ret_value = read(STDIN_FILENO, current_char, 1);
  // if the input is empty
  if (!ret_value) {
    fprintf(stderr, "Error: file is empty\n");
    exit(1);
  }
  if (ret_value == -1)
	  error_report("read()");

  // check the file size and malloc with it
  int file_size = 0;
  int fstat_ret = 0;
  struct stat buf;
  fstat_ret = fstat(STDIN_FILENO, &buf);
  if (fstat_ret == -1)
	  error_report("fstat()");
  file_size = buf.st_size;
  
  one_line = (char*)malloc(sizeof(char)*file_size);
  if (one_line == NULL) 
    error_report("malloc");
  multiple_lines = (char**)malloc(sizeof(char*));
  if (multiple_lines == NULL)
    error_report("malloc");
  
  
  while (ret_value != 0) {

    // ignore the leading space
    // consider the consecutive spaces as just one
    if ((one_line_size != 0) && (current_char[0] == ' ')) {
      one_line[one_line_size] = current_char[0];
      multiple_lines[multiple_lines_size] = one_line;
      multiple_lines_size++;
      multiple_lines = (char**)realloc(multiple_lines, (multiple_lines_size+2) * sizeof(char*));  
      if (multiple_lines == NULL)
	error_report("realloc");
      one_line_size = 0;
      one_line = NULL; // otherwise it will points to the same address
      one_line = (char*)malloc(sizeof(char)*file_size);
    } else {
      if (current_char[0] != ' ') {
	one_line[one_line_size] = current_char[0];
	one_line_size++;
	// if it grows more the inital file_size reallocate
	if (one_line_size >= file_size)
		one_line = (char*)realloc(one_line, (one_line_size+1)*sizeof(char));
	if (one_line == NULL)
	  error_report("realloc");
      }
    }
    previous_char = current_char[0];
    //current_char = getchar();
    ret_value = read(STDIN_FILENO, current_char, 1);
    if (ret_value == -1)
      error_report("read()");
    // if there is no space in the end, we think as there is space
    // and store the one_line to multiple_lines
    if (ret_value ==
	0 && previous_char != ' ') {
      one_line[one_line_size] = ' ';
      multiple_lines[multiple_lines_size] = one_line;
      multiple_lines_size++;
    }

  }
  
  
  qsort(multiple_lines, multiple_lines_size, sizeof(char*), wrapfrobcmp);
  char write_char[1];
  for (int i = 0; i < multiple_lines_size; i++) {
    for (int j = 0;; j++) {
	    //putchar(multiple_lines[i][j]);
	    write_char[0] = multiple_lines[i][j];
	    ret_value = write(STDOUT_FILENO, write_char, 1);
	    if (ret_value == -1)
		    error_report("write()");
	    if (multiple_lines[i][j] == ' ')
		    break;
    }
  }

  for (int i = 0; i < multiple_lines_size; i++)
    free(multiple_lines[i]);
  
  free(multiple_lines);
  return 0;
}
