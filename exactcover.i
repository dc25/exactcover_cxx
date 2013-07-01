/* exactcover.i */
%module exactcover
%{
/* Put header files here or function declarations like below */
#include "Coverings.h"
%}

%include "std_vector.i"
%include "std_string.i"
%include "windows.i"

namespace std {
%template(stringVector)  vector < string >;
%template(stringArray)  vector< vector < string > >;
%template(intVector)  vector < int >;
%template(intArray) vector < vector < int> >;
}   

%include "Coverings.h"

