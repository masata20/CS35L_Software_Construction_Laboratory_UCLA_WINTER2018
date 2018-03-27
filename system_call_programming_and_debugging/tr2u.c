// 
// Name: Masataka Mori

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// check if the text has duplicates in it or not
// return 0, if they have dupolicate
// return 1, if not
int duplicate_test(const char *text, int size) {

       	
	for (int i = 0; i < size; i++) {
		char curr_char = text[i];
		int j = i + 1;
		for (j; j < size; j++) {
			if (curr_char == text[j])
				return 0;
		}
	}
	return 1;      
}

// in the mapper,
// at even indices, it contains the from characters
// at odd indices, it contains the to characters
void mapping(const int size, const char *from, const char *to, char *mapper) {

	for (int i = 0; i < size; i++) {
		mapper[2*i] = from[i];
		mapper[2*i+1] = to[i];
	}

}

// from_size is passed
char transliterator(const char target, const int size, const char *mapper) {

	for (int i = 0; i < size; i++) {
		// compare with even indices
		// which contain 'from'
		if (target == mapper[i*2])
			return mapper[i*2+1]; // return corresponding, so odd indices
	}

	// unmathed with anything
	return target;
}
	

// through error to stderr
// and exit
void error_report(const char *message) {
	fprintf(stderr, "Error, %s: %s\n", message, strerror(errno));
	exit(1);
}


int main(int argc, const char *argv[]) {

	// if text argument wasn't two, exit
	if (argc != 3) {
		fprintf(stderr, "Error: There has to be two arguments\n");
		exit(1);
	}
	int from_size = strlen(argv[1]);
	int to_size = strlen(argv[2]);

	// avoid '' charcter
	if (! from_size) {
		fprintf(stderr, "Error: No first operand\n");
		exit(1);
	}

	// they must have the same size
	if (from_size != to_size) {
		fprintf(stderr, "Error: Two argument must have the same length\n");
		exit(1);
	}
	// 'From' cannot contain duplicates
	if (!duplicate_test(argv[1], from_size)) {
		fprintf(stderr, "Error: First argument 'From' cannot contain duplicates\n");
		exit(1);
	}
	char *mapper;
	//int mapper_size = 2*from_size;
	// allocate, it will contian the twice size as arument string
	mapper = (char*)malloc(2*from_size);
	if (mapper == NULL)
		error_report("Fail to malloc");

	mapping(from_size, argv[1], argv[2], mapper);

	// Read the stdin and transliterate
	char current_char[1];
	int ret_value = 0;
	
	char transliterated_char[1];

	ret_value = read(STDIN_FILENO, current_char, 1);
	if (!ret_value) {
		fprintf(stderr, "Error: file is empty\n");
		exit(1);
	}
	if (ret_value == -1)
		error_report("Fail to read()");

	while (ret_value != 0) {
		
		transliterated_char[0] = transliterator(current_char[0], from_size, mapper);
		ret_value = write(STDOUT_FILENO, transliterated_char, 1);
		if (ret_value == -1)
			error_report("Fail to write()");

		// get next charcter
		ret_value = read(STDIN_FILENO, current_char, 1);
		if (ret_value == -1)
			error_report("Fail to read()");
	}

	free(mapper);

	return 0;
}
