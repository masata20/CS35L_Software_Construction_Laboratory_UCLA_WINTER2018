/*
 * Name: Masataka Mori
 * 
 *
 * You have been assigned the task of writing a C function to compute a
 * floating point representation of 2^x. You decide that the best way to
 * do this is to direclty construct the IEEE single-precesion representation
 * of the result. When x is too small, your rountine will return 0.0.
 * When x is too large, it will return +inf. Fill in the blank portions of the
 * code that fllows to computer the correct result. Assume the fucnction
 * u2f returns a floating-point value having an identical bit representation as
 * its unsigned argument.
 *
 * 		float fpwr2 (int x) {
 * 			// Result exponent and fraction 
 * 			unsigned exp, frac;
 * 			unsigned u;
 *
 * 			if (x < ______) {
 * 				// Too small. Return 0.0
 * 				exp = ______;
 * 				frac = ______;
 * 			} else if (x < _______) {
 * 				// De normalized result
 * 				exp = _______;
 * 				frac = ______;
 * 			} else if (x < ______) {
 * 				// Normalized result
 * 				exp = ______;
 * 				frac = ______;
 * 			} else {
 * 				// Too big. Return +INF
 * 				exp = ______;
 * 				frac = ______;
 * 			}
 *
 * 				//Pack exp and grac into 32 bits
 *			u = exp << 23 | frac; 
 *				//Return as float
 *			return u2f(u);
 *			
 *	Compute 8^x rather than 2^x and call resulting function fpwr8 instead of
 *	fpwr2.
 *	Contain all the source code for fpwr8, preceded by the definition of a function
 *	static float u2f(unsigned u) { ..... } that returns value as desribed in the above.
 *
 */

#include <stdio.h>



// returns a floating-point value having an identical bit representation as
// its unsigned argument.
static float u2f(unsigned u) {

	union {
		float f;
		unsigned u;
	} convt;

	convt.u = u;

	return convt.f;
}
/*
float fpwr2 (int x) {
	// Result exponent and fration
	unsigned exp, frac;
	unsigned u;

	// smallest denorm is exp = 0s, and frac = 0....1
	// which is 2^(1-127) * 2^-23 = 2^-126 * 2^-23 = 2^-149
	// so too small value would be smaller than -149
	if (x < -149) {
		// Too small return 0.0
		exp = 0;
		frac = 0;
	} else if (x < -126) {
		// Denormalized result
		// the largest denorm value is exp = 0s, and frac = not 0s or 1
		// exp = 0, so E = 1 - 127 = -126
		exp = 0;
		// to have fraction in bits, we need to remember how to convert int to float.
		// when we have 2^x, which means we have 1 at the position of x shifted by left or right
		// for example, if x = -130, so we want 2^-130 in the end.
		// but we have 2^-126 since it is denormalized, so 
		// 2^-126 *2^-4 = 2^-130
		// so we need to have 2^-4 for fraction part, which has 1 from the binary point to the right
		// shifted by 4, which is 
		// _ ________ 00010000000000000000000
		// means 1 << 23 - 4
		// if I generalize this amount of shift, which is
		// 23 + (x + 126) = x + 149
		// so
		frac = 1 << (x + 149) ;
	} else if (x < 128) {
		// Normalized result
		// the largest value the nomralized value can represent is
		// exp = 11111110, f = 0....0
		// e cannot be 11111111, which is 255
		// E = e - bias = 255 - 127 = 128 = x, so the above condition will be x < 128
		// we want to find 2^x, so 
		// x = E = exp - bias
		// exp = x + bias = x + 127
		exp = x + 127;
		// since we only want 2^x, we don't care about fraction part
		// this time
		frac = 0;
	} else {
		// Too big. Return +INF
		// to get +INF, all exponential will be 1, which is 11111111
		// which is 255
		// and all fraction is 0
		exp = 255;
		frac = 0;
	}


	// pack exp and grac into 32 bits
	u = exp << 23 | frac;
	// return as float
	return u2f(u);
}
*/


// the only diffeence between fpwr8 and fpwr2 is that fpwr 8 has x which is multiply by 3
// in the fpwr8 because 8^x = (2^3)^x = 2^(3x), so we put x = 3x in fpwr2 case
// for detail logic see above fpwr2 function
float fpwr8 (int x) {
	
	// Result exponent and fration
	unsigned exp, frac;
	unsigned u;

	// -149/3 = - 49.6667
	// if it is -50, it would be over the minimum
	// so round toward 0
	if (x < -49) {
		// Too small return 0.0
		exp = 0;
		frac = 0;
	} else if (x < -42) {
		// -126/3 = -42
		// Denormalized result
		exp = 0;
		// to have fraction in bits, we need to remember how to convert int to float.
		// when we have 2^3*x, which means we have 1 at the position of 3*x shifted by left or right
		// for example, if x = -45, so we want 2^-135 in the end.
		// but we have 2^-126 = 8^-42 since it is denormalized, so 
		// 2^-126 *2^-9 = 2^-135, or
		// 8^-42 * 8^-3 = 8^-45
		// so we need to have 2^-9 for fraction part, which has 1 from the binary point to the right
		// shifted by 9, which is 
		// _ ________ 00000000100000000000000
		// means 1 << 23 - 9
		// if I generalize this amount of shift, which is
		// 23 + (3*x + 126) = x + 149
		// so

		frac = 1 << (3*x + 149) ;
	} else if (x < 42) {
		// 128/3 = 42.667
		// round to zero, so 42
		// Normalized resul
		exp = 3*x + 127;
		frac = 0;
	} else {
		// Too big. Return +INF	
		exp = 255;
		frac = 0;
	}


	// pack exp and grac into 32 bits
	u = exp << 23 | frac;
	// return as float
	return u2f(u);

}



int main (void) {
	
	int x;
	// 0 case
	printf("x is -50\n");
	x = -50;
	float f = fpwr8(x);
	printf("value is %.100f\n", f);

	// denormalized
	printf("x is -45\n");
	x = -45;
	f = fpwr8(x);
	printf("value is %.100f\n", f);
	printf("\n");

	printf("x is -49\n");
	x = -49;
	f = fpwr8(x);
	printf("value is %.100f\n", f);
	printf("\n");


	// normalized
	printf("x is -10\n");
	x = -10;
	f = fpwr8(x);
	printf("value is %.100f\n", f);
	printf("\n");
	
	printf("x is 10\n");
	x = 10;
	f = fpwr8(x);
	printf("value is %.100f\n", f);
	printf("\n");
	
	printf("x is 41\n");
	x = 41;
	f = fpwr8(x);
	printf("value is %.100f\n", f);
	printf("\n");

	// infinity
	printf("x is 42\n");
	x = 42;
	f = fpwr8(x);
	printf("value is %.100f\n", f);
	printf("\n");

/*
	// 0 case
	printf("x is -150\n");
	x = -150;
	float f = fpwr2(x);
	printf("value is %.100f\n", f);

	// denormalized
	printf("x is -130\n");
	x = -130;
	f = fpwr2(x);
	printf("value is %.100f\n", f);
	printf("\n");

	// normalized
	printf("x is -10\n");
	x = -10;
	f = fpwr2(x);
	printf("value is %.100f\n", f);
	printf("\n");
	
	printf("x is 10\n");
	x = 10;
	f = fpwr2(x);
	printf("value is %.100f\n", f);
	printf("\n");


	printf("x is 127\n");
	x = 127;
	f = fpwr2(x);
	printf("value is %.100f\n", f);
	printf("\n");

	// infinity
	printf("x is 130\n");
	x = 130;
	f = fpwr2(x);
	printf("value is %.100f\n", f);
	printf("\n");
*/
	return 0;
}










