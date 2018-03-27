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
 */

#include <stdio.h>
#include <limits.h> // for INT_MAX and INT_MIN

int saturating_add(int x, int y) {

	int sum = x + y;
	printf("sum is %d\n", sum);

	int w = sizeof(int) << 3; // Compute how many bits long for integer
	printf("w is %d\n", w);

	// if the sum was positively overflowed ( > 2^(w-1)),
	// the machine always yields the result to the range -2^w-1 to -2 (not 0)
	// (subtracting 2^w from the result)
	// , which is minus even when x > 0 and y > 0
	// Therefore, the most significant bit of the result will be 1, 
	// and rest of bits are depending on x and y
	// The condition to be positive overflow is 
	// x > 0 , y > 0 , sum < 0. In other words
	// the most significant bit of each must be
	//   0   ,   0   ,   1
	
	// To determine whether the value is negative, we use right shift with w - 1,
	// if the value was positve, it should be 0, which is 0x00000000
	// which correspond to false
	// if the value was negative, it should be -1, which 0xffffffff		
	// which correspond to true
	int positive_flow = !(x >> (w - 1)) && !(y >> (w - 1)) && (sum >> (w - 1));

	// if the sum was negatively overflowed (< -2^w-1)
	// the machine always yields the result to the range 0 to 2^(w-1)
	// which is positve, even when x < 0 and y < 0
	// The most siginificant bit of the result will be 0
	// The condidtion to be negative overflow is
	// x < 0  , y < 0  , sum > 0
	// The most siginificant bit of each must be
	//   1    ,   1    ,   0
	// with simalar argument as above
	int negative_flow = (x >> (w - 1)) && (y >> (w - 1)) && !(sum >> (w - 1));


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
