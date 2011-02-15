// $Id: interp.cc,v 1.1 2011-01-25 18:56:05-08 - - $
//bpross, esteggall

#include <list>
#include <map>
#include <string>

using namespace std;

#include "interp.h"
#include "object.h"
#include "util.h"

static int command_num = 1;

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
   outfile << "%%Command["<< command_num <<"]: define "
                                     << params << endl;
   TRACE ('i', params);
   string name = shift (params);
   objmap[name] = make_object (params);
   command_num += 1;
}

void interpreter::do_draw (parameters &params) {
   TRACE ('i', params);
   string name = shift (params);
   outfile << "%%Command["<< command_num <<"]: draw " << name << endl;
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
   command_num += 1;
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
   outfile << "0 746 moveto" << endl << "(" << infilename << ",p."
           << pagenr << ") show" << endl;
   outfile << "576 (" << datestring() << 
           ") stringwidth pop sub 746 moveto" <<
   endl << "(" << datestring() << ") show" << endl;

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
   string ft_size = shift (command);
   string fontname;
   string fontshit;
   double fontsize = 12;
   if (ft_size != "font"){ 
      fontsize = from_string<double>(ft_size);
      fontname = shift (command);
   }else{
      fontname = ft_size;
   }
   string textdata;
   string tempdata;
   while( command.size() > 0){
     tempdata = shift (command);
     textdata.append(tempdata);
   }
   return new text (string(fontname), points(fontsize), 
                    string(textdata) );
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
   TRACE ('f', command);
   return new ellipse (inches(height), inches(width), 
                       points(thickness));
}

object *interpreter::make_circle (parameters &command) {
   string strdiam = shift (command);
   double diameter = from_string<double>(strdiam);
   double thickness = 2;
   if (command.size() > 0){
      string strthick = shift (command);
      thickness = from_string<double>(strthick);
   }
   TRACE ('f', command);
   return new circle (inches(diameter), points(thickness));
}

object *interpreter::make_polygon (parameters &command) {
   coordlist coords;
   double point = 2;
   while(command.size() > 1){
     string strcoordx = shift (command);
     string strcoordy = shift (command);
     double coordx = from_string<double>(strcoordx);
     double coordy = from_string<double>(strcoordy);
     coordx *= 72;
     coordy *= 72;
     inches polyX(coordx);
     inches polyY(coordy);
     xycoords side(polyX, polyY);
     coords.push_back(side);
   }
   if(command.size() == 1){
      string strpoint = shift (command);
      point = from_string<double>(strpoint);
   }
   TRACE ('f', command);
   return new polygon (coordlist(coords), points(point));
}

object *interpreter::make_rectangle (parameters &command) {
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
   return new rectangle (inches(height), inches(width), 
                         points(thickness));

}

object *interpreter::make_square (parameters &command) {
   string strwidth = shift (command);
   double width = from_string<double>(strwidth);
   double thickness = 2;
   if (command.size() > 0){
     string strthick = shift (command);
     thickness = from_string<double>(strthick);
   }
   TRACE ('f', command);
   return new square (inches(width), points(thickness));
}

object *interpreter::make_line (parameters &command) {
   string strlength = shift (command);
   double length = from_string<double>(strlength);
   double thickness = 2;
   if( command.size() > 0){
      string strthick = shift (command);
      thickness = from_string<double>(strthick);
   }
   TRACE ('f', command);
   return new line (inches(length), points(thickness));
}

