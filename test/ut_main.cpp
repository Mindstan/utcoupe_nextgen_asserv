#include <boost/ut.hpp>

int main () {

    using namespace boost::ut;
        
    cfg<override> = {.tag = { "serial", "state-machine", "utilities"} }; // CppCheck false positive, value is indeed used!
    
    return 0;
}
