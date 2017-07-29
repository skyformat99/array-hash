#define BOOST_TEST_DYN_LINK

#include <tuple>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include "array_set.h"
#include "utils.h"


BOOST_AUTO_TEST_SUITE(test_array_set)


using test_types = boost::mpl::list<
                        tsl::array_set<char>,
                        tsl::array_set<wchar_t>,
                        tsl::array_set<char16_t>,
                        tsl::array_set<char32_t>,
                        tsl::array_set<char, tsl::str_hash_ah<char>, tsl::str_equal_ah<char>, false>,
                        tsl::array_set<wchar_t, tsl::str_hash_ah<wchar_t>, tsl::str_equal_ah<wchar_t>, false>,
                        tsl::array_set<char16_t, tsl::str_hash_ah<char16_t>, tsl::str_equal_ah<char16_t>, false>,
                        tsl::array_set<char32_t, tsl::str_hash_ah<char32_t>, tsl::str_equal_ah<char32_t>, false>
                    >;


/**
 * insert
 */                                      
BOOST_AUTO_TEST_CASE_TEMPLATE(test_insert, ASet, test_types) {
    // insert x values, insert them again, check values
    using char_tt = typename ASet::char_type;
    
    
    const size_t nb_values = 1000;
    ASet map;
    typename ASet::iterator it;
    bool inserted;
    
    for(size_t i = 0; i < nb_values; i++) {
        const auto key = utils::get_key<char_tt>(i);
        std::tie(it, inserted) = map.insert(key);
        
        BOOST_CHECK(map.key_eq()(it.key(), it.key_size(), key.c_str(), key.size()));
        BOOST_CHECK(inserted);
    }
    BOOST_CHECK_EQUAL(map.size(), nb_values);
    
    for(size_t i = 0; i < nb_values; i++) {
        const auto key = utils::get_key<char_tt>(i);
        std::tie(it, inserted) = map.insert(key);
        
        BOOST_CHECK(map.key_eq()(it.key(), it.key_size(), key.c_str(), key.size()));
        BOOST_CHECK(!inserted);
    }
    
    for(size_t i = 0; i < nb_values; i++) {
        const auto key = utils::get_key<char_tt>(i);
        it = map.find(key);
        
        BOOST_CHECK(it != map.end());
        BOOST_CHECK(map.key_eq()(it.key(), it.key_size(), key.c_str(), key.size()));
    }
}

BOOST_AUTO_TEST_CASE(test_insert_more_than_max_size) {
    tsl::array_set<char, tsl::str_hash_ah<char>, tsl::str_equal_ah<char>, true, 
                   std::uint16_t, std::uint8_t> map;
    for(std::size_t i=0; i < map.max_size(); i++) {
        map.insert(utils::get_key<char>(i));
    }
    
    BOOST_CHECK_EQUAL(map.size(), map.max_size());
    BOOST_CHECK_THROW(map.insert(utils::get_key<char>(map.max_size())), 
                      std::length_error);
}

BOOST_AUTO_TEST_CASE(test_insert_with_too_long_string) {
    tsl::array_set<char, tsl::str_hash_ah<char>, tsl::str_equal_ah<char>, true,
                   std::uint8_t, std::uint16_t> map;
    for(std::size_t i=0; i < 10; i++) {
        map.insert(utils::get_key<char>(i));
    }
    
    const std::string long_string("a", map.max_key_size());
    BOOST_CHECK(map.insert(long_string).second);
    
    const std::string too_long_string("a", map.max_key_size() + 1);
    BOOST_CHECK_THROW(map.insert(too_long_string), std::length_error);
}

BOOST_AUTO_TEST_SUITE_END()
