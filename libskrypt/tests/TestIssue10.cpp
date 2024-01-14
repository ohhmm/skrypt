#define BOOST_TEST_MODULE skryptissue10 test
#include <boost/test/unit_test.hpp>
#include "skrypt.h"

using namespace ::skrypt;

BOOST_AUTO_TEST_CASE(skryptissue10test){
    Skrypt skrypt;
    skrypt.Load(TEST_SRC_DIR "10.skrypt");
}
