/*
 * Trie.h
 *
 *  Created on: Aug 19, 2016
 *      Author: uri
 */

#ifndef UTIL_TRIE_H_
#define UTIL_TRIE_H_

#include <string>

namespace BoogieParser
{

  template<typename K,typename V>class Trie
  {
  public:
    typedef K Key;
    typedef V Value;
    typedef std::basic_string<K> Str;
    Trie ();
    virtual ~Trie ();
    void insert(const Str& s,V v);
    bool tryGet(const Str& s,V& v);
    V get(const Str& s);
  private:

  };

} /* namespace BoogieParser */

#endif /* UTIL_TRIE_H_ */
