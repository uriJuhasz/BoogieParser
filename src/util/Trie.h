/*
 * Trie.h
 *
 *  Created on: Aug 19, 2016
 *      Author: uri
 */

#ifndef UTIL_TRIE_H_
#define UTIL_TRIE_H_

#include <string>
#include <memory>
#include <list>
#include <tuple>

namespace BoogieParser {

    template<typename K, typename V>
    class Trie {
    public:
        typedef K Key;
        typedef V Value;
        typedef std::basic_string<K> Str;

        class Exception : public std::exception {
        };

        class DuplicateValueException : public Exception {
        public:
            const Str k;
            const V v1, v2;

            DuplicateValueException(const Trie<K, V>::Str &_k, const V _v1, const V _v2);
        };

        class KeyNotFoundException : public Exception {
        public:
            KeyNotFoundException(const Str &_k);

        private:
            const Str k;
        };

        Trie();

        Trie(std::initializer_list<std::tuple<Str, V>> l);

        virtual ~Trie();

        void insert(const Str &s, V v);

        bool tryGet(const Str &s, V &v) const;

        V getOrElse(const Str &s, const V &v) const;

        V get(const Str &s) const;

    private:
        class Node;

        Node *getNode(const Str &s);

        std::unique_ptr<Node> head;
    };

} /* namespace BoogieParser */

#endif /* UTIL_TRIE_H_ */
