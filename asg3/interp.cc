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
//   cout << "In do_define: " << params << endl;
   outfile << "%%Command[1]: define " << params << endl;
   TRACE ('i', params);
   string name = shift (params);
   objmap[name] = make_object (params);
}

void interpreter::do_draw (parameters &params) {
//   cout << "In do_draw: " << params << endl;
   TRACE ('i', params);
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
   TRACE ('f', command);
   return new ellipse (inches(0), inches(0), points(0));
}

object *interpreter::make_circle (parameters &command) {
   TRACE ('f', command);
   return new circle (inches(0), points(0));
}

object *interpreter::make_polygon (parameters &command) {
   TRACE ('f', command);
   return new polygon (coordlist(), points(0));
}

object *interpreter::make_rectangle (parameters &command) {
   TRACE ('f', command);
   return new rectangle (inches(0), inches(0), points(0));
}

object *interpreter::make_square (parameters &command) {
   TRACE ('f', command);
   return new square (inches(0), points(0));
}

object *interpreter::make_line (parameters &command) {
   TRACE ('f', command);
   return new line (inches(0), points(0));
}

