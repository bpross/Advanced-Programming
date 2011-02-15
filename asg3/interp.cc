// $Id: interp.cc,v 1.1 2011-01-25 18:56:05-08 - - $

#include <list>
#include <map>
#include <string>

using namespace std;

#include "interp.h"
#include "object.h"
#include "util.h"

interpreter::interpreter(const string &filename, ostream &outfile,
                         objectmap &objmap):
   outfile(outfile), pagenr(1), objmap(objmap), infilename(filename),
   page_xoffset (inches (.25)), page_yoffset (inches (.25)) {
   if (interpmap.size() == 0) {
      interpmap["define" ] = &interpreter::do_define ;
      interpmap["draw"   ] = &interpreter::do_draw   ;
      interpmap["newpage"] = &interpreter::do_newpage;
   }
   if (factorymap.size() == 0) {
      factorymap["text"     ] = &interpreter::make_text     ;
      factorymap["ellipse"  ] = &interpreter::make_ellipse  ;
      factorymap["circle"   ] = &interpreter::make_circle   ;
      factorymap["polygon"  ] = &interpreter::make_polygon  ;
      factorymap["rectangle"] = &interpreter::make_rectangle;
      factorymap["square"   ] = &interpreter::make_square   ;
      factorymap["line"     ] = &interpreter::make_line     ;
   }
   prolog ();
   startpage ();
}

interpreter::~interpreter () {
   endpage ();
   epilog ();
}

map <string, interpreter::interpreterfn> interpreter::interpmap;
map <string, interpreter::factoryfn> interpreter::factorymap;

string shift (list<string> &words) {
   if (words.size() == 0) throw runtime_error ("syntax error");
   string front = words.front();
   words.pop_front();
   return front;
}

void interpreter::interpret (parameters &params) {
   TRACE ('i', params);
   string command = shift (params);
   interpreterfn function = interpmap[command];
   if (function == NULL) throw runtime_error ("syntax error");
   (this->*function) (params);
}

void interpreter::do_define (parameters &params) {
   cout << "In do_define: " << params << endl;
   outfile << "%%Command[1]: define " << params << endl;
   TRACE ('i', params);
   string name = shift (params);
   objmap[name] = make_object (params);
}

void interpreter::do_draw (parameters &params) {
//   cout << "In do_draw: " << params << endl;
   TRACE ('i', params);
   cout << "PARAMS SIZE: " << params.size() << endl;
   string name = shift (params);
   cout << "%%Command[1]: draw " << params << endl;
   outfile << "%%Command[1]: draw " << name << endl;
   object *thing = objmap[name];
   if (thing == NULL) throw runtime_error (name + ": no such object");
   degrees angle = degrees (0);
   if (params.size() == 3) {
      angle = degrees (from_string<double> (params.back()));
      params.pop_back();
   }
   if (params.size() != 2) throw runtime_error ("syntax error");
   xycoords coords (inches (from_string<double> (params.front())),
                    inches (from_string<double> (params.back())));
   thing->draw (outfile, coords, angle);
}

void interpreter::do_newpage (parameters &params) {
   if (params.size() != 0) throw runtime_error ("syntax error");
   endpage ();
   ++pagenr;
   startpage ();
}

void interpreter::prolog () {
   outfile << "%!PS-Adobe-3.0" << endl;
   outfile << "%%Creator: " << sys_info::get_execname () << endl;
   outfile << "%%CreationDate: " << datestring() << endl;
   outfile << "%%PageOrder: Ascend" << endl;
   outfile << "%%Orientation: Portrait" << endl;
   outfile << "%%SourceFile: " << infilename << endl;
   outfile << "%%EndComments" << endl;
}

void interpreter::startpage () {
   outfile << endl;
   outfile << "%%Page: " << pagenr << " " << pagenr << endl;
   outfile << page_xoffset << " " << page_yoffset
           << " translate" << endl;
   outfile << "/Courier findfont 10 scalefont setfont" << endl;
   outfile << "0 0 moveto" << endl << " (" << infilename << ":"
           << pagenr << ") show" << endl;

}

void interpreter::endpage () {
   outfile << endl;
   outfile << "showpage" << endl;
   outfile << "grestoreall" << endl;
}

void interpreter::epilog () {
   outfile << endl;
   outfile << "%%Trailer" << endl;
   outfile << "%%Pages: " << pagenr << endl;
   outfile << "%%EOF" << endl;

}

object *interpreter::make_object (parameters &command) {
   TRACE ('f', command);
   string type = shift (command);
   factoryfn func = factorymap[type];
   if (func == NULL) throw runtime_error (type + ": no such object");
   return (this->*func) (command);
}

object *interpreter::make_text (parameters &command) {
   TRACE ('f', command);
   const string size = shift (command);
   double fontsize = from_string<double>(size);
   string fontname = shift (command);
   string textdata = shift (command);
   return new text (string(fontname), points(fontsize), string(textdata) );
}

object *interpreter::make_ellipse (parameters &command) {
   string strheight = shift (command);
   string strwidth = shift (command);
   double height = from_string<double>(strheight);
   double width = from_string<double>(strwidth);
   double thickness = 2;
   if (command.size() > 0){
     string strthick = shift (command);
     thickness = from_string<double>(strthick);
   }
   cout << "In make elipse: command = " << command << endl;
   TRACE ('f', command);
   return new ellipse (inches(height), inches(width), points(thickness));
}

object *interpreter::make_circle (parameters &command) {
   cout << "In make circle: command = " << command << endl;
   string strdiam = shift (command);
   double diameter = from_string<double>(strdiam);
   string strthick = shift (command);
   double thickness = from_string<double>(strthick);
   
   TRACE ('f', command);
   return new circle (inches(diameter), points(thickness));
}

object *interpreter::make_polygon (parameters &command) {
   cout << "In make polygon: command = " << command << endl;
   coordlist coords;
   double point = 2;
   while(command.size() > 1){
     string strcoordx = shift (command);
     string strcoordy = shift (command);
     double coordx = from_string<double>(strcoordx);
     double coordy = from_string<double>(strcoordy);
     coordx *= 72;
     coordy *= 72;
     cout << "x " << strcoordx << "y " << strcoordy << endl;
     inches polyX(coordx);
     inches polyY(coordy);
     xycoords side(polyX, polyY);
     coords.push_back(side);
   }
   if(command.size() == 1){
      string strpoint = shift (command);
      cout << "strpoint = " << strpoint << endl;
      point = from_string<double>(strpoint);
   }
   TRACE ('f', command);
   return new polygon (coordlist(coords), points(point));
}

object *interpreter::make_rectangle (parameters &command) {
   cout << "In make rectangle: command = " << command << endl;
   string strheight = shift (command);
   string strwidth = shift (command);
   double thickness = 2;
   if (command.size() > 0){
      string strthick = shift (command);
      thickness = from_string<double>(strthick);
   }
   double height = from_string<double>(strheight);
   double width = from_string<double>(strwidth);
   TRACE ('f', command);
   return new rectangle (inches(height), inches(width), points(thickness));

}

object *interpreter::make_square (parameters &command) {
   string strwidth = shift (command);
   double width = from_string<double>(strwidth);
   double thickness = 2;
   if (command.size() > 0){
     string strthick = shift (command);
     thickness = from_string<double>(strthick);
   }
   cout << "In make square: command = " << command << endl;
   TRACE ('f', command);
   return new square (inches(width), points(thickness));
}

object *interpreter::make_line (parameters &command) {
   string strlength = shift (command);
   string strthick = shift (command);
   double length = from_string<double>(strlength);
   double thickness = from_string<double>(strthick);
   cout << "In make line: command = " << command << endl;
   TRACE ('f', command);
   return new line (inches(length), points(thickness));
}

