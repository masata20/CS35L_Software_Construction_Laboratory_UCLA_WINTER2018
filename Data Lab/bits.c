/* 
 * CS:APP Data Lab 
 * 
 * <Masataka Mori>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
	// x is 0 only when all bits are 0s
	// by property of two's complement integer
	// all integer have its additive inverse like x -x = 0
	// and if we operate x | (-x), we will usually get the sign bit as 1
	// since -x is negative number.(even for expecption when x = INT_MIN = -x,
	// it still has negative sign, so we will still get the sign bit as 1 after
	// operation)
	// However, when x = 0, it will give the sign bit as 0 since 0 doesn't have
	// nagative of itself	
	x = x | (~x+1); // -x = ~x+1

	// now if we operate >> 31 on x, it will give 0 when x = 0
	// and it will give -1 when x != 0(since arithmetic operation)
	x >>= 31;
	
	//if we add 1 on to x, we will get:
	//1 when x = 0
	//0 when x = -1
  return x + 1;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
		
	// I use Divide and Conquer algorithm
	int sum;

	// First, by using the bit mask, I divide the bit
	// into even indecies and odd indecies.
	// For even, we use 0101, which is 0x55555555
	// since we cannot use big constant
	// I made by shfiting and add 0x55
	int mask = 0;
	mask = 0x55 + (0x55 << 8);
	mask = mask + (mask << 16);
	// For odd part, we want to use 1010, which is 0xcccccccc
	// but to save operation cost, we right shift the x and
	// use the same mask to get odd part
	// since the mask is 0101, it doen't matter whether
	// arithmetic or logical shift
	sum = (x & mask) + ((x >> 1) & mask);

	// Now in the sum, each 2 bits contains the number of 1s in
	// 2 bits in orignal x
	// by again dividng the sum into even indices and odd indecies
	// but with 2bits as one index this time,then we can get the 
	// number of 1, which containted in 4bits for original bits, in each 4bits
	// For even part, we use the mask 0011, which is 0x33333333
	mask = 0x33 + (0x33 << 8);
	mask = mask + (mask << 16);
	// to calculate odd part we shift sum by 2(since 00 11)
	// and use the same mask
	sum = (sum & mask) + ((sum >> 2) & mask);

	// Again, we divide even and odd indecis with 
	// 4 bits as one index ,then we get the number of 1s in each 8 bits
	// For even part, we use 00001111, which is 0x0f0f0f0f
	mask = 0x0f + (0x0f << 8) + (0x0f << 16) + (0x0f<<24);
	// for odd part, we shift the sum by 4(since 0000 1111)
	// and use same mask
	sum = (sum & mask) + ((sum >> 4) & mask);


	// Again, we do the same thing but with 
	// 8 bits as one index, the we can get the number of 1s in each 
	// 16 bits
	// for even part, we use 0000000011111111, which is 0x00ff00ff
	mask = 0xff + (0xff << 16);
	// by same argument as above
	sum = (sum & mask) + ((sum >> 8) & mask);

	// Last is 16 bits as one index to get the number of 1s in
	// in 32bits for original bits, in 32bits, it means
	// sum's value acutally represents the number of 1s in x
	// for even part, 0x0000ffff
	mask = ~((~0x00) << 16);
	sum = (sum & mask) + ((sum >> 16)& mask);
	
  return sum;
}
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
	// we can use De Morgan's law, whch states
	// ~(x|y) = ~x & ~y
	// and if we negate both sides, we can get
	// ~(~(x|y)) = ~(~x & ~y)
	// x|y = ~(~x & ~y), which satisfiy the legal ops

  return ~(~x&~y); // didn't work on old lnxsrv like 02
}
/*
 * bitRepeat - repeat x's low-order n bits until word is full.
 *   Can assume That 1 <= n <= 32.
 *   Examples: bitRepeat(1, 1) = -1
 *             bitRepeat(7, 4) = 0x77777777
 *             bitRepeat(0x13f, 8) = 0x3f3f3f3f
 *             bitRepeat(0xfffe02, 9) = 0x10080402
 *             bitRepeat(-559038737, 31) = -559038737
 *             bitRepeat(-559038737, 32) = -559038737
 *   Legal ops: int and unsigned ! ~ & ^ | + - * / % << >>
 *             (This is more general than the usual integer coding rules.)
 *   Max ops: 40
 *   Rating: 4
 */
int bitRepeat(int x, int n) {
	int neg_1 = ~0x00;
	int n_mask;
	int less_than_32;
	// First by using the mask with 1s in low n bits
	// we can obtain low n bits in x
	int mask = neg_1 << (n-1);
	mask = mask << 1;

	x = x & ~mask;

	// now we make the we operate or on x and x which is
	// left shifted by n to get n bit repeated twice.
	x = x | (x << n);


	// then now, we have low 2 repeated
	// so we can operate on x and x which is shifted 
	// by 2*n to get 4*n repeated.
	n = n*2;
	// since we want n = 0 any case when
	// n >= 32
	// first determine the most 32-5 bits, whehter it has 0
	// or not
	//n_mask = ~0x00;
	//n_mask <<= 5;
	n_mask = neg_1 << 5;
	// if less_than_32 has at least 1 in anywhere
	// it is not in range with 0 to 31, and some number, which means true
	// so negate and get 0: case1
	// if n is in range, less_than_32 are all 0s,so negate 
	// to get true which is 1: case2
	// in case 1, we want to make least 5 bits to all 0s
	// so that it won't effect on using x << n, so we & with
	// in case 2, we want least 5 bits to remain same
	// we will make all 1s by multiply by -1 (0 remains 0)
	less_than_32 = (!(n & n_mask)) * neg_1;
	x = x | (x << (n & less_than_32));

	// by doing the same thing we can get 4 bit repeatet
	n *= 2;
	less_than_32 = (!(n & n_mask)) * neg_1;
	x = x | (x << (n & less_than_32));

//printf("third x is %08x\n", x);

	// by doing the same thing we can get 8 bit repeate
	n *= 2;
	less_than_32 = (!(n & n_mask)) * neg_1;
	x = x | (x << (n & less_than_32));

// by doing the same thing we can get 16 bit repeated
	n *= 2;
	less_than_32 = (!(n & n_mask)) * neg_1;
	x = x | (x << (n & less_than_32));

  return x; 
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {

	// We can say that number fits in n bits
	// when we operate the left shift on 32-n bits
	// and shift back with the same amount and we still
	// get the original number
	// If sign changed to positive from negative because 
	// of n bits(most significant bit in n might be 1),
	// we can still say that it won't be the same bits
	// by applying right arithmetic shift since
	// it fills with 1 when negative
	
	int n_bits_num = ((x << (32+~n+1)) >> (32+~n+1));

	// to determine if n_bits_num is still same as
	// orignal x, use ^ operator, which gives 0 when it
	// is identical, so negate it to return true when 
	// they are same
	
	return !(x ^ n_bits_num);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
	int mask = 0xff;// extract least 8 bytes
	int n_mult;

	// Then depending on n,number of shift
	// from mask will differ with n*8;
	// since we cannot use mulitplication
	// n*8 = n*2^3 = (n<<3)
	n_mult = n << 3;
	x = (x & (mask << n_mult));
	
	// now we have 1s only for extracted bytes
	// but still have 0s for for other bytes area
	// so we need to apply right shift to make them back
	// to least area
	// and mask with least byte back to make 0s for the rest of 
	// bytes since if it was negative, it will make 1s by right shift
	x = (x >> (n_mult)) & mask;	
  return x; 
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {

	// there are 3 cases that x is less or equal to y
	// case1: x is negative an y is postive
	// case2: x is positive and y is positive, 
	// 			difference is negative 
	// case3: x is negative and y is negative,
	// 			difference is negative
	// case 2 and 3 can be combine as when sign agree,
	// difference is negative
	//
	// case4: they are equal
	
	//int diff = x - y;
	//since we are not allowed to use minus
	int diff = x + (~y+1);

	
	// determine sing of difference, x ,and y
	// if pos or 0, all 0s
	// if neg, all 1s
	// since I want just true or false, 1 or 0 as result
	// I apply !!
	// true when it is negative
	int diff_sign = !!(diff >> 31);
	int x_sign = !!(x >> 31);
	int y_sign = !!(y >> 31);
	// when same sign ^ gives zero
	// when diff sign ^ gives 1
	int sign_agree = !((x_sign) ^ (y_sign));


	// To determine if x = y
	// since it is subtraction, if it has the same bits
	// it yields to zero
	// it also takes care the case when x = INTMIN = y
	// which makes positive overflow as calculation.
	int diff_zero = !(x^y);
	
 return diff_zero | (sign_agree & diff_sign) | ((x_sign) & !(y_sign));
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
	// when we applly arithmetic right shift on x, we can get
	// all 0s when x is positive or 0 ------> false  (1)
	// all 1s when x is negative ------> true  (2)
	// now we need to take care when x = 0 
	// when x = 0, if we negate(!) them, we can get ----->true (3)
	// when x is not zero, if we negate them, we can get 0 ----> false (4)
	// now only the case we want to return is x is positve and not zero
	// which means satisfying the (1) and (4)
	// and since they both give false, we negate to return true.
	return !((x >> 31) | !x);
}

/* 
 * 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
	// To make logical right shift, we need a mask which 
	// has 1s in n low-order bits
	int mask = ~0x00;
	
	// here in case n = 0, we gonna have 32 shift which is not allowed
	// so I did separetley, which is not illegal to do.
	//mask <<= 31-n;
	// since we cannot use minus sign, use
	// -x = ~x +1
	mask <<= 31+(~n+1);
	mask <<= 1;
	mask = ~mask;

	// now when we opearte & between the arithmetically right shifted x and
	// mask
	// we can get the equivalent bit whic is logically right shifted x	
  return (x >> n) & mask;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
	int x = 0x01;
	
	return (x << 31);
  return 2;
}
