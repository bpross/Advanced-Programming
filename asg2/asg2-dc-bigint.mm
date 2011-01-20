.so Tmac.mm-etc
.if t .Newcentury-fonts
.INITR* \n[.F]
.TITLE CMPS-109 Winter\~2010 Program\~2 "Overloading and memory mgmt"
.RCS "$Id: asg2-dc-bigint.mm,v 1.3 2011-01-19 14:13:37-08 - - $"
.PWD
.EQ
delim $$
.EN
.H 1 "Overview"
This assignment will involve overloading basic integer operators
to perform arbitrary precision integer arithmetic in the style of
.V= dc (1).
You will also perform explicit memory management using
.V= new
and
.V= delete ,
and eliminate memory leak.
Your class
.V= bigint
will intermix arbitrarily with simple
.V= int
arithmetic.
.P
To begin read the 
.V= man (1)
page for the command
.V= dc (1)\(::
.VCODE* 1 "man -s 1 dc" 
A copy of that page is also in this directory.
Your program will use the standard 
.V= dc
as a reference  implemention and must produce
.E= exactly
the same output for the commands you have to implement\(::
.VCODE* 1 "+\0 -\0 *\0 /\0 %\0 \[ha]\0 c\0 d\0 f\0 p\0 q\0"
You may also choose to print something with the debug function
.V= Y ,
but this is optional.
.H 1 "Implementation strategy"
As before, you have been given starter code.
.ALX a ()
.LI
.V= Makefile ,
.V= trace ,
and
.V= util
are similar to the previous program.
If you find you need a function which does not properly belong to
a given module, you may add it to
.V= util .
.LI
The module 
.V= scanner
reads in tokens, namely a
.V= NUMBER ,
an
.V= OPERATOR ,
or
.V= SCANEOF .
Each token returns a
.V= token_t ,
which indicates what kind of token it is
(the
.V= "terminal_symbol symbol" ),
and the
.V= "string lexinfo"
associated
with the token.
Only in the case of a number is there more than one character.
Note that on input, an underscore
.=V ( _ )
indicates a negative number.
The minus sign
.=V ( - )
is reserved only as a binary operator.
The scanner also has defined a couple of
.V= operator<<
for printing out scanner results in
.V= TRACE
mode.
.LI
The main program
.V= main.cc ,
has been implemented for you.
For the six binary arithmetic functions,
the right operand is popped from the stack,
then the left operand,
then the result is pushed onto the stack.
.LI
The module
.V= iterstack
makes use of the STL
.V= deque
(double ended queue).
We want to iterate from top to bottom,
and the STL
.V= stack
does not have an iterator.
In order for iteration to work in the correct order,
we push and pop from the front of the deque.
Note that
.V= deque.pop_front
does not return a value.
This class uses the term ``front'' instead of the more usual ``top''
when referring to the elements of a stack.
A
.V= deque
is more efficient than a 
.V= vector
since we do not need any direct access to the elements.
.LE
.H 1 "Class \f[CB]bigint\f[P]"
Then we come to the most complex part of the assignment,
namely the class
.V= bigint .
Operators in this class are heavily overloaded.
.ALX a ()
.LI
Note that most of the functions take a right argument of type
.V= "const bigint &" ,
that is a constant reference, for the sake of efficiency.
But they have to return the result by value.
.LI
We want all of the operators to be able to take either a
.V= bigint
or an
.V= int
as either the left or right operand.
.LI
When the left operand is an
.V= int ,
we make them non-member operators,
with two arguments, the left being an 
.V= int
and the right a
.V= bigint .
Note that the implementation of these functions,
.V= + ,
.V= = ,
.V= * ,
.V= / ,
.V= % ,
.V= == ,
.V= != ,
.V= <  ,
.V= <= ,
.V= > ,
.V= >= ,
are all identical, namely that they cast the
.V= int
to a
.V= bigint
and call the other operator.
.LI
The
.V= operator<<
can't be a member since its left operand is an
.V= ostream ,
so we make it a
.V= friend ,
so that it can see the innards of a
.V= bigint .
Note now
.V= dc
prints really big numbers.
.LI
The
.V= pow
function exponentiates in
.if n { O ( log sub 2 n ) }
.if t $ O ( log sub 2 n ) $
and need not be changed.
While it is a member of
.V= bigint ,
it behaves like a non-member, using only other functions.
.LI
The relational operators all trivially call a
.V= compare
function, which is private, and needs to be rewritten.
.LI
The
.V= /
and
.V= %
functions call
.V= divrem ,
which is private.
One can not produce a quotient without a remainder, 
and vice versa.
.LI
The operator
.V= *
and
.V= divrem
use the Ancient Egyptian multiplication and division algorithms,
which only need the ability to add and subtract,
and use a stack.
Presumably Pharaoh's engineers used this algorithm.
Fill in the missing code.
.LI
Finally,
the given implementation works for small integers,
but overflows for large integers.
.H 1 "Representation of a bigint"
Now we turn to the representation of a 
.V= bigint ,
which will be represented by a boolean flag and a vector of integers.
.ALX a ()
.LI
Replace the declaration
.VCODE* 1 "int small_value;"
with
.VCODE* 1 "typedef unsigned char digit_t;"
.VCODE* 1 "typedef vector <digit_t> bigvalue_t;"
.VCODE* 1 "bool negative;"
.VCODE* 1 "bigvalue_t *big_value;"
in 
.V= bigint.h .
.LI
In storing the long integer it is recommended that each digit in
the range 0 to 9 is kept in an element, 
although true
.V= dc (1)
stores two digits per byte.
But we are not concerned here with extreme efficiency.
Since the arithmetic operators add and subtract work from least
significant digit to most significant digit,
store the elements of the vector in the same order.
That means, for example, that the number $4622$ would be stored in
a vector $v$ as\(::
$v sub 3 = 4 $,
$v sub 2 = 6 $,
$v sub 1 = 2 $,
$v sub 0 = 2 $.
In other words,
if a digit's value is
$d times 10 sup k$,
then $v sub k = d$.
.LI
Then use 
.V= grep
or your editor's search function to find all of the occurrences of
.V= small_value .
Each of these occurrences needs to be replaced.
.LI
The representation of a number will be as follows\(::
.V= negative
is a flag which indicates the sign of the number\(;;
.V= big_value 
contains the digits of the number.
.LI
Change all of the constructors so that instead of initializing
.V= small_value ,
they initialize the replacement value.
.LI
The scanner will produce numbers as
.V= string s,
so scan each string four digits at a time,
using
.V= push_back
to append them to the vector.
.LI
Add two new private functions
.V= do_bigadd
and
.V= do_bigsub .
.LI
Change
.V= operator+
so that it compares the two numbers it gets.
If the signs are the same,
it calls
.V= do_bigadd
to add the vectors and keeps the sign as the result.
If the signs are different,
call
.V= abs_compare
to determine which one is larger,
and then call 
.V= do_bigsub
to subtract the larger minus the smaller.
Note that this is a different comparison function
which compares absolute values only.
Avoid duplicate code wherever possible.
.LI
The
.V= operator-
should perform similarly.
If the signs are different, it uses
.V= do_bigadd ,
but if the same, it uses
.V= do_bigsub .
.LI
To implement
.V= do_bigadd ,
create a new 
.V= bigvalue_t
and proceed from the low order end to the high order end,
adding digits pairwise.
If any sum is >=\~10000,
take the remainder and add the carry to the next digit.
Use
.V= push_back
to append the new digits to the
.V= bigvalue_t .
When you run out of digits in the shorter number,
continue, matching the longer vector with zeros,
until it is done.
Make sure the sign of 0 is positive.
.LI
To implement
.V= do_bigsub ,
also create a new empty vector, starting from the low order end
and continuing until the high end.
In this case, if the left number is smaller than the right number,
the subtraction will be less than zero.
In that case, add 10000, and set the borrow to the next number to \-1.
You are, of course, guaranteed here, that the left number is
at least as large as the right number.
After the algorithm is done,
.V= pop_back
all high order zeros from the vector before returning it.
Make sure the sign of 0 is positive.
.LI
To implement
.V= compare ,
return a value that is $< 0$, $= 0$, or $> 0$,
to show the relationship.
First check the signs.
If different,
you immediately know which inequality to return.
If the same, and for positive numbers,
the longer vector (with more digits) is greater than the shorter one.
If they are the same length,
start comparing digits from the (high-order) end
of the vector to the (low-order) front for a difference,
For negative numbers,
the smaller number is greater.
which tells you the inequality.
Otherwise return equal.
This assumes that vectors are stored in a canonical manner
without high-order zeros.
.LI
Modify
.V= operator<< ,
first just to print out the number all in one line.
You will need this to debug your program.
When you are finished,
make it print numbers in the same way as
.V= dc (1)
does\(::
.VCODE* 1 "% dc"
.VCODE* 1 "99999 40\[ha]p"
.VCODE* 1 \
99960007799012091383420303819357209019506959637410544273315009185275\(rs
.VCODE* 1 \
92834561433754989489897005216007253937448249716993988701209508329883\(rs
.VCODE* 1 \
0362070365779361889044985647838373419929138990120007799996000001
Note that it prints 68 digits per line starting from the high order
end of the vector.
.LE
.H 1 "Memory leak"
Make sure that you test your program completely so that it does not
crash on a Segmentation Fault or any other unexpected error.
Then implement the destructor
.V= ~bigint
so that there is no memory leak.
But if you don't have time to do this, remember that
memory leak is not as bad as a core dump.
.P
To check for memory leak, use
.V= dbx
interactively, but you can also test your program with
.V= "bcheck -all ydc"
.V= bcheck (1)
is a shell script that runs
.V= dbx (1).
Unfortunately on the Intel CPUs it is rather slow.
.H 1 "What to submit"
Submit source files and only source files\(::
.V= Makefile ,
.V= README ,
and all of the header and implementation files necessary to build
the target executable.
If
.V= gmake
does not build
.V= ydc 
your program can not be tested and you lose 1/2 of the points for
the assignment.
Use
.V= checksource
on  your code.
Use
.V= valgrind
to check for memory leaks.
.P
If you are doing pair programming,
follow the additional instructions in
.V= /afs/cats.ucsc.edu/courses/cmps012b-wm/Syllabus/pair-programming/ 
and also submit
.V= PARTNER .
.FINISH
