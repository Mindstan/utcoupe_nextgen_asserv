#include <boost/ut.hpp>

int main () {

    using namespace boost::ut;
        
    cfg<override> = {.tag = { "serial" }};
    
    return 0;
}
