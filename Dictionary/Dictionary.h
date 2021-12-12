#ifndef __MINE_DICTIONARY_H
#define __MINE_DICTIONARY_H


namespace mine
{
// 词典，提供插入、查询、删除词条操作
template<typename K, typename V> 
struct Dictionary
{
    virtual int size() const = 0;
    virtual bool put(K key, V value) = 0;
    virtual V* get(K key) = 0;
    virtual bool remove(K key) = 0;
};

}

#endif
