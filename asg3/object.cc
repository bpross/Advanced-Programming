// $Id: object.cc,v 1.1 2011-01-25 18:56:05-08 - - $

#include <typeinfo>

using namespace std;

#include "object.h"
#include "util.h"

#define WHOAMI \
        "[" << typeid(*this).name() << "@" << (void *) this << "]"

#define CTRACE(ARGS) \
        TRACE ('c', WHOAMI << " " << ARGS)

#define DTRACE(ARGS) \
        TRACE ('d', WHOAMI << " coords=" << coords \
               << " angle=" << angle << endl << ARGS);

object::~object () {
   CTRACE ("delete");
}

text::text (const string &font, const points &size, const string &data):
      fontname(font), fontsize(size), textdata(data) {
   CTRACE ("font=" << fontname << " size=" << fontsize
           << " \"" << textdata << "\"")
}

ellipse::ellipse (const inches &initheight, const inches &initwidth,
                  const points &initthick):
      shape(initthick), height(initheight), width(initwidth) {
   CTRACE ("height=" << height << " width=" << width
           << " thick=" << thick);
}

circle::circle (const inches &diameter, const points &thick):
      ellipse (diameter, diameter, thick) {
}

polygon::polygon (const coordlist &coords, const points &initthick):
      shape(initthick), coordinates(coords) {
   CTRACE ( "thick=" << thick << " coords=" << endl
            << coordinates);
}

rectangle::rectangle (const inches &height, const inches &width,
                      const points &initthick):
      polygon (make_list (height, width), initthick) {
}

square::square (const inches &width, const points &thick):
      rectangle (width, width, thick) {
}

line::line (const inches &length, const points &initthick):
      polygon (make_list (length), initthick) {
      
}

void text::draw (ostream &out, const xycoords &coords,
                const degrees &angle) {
   DTRACE ("font=" << fontname << " size=" << fontsize
           << " \"" << textdata << "\"")
   double first = double(coords.first);
   double second = double(coords.second);
   cout << "font_size: " << fontsize << " font: " << fontname << " writing: " << textdata << endl;
   cout << "xycoords: " << coords << " degrees: " << angle << endl;
   out << "gsave" << endl;
   out << "    /" << fontname << " findfont" << endl;
   out << "    " << fontsize << " scalefont setfont" << endl;
   out << "    " << first << " " << second << " translate" << endl << "    0 rotate" << endl;
   out << "    0 0 moveto" << endl;
   out << "    (";
   unsigned int itor;
   for(itor = 0; itor < textdata.size(); ++itor){
       out << "\\" << textdata.at(itor);
   }
   out << ")" << endl;
   out << "    show" << endl;
   out << "grestore" << endl;
}

void ellipse::draw (ostream &out, const xycoords &coords,
                const degrees &angle) {
   out << " Hello" << endl;
   DTRACE ("height=" << height << " width=" << width
           << " thick=" << thick);
   double first = double(coords.first);
   double second = double(coords.second);
   cout << "first = " << first << "second = " << second << endl;
   cout << "angle = " << angle << endl;
   out << "gsave" << endl;
   out << "   newpath" << endl;
   out << "   /save matrix currentmatrix def" << endl;
   out << "   " << first << " " << second << " traslate" << endl;

   list<xycoords>::const_iterator itor;
   for(itor = coordinates.begin(); itor != coordinates.end();++itor){
     cout << "   " << *itor << " rlineto " << endl;
   }

   out << "   " << angle << " rotate" << endl;
   out << "   " << thick <<  " setlinewidth" << endl;
   out << "   stroke" << endl;
   out << "grestore" << endl;
   
}

void polygon::draw (ostream &out, const xycoords &coords,
                const degrees &angle) {
   DTRACE ( "thick=" << thick << " coords=" << endl
            << coordinates);
   out << "gsave" << endl;
   out << "   newpath" << endl;
   double first = double(coords.first);
   double second = double(coords.second);
   out << "   " << first << " " << second << " translate" << endl;
   out << "   " << angle << " rotate" << endl;
   out << "   0 0 moveto" << endl;
   list<xycoords>::const_iterator itor;
   for(itor = coordinates.begin(); itor != coordinates.end();++itor){
     out << "   " << *itor << " rlineto " << endl;
   }
   out << "   closepath" << endl;
   out << "   " << thick <<  " setlinewidth" << endl;
   out << "   stroke" << endl;
   out << "grestore" << endl;
   
}

coordlist rectangle::make_list (
            const inches &height, const inches &width) {
   coordlist coordlist; 
   double rect_height_d = double(height);
   double rect_width_d = double(width);
   inches rect_height(rect_height_d);
   inches rect_width(rect_width_d);
   inches rect_height_neg(-rect_height_d);
   inches zero(0);
   xycoords side1(zero, rect_height);
   xycoords side2(rect_width, zero);
   xycoords side3(zero, rect_height_neg);
   coordlist.push_back(side1);
   coordlist.push_back(side2);
   coordlist.push_back(side3);
   return coordlist;
}

coordlist line::make_list (const inches &length) {
   coordlist coordlist;
   double line_length = double(length);
   inches start(0);
   inches end(line_length);
   xycoords linecoord(end, start);
   coordlist.push_back(linecoord);
   cout << length << endl;
   return coordlist;
}

