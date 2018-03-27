/*
 * Write a function int_shits_are_arithmetic() that yields 1 when run on a
 * machine that uses arithmetic right shifts for data type int and yields 0 otherwise. Your code should work on a machine with any word size. Test your code on several machine.
 *
 * -> Determine that your machine use wheather arithmetic right shits for data type int, or
 *  logical arithmetic right shifts for data type int.
 *  if the machine uses arithmetic right shift return 1 from the int_shifts_are_arithmetic()
 *  if the machine uses logical right shift return 0 from the int_shifts_are_arithmetic()
 *
 */

#include <stdio.h>

int int_shifts_are_arithmetic(void) {

	int x = ~0x00; // x = 1111 1111 1111 1111 1111 1111 1111 1111 = -1

	// if it is arithmetic right shift, it is still -1 -> return true
    // if not, x become 0111 1111 ......, and not same as -1 -> return false
	return (x >> 1) == -1; 
}

int main(void) 
{
	// change it later, don't use conditional statement
	if (int_shifts_are_arithmetic())
	{
		printf("this machine uses arthmetic right shift\n");
	}
	else
	{
		printf("this machine uses logical right shift\n");
	}
	return 0;
}
