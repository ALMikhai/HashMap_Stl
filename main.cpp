#define CATCH_CONFIG_MAIN
#include "hash_map.hpp"
#include "catch2/catch.hpp"
#include <string>
#include <iostream>
#include <cmath>

using namespace fefu;
using namespace std;

TEST_CASE("Functions") {
  SECTION("merge &&") {
    hash_map<char, string> map(10);
    hash_map<char, string> tmp({pair('!', "asd"), pair('@', "kmf")}, 10);
    map.merge(tmp);
    CHECK(map['!'] == "asd");
    CHECK(map['@'] == "kmf");
    CHECK(map.size() == 2);
  }

  SECTION("insert") {
    hash_map<string, int32_t> hash_map(11);

    pair<string, int> p = pair("123", 123);
    auto res = hash_map.insert(p);
    CHECK(123 == res.first->second); // returned iterator.
    CHECK(res.second); // element inserted.
    pair<string, int> p1 = pair("123", 321);
    auto res1 = hash_map.insert(p1);
    CHECK(!res1.second); // element stored.

    hash_map.insert(make_pair("321", 123));
    CHECK(hash_map["321"] == 123);
  }

  SECTION("bucket-count") {
    hash_map<size_t , size_t> hash_map(10);
    CHECK(hash_map.bucket_count() == 10);
  }

  SECTION("load-factory") {
    hash_map<size_t , size_t> hash_map(10);
    size_t count = 3;
    for (size_t i = 0; i < count; i++) {
      pair<size_t , size_t > p = pair(i, i);
      hash_map.insert(p);
    }
    hash_map.max_load_factor(0.5);
    CHECK(hash_map.max_load_factor() == 0.5);
    CHECK(hash_map.load_factor() == 0.3f);
  }

  SECTION("iterator ++"){
    hash_map<char , string> hash_map(100);
    hash_map.insert(std::pair('0', "abc"));
    hash_map.insert(std::pair('1', "zxc"));
    hash_map.insert(std::pair('2', "klj"));

    auto iterator = hash_map.begin();
    CHECK("abc" == iterator->second);
    CHECK("zxc" == iterator.operator++()->second);
    CHECK("klj" == iterator.operator++()->second);
  }

  SECTION("[]"){
    hash_map<size_t , size_t> hash_map(10);
    hash_map[0] = 123;
    CHECK(hash_map[0] == 123);
    CHECK(hash_map[1] == 0);
    hash_map[1] = 1000;
    CHECK(hash_map[1] == 1000);
  }

 SECTION("contains"){
    hash_map<size_t , size_t> hash_map(10);
    CHECK(!hash_map.contains(0));
    hash_map.insert(std::pair(0, 123));
    CHECK(hash_map.contains(0));
 }

  SECTION("count"){
    hash_map<size_t , size_t> hash_map(10);
    CHECK(hash_map.count(0) == 0);
    hash_map.insert(std::pair(0, 123));
    CHECK(hash_map.count(0) == 1);
  }

  SECTION("find") {
    hash_map<size_t , size_t> hash_map(10);
    CHECK(hash_map.find(0) == hash_map.end());
    hash_map.insert(std::pair(0, 123));
    CHECK(hash_map.find(0)->second == 123);
  }

  SECTION("hash, equal"){
    hash_map<size_t , size_t> hash_map(10);
    hash_map.hash_function();
    hash_map.key_eq();
  }

  SECTION("erase (iterator)"){
    hash_map<string, int32_t> hash_map(10);
    pair<string, int> p = pair("123", 123);
    hash_map.insert(p);
    auto iterator = hash_map.find("123");
    CHECK(iterator->second == 123);
    auto res = hash_map.erase(iterator);
    CHECK(res == hash_map.end());
  }

  SECTION("erase (key)") {
    hash_map<string, int32_t> hash_map(10);
    pair<string, int> p = pair("123", 123);
    hash_map.insert(p);
    auto res = hash_map.erase("123");
    CHECK(res == 1);
    res = hash_map.erase("123");
    CHECK(res == 0);
  }

  SECTION("clear") {
    hash_map<char, string> hash_map(100);
    hash_map.insert(std::pair('0', "abc"));
    hash_map.insert(std::pair('1', "zxc"));
    hash_map.insert(std::pair('2', "klj"));

    CHECK(hash_map.find('2')->second == "klj");
    CHECK(hash_map.size() == 3);

    hash_map.clear();
    CHECK(hash_map.find('0') == hash_map.end());
    CHECK(hash_map.find('1') == hash_map.end());
    CHECK(hash_map.find('2') == hash_map.end());
    CHECK(hash_map.size() == 0);
    CHECK(hash_map.empty());
  }

  SECTION("insert_or_assign &"){
    hash_map<string, int32_t> hash_map(10);
    string key = "123";
    auto res1 = hash_map.insert_or_assign<int32_t>(key, 123);
    CHECK(res1.second);
    auto res2 = hash_map.insert_or_assign<int32_t>(key, 666);
    CHECK(!res2.second);
    CHECK(hash_map[key] == 666);
  }

  SECTION("insert_or_assign &&"){
    hash_map<string, int32_t> hash_map(10);
    auto res1 = hash_map.insert_or_assign<int32_t>("123", 123);
    CHECK(res1.second);
    auto res2 = hash_map.insert_or_assign<int32_t>("123", 666);
    CHECK(!res2.second);
    CHECK(hash_map["123"] == 666);
  }


  SECTION("begin == end") {
    hash_map<string, int32_t> hash_map(10);
    CHECK(hash_map.begin() == hash_map.end());
    CHECK(hash_map.cbegin() == hash_map.cend());
  }

  SECTION("const iterator ++"){
    hash_map<char , string> hash_map(100);
    hash_map.insert(std::pair('0', "abc"));
    hash_map.insert(std::pair('1', "zxc"));
    hash_map.insert(std::pair('2', "klj"));

    auto iterator = hash_map.cbegin();
    CHECK("abc" == iterator->second);
    ++iterator;
    CHECK("zxc" == iterator->second);
    ++iterator;
    CHECK("klj" == iterator->second);
  }

  SECTION("try emplace") {
    hash_map<char , string> hash_map(100);
    hash_map.try_emplace('a', "123");

    CHECK(hash_map['a'] == "123");
    CHECK(!hash_map.try_emplace('a', "321").second);

    char key = 'b';
    CHECK(hash_map.try_emplace(key, "321").second);
    CHECK(hash_map[key] == "321");
  }

  SECTION("erase (const_iterator)"){
    hash_map<string, int32_t> hash_map(10);
    pair<string, int> p = pair("123", 123);
    hash_map.insert(p);
    auto iterator = hash_map.find("123");
    CHECK(iterator->second == 123);
    auto res = hash_map.erase(iterator);
    CHECK(res == hash_map.end());
  }

  SECTION("insert on iterators") {
    vector<pair<char, string>> vec = {pair('0', "123"), pair('1', "234"), pair('2', "345")};
    hash_map<char, string> hash_map(10);
    hash_map.insert(vec.begin(), vec.end());

    CHECK(hash_map['0'] == "123");
    CHECK(hash_map.size() == 3);
  }

  SECTION("erase (first, last)") {
    hash_map<char , string> hash_map(100);
    hash_map.insert(std::pair('0', "abc"));
    hash_map.insert(std::pair('1', "zxc"));
    hash_map.insert(std::pair('2', "klj"));

    hash_map.erase(hash_map.cbegin(), hash_map.cend());
    CHECK(hash_map.find('0') == hash_map.end());
    CHECK(hash_map.size() == 0);
  }

  SECTION("megre &") {
    hash_map<char, string> map(10);
    map.insert(std::pair('0', "abc"));
    map.insert(std::pair('1', "zxc"));
    map.insert(std::pair('2', "klj"));

    hash_map<char, string> tmp(100);
    tmp.insert(std::pair('3', "654"));
    tmp.insert(std::pair('4', "678"));
    tmp.insert(std::pair('2', "10001`3812"));

    map.merge(tmp);

    CHECK(map['3'] == "654");
    CHECK(map['4'] == "678");
    CHECK(map['2'] == "klj");
    CHECK(tmp['2'] == "10001`3812");
    CHECK(tmp.size() == 1);
    CHECK(map.size() == 5);
  }

  SECTION("emplace") {
    hash_map<char, string> map(5);
    map.emplace('0', "123");

    CHECK(map['0'] == "123");
    CHECK(map.size() == 1);
  }

  SECTION("hash_map allocator") {
    hash_map<char, string> map(hash_map<char, string>(10).get_allocator());
    map.emplace('0', "123");
    CHECK(map['0'] == "123");
  }

  SECTION("at") {
    hash_map<char, string> map(10);
    map.insert(std::pair('0', "abc"));
    map.insert(std::pair('1', "zxc"));
    map.insert(std::pair('2', "klj"));

    CHECK(map.at('0') == "abc");
  }

  SECTION("==") {
    hash_map<char, string> map(10);
    map.insert(std::pair('0', "abc"));
    map.insert(std::pair('1', "zxc"));
    map.insert(std::pair('2', "klj"));

    hash_map<char, string> map1(10);
    map1.insert(std::pair('0', "abc"));
    map1.insert(std::pair('1', "zxc"));
    map1.insert(std::pair('2', "klj"));

    CHECK(map == map1);

    map1.emplace('r', "3123");
    CHECK(!(map == map1));
    map.emplace('z', "asdfa");
    CHECK(!(map == map1));
  }
  SECTION("map = map2") {
    hash_map<int, char> map;
    hash_map<int, char> map2({std::make_pair(0, '0'), std::make_pair(1, '1'), std::make_pair(100, '@')}, 100);

    map = map2;

    CHECK(map[0] == '0');
    CHECK(map[1] == '1');
    CHECK(map[100] == '@');
    CHECK(map.size() == 3);
  }

  SECTION("map = &&map2") {
    hash_map<int, char> map;

    map = hash_map<int, char>({std::make_pair(0, '0'), std::make_pair(1, '1'), std::make_pair(100, '@')});;

    CHECK(map[0] == '0');
    CHECK(map[1] == '1');
    CHECK(map[100] == '@');
    CHECK(map.size() == 3);
  }

  SECTION ("hash_map copy") {
    hash_map<char , string> hash_map1(100);
    hash_map1.insert(std::pair('0', "abc"));
    hash_map1.insert(std::pair('1', "zxc"));
    hash_map1.insert(std::pair('2', "klj"));

    hash_map<char, string> hash_map_copy(hash_map1);

    CHECK(hash_map_copy['0'] == "abc");
    CHECK(hash_map1.size() == hash_map_copy.size());
    CHECK(hash_map1.bucket_count() == hash_map_copy.bucket_count());
  }

  SECTION ("hash_map copy") {
    hash_map<char, string> hash_map_copy(hash_map<char, string>({std::pair('0', "abc"), std::pair('1', "zxc"), std::pair('2', "klj")}));

    CHECK(hash_map_copy['0'] == "abc");
    CHECK(hash_map_copy['1'] == "zxc");
    CHECK(hash_map_copy['2'] == "klj");
    CHECK(hash_map_copy.size() == 3);
  }

  SECTION("reserve") {
    hash_map<char, string> hash_map(10);
    hash_map.insert(std::pair('0', "abc"));
    hash_map.insert(std::pair('1', "zxc"));
    hash_map.insert(std::pair('2', "klj"));

    hash_map.reserve(15);
    CHECK(hash_map.bucket_count() == ceil(15 / hash_map.max_load_factor()));
    CHECK(hash_map['0'] == "abc");
    CHECK(hash_map['1'] == "zxc");
    CHECK(hash_map['2'] == "klj");

    hash_map.reserve(1000);
  }

  SECTION("default map") {
    hash_map<int, char> m;
    m.emplace(1, '2');
    m.emplace(2, '3');

    CHECK(m[1] == '2');
    CHECK(m[2] == '3');
  }

  SECTION("begin end") {
    hash_map<int, char> m({make_pair(1, '1'), make_pair(2,'2'), make_pair(3, '!')});
    unordered_map<int, char> um(m.begin(), m.end());

    CHECK(um[1] == '1');
    CHECK(um[2] == '2');
    CHECK(um[3] == '!');
  }

  SECTION("cbegin cend") {
    hash_map<int, char> m({make_pair(1, '1'), make_pair(2,'2'), make_pair(3, '!')});
    unordered_map<int, char> um(m.cbegin(), m.cend());

    CHECK(um[1] == '1');
    CHECK(um[2] == '2');
    CHECK(um[3] == '!');
  }
}

TEST_CASE("_stress")
{
  hash_map<int, int> m(1000000);
  m.max_load_factor(0.1f);
  for (int i = 0; i < 1000000; ++i)
  {
    m.insert({i, i * 3});
  }
  for (int i = 100; i < 999999; ++i)
  {
    m.erase(i);
  }
  for (int i = 0; i < 1000000; ++i)
  {
    m.insert({i, i * 3});
  }
  for (int i = 0; i < 1000000; ++i)
  {
    m.insert({i, i * 3});
  }
  for (int i = 100; i < 999999; ++i)
  {
    m.erase(i);
  }
  for (int i = 100; i < 999999; ++i)
  {
    m.erase(i);
  }
  for (int i = 0; i < 100; ++i)
    REQUIRE(m[i] == i * 3);
  REQUIRE(m[999999] == 2999997);
  REQUIRE(m.size() == 101);
  for (int i = 0; i < 1000; ++i)
    m.insert({rand() % 1000000, rand()});
}
