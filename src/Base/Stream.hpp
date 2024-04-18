#pragma once

namespace sb {
    template<class ostream, class T>
    ostream& write(ostream& out, const T& value) {
        out.write(reinterpret_cast<const char*>(&value), sizeof(T));
        return out;
    }
    template<class istream, class T>
    istream& read(istream& in, T& value) {
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        return in;
    }
} // namespace sb