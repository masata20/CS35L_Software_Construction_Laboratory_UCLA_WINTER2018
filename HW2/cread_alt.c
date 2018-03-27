/*
 * Masataka Mori
 *
 * Problem:3.61
 * In section 3.6.6, we examined the following code as a candidate
 * for the use of conditional data transfer
 *
 * long cread(long *xp) {
 * 		return (xp ? *xp : 0);
 * }
 * invalid implementation of function cread
 * 		movl $0, %eax
 * 		testl %edx, %edx
 * 		cmovne  (%edx), %eax
 *
 * We showed a trail implementation using a condictional move instruction
 * but argued that it was not valid, since it could atempt to read from 
 * a null address.
 * 		Write a C function cread_alt that has the same behavior as cread,
 * 		except that it can be compiled to use conditional data transfer.
 * 		When compiled, the genrated code should use a conditional move
 * 		instruction rather than one of the jump instruction.
 */


#include <stdio.h>

long cread_alt(long *xp) {
	
	// The problem of above code is at line
	// cmone (%edx), %eax
	// it will be executed even if %edx is pointed to null,
	// and it causes the null deference
	// to resolve this problem we need to make sure
	// we don't deference the pointer which may have null
	// so what we want is something like this
	// movl $0, %eax     // eax has zero initially
	// cmovne %edx, %eax // if xp is not null or zero, store the poiter address(not value) in %eax
	// if above succeed, i need to return the value of the address point to
	// so need to deference
	// but if it failed, %eax still has value zero, and cannot deference it
	// so we can make it as pointer to 0, so we want first line to be
	// movl addressof($0), %eax
	// testl %edx, %edx
	// cmovne %edx, %eax
	// deference %eax and return
	// movl (%eax), %eax
	// ret
	
	// use -O to compile
	long *result;
	long var = 0x00;
	result = &var;
	return *(xp ? xp : result);


	/* with -O
	 * cread_alt:
	 * .LFB11:
	 * .cfi_startproc
	 * 		movq	$0, -8(%rsp)
	 * 		leaq	-8(%rsp), %rax
	 * 		testq	%rdi, %rdi
	 * 		cmove	%rax, %rdi // not deferencing pointer! so safe
	 * 		movq	(%rdi), %rax
	 * 		ret
	 * .cfi_endproc
	 */

	/* with -O2
	 * cread_alt:
	 * .LFB11:
	 * 		.cfi_startproc
	 * 		xorl	%eax, %eax
	 * 		testq	%rdi, %rdi
	 * 		je	.L1      // using jump inst, -O2 is not good for this code
	 * 		movq	(%rdi), %rax
	 * .L1:
	 * 		rep ret
	 * 		.cfi_endproc
	 */


}


int main(void) {
	long *xp;
	long result;


	xp = NULL;
	printf(" xp is null %08lx  \n", xp); // since null *xp give me error
	result = cread_alt(xp);
	printf("result is %08lx\n", result);

	long var = 0x00;
	xp = &var;
	printf(" *xp is %08lx  \n :", *xp);
	result = cread_alt(xp);
	printf("result is %08lx\n", result);

	var = 0x01;
	xp = &var;
	printf(" *xp is %08lx   \n:", *xp);
	result = cread_alt(xp);
	printf("result is %08lx\n", result);

	var = 0x80000000;
	xp = &var;
	printf(" *xp is %08lx   \n:", *xp);
	result = cread_alt(xp);
	printf("result is %08lx\n", result);

	return 0;

}
