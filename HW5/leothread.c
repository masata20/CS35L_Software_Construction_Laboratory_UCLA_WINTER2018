/*
 * Assignment:
 * 		Submit a file leothread.c that is like hello.c (Fig 12.13),
 * 		except that it creates and reaps L(N) - 1 joinable threads,
 * 		where N is a positive command-line argument and 
 * 		L(N) is the Nth Leonardo number.
 * 		These threads should be created as a Fibonacci tree of order N
 * 		rooted in the main thread, where thread corresponding to a node
 * 		creates and reaps the threads corresponding to the node's
 * 		children.
 * 		The Fibonacci trees of order 0 and 1 have just a root node,
 * 		and a Fibonacci tree of order N > 1 has left and right
 * 		Fibonacci subtrees of order N-1 and N-2, respectively.
 *
 * 		Nodes in the trees of threads should be labeled 0 through
 * 		L(N)-1 in preorder: that is, each thread should have a label that
 * 		is a unique integer in the range 0 ... L(N) - 1, where the main
 * 		thread's label is 0 and each non-leaf thread labeled T and with
 * 		L(k) - 1 descendants (for some k) has two children labeled
 * 		T + 1 and T+1+L(k-1). Each leaf thread should siply print "[T]\n"
 * 		where T is the thread's label; each non -leaf thread with
 * 		label T should first print "(T\n", then create and reap its
 * 		two children, and finaly print "T)\ñ" just before exiting.
 * 		The main thread counts as a thread and so should print too.
 * 		The main thread is a leaf if and only if N=1.
 *
 * 		Also, submit a file leothread.tr created by
 * 		'strace -o leothread.tr -f ./leothread4' on a SEASnet
 * 		GNU/Linux mahcine, where ./leaothread is the executable produced
 * 		by compiling leothread.c.
 * 		This example should create 8 joinable threads, since
 * 		L(4)-1 = 8, which means that it runs 9 threads total
 * 		(counting the mian thread)
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h> // for exit(0)

typedef struct {
	unsigned int label; // own label
	unsigned int order; // own order
} Node_Info;

void *thread(void *vargp);

// Calculate the lable of childeren with 
// given current label and parent_order
unsigned int right_child_label(Node_Info parent);

// return true if order <= 1
// return false if order > 1
bool leaf_checker(unsigned int order);

// return a leonard number with given order
unsigned int leonard(unsigned int order);

// Here I use unsigned integer for order and labels.
// Since we assume the command line argument will pass
// the postive numbers.
// Also, we assume the number of argument passed will be only one
int main(int argc, char *argv[]) {

	// check argument to make sure it has argument
	// if they don't just print and shut the program
	if (argc == 1) {
		printf("Please add the argument to run the program\n");
		exit(0);
	}
	// order of trees
	// since we are assuming the passed argument is only one
	// we use argv[1] to get the order.
	// argv[0] is for the name of program.
	// it is also good for the case when users put too many args
	// we just take the first one
	// Also, since it is passed as string we need to convert to
	// integer and to unsigned, I'll use atoi
	int argument = atoi(argv[1]);
	unsigned int given_order = (unsigned int)argument;
	//printf("raw argument is %d\n", argument);
	//printf("given_order is %u\n", given_order);
	//unsigned int given_order = 4;
	unsigned int bound = 2;
	// For the case N = 0, 1
	// The thread is only main thread
	if (given_order < bound) { 
		// print label
		// label = 0 for both case
		// since only one node
		printf("[0]\n");
		return 0;
	}

	// init the Node_Info
	Node_Info root;	
	root.label = 0;
	root.order = given_order;
	// init the node for children
	// with will pass to the children thread.
	Node_Info left_node;
	Node_Info right_node;

	left_node.label = root.label + 1;
	left_node.order = root.order - 1;
	right_node.label = right_child_label(root);
	right_node.order = root.order - 2;

//	printf("left label is %u\n", left_node.label);
//	printf("left order is %u\n", left_node.order);
//	printf("right label is %u\n", right_node.label);
//	printf("right order is %u\n", right_node.order);

	printf("(%u\n", root.label);

	pthread_t tid;
	pthread_create(&tid, NULL, thread, &left_node);
	pthread_join(tid, NULL);
	pthread_create(&tid, NULL, thread, &right_node);
	pthread_join(tid, NULL);

	printf("%u)\n", root.label);

//	pthread_t tid;
//	pthread_create(&tid, NULL, thread, &root);
//	pthread_join(tid, NULL);
	exit(0);
//	return 0;
}

// its own node information will be passed
// from parent threads
void *thread(void *vargp)
{
	Node_Info node = *((Node_Info*)vargp);
	bool leaf = leaf_checker(node.order);
		
	// if it is leaf, that node won't create threads.
	if (leaf) {
		printf("[%u]\n", node.label);
		return NULL;
	}

	// print its own label, first half
	printf("(%u\n", node.label);
	// init the node for children
	// with will pass to the children thread.
	Node_Info left_node;
	Node_Info right_node;

	left_node.label = node.label + 1;
	left_node.order = node.order - 1;
	right_node.label = right_child_label(node);
	right_node.order = node.order - 2;

//	printf("left label is %u\n", left_node.label);
//	printf("left order is %u\n", left_node.order);
//	printf("right label is %u\n", right_node.label);
//	printf("right order is %u\n", right_node.order);


	// create and reap its children
	pthread_t tid;
	pthread_create(&tid, NULL, thread, &left_node);
	pthread_join(tid, NULL);
	pthread_create(&tid, NULL, thread, &right_node);
	pthread_join(tid, NULL);

	// print before exit
	// how?????
	printf("%u)\n", node.label);
	return NULL;// same as pthread_exit(NULL):
}

// Calculate the lable of childeren with 
// given current label and parent_order
unsigned int right_child_label(Node_Info parent) {
	
	// Formula
	// new_label = T+1+L(order-1);
	unsigned int new_label;
	
	new_label = parent.label + 1 + leonard(parent.order-1); 


	return new_label;
}

// return true if order <= 1
// return false if order > 1
bool leaf_checker(unsigned int order) {
	
	if (order <= 1)
		return true;
	return 0;
}


// return a leonard number with given order
unsigned int leonard(unsigned int order) {
	
	// possible order is from 1 in this function
	// since only from order 2 can call this function
	if (order == 1)
		return 1;

	unsigned int leo_number = 0;	
	unsigned int n_1_order = 1; // N-1 order, also L(1)
	unsigned int n_2_order = 1; // N-2 order,also L(0)
	int i;

	for (i = 1; i < order; i++) {
		leo_number = (n_2_order) + (n_1_order) + 1;
		n_2_order = n_1_order;
		n_1_order = leo_number;
	}


	//printf("leo_number is %u\n", leo_number);

	return leo_number;
}

