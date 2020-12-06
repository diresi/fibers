#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>

#include <boost/fiber/all.hpp>
#include <boost/random.hpp>

boost::random::mt19937 rng;
boost::random::uniform_int_distribution<> six(1,6);

using namespace std;
namespace bf = boost::fibers;

void print_(ostream& out) {
    out << '\n';
}

template < typename T, typename... Ts >
void print_(ostream& out, T const& arg, Ts const&... args) {
    out << arg;
    print_(out, args...);
}

template < typename... T >
void print(T const&... args) {
    ostringstream buffer;
    print_( buffer, args...);
    cout << buffer.str() << flush;
}

struct test_t {
    uint32_t v;
    test_t(uint32_t v)
        :v(v)
    {}
};

ostream& operator<<(ostream& out, const test_t& t) {
    out << "T[" << t.v << "]";
    return out;
}

struct _W_t {
    void operator()() {
        for (int i=0; i<3; ++i) {
            int tmo = six(rng);
            chrono::seconds d(tmo);
            print(boost::this_fiber::get_id(), ": hi + ", tmo);
            boost::this_fiber::sleep_for(d);
        }
    }
};

void test() {
    auto f1 = bf::fiber(_W_t());
    auto f2 = bf::fiber(_W_t());
    f1.join();
    f2.join();
}

int main(int argc, char* argv[]) {

    vector<test_t> v;
    v.emplace_back(test_t(1));
    v.emplace_back(test_t(2));
    v.emplace_back(test_t(3));

    for(const auto& e : v) {
        print(e);
    }

    test();

    return 0;
}
