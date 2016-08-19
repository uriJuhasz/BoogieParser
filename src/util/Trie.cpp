
#include "Trie.h"
#include <unordered_map>
#include <cassert>

using namespace std;

namespace BoogieParser
{
    template<typename K, typename V>
    class Trie<K, V>::Node {
    public:
        Node() {};
        unordered_map<K, unique_ptr<Node>> succs;
        unique_ptr<V> v;
    };

  template<typename K,typename V> Trie<K,V>::Trie ()
  {
      unique_ptr<Node> h = make_unique<Node>();
  }

    template<typename K, typename V>
    Trie<K, V>::Trie(std::initializer_list<std::tuple<Str, V>> l)
            :Trie() {
        for (auto &p : l)
            insert(get < 0 > (p), get < 1 > (p));
    }

  template<typename K,typename V> Trie<K,V>::~Trie ()
  {
  }

    template<typename K, typename V>
    Trie<K, V>::DuplicateValueException::DuplicateValueException(const typename Trie<K, V>::Str &_k, const V _v1,
                                                                 const V _v2)
            : k(_k), v1(_v1), v2(_v2) {}

    template<typename K, typename V>
    Trie<K, V>::KeyNotFoundException::KeyNotFoundException(const typename Trie<K, V>::Str &_k)
            : k(_k) {}

    template<typename K, typename V>
    void Trie<K, V>::insert(const Str &s, V v) {
        assert(s.length() > 0);
        Node &n = *head.get();
        for (auto &c : s) {
            auto sni = n.succs.find(c);
            if (sni == n.succs.end()) {
                Node *sn = new Node();
                n.succs[c] = sn;
                n = *sn;
            } else
                n = *sni->second.get();
        }
        if (n.v)
            if (*n.v == v) {}
            else
                throw new DuplicateValueException(s, v, *n.v);

    }

    template<typename K, typename V>
    typename Trie<K, V>::Node *Trie<K, V>::getNode(const Str &s) {
        Trie<K, V>::Node *n = head.get();
        for (auto &c : s) {
            auto sni = n->succs.find(c);
            if (sni == n->succs.end())
                return nullptr;
            else
                n = sni->second.get();
        }
        return n;
    };

    template<typename K, typename V>
    bool Trie<K, V>::tryGet(const Str &s, V &v) const {
        Node *n = getNode(s);
        if (n != nullptr) {
            v = n->v;
            return true;
        } else {
            return false;
        }
    };

    template<typename K, typename V>
    V Trie<K, V>::getOrElse(const Str &s, const V &v) const {
        Node *n = getNode(s);
        if (n != nullptr)
            return n->v;
        else
            return v;
    };

    template<typename K, typename V>
    V Trie<K, V>::get(const Str &s) const {
        Node *n = getNode(s);
        if (n != nullptr)
            return n->v;
        throw new KeyNotFoundException(s);
    };


} /* namespace BoogieParser */
