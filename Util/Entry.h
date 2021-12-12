#ifndef __MINE_ENTRY_H
#define __MINE_ENTRY_H

namespace mine
{
// 词条模板类，即键值对
// 支持构造、拷贝构造、逻辑比较运算
template<typename K, typename V> 
struct Entry
{
    Entry(K k = K(), V v = V()):
        key(k), value(v){}
    Entry(const Entry<K, V> &e): 
        key(e.key), value(e.value){}
    
    bool operator==(const Entry& e) { return key == e.key;}
    bool operator<(const Entry& e) { return key < e.key;}
    bool operator>(const Entry& e) { return key > e.key;}
    bool operator!=(const Entry& e) { return key != e.key;}
    
    K key;
    V value;   
};

}

#endif
