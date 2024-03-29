#include <fstream>

namespace tape_sorter {
namespace tape {
    template <typename elem_t = int>
        requires std::is_arithmetic_v<elem_t>
    class tape final{
        
        std::fstream file;

    public:
        tape(std::string& filename) : file(filename, std::ios::binary | std::ios::in | std::ios::out) {}

        void rewind(); 
        elem_t read();
        void write(elem_t val);
        void go_next();
        void go_prev();

    };
   
} //<-- tape
} //<-- tape_sorter