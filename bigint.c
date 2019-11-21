
#include <stdio.h>
#include <string.h>


// ****************
// C BigInt Library
// ****************


// ***
// Standard Defines
// ***

#define T 1
#define F 0


// ***
// Type & Constant Definitions
// ***

typedef unsigned char Byte;

Byte const MAX_DIGITS[255] = {
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};

Byte const ZERO_DIGITS[255] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

typedef struct digit_set_type
{
    Byte digit[255];

} DigitSet;

typedef struct bigint_type
{
    boolean isNeg;
    Byte numDigits;
    Byte numEndZeroes;
    boolean isTenFactor;
    boolean isMinOrMax;
    DigitSet digits;

} Bigint;

// note that isNeg for zero is generally ignored, but should always be FALSE
Bigint const BIGINT_ZERO = {F,1,1,F,F,ZERO_DIGITS};



// ******************************************************************************* //
// ******************************************************************************* //
// ******************************************************************************* //


// ***
// Returns a digit-byte array of max value
function getMaxValueDigits()
{
  // initialize
  var i, digits = [];
  digits.length = MAX_NUM_DIGITS;
  
  // fill
  for (i = 0; i < MAX_NUM_DIGITS; i++)
    {digits[i] = 9;}
  
  // return
  return digits;
}


// ***
// Given an integer value, creates a new bigint
// of max value, positive or negative based on the
// sign of the given value (0 yields positive,)
// returning the new, maxed bigint object
function newMaxBigint(intValue)
{
  // *** TESTING CODE
  if (! Number.isInteger(intValue))
    {}
  // *** END TESTING CODE

  // initialize
  var bigint = {};
  
  // determine the sign
  if (intValue < 0)
    {bigint.isNeg = true;}
  else
    {bigint.isNeg = false;}
  
  // fill in rest of values
  bigint.numDigits = MAX_NUM_DIGITS;
  bigint.numEndZeroes = 0;
  bigint.isTenFactor = false;
  bigint.isMinOrMax = true;
  bigint.digits = getMaxValueDigits();
  
  // return the final bigint object
  return bigint;
}


// ***
// Given an integer value, creates a
// new bigint based on that value,
// returning the bigint object
function newBigint(intValue)
{
  // *** TESTING CODE
  if (! Number.isInteger(intValue))
    {}
  // *** END TESTING CODE

  // initialize
  var L, neg, zeroes, nextDigit, leftover, digits;
  var isFactor = false;
  var bigint = {};
  
  // easy edge case first
  if (intValue == 0)
  {
    bigint.isNeg = false;
    bigint.numDigits = 1;
    bigint.numEndZeroes = 1;
    bigint.isTenFactor = false;
    bigint.isMinOrMax = false;
    bigint.digits = [0];
  }
  else
  {
    // fill in the immediately-knowable values
    neg = (intValue < 0);
    bigint.isNeg = neg;
    
    // we only need to work with
    // the absolute value now
    if (neg)
      {intValue = -intValue;}
    
    // easy edge cases first
    if (intValue < 10)
    {
      bigint.numDigits = 1;
      bigint.numEndZeroes = 1;
      bigint.isTenFactor = false;
      bigint.isMinOrMax = false;
      bigint.digits = [intValue];
    }
    else
    {
      // convert to digit-byte array
      nextDigit = intValue % 10;
      leftover = Math.floor(intValue / 10);
      digits = [nextDigit];
      while (leftover > 0)
      {
        nextDigit = leftover % 10;
        leftover = Math.floor(leftover / 10);
        digits.unshift(nextDigit);
      }
      // finish filling in the bigint specs
      L = digits.length;
      zeroes = getNumEndingZeroes(digits);
      bigint.numDigits = L;
      bigint.numEndZeroes = zeroes;
      if ((digits[0] == 1) && (zeroes == (L - 1)))
        {isFactor = true;}
      bigint.isTenFactor = isFactor;
      bigint.isMinOrMax = false;
      bigint.digits = digits;
    }
  }
  
  // return the final bigint object
  return bigint;
}


// ***
// Given a digit-byte array,
// determines if it is zero
function digitsAreZero(digits)
{
  // *** TESTING CODE
  if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  // *** END TESTING CODE
  
  // initialize
  var L, currDigit, isZero = true;

  L = digits.length
  if (L > 1)
    {isZero = false;}
  else
  {
    currDigit = digits[0];
    // *** TESTING CODE
    if (! Number.isInteger(currDigit))
      {}
    else if ((currDigit < 0) || (currDigit > 9))
      {}
    // *** END TESTING CODE
    if (currDigit != 0)
      {isZero = false;}
  }
  
  // return the result
  return isZero;
}


// ***
// Given a digit-byte array, determines how
// many zeroes are on the end, returning the
// result as an integer
function getNumEndingZeroes(digits)
{
  // *** TESTING CODE
  if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  // *** END TESTING CODE

  // initialize
  var i, L, currDigit, numZeroes = 0;
  
  // loop through from the end until we hit a non-zero
  L = digits.length;
  for (i = L - 1; i >= 0; i--)
  {
    currDigit = digits[i];
    // *** TESTING CODE
    if (! Number.isInteger(currDigit))
      {}
    else if ((currDigit < 0) || (currDigit > 9))
      {}
    // *** END TESTING CODE
    if (currDigit == 0)
      {numZeroes++;}
    else
      {break;}
  }
  
  // return the result
  return numZeroes;
}


// ***
// Given a digit-byte array, determines if it
// a factor of 10 (1, 10, 100, 1000, etc.)
// returning -1 if not a factor, or if it is
// a factor, returning the size (# of zeroes)
function digitsAreFactor(digits)
{
  // *** TESTING CODE
  if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  //else if (digits.length > MAX_NUM_DIGITS)
    //{}
  else if (! Number.isInteger(digits[0]))
    {}
  else if ((digits[0] < 0) || (digits[0] > 9))
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, L, numZeroes, isFactor = true;

  // if first digit isn't a 1, it's not a factor
  if (digits[0] != 1)
    {isFactor = false;}
  else
  {
    // determine how many remaining digits are zero
    L = digits.length;
    numZeroes = getNumEndingZeroes(digits); // <= *******
    if (numZeroes < (L - 1))
      {isFactor = false;}
  }
  
  // return the result; -1 if not a factor,
  // else the size of the factor (# of zeroes)
  if (isFactor)
    {result = numZeroes;}
  else
    {result = -1;}

  return result;
}


// ***
// Given a digit-byte array, determines
// if it is a min/max value (positive or neg)
function digitsAreMinOrMax(digits)
{
  // *** TESTING CODE
  if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  else if (digits.length > MAX_NUM_DIGITS)
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, currDigit, isEndCase = true;

  if (digits.length == MAX_NUM_DIGITS)
  {
    for (i = 0; i < MAX_NUM_DIGITS; i++)
    {
      currDigit = digits[i];
      // *** TESTING CODE
      if (! Number.isInteger(currDigit))
        {}
      else if ((currDigit < 0) || (currDigit > 9))
        {}
      // *** END TESTING CODE
      if (currDigit < 9)
      {
        isEndCase = false;
        break;
      }
    }
  }
  else
    {isEndCase = false;}

  // return result
  return isEndCase;
}


// ***
// Given a digit-byte array and a single-digit value,
// multiplies the array by the value, returning a
// (presumably longer) digit-byte array
function multiplyDigitsBySingleDigit(digits, singleDigit)
{
  // *** TESTING CODE
  if (! Number.isInteger(singleDigit))
    {}
  else if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  else if (digits.length > MAX_NUM_DIGITS)
    {}
  else if ((singleDigit < 0) || (singleDigit > 9))
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, currDigit, nextDigit, currResult;
  var resultArr = [], carryOver = 0;
  
  // check for easy edge cases
  if (singleDigit == 0)
    {resultArr = [0];}
  else if (singleDigit == 1)
    {resultArr = digits.slice();} // copies array
  else
  {
    // loop through all digits, multiplying by each in turn
    for (i = digits.length - 1; i >= 0; i--)
    {
      currDigit = digits[i];
      // *** TESTING CODE
      if (! Number.isInteger(currDigit))
        {}
      else if ((currDigit < 0) || (currDigit > 9))
        {}
      // *** END TESTING CODE
      currResult = currDigit * singleDigit;
      currResult += carryOver;
      if (currResult > 9)
      {
        nextDigit = currResult % 10;
        carryOver = Math.floor(currResult / 10);
      }
      else
      {
        nextDigit = currResult;
        carryOver = 0;
      }
      resultArr.unshift(nextDigit);
    }
    
    // add anything left in the carry-over
    if (carryOver > 0)
      {resultArr.unshift(carryOver);}
  }
  
  // return the final resultant digit-byte array
  return resultArr;
}


// ***
// Given a digit-byte array and a value for the
// factor, increases the number by the specified
// factor of ten, returning the resulting
// (longer) digit-byte array with extra zeroes
function incDigitsByFactor(digits, factor)
{
  // *** TESTING CODE
  if (! Number.isInteger(factor))
    {}
  else if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  else if (digits.length > MAX_NUM_DIGITS)
    {}
  else if (factor < 1)
    {}
  // *** END TESTING CODE
  
  // initialize
  var i;
  var resultArr = digits.slice(); // copies the array
  
  // do the factor increasing
  for (i = 0; i < factor; i++)
    {resultArr.push[0];}

  // return the final resultant digit-byte array
  return resultArr;  
}


// ***
// Given an array of single digit values to be added,
// adds them up, returning two values (in an object,)
// the result digit ("digit") and any overflow
// ("overflow")
function addSingleDigits(digits)
{
  // *** TESTING CODE
  if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  else if (digits.length > MAX_NUM_DIGITS)
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, currDigit, resultDigit, overflow, resultValue = 0, resultObj = {};
  
  // add them all
  for (i = 0; i < digits.length; i++)
  {
    currDigit = digits[i];
    // *** TESTING CODE
    if (! Number.isInteger(currDigit))
      {}
    else if ((currDigit < 0) || (currDigit > 9))
      {}
    // *** END TESTING CODE  
    resultValue += currDigit;
  }
  
  // parse and store the results
  resultDigit = resultValue % 10;
  overflow = Math.floor(resultValue / 10);
  resultObj.digit = resultDigit;
  resultObj.overflow = overflow;
  
  // return the results
  return resultObj;
}


// ***
// Given two digit-byte arrays, compares them
// and returns the appropriate result: -1 (first
// is less than the second,) 0 (they're equal)
// or 1 (if the first is greater than second,)
// treating them both as positive numbers since
// there are no signs involved
function compareDigits(digits1, digits2)
{
  // *** TESTING CODE
  if (! Array.isArray(digits1))
    {}
  else if (! Array.isArray(digits2))
    {}
  else if (digits1.length < 1)
    {}
  //else if (digits1.length > MAX_NUM_DIGITS)
    //{}
  else if (digits2.length < 1)
    {}
  //else if (digits2.length > MAX_NUM_DIGITS)
    //{}
  else if (! Number.isInteger(digits1[0]))
    {}
  else if (! Number.isInteger(digits2[0]))
    {}
  else if ((digits1[0] < 0) || (digits1[0] > 9))
    {}
  else if ((digits2[0] < 0) || (digits2[0] > 9))
    {}
  // *** END TESTING CODE

  // initialize
  var i, L1, L2, currDigit1, currDigit2, comparison = 0;
  
  L1 = digits1.length;
  L2 = digits2.length;
  // see which digit array is the longer one
  if (L1 > L2)
    {comparison = 1;}
  else if (L2 > L1)
    {comparison = -1;}
  else
  {
    // first one to show the lower digit loses
    for (i = 0; i < L1; i++)
    {
      currDigit1 = digits1[i];
      currDigit2 = digits2[i];
      // *** TESTING CODE
      if (! Number.isInteger(currDigit1))
        {}
      else if (! Number.isInteger(currDigit2))
        {}
      else if ((currDigit1 < 0) || (currDigit1 > 9))
        {}
      else if ((currDigit2 < 0) || (currDigit2 > 9))
        {}
      // *** END TESTING CODE
      if (currDigit1 > currDigit2)
      {
        comparison = 1;
        break;
      }
      else if (currDigit1 < currDigit2)
      {
        comparison = -1;
        break;
      }
    }
  }
  
  // return result
  return comparison;
}


// ***
// Given a digit-byte array, adds one to
// the digits, returning the resultant
// digit-byte array
function incDigitsByOne(digits)
{
  // *** TESTING CODE
  if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  else if (digits.length > MAX_NUM_DIGITS)
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, currDigit, nextDigit;
  var L = digits.length, last = L - 1, resultArr = [], carryOver = 1;

  // easy edge case first
  if ((L == 1) && (digits[0] == 0))
    {resultArr == [1];}
  else if (digits[last] < 9)
  {
    resultArr = digits.slice(); // copies array
    resultArr[last] = digits[last] + 1;
  }
  else
  {
    // loop through the digits adding
    // (can stop adding once there's no
    // longer any carryover)
    for (i = last; i >= 0; i--)
    {
      currDigit = digits[i];
      // *** TESTING CODE
      if (! Number.isInteger(currDigit))
        {}
      else if ((currDigit < 0) || (currDigit > 9))
        {}
      // *** END TESTING CODE
      if (carryOver < 1)
        {resultArr.unshift(currDigit);}
      else
      {
        nextDigit = currDigit + 1;
        if (nextDigit > 9)
        {
          nextDigit -= 10;
          carryOver = 1;
        }
        else
          {carryOver = 0;}
        resultArr.unshift(nextDigit);
      }
    }
  }
  // add a 1 to front if there's still carryover
  if (carryOver > 0)
    {resultArr.unshift(1);}

  // return the results
  return resultArr;
}


// ***
// Given a digit-byte array, subtracts one
// from the digits, returning the resultant
// digit-byte array; if given 0, returns 1
function decDigitsByOne(digits)
{
  // *** TESTING CODE
  if (! Array.isArray(digits))
    {}
  else if (digits.length < 1)
    {}
  else if (digits.length > MAX_NUM_DIGITS)
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, currDigit, nextDigit;
  var L = digits.length, last = L - 1, resultArr = [], borrow = 1;

  // easy edge cases first
  if ((L == 1) && (digits[0] == 0))
    {resultArr = [1];}
  else if (digits[last] > 0)
  {
    resultArr = digits.slice(); // copies array
    resultArr[last] = digits[last] - 1;
  }
  else
  {
    // loop through the digits subtracting
    // (can stop subtracting once there's no
    // longer any borrowing)
    for (i = last; i >= 0; i--)
    {
      currDigit = digits[i];
      // *** TESTING CODE
      if (! Number.isInteger(currDigit))
        {}
      else if ((currDigit < 0) || (currDigit > 9))
        {}
      // *** END TESTING CODE
      if (borrow < 1)
        {resultArr.unshift(currDigit);}
      else
      {
        nextDigit = currDigit - 1;
        if (nextDigit < 0)
        {
          nextDigit += 10;
          borrow = 1;
        }
        else
          {borrow = 0;}
        resultArr.unshift(nextDigit);
      }
    }
  
    // if first digit is now a zero, drop it
    if (resultArr[0] == 0)
      {resultArr.shift();}
  }
  
  // return the results
  return resultArr;
}


// ***
// Given two sets of digit-byte arrays, adds
// them together, treating them as if both
// were positive numbers; returns the resultant
// digit-byte array--if they overflow, instead
// returns the max value; also returns a flag
// indicating if the result overflowed, for two
// return elements: "digits" and "overflow"
function addDigitsToDigits(digits1, digits2)
{
  // *** TESTING CODE
  if (! Array.isArray(digits1))
    {}
  else if (! Array.isArray(digits2))
    {}
  else if (digits1.length < 1)
    {}
  else if (digits1.length > MAX_NUM_DIGITS)
    {}
  else if (digits2.length < 1)
    {}
  else if (digits2.length > MAX_NUM_DIGITS)
    {}
  // *** END TESTING CODE

  // initialize
  var i, L1, L2, maxL, minL, offsetL, j, currDigit1, currDigit2, nextDigit, carryOver = 0;
  var resultArr = [], overflow = false;
  
  L1 = digits1.length;
  L2 = digits2.length;
  // ensure that the first digit array is the longer one
  if (L2 > L1)
  {
    maxL = L2;
    minL = L1;
    L2 = minL;
    L1 = maxL;
    j = digits2;
    digits2 = digits1;
    digits1 = j;
  }
  else
  {
    maxL = L1;
    minL = L2;
  }
  offsetL = maxL - minL;

  // loop through the smaller number of digits,
  // starting from the end, adding them as we go
  for (i = maxL - 1; i >= offsetL ; i--)
  {
    currDigit1 = digits1[i];
    currDigit2 = digits2[i - offsetL];
    // *** TESTING CODE
    if (! Number.isInteger(currDigit1))
      {}
    else if (! Number.isInteger(currDigit2))
      {}
    else if ((currDigit1 < 0) || (currDigit1 > 9))
      {}
    else if ((currDigit2 < 0) || (currDigit2 > 9))
      {}
    // *** END TESTING CODE
    nextDigit = currDigit1 + currDigit2 + carryOver;
    if (nextDigit > 9)
    {
      nextDigit -= 10;
      carryOver = 1;
    }
    else
      {carryOver = 0;}
    resultArr.unshift(nextDigit);
  }
  
  // loop through the rest of the longer
  // set of digits, adding any carryover
  for (i = offsetL - 1; i >= 0; i--)
  {
    currDigit1 = digits1[i];
    // *** TESTING CODE
    if (! Number.isInteger(currDigit1))
      {}
    else if ((currDigit1 < 0) || (currDigit1 > 9))
      {}
    // *** END TESTING CODE
    if (carryOver == 0)
      {nextDigit = currDigit1;}
    else
    {
      nextDigit = currDigit1 + carryOver;
      if (nextDigit > 9)
      {
        nextDigit -= 10;
        carryOver = 1;
      }
      else
        {carryOver = 0;}
    }
    resultArr.unshift(nextDigit);
  }
  
  // if any carryover left, add a one to the front
  if (carryOver > 0)
    {resultArr.unshift(1);}
  
  // determine if we had an overflow
  if (resultArr.length > MAX_NUM_DIGITS)
  {
    resultArr = getMaxValueDigits();
    overflow = true;
  }
  
  // return the results
  return {digits: resultArr, overflow: overflow};
}


// ***
// Given any number of sets of digit-byte arrays,
// adds them all together, treating them as if all
// were positive numbers; returns the resultant
// digit-byte array, even if there was overflow;
// also returns a flag indicating if the result
// overflowed, for a total of two returned
// elements: "digits" and "overflow"
function addDigitsToDigitsToDigits(digitSets)
{
  // *** TESTING CODE
  if (! Array.isArray(digitSets))
    {}
  else if (digitSets.length < 1)
    {}
  // *** END TESTING CODE

  // initialize
  var d, i, j, L, maxL, offsetL, currSet, currDigit, currSum, currIndex, nextDigit, carryOver = 0;
  var resultArr = [], overflow = false;
  
  // ensure we have the longest length of digits
  maxL = 0;
  for (i = 0; i < digitSets.length; i++)
  {
    currSet = digitSets[i];
    // *** TESTING CODE
    if (! Array.isArray(currSet))
      {}
    else if (currSet.length < 1)
      {}
    // *** END TESTING CODE
    L = currSet.length;
    if (L > maxL)
      {maxL = L;}
  }
  
  // loop through all digits; add up all the
  // corresponding digits from each set
  for (d = maxL - 1; d >= 0 ; d--)
  {
    // loop through all sets; grab the appropriate
    // digit from this set (if it has one,) and
    // add them all together for the next digit
    // of the sum, including any carryover
    currSum = carryOver;
    for (i = 0; i < digitSets.length; i++)
    {
      currSet = digitSets[i];
      // *** TESTING CODE
      if (! Array.isArray(currSet))
        {}
      else if (currSet.length < 1)
        {}
      //else if (currSet.length > MAX_NUM_DIGITS)
        //{}
      // *** END TESTING CODE
      L = currSet.length;
      offsetL = maxL - L;
      currIndex = d - offsetL;
      if (currIndex >= 0)
      {
        currDigit = currSet[currIndex];
        // *** TESTING CODE
        if (! Number.isInteger(currDigit))
          {}
        else if ((currDigit < 0) || (currDigit > 9))
          {}
        // *** END TESTING CODE
        currSum += currDigit;
      }
    }
    nextDigit = currSum % 10;
    carryOver = Math.floor(currSum / 10);
    resultArr.unshift(nextDigit);
  }
  
  // if any carryover left, add to front
  while (carryOver > 0)
  {
    nextDigit = carryOver % 10;
    carryOver = Math.floor(carryOver / 10);
    resultArr.unshift(nextDigit);
  }
    
  // determine if we had an overflow
  if (resultArr.length > MAX_NUM_DIGITS)
    {overflow = true;}
  
  // return the results
  return {digits: resultArr, overflow: overflow};
}


// ***
// Given two digit-byte arrays, attempts to subtract
// the second from the first, returning the resultant
// digit-byte array; note that there are no signs
// involved, and the first array must represent a
// value larger than the second
function subDigitsFromDigits(digits1, digits2)
{
  // *** TESTING CODE
  if (! Array.isArray(digits1))
    {}
  else if (! Array.isArray(digits2))
    {}
  else if (digits1.length < 1)
    {}
  else if (digits1.length > MAX_NUM_DIGITS)
    {}
  else if (digits2.length < 1)
    {}
  else if (digits2.length > MAX_NUM_DIGITS)
    {}
  else if (compareDigits(digits1, digits2) == -1)
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, L1, L2, offsetL, currDigit1, currDigit2, nextDigit, borrow = 0;
  var resultArr = [];
  
  // easy edge case first
  L1 = digits1.length;
  L2 = digits2.length;
  if ((L2 == 1) && (digits2[0] == 0))
    {resultArr = digits1.slice();} // copies array
  else
  {
  
    offsetL = L1 - L2;
    // loop through the digits subtracting
    // (can stop subtracting once there's no
    // longer any borrowing)
    for (i = L1 - 1; i >= offsetL; i--)
    {
      currDigit1 = digits1[i];
      currDigit2 = digits2[i - offsetL];
      // *** TESTING CODE
      if (! Number.isInteger(currDigit1))
        {}
      else if (! Number.isInteger(currDigit2))
        {}
      else if ((currDigit1 < 0) || (currDigit1 > 9))
        {}
      else if ((currDigit2 < 0) || (currDigit2 > 9))
        {}
      // *** END TESTING CODE
      nextDigit = currDigit1 - currDigit2 - borrow;
      if (nextDigit < 0)
      {
        nextDigit += 10;
        borrow = 1;
      }
      else
        {borrow = 0;}
      resultArr.unshift(nextDigit);
    }
    
    // loop through remainder of digits
    // (for which the second array wasnt
    // long enough) and continue borrowing
    // as needed or simply copying digits
    for (i = offsetL - 1; i >= 0; i--)
    {
      currDigit1 = digits1[i];
      // *** TESTING CODE
      if (! Number.isInteger(currDigit1))
        {}
      else if ((currDigit1 < 0) || (currDigit1 > 9))
        {}
      // *** END TESTING CODE
      if (borrow < 1)
        {resultArr.unshift(currDigit1);}
      else
      {
        nextDigit = currDigit1 - borrow;
        if (nextDigit < 0)
        {
          nextDigit += 10;
          borrow = 1;
        }
        else
          {borrow = 0;}
        resultArr.unshift(nextDigit);
      }
    }
  
    // remove leading zeroes
    while ((resultArr.length > 1) && (resultArr[0] == 0))
      {resultArr.shift();}
  }
  
  // return the results
  return resultArr;
}


// ***
// Given two digit-byte arrays, divides the first by
// the second to determine a single-digit quotient,
// returning both the quotient ("quotient") and the
// digit-byte array remainder ("remainder")... note
// that the second array cannot represent a "0"
// (and really, cannot even have a leading zero)
function divideDigitsByDigitsForSingleDigitQuotient(digits1, digits2)
{
  var L1 = digits1.length;
  var L2 = digits2.length;

  // *** TESTING CODE
  if (! Array.isArray(digits1))
    {}
  else if (! Array.isArray(digits2))
    {}
  else if (L1 < 1)
    {}
  else if (L1 > MAX_NUM_DIGITS)
    {}
  else if (L2 < 1)
    {}
  else if (L2 > MAX_NUM_DIGITS)
    {}
  else if (digits2[0] == 0)
    {}
  // *** END TESTING CODE

  // initialize
  var i, currProduct, addSet, comparison, exceeded;
  var quotient, remainder = [];
  var workObj, resultObj = {};
  
  // easy edge cases first:
  // if dividing by 1
  if ((L2 == 1) && (digits2[0] == 1))
  {
    quotient = digits1[0];
    remainder = [0];
  }
  
  // else if dividing by a clearly larger number
  else if (L2 > L1)
  {
    quotient = 0;
    remainder = digits1.slice(); // copies array
  }
  
  else
  {
    comparison = compareDigits(digits1, digits2);
    // easy edge case: if numbers are the same
    if (comparison == 0)
    {
      quotient = 1;
      remainder = [0];
    }
    
    // easy edge case: if dividing by a larger number
    else if (comparison == -1)
    {
      quotient = 0;
      remainder = digits1.slice(); // copies array
    }
    
    // normal case
    else
    {
      // loop until we determine how many times
      // the second number goes into the first
      // without exceeding it
      exceeded = false;
      currProduct = digits2.slice(); // copies array
      quotient = 1;
      while ((!exceeded) && (quotient < 9))
      {
        workObj = addDigitsToDigits(currProduct, digits2);
        currProduct = workObj.digits;
        comparison = compareDigits(currProduct, digits1);
        quotient++;
        if (comparison == 0)
          {exceeded = true;}
        else if (comparison == 1)
        {
          exceeded = true;
          workObj = subDigitsFromDigits(currProduct, digits2);
          currProduct = workObj;
          quotient--;
        }
      }
      // determine remainder
      if (comparison == 0)
        {remainder = [0];}
      else
        {remainder = subDigitsFromDigits(digits1, currProduct);}
    }
  }
  
  // package & return the results
  if (remainder.length == 0)
    {remainder = [0];}
  resultObj.quotient = quotient;
  resultObj.remainder = remainder;
  return resultObj;
}



// ******************************************************************************* //
// ******************************************************************************* //
// ******************************************************************************* //

/*******
 *
 * Given a bigint, determines if it is zero.
 *
 *******/
function IsZero(bigint)
{
  return (bigint.numDigits == bigint.numEndZeroes);
}


/*******
 *
 * Given a bigint, determines if it is negative.
 *
 *******/
function IsNeg(bigint)
{
  return bigint.isNeg;
}


/*******
 *
 * Given a bigint, determines if it is the max digit value.
 *
 *******/
function IsMinOrMax(bigint)
{
  return bigint.isMinOrMax;
}


/*******
 *
 * Given a bigint, determines if it is a factor of ten.
 *
 *******/
function IsFactorOfTen(bigint)
{
  return bigint.isTenFactor;
}


/*******
 *
 * Given a bigint, returns the number of digits.
 *
 *******/
function GetNumDigits(bigint)
{
  return bigint.numDigits;
}


/*******
 *
 * Given a bigint, returns the array of digits.
 *
 *******/
function GetDigits(bigint)
{
  return bigint.digits;
}


/*******
 *
 * Given a bigint, returns its negative.
 * A bigint of 0 returns 0, but the sign
 * will remain positive.
 *
 *******/
function Negate(bigint)
{
  var copyBigintData = false;
  var isNeg = bigint.isNeg;
  var newBI = newBigint(0);
  
  if (isNeg)
  {
    copyBigintData = true;
  }
  else if (bigint.numDigits > bigint.numEndZeroes)
  {
    newBI.isNeg = true;
    copyBigintData = true;
  }
  
  if (copyBigintData)
  {
    // does not copy the sign
    newBI.numDigits = bigint.numDigits
    newBI.numEndZeroes = bigint.numEndZeroes;
    newBI.isTenFactor = bigint.isTenFactor;
    newBI.isMinOrMax = bigint.isMinOrMax;
    newBI.digits = bigint.digits.slice(); // copies array
  }
  
  return newBI;
}

/*******
 *
 * Given an integer, returns the equivalent Bigint.
 *
 *******/
function IntToBigint(intValue)
{
  // *** TESTING CODE
  if (! Number.isInteger(intValue))
    {}
  // *** END TESTING CODE
  
  // initialize
  var theBigint;
  
  // create
  theBigint = newBigint(intValue);
  
  // return
  return theBigint;
}


/*******
 *
 * Given a bigint, copies it, returning a new,
 * distinct one of identical value. (This is
 * to work around passing objects by reference.)
 *
 *******/
function CopyBigint(bigint)
{
  var newBI = newBigint(0);
  newBI.isNeg = bigint.isNeg;
  newBI.numDigits = bigint.numDigits
  newBI.numEndZeroes = bigint.numEndZeroes;
  newBI.isTenFactor = bigint.isTenFactor;
  newBI.isMinOrMax = bigint.isMinOrMax;
  newBI.digits = bigint.digits.slice(); // copies array
  return newBI;  
}


/*******
 *
 * Given a string of digits, with an optional leading sign,
 * returns the equivalent Bigint. (If too many digits, or
 * malformed string numeric, instead returns a boolean
 * FALSE to indicate error.)
 *
 *******/
function StrToBigint(strValue)
{
  // *** TESTING CODE
  if ((typeof strValue) !== "string")
    {}
  else if (strValue.length < 1)
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, L, start, currChar, nextDigit, digits, sign = 1, foundNonZero = false;
  var isFactor = false, theBigint = newBigint(0);
  
  // determine if there's a sign;
  // if so, note it and adjust starting
  // index so we skip it
  currChar = strValue[0];
  if (currChar == "-")
  {
    sign = -1;
    start = 1;
  }
  else if (currChar == "+")
    {start = 1;}
  else
    {start = 0;}
  
  // skip leading zeroes
  L = strValue.length;
  while ((start < L) && (!foundNonZero))
  {
    currChar = strValue[start];
    if ((currChar < "0") || (currChar > "9"))
      {return false;}
    else if (currChar == "0")
      {start++;}
    else
      {foundNonZero = true;}
  }
  
  // if we only had zeroes, just return the zero
  if ((start == L) && (!foundNonZero))
    {return theBigint;}
  
  // ensure we won't have too many digits
  if ((L - start) > MAX_NUM_DIGITS)
    {return false;}

  // loop through each character
  // to construct the digits
  digits = [];
  for (i = start; i < L; i++)
  {
    nextDigit = strValue.charCodeAt(i) - 48;
    if ((nextDigit > 9) || (nextDigit < 0))
      {return false;}
    digits.push(nextDigit);
  }
  
  // adjust the bigint to the new value
  theBigint.isNeg = (sign < 0);
  theBigint.numDigits = digits.length;
  theBigint.digits = digits;
  theBigint.numEndZeroes = getNumEndingZeroes(digits);
  if ((digits[0] == 1) && (theBigint.numEndZeroes == (digits.length - 1)))
    {isFactor = true;}
  theBigint.isTenFactor = isFactor;
  theBigint.isMinOrMax = digitsAreMinOrMax(digits);
  
  // return the bigint
  return theBigint;
}


/*******
 *
 * Given a bigint, returns the string representation
 * of the value; if the optional sign parameter is set
 * to TRUE, then if the value is positive and non-zero,
 * a "+" will be attached to the front. Regardless, if
 * the value is negative, a "-" will be attached.
 *
 *******/
function BigintToStr(bigint, sign = false)
{
  // initialize
  var i, L, currDigit, digits;
  var signChar = "";
  var strValue = "";
  
  // determine sign (if any)
  if (bigint.isNeg)
    signChar = "-";
  else if (sign && (bigint.numDigits == bigint.numEndZeroes))
    signChar = "+";
  
  // begin constructing string representation
  digits = bigint.digits;
  L = digits.length;
  for (i = 0; i < L; i++)
  {
    currDigit = digits[i];
    strValue += String.fromCharCode(currDigit + 48);
  }
  
  // return the result
  return signChar + strValue;
}


/*******
 *
 * Given two bigints, compares them and returns
 * an appropriate result; -1 if the first is less
 * than the second, 1 if the first is greater, or
 * zero if the two bigints are numerically equal.
 *
 *******/
function CompareBigints(bigint1, bigint2)
{
  // initialize
  var i, isNeg1, isNeg2, digits1, digits2, finalComparison = 0;
  
  // first, check signs:
  isNeg1 = bigint1.isNeg;
  isNeg2 = bigint2.isNeg;
  // if signs are different, the positive is greater
  if (isNeg1 !== isNeg2)
  {
    if (isNeg1)
      {finalComparison = -1;}
    else
      {finalComparison = 1;}
  }
  
  // else if the signs are the same, we have more work to do
  else
  {
    // actually compare the digits
    digits1 = bigint1.digits;
    digits2 = bigint2.digits;
    // the comparison is the true result
    finalComparison = compareDigits(digits1, digits2);
  }
  
  // return the result
  return finalComparison;
}


/*******
 *
 * Given a bigint, increments it by one. If the
 * bigint is at the positive max value and would
 * overflow, instead a boolean FALSE is returned
 * to indicate error.
 *
 *******/
function IncBigintByOne(bigint)
{
  // initialize
  var inc, newBI, numDigits, numZeroes, isMax = false, isFactor = false, neg = bigint.isNeg;
  
  // if positive/zero (we know it won't go negative)
  if (!neg)
  {
    if (IsMinOrMax(bigint))
      {return false;}
    
    newBI = newBigint(0);
    inc = incDigitsByOne(bigint.digits);
    numDigits = inc.length;
    numZeroes = getNumEndingZeroes(inc);
    if ((numDigits == MAX_NUM_DIGITS) && (numZeroes == 0))
      {isMax = digitsAreMinOrMax(inc);}
    if ((inc[0] == 1) && (numZeroes == numDigits - 1))
      {isFactor = true;}
    newBI.numDigits = numDigits;
    newBI.numEndZeroes = numZeroes;
    newBI.isMinOrMax = isMax;
    newBI.isTenFactor = isFactor;
    newBI.digits = inc;
  }
  // if negative
  else
  {
    newBI = newBigint(0);
    numDigits = bigint.numDigits;
    // we already have a 0; only continue if
    // incrementing a value less than -1
    if ((numDigits > 1) || (bigint.digits[0] > 1))
    {
      inc = decDigitsByOne(bigint.digits);
      numDigits = inc.length;
      numZeroes = getNumEndingZeroes(inc);
      if ((inc[0] == 1) && (numZeroes == numDigits - 1))
        {isFactor = true;}
      newBI.isNeg = true;
      newBI.numDigits = numDigits;
      newBI.numEndZeroes = numZeroes;
      newBI.isTenFactor = isFactor;
      newBI.digits = inc;
    }
  }
  
  // return result
  return newBI;
}


/*******
 *
 * Given a bigint, decrements it by one. If the
 * bigint is at the negative max value and would
 * overflow, instead a boolean FALSE is returned
 * to indicate error.
 *
 *******/
function DecBigintByOne(bigint)
{
  // initialize
  var dec, newBI, numDigits, numZeroes, isMax = false, isFactor = false, neg = bigint.isNeg;
  
  // if positive/zero
  if (!neg)
  {
    newBI = newBigint(0);
    // if zero, we know result is -1
    if (bigint.numDigits == bigint.numEndZeroes)
    {
      newBI.isNeg = true;
      newBI.isTenFactor = true;
      newBI.numEndZeroes = 0;
      newBI.digits = [1];
    }
    // else since non-zero positive
    else
    {
      dec = decDigitsByOne(bigint.digits);
      numDigits = dec.length;
      numZeroes = getNumEndingZeroes(dec);
      if ((dec[0] == 1) && (numZeroes == numDigits - 1))
        {isFactor = true;}
      newBI.numDigits = numDigits;
      newBI.numEndZeroes = numZeroes;
      newBI.isTenFactor = isFactor;
      newBI.digits = dec;
    }
  }
  // if negative (we know it won't go positive)
  else
  {
    if (IsMinOrMax(bigint))
      {return false;}
    
    newBI = newBigint(0);
    dec = decDigitsByOne(bigint.digits);
    numDigits = dec.length;
    numZeroes = getNumEndingZeroes(dec);
    if ((numDigits == MAX_NUM_DIGITS) && (numZeroes == 0))
      {isMax = digitsAreMinOrMax(dec);}
    if ((dec[0] == 1) && (numZeroes == numDigits - 1))
      {isFactor = true;}
    newBI.isNeg = true;
    newBI.numDigits = numDigits;
    newBI.numEndZeroes = numZeroes;
    newBI.isMinOrMax = isMax;
    newBI.isTenFactor = isFactor;
    newBI.digits = dec;
  }
  
  // return result
  return newBI;
}


/*******
 *
 * Given a bigint and a factor-num, multiplies the
 * bigint by the specified factor of ten, returning
 * the larger bigint. In case of overflow, or an
 * invalid factor-num (must be an integer of 0+,)
 * returns a boolean FALSE to indicate error.
 *
 *******/
function MultiplyByFactorOfTen(bigint, factor)
{
  // init
  var i, L, newL, newBI, digits;
  
  // determine if invalid factor
  if ((! Number.isInteger(factor)) || (factor < 0))
    {return false;}
  
  // determine if overflow
  L = bigint.numDigits;
  if (L + factor > MAX_NUM_DIGITS)
    {return false;}
  
  // begin creating new bigint
  newBI = newBigint(0);
  
  // copy over changed values
  newBI.isNeg = bigint.isNeg;
  newBI.numDigits = bigint.numDigits + factor;
  newBI.numEndZeroes = bigint.numEndZeroes + factor;
  newBI.isTenFactor = bigint.isTenFactor;
  newBI.isMinOrMax = false;
  
  // easy edge case first
  if (factor == 0)
    {newBI.digits = bigint.digits.slice();} // copies array
  else
  {
    // expand the digits
    digits = bigint.digits.slice(); // copies array
    newL = L + factor;
    digits.length = newL;
    for (i = L; i < newL; i++)
      {digits[i] = 0;}
    newBI.digits = digits;
  }
  
  // return the new bigint
  return newBI;
}


/*******
 *
 * Given a set of one or more Bigints, adds all their values
 * together, returning the final Bigint value, along with a
 * flag indicating overflow or underflow.
 *
 *******/
function AddBigints(bigintSet)
{
  // *** TESTING CODE
  if (! Array.isArray(bigintSet))
    {}
  else if (bigintSet.length < 1)
    {}
  else if (Array.isArray(bigintSet[0]) || ((typeof bigintSet[0]) !== "object"))
    {}
  // *** END TESTING CODE
  
  // initialize
  var i, L, currBigint, isFactor, comparison, overflow = 0;
  var numPos, numNeg, workSet, workResult, posSum, negSum, posSet, negSet;
  var digitsL, finalSum, finalDigits, finalBigint, finalObj = {};
  
  // easy edge case first
  L = bigintSet.length;
  if (L == 1)
    {finalBigint = CopyBigint(bigintSet[0]);}
  else
  {
    // prepare final bigint
    finalBigint = newBigint(0);
    
    // separate bigints into positives & negatives (ignore zeroes)
    posSet = [];
    negSet = [];
    for (i = 0; i < L; i++)
    {
      currBigint = bigintSet[i];
      // *** TESTING CODE
      if (Array.isArray(currBigint) || ((typeof currBigint) !== "object"))
        {}
      // *** END TESTING CODE
      if (currBigint.isNeg)
        {negSet.push(currBigint);}
      else if (currBigint.numDigits > currBigint.numEndZeroes)
        {posSet.push(currBigint);}
    }
    
    // only continue calcs if positives or negatives
    // exist (otherwise we skip calcs & return 0)
    numPos = posSet.length;
    numNeg = negSet.length;
    if ((numPos > 0) || (numNeg > 0))
    {
      // sum all the positives
      if (numPos > 0)
      {
        workSet = [];
        for (i = 0; i < numPos; i++)
        {
          currBigint = posSet[i];
          workSet.push(currBigint.digits);
        }
        posSum = addDigitsToDigitsToDigits(workSet); // "digits" and "overflow"
      }
      
      // sum all the negatives
      if (numNeg > 0)
      {
        workSet = [];
        for (i = 0; i < numNeg; i++)
        {
          currBigint = negSet[i];
          workSet.push(currBigint.digits);
        }
        negSum = addDigitsToDigitsToDigits(workSet); // "digits" and "overflow"
      }
      
      // if only positives, check for overflow & return positive bigint
      if (numNeg < 1)
      {
        if (posSum.overflow)
        {
          overflow = 1;
          finalBigint = newMaxBigint(1);
        }
        else
        {
          finalDigits = posSum.digits;
          finalBigint.isMinOrMax = digitsAreMinOrMax(finalDigits);
        }
      }
      
      // if only negatives, check for underflow & return negative bigint
      else if (numPos < 1)
      {
        if (negSum.overflow)
        {
          overflow = -1;
          finalBigint = newMaxBigint(-1);
        }
        else
        {
          finalDigits = negSum.digits;
          finalBigint.isMinOrMax = digitsAreMinOrMax(finalDigits);
          finalBigint.isNeg = true;
        }
      }
      
      // if both, calculate the final bigint, then check for over/underflow
      else
      {
        // see which absolute value is bigger
        var posDigits = posSum.digits;
        var negDigits = negSum.digits;
        comparison = compareDigits(posDigits, negDigits);
        
        // if negative, then subtract smaller from larger and make final negative
        if (comparison == -1)
        {
          finalSum = subDigitsFromDigits(negDigits, posDigits);
          if (finalSum.length > MAX_NUM_DIGITS)
          {
            overflow = -1;
            finalBigint = newMaxBigint(-1);
          }
          else
          {
            finalDigits = finalSum;
            finalBigint.isMinOrMax = digitsAreMinOrMax(finalDigits);
            finalBigint.isNeg = true;
          }
        }
        
        // else if positive, subtract smaller from larger and leave final positive
        else if (comparison == 1)
        {
          finalSum = subDigitsFromDigits(posDigits, negDigits);
          if (finalSum.length > MAX_NUM_DIGITS)
          {
            overflow = 1;
            finalBigint = newMaxBigint(1);
          }
          else
          {
            finalDigits = finalSum;
            finalBigint.isMinOrMax = digitsAreMinOrMax(finalDigits);
          }
        }
        
        // else if same, no need to actually subtract; just return the zero
        else
        {
          // finalBigint is already zero; do nothing
        }
        
      } // end check for if only positives, or only negatives, or both
      
      // finalize the bigint (only needed if no overflow)
      if (overflow == 0)
      {
        finalBigint.numDigits = finalDigits.length;
        finalBigint.digits = finalDigits;
        finalBigint.numEndZeroes = getNumEndingZeroes(finalDigits);
        if ((finalDigits[0] == 1) && (finalBigint.numEndZeroes == (finalDigits.length - 1)))
          {isFactor = true;}
        finalBigint.isTenFactor = isFactor;
      }
      
    } // end check for positives or negatives
  }
  
  // return the final result in an object
  // with a flag indicating over/underflow
  finalObj.bigint = finalBigint;
  finalObj.overUnderFlow = overflow;
  return finalObj;
}


/*******
 *
 * Given two bigints, subtracts the second from the
 * first, returning the resultant bigint, along with
 * a flag indicating if there was over/underflow.
 *
 *******/
function SubtractBigints(bigint1, bigint2)
{
  // initialize
  var overflow = 0;
  var workSet, workBigint, workObj;
  var finalBigint, finalObj = {};
  
  // easy edge cases first:
  // if first bigint is zero
  if (bigint1.numDigits == bigint1.numEndZeroes)
  {
    // result is simply the negative of the second bigint
    finalBigint = Negate(bigint2);
  }
  // else if second bigint is zero
  else if (bigint2.numDigits == bigint2.numEndZeroes)
  {
    // result is just the first bigint
    finalBigint = CopyBigint(bigint1);
  }
  // else normal cases
  else
  {
    // simplest method to avoid replicating code
    // is to negate the second bigint, and add them
    workBigint = Negate(bigint2);
    workSet = [bigint1, workBigint];
    workObj = AddBigints[workSet];
    finalBigint = workObj.bigint;
    overflow = workObj.overUnderFlow;
  }
  
  // return the final result in an object
  // with a flag indicating over/underflow
  finalObj.bigint = finalBigint;
  finalObj.overUnderFlow = overflow;
  return finalObj;
}


/*******
 *
 * Given two bigints, multiplies them together,
 * returning the resultant bigint, along with
 * a flag indicating if there was over/underflow.
 *
 *******/
function MultiplyBigints(bigint1, bigint2)
{
  // initialize
  var numDigits1, numDigits2, numZeroes1, numZeroes2, digits, digits1, digits2, subProducts;
  var i, zeroesToAdd, currDigit, currProd, currTotal, currZeroes, newSize, overflow = 0;
  var workResult, preZeroes, isFactor, finalBigint, finalSign, finalObj = {};
  
  // determine final sign
  // (ignored if returning zero)
  if (bigint1.isNeg == bigint2.isNeg)
    {finalSign = 1;}
  else
    {finalSign = -1;}
  
  // easy edge cases first:
  numDigits1 = bigint1.numDigits;
  numDigits2 = bigint2.numDigits;
  numZeroes1 = bigint1.numEndZeroes;
  numZeroes2 = bigint2.numEndZeroes;
  // if one of the bigints is zero
  if ((numDigits1 == numZeroes1) || (numDigits2 == numZeroes2))
  {
    // multiplying by zero results in zero
    finalBigint = newBigint(0);
  }
  
  // else if the first bigint is a factor of ten
  else if (bigint1.isTenFactor)
  {
    // just add zeroes to the second bigint
    zeroesToAdd = bigint1.numEndZeroes;
    newSize = numDigits2 + zeroesToAdd;
    if (newSize > MAX_NUM_DIGITS)
    {
      finalBigint = newMaxBigint(finalSign);
      overflow = finalSign;
    }
    else
    {
      finalBigint = newBigint(0);
      digits = incDigitsByFactor(bigint2.digits, zeroesToAdd);
      finalBigint.digits = digits;
      finalBigint.numDigits = newSize;
      finalBigint.numEndZeroes = bigint2.numEndZeroes + zeroesToAdd;
      finalBigint.isTenFactor = bigint2.isTenFactor;
      finalBigint.isNeg = (finalSign < 0);
    }
  }
  
  // else if the second bigint is a factor of ten
  else if (bigint2.isTenFactor)
  {
    // just add zeroes to the first bigint
    zeroesToAdd = bigint2.numEndZeroes;
    newSize = numDigits1 + zeroesToAdd;
    if (newSize > MAX_NUM_DIGITS)
    {
      finalBigint = newMaxBigint(finalSign);
      overflow = finalSign;
    }
    else
    {
      finalBigint = newBigint(0);
      digits = incDigitsByFactor(bigint1.digits, zeroesToAdd);
      finalBigint.digits = digits;
      finalBigint.numDigits = newSize;
      finalBigint.numEndZeroes = bigint1.numEndZeroes + zeroesToAdd;
      finalBigint.isTenFactor = bigint1.isTenFactor;
      finalBigint.isNeg = (finalSign < 0);
    }
  }
  
  // else normal cases
  else
  {
    // quick check to see if we'll overflow
    newSize = numDigits1 + numDigits2;
    if (newSize > MAX_NUM_DIGITS)
    {
      finalBigint = newMaxBigint(finalSign);
      overflow = finalSign;
    }
    else
    {
      // finally do some actual multiplying!
      // prep the final bigint
      finalBigint = newBigint(0);
      // prep the values to be multiplied
      digits1 = bigint1.digits.slice(0, (numDigits1 - numZeroes1));
      digits2 = bigint2.digits.slice(0, (numDigits2 - numZeroes2));
      // determine how many zeroes to add to the end
      zeroesToAdd = numZeroes1 + numZeroes2;
      // prep to collect the sub-products
      subProducts = [];
      // loop through and multiply
      currZeroes = 0;
      for (i = digits2.length - 1; i >= 0; i--)
      {
        currDigit = digits2[i];
        currProd = multiplyDigitsBySingleDigit(digits1, currDigit);
        if ((currProd.length > 1) || (currProd[0] > 0))
          {currProd = incDigitsByFactor(currProd, currZeroes);}
        subProducts.push(currProd);
        currZeroes++;
      }
      // add all the sub-products, getting digits & overflow
      workResult = addDigitsToDigitsToDigits(subProducts);
      if (workResult.overflow)
      {
        finalBigint = getMaxBigint(finalSign);
        overflow = finalSign;
      }
      else
      {
        preZeroes = getNumEndingZeroes(workResult.digits);
        currTotal = incDigitsByFactor(workResult.digits, zeroesToAdd);
        if (currTotal.length > MAX_NUM_DIGITS)
        {
          finalBigint = getMaxBigint(finalSign);
          overflow = finalSign;
        }
      }
      // test for factor of ten
      isFactor = false;
      if (overflow == 0)
      {
        if ((currTotal.length == preZeroes + zeroesToAdd + 1) && (currTotal[0] == 1))
          {isFactor = true;}
      }
      // finalize new bigint
      if (overflow == 0)
      {
        finalBigint.isNeg = (finalSign < 0);
        finalBigint.numDigits = currTotal.length;
        finalBigint.numEndZeroes = preZeroes + zeroesToAdd;
        finalBigint.isTenFactor = isFactor;
        finalBigint.digits = currTotal;
      }
      
    } // end of quick check for overflow (before multiplying)
  } // end normal cases
  
  // return the final result in an object
  // with a flag indicating over/underflow
  finalObj.bigint = finalBigint;
  finalObj.overUnderFlow = overflow;
  return finalObj;
}


/*******
 *
 * Given two bigints, divides the first by the
 * second, returning the resultant bigint quotient,
 * ("quotient") alongside a bigint leftover
 * ("leftover") that will always be the same sign as
 * the quotient (or as the first bigint, if the
 * quotient is zero.) Note that if the second bigint
 * given is zero, a boolean FALSE is returned to
 * indicate error.
 *
 *******/
function DivideBigints(bigint1, bigint2)
{
  // initialize
  var numDigits1, numDigits2, numZeroes1, numZeroes2, digits, digits1, digits2;
  var i, comparison, newSize, quotient, remainder;
  var d, workObj, workDigits, nextDigitIndex, finalBigint, finalSign, finalObj = {};
  
  // determine final sign
  // (ignored if returning zero)
  if (bigint1.isNeg == bigint2.isNeg)
    {finalSign = 1;}
  else
    {finalSign = -1;}
  
  // easy edge cases first:
  numDigits1 = bigint1.numDigits;
  numDigits2 = bigint2.numDigits;
  numZeroes1 = bigint1.numEndZeroes;
  numZeroes2 = bigint2.numEndZeroes;
  // if the second bigint is zero
  if (numDigits2 == numZeroes2)
  {
    // cannot divide by zero; ERROR!
    return false;
  }
  
  // else if dividing zero by something
  else if (numDigits1 == numZeroes1)
  {
    finalBigint = newBigint(0);
    remainder = newBigint(0);
  }
  
  // else if dividing something by 1
  else if ((numDigits2 == 1) && (bigint2.digits[0] == 1))
  {
    finalBigint = CopyBigint(bigint1);
    remainder = newBigint(0);
    finalBigint.isNeg = (finalSign < 0);
  }
  
  // else if dividing something by a clearly bigger value
  else if (numDigits1 < numDigits2)
  {
    finalBigint = newBigint(0);
    remainder = CopyBigint(bigint1);
    remainder.isNeg = bigint1.isNeg;
  }
  
  // else we need to compare
  else
  {
    digits1 = bigint1.digits;
    digits2 = bigint2.digits;
    comparison = compareDigits(digits1, digits2);
    
    // easy edge case: if both numbers are the same
    if (comparison == 0)
    {
      finalBigint = newBigint(1);
      remainder = newBigint(0);
      finalBigint.isNeg = (finalSign < 0);
    }
    
    // easy edge case: dividing by a bigger value
    else if (comparison == -1)
    {
      finalBigint = newBigint(0);
      remainder = CopyBigint(bigint1);
      remainder.isNeg = bigint1.isNeg;
    }
    
    // easy edge case: if dividing by a factor of ten and you can lose the zeroes
    else if (bigint2.isTenFactor && (bigint1.numEndZeroes >= bigint2.numEndZeroes))
    {
      digits = digits1.slice(); // copies array
      newSize = numDigits1 - bigint2.numEndZeroes;
      digits.length = newSize;
      finalBigint = newBigint(0);
      remainder = newBigint(0);
      finalBigint.isNeg = (finalSign < 0);
      finalBigint.numDigits = newSize;
      finalBigint.numEndZeroes = numZeroes1 - numZeroes2;
      finalBigint.isTenFactor = bigint1.isTenFactor;
      finalBigint.digits = digits;
    }
    
    // else normal case
    else
    {
      // prep to build the quotient
      quotient = [];
      // setup initial params for long division loop
      nextDigitIndex = numDigits2 - 1;
      workDigits = digits1.slice(0, nextDigitIndex);
      // the long division loop
      do
      {
        // push the next digit onto work string,
        // but make sure we dont have leading zero
        if ((workDigits.length == 1) && (workDigits[0] == 0))
          {workDigits = [digits1[nextDigitIndex]];}
        else
          {workDigits.push(digits1[nextDigitIndex]);}
        // how many times (single digit) does bigint2 go into it?
        workObj = divideDigitsByDigitsForSingleDigitQuotient(workDigits, digits2);
        workDigits = workObj.remainder;
        d = workObj.quotient;
        // 
        if (d == 0)
          {if (quotient.length > 0) quotient.push(0);}
        else
          {quotient.push(d);}
        nextDigitIndex++;
      } while (nextDigitIndex < numDigits1);
      // quotient should be built; take
      // leftover working digits as remainder
      remainder = newBigint(0);
      remainder.isNeg = bigint1.isNeg;
      remainder.numDigits = workDigits.length;
      remainder.numEndZeroes = getNumEndingZeroes(workDigits);
      remainder.isTenFactor = ((workDigits[0] == 1) &&
        (remainder.numDigits == remainder.numEndZeroes + 1));
      remainder.digits = workDigits;
      finalBigint = newBigint(0);
      finalBigint.isNeg = (finalSign < 0);
      finalBigint.numDigits = quotient.length;
      finalBigint.numEndZeroes = getNumEndingZeroes(quotient);
      finalBigint.isTenFactor = ((quotient[0] == 1) &&
        (finalBigint.numDigits == finalBigint.numEndZeroes + 1));
      finalBigint.digits = quotient;
    }
  } // end of comparisons
  
  // prep and return the final result
  finalObj.quotient = finalBigint;
  finalObj.leftover = remainder;
  return finalObj;
}

