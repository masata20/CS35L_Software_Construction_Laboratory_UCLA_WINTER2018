/*
 * Write code for a function with the following prototype:
 * Addtion that saturate to TMin or TMax
 * int saturating_add(int x, int y);
 *
 * 	Instead of overflowing the way normal two's-complement addition does,
 * 	saturating addition returns TMaxs when there would be positive overflow,
 * 	and TMin when there would be negative overflow. Saturating arithmetic is
 * 	commonly used in programs that perform digital signal processing.
 * 	
 * 	Your function should follow the bit-level intger coding rules
 *
 *
 * 	this time using a call to the __builtin_add_overflow_p function available
 * 	in GCC 7 and later; the third argument of the call should be a cast that
 * 	consists of a parenthesized type followed by the constant 0.
 * 	In other respects your function should continue to follow
 * 	the bit-level integer coding rules.
 */

#include <stdio.h>
#include <limits.h> // for INT_MAX and INT_MIN

int saturating_add(int x, int y) {

	int sum = x + y;
	printf("sum is %d\n", sum);

	int w = sizeof(int) << 3; // Compute how many bits long for integer
	printf("w is %d\n", w);

	// the builtin function written below returns 0 when there is positive
	// or nagative flow in sum.
	// Therefore, the condition to be positive overflow is
	// bulitin funct == true , x > 0 , y > 0
		
	// To determine whether the value is negative, we use right shift with w - 1,
	// if the value was positve, it should be 0, which is 0x00000000
	// which correspontd to false
	// if the value was negative, it should be -1, which 0xffffffff		
	// which corresepond to true 
	int positive_flow = (__builtin_add_overflow_p(x, y, (int)0)) 
						&& !(x >> (w - 1)) && !(y >> (w - 1));

	// with similar argument above
	// The condidtion to be negative overflow is
	// bultin func == true, x < 0, y < 0	
	int negative_flow = (__builtin_add_overflow_p(x, y, (int)0)) 
						&& (x >> (w - 1)) && (y >> (w - 1));


	printf("the right shifted x is %d\n", x >> (w - 1));
	printf("the right shifted y is %d\n ", y >> (w - 1));
	printf("the right shifted s is %d\n", sum >> (w - 1));
	printf("positive_flow is %d\n", positive_flow);
	printf("negative_flow is %d\n", negative_flow);

	((positive_flow) && (sum = INT_MAX)) || ((negative_flow) && (sum = INT_MIN)); 
	



	return sum;
}

int main(void) {

	int sum1 = saturating_add(0x7fffffff, 0x7fffffff); // should be positive overflow  	
	printf("sum1 is %d\n\n", sum1);

	int sum2 = saturating_add(0x80000000, 0x80000000); // should be negative  overflow  	
	printf("sum2 is %d\n\n", sum2);

	int sum3 = saturating_add(INT_MIN, INT_MIN); // should be same as above  	
	printf("sum3 is %d\n\n", sum3);

	int sum4 = saturating_add(0x00000001, 0x00000001); // should be same just 2. No overflow  	
	printf("sum4 is %d\n\n", sum4);


	return 0;

}
