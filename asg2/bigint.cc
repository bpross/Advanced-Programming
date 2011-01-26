// $Id: bigint.cc,v 1.1 2011-01-25 17:04:51-08 - - $

#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>

using namespace std;

#include "bigint.h"
#include "trace.h"

bigint::bigint (): big_value (0) {
}

bigint::bigint (const bigint &that): big_value (that.big_value) {
   *this = that;
}

bigint &bigint::operator= (const bigint &that) {
   if (this == &that) return *this;
   this->big_value = that.big_value;
   return *this;
}

bigint::~bigint() {
   TRACE ('~', cout << *this);
}

bigint::bigint (int that): small_value (that) {
}

bigint::bigint (const string &that) {
   TRACE ('b', that);
   string::const_iterator itor = that.begin();
   string::const_iterator end = that.end();
   bool isnegative = false;
   if (*itor == '_') {isnegative = true; ++itor; }
   //bigvalue newval = 0;
   digit_t temp;
   for (; end != itor; --end){
     temp = *end;
     this->big_value->push_back(temp);
   } 
//   big_value = isnegative ? - newval : + newval;
}

bigint bigint::operator+ (const bigint &that) const {
  int comp = 0;
  int abs = 0;
  comp = this->compare(that);
  if(comp == 0){
    this->do_bigadd(that);
  }elseif(comp == -1){
    abs = this->abscompare(that);
  }elseif(comp == 1){
    abs = this->abscompare(that);
  }elseif(comp == 2){
    cout << "something is broken" << endl;
  }
  return *this;
//   return this->big_value + that.big_value;
}


bigint bigint::do_bigadd(const bigint &that) const{
   int smaller = 0;
   int abs = 0;
   int itor = 0;
   int itordigit = 0;
   int thisdigit = 0;
   int thatdigit = 0;
   int carry = 0;
   if(abs == 0 || abs == 1){
     smaller = that->big_value->size();
   }else if(abs == -1){
     smaller = this->big_value->size();
   }else{
     cout << "something's broken in bigadd" << endl;
   }
   for(itor = 0; itor <= smaller; itor++){
      thisdigit = this->big_value[itor];
      thisdigit -= 48;
      thatdigit = that.big_value[itor];
      thatdigit -= 48;
      itordigit = thisdigit + thatdigit;
      if(carry == 1) itordigit++;
      if(itordigit >= 10){
        carry = 1;
        itordigit -= 10;
      }else{
        carry = 0;
      }
      itordigit += 48;
      this->big_value[itor] = itordigit;
   }
   while(carry != 0){
     itor++;
     this->big_value[itor].
      
     carry--;
   }
}

bigint bigint::operator- (const bigint &that) const {
   return this->small_value - that.small_value;
}

bigint bigint::operator- () const {
   return -small_value;
}

int bigint::compare (const bigint &that) const {
  if(this->negative == true && that.negative == false){ 
    return -1;
  }
  if(this->negative == false && that.negative == true){
    return 1;
  }
  if(this->negative == true && that.negative == true){
    return 0;
  }
  if(this->negative == false && that.negative == false){
    return 0;
  }
  return 2;

//   return this->small_value < that.small_value ? -1
//        : this->small_value > that.small_value ? +1 : 0;
}

int bigint::abscompare (const bigint &that) const {
  if(this->big_value->size() < that.big_value->size() ){
    return -1;
  }
  if(this->big_value->size() > that.big_value->size() ){
    return 1;
  }
  if(this->big_value->size() == that.big_value->size() ){
    return 0;
  }
  return 2;
//   return abs (this->small_value) < abs (that.small_value) ? -1
//        : abs (this->small_value) > abs (that.small_value) ? +1 : 0;
}

int bigint::smallint () const {
   if (*this < numeric_limits<int>::min()
    || *this > numeric_limits<int>::max())
               throw range_error ("smallint: out of range");
   return small_value;
}

bigint bigint::mul_by_2 () {
   return this->small_value *= 2;
}

static bigpair popstack (stack <bigpair> &egyptstack) {
   bigpair result = egyptstack.top ();
   egyptstack.pop();
   return result;
}

//
// Ancient Egyptian multiplication algorithm.
//
bigint bigint::operator* (const bigint &that) const {
   bigint top = that;
   bigint count = 1;
   TRACE ('*', *this << " * " << that);
   stack <bigpair> egyptstack;
   popstack (egyptstack); // junk to suppress a warning
   bigint result = 0;
   if ((*this < 0) != (that < 0)) result = - result;
   return result;
}

//
// Ancient Egyptian division algorithm.
//
bigpair bigint::div_rem (const bigint &that) const {
   if (that == 0) throw range_error ("divide by 0");
   bigint count = 1;
   bigint top = abs (that.small_value);
   TRACE ('/', *this << " /% " << that);
   stack <bigpair> egyptstack;
   bigint quotient = 0;
   bigint remainder = abs (this->small_value);
   return bigpair (quotient, remainder);
}

bigint bigint::operator/ (const bigint &that) const {
   return div_rem (that).first;
}

bigint bigint::operator% (const bigint &that) const {
   return div_rem (that).second;
}

#define TRACE_POW \
   TRACE ('^', "result: " << result << ", base: " << base \
            << ", expt: " << expt);
bigint bigint::pow (const bigint &that) const {
   bigint base = *this;
   if (that > 999) throw range_error ("exp too big");
   int expt = that.smallint();
   bigint result = 1;
   TRACE_POW;
   if (expt < 0) {
      base = 1 / base;
      expt = - expt;
   }
   while (expt > 0) {
      TRACE_POW;
      if (expt & 1) { //odd
         result = result * base;
         --expt;
      }else { //even
         base = base * base;
         expt /= 2;
      }
   }
   TRACE_POW;
   return result;
}

//
// Macros can make repetitive code easier.
//

#define COMPARE(OPER) \
   bool bigint::operator OPER (const bigint &that) const { \
      return compare (that) OPER 0; \
   }
COMPARE (==)
COMPARE (!=)
COMPARE (< )
COMPARE (<=)
COMPARE (> )
COMPARE (>=)

#define INT_LEFT(RESULT,OPER) \
   RESULT operator OPER (int left, const bigint &that) { \
      return bigint (left) OPER that; \
   }
INT_LEFT (bigint, +)
INT_LEFT (bigint, -)
INT_LEFT (bigint, *)
INT_LEFT (bigint, /)
INT_LEFT (bigint, %)
INT_LEFT (bool, ==)
INT_LEFT (bool, !=)
INT_LEFT (bool, < )
INT_LEFT (bool, <=)
INT_LEFT (bool, > )
INT_LEFT (bool, >=)

ostream &operator<< (ostream &out, const bigint &that) {
   out << that.small_value;
   return out;
}

