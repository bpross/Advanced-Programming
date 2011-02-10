// $Id: numbers.cc,v 1.1 2011-01-25 18:56:05-08 - - $

#include <cstdlib>

using namespace std;

#include "numbers.h"
#include "util.h"

ostream &operator<< (ostream &out, const degrees &that) {
   out << that.angle << "deg";
   return out;
}

ostream &operator<< (ostream &out, const points &that) {
   out << that.pointvalue << "pt";
   return out;
}

ostream &operator<< (ostream &out, const inches &that) {
   out << that.pointvalue / PTS_PER_INCH << "in";
   return out;
}

ostream &operator<< (ostream &out, const xycoords &coords) {
   out << "(" << coords.first << "," << coords.second << ")";
   return out;
}

