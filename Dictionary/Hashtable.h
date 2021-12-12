#ifndef __MINE_HASHTABLE_H
#define __MINE_HASHTABLE_H

#include<../Dictionary/Dictionary.h>
#include<../Util/Entry.h>
#include<../Util/Bitmap.h>
#include<memory.h>

namespace mine
{
template<typename K, typename V> 
class Hashtable : Dictionary<K, V>
{
public:
    Hashtable(int capacity = 5);
    ~Hashtable();
    int capacity() const;
    int size() const;
    bool put(K key, V value);
    V* get(K key);
    bool remove(K key);

protected:
    int primeNLT(int c, int n, const char* file); // 根据file文件中的记录，在[c, n)内取最小的素数

    int probe4Hit(const K& key); // 沿关键码key对应的查找链，找到词条匹配的桶, 没有找到会返回首个空桶位置
    int probe4Free(const K& key); // 沿着关键码key对应的查找链， 找到首个空的桶
    
    int hashCode(char c); // 字符散列转换
    int hashCode(int k); // 整数散列转换 
    int hashCode(long long i); // 长整型散列码转换 
    int hashCode(char s[]); // 字符串散列码转换 

    int hashFunc(int code); // 散列函数
    void rehash();  // 重散列

private:
    int M; // 桶数组数量
    int N; // 当前词条数量
    Entry<K, V> **ht; // 桶数组指针，指向桶数组， 数组存放词条指针
    Bitmap* lazyRemoval; // 用位图实现的懒惰删除标记
};

template<typename K, typename V>
Hashtable<K, V>::Hashtable(int capacity)
{
    M = primeNLT(capacity, 1048576, "primesnlt.txt"); // 确保桶数组数量为素数
    N = 0;
    ht = new Entry<K, V>*[M];
    std:memset(ht, 0, sizeof(Entry<K, V>*) * M); // 初始化为空指针
    lazyRemoval = new Bitmap(M);
}

template<typename K, typename V>
Hashtable<K, V>::~Hashtable()
{
    for (int i = 0; i < M; i++)
        if (ht[i]) delete ht[i]; // 逐一检查各桶，释放非空桶
    delete[] ht; // 释放桶数组
    delete lazyRemoval;
}

template<typename K, typename V>
int Hashtable<K, V>::size() const
{
    return N;
}

template<typename K, typename V>
int Hashtable<K, V>::capacity() const
{
    return M;
}

template<typename K, typename V>
V* Hashtable<K, V>::get(K key)
{
    int index = probe4Hit(key);
    return ht[index] ? &(ht[index]->value) : NULL;
}

template<typename K, typename V>
bool Hashtable<K, V>::put(K key, V value)
{
    if (ht[probe4Hit(key)]) return false; // 重复元素不插入
    int index = probe4Free(key); // 只要装填因子控制得到，必然成功
    ht[index] = new Entry<K, V>(key, value); // 建立词条对象并插入 
    N++;
    if (N * 2 > M) rehash();    // 装填因子高于50%后重散列
    return true;
}

template<typename K, typename V>
bool Hashtable<K, V>::remove(K key)
{
    int index = probe4Hit(key);
    if (!ht[index]) return false; // 元素不存在
    delete ht[index];
    ht[index] = NULL;// 释放词条， 清空指针
    lazyRemoval->set(index);    // 添加懒惰标记
    N--;
    return true;
}

template<typename K, typename V>
int Hashtable<K, V>::primeNLT(int c, int n, const char* file)
{
    Bitmap bmap(n, file); // file已经按照位图格式，记录了n以内的所有素数
    while(c < n)    // 从c开始测试找到第一个比c大的素数
        if (!bmap.test(c)) c++;
        else return c;
    return c;
}

template<typename K, typename V>
int Hashtable<K, V>::probe4Hit(const K& key)
{
    int index = hashFunc(hashCode(key)); // 通过散列码转换、散列函数映射到地址空间
    while((ht[index] && ht[index]->key != key) || 
        (!ht[index] && lazyRemoval->test(index)))
        index = (index + 1) % M; // 从起始桶出发沿着查找链线性试探，需要判断是否满一圈吗？
    return index;
}

template<typename K, typename V>
int Hashtable<K, V>::probe4Free(const K& key)
{
    int index = hashFunc(hashCode(key)); // 通过散列码转换、散列函数映射到地址空间
    while(ht[index])
        index = (index + 1) % M; // 从起始桶出发沿着查找链线性试探，需要判断是否满一圈吗？
    return index;
}

template<typename K, typename V>
int Hashtable<K, V>::hashCode(char c)
{
    return static_cast<int>(c);
}

template<typename K, typename V>
int Hashtable<K, V>::hashCode(int k)
{
    return k;
}

template<typename K, typename V>
int Hashtable<K, V>::hashCode(long long i)
{
    return static_cast<int>(i);
}

template<typename K, typename V>
int Hashtable<K, V>::hashCode(char s[])
{
    int code = 0;
    for (int n = strlen(s), i = 0; i < n; i++)
    {
        code = (code << 5) | (code >> 27); // 散列码循环左移，再累加当前字符，近似“多项式”
        code += (int)s[i];
    }
    return code;
}

template<typename K, typename V>
int Hashtable<K, V>::hashFunc(int code)
{
    return code % M;
}

template<typename K, typename V>
void Hashtable<K, V>::rehash()
{
    int old_capacity = M;
    Entry<K, V>** old_ht = ht;
    M = primeNLT(2*M, 1048576, "primesnlt.txt"); // 容量增加至少两倍
    N = 0;
    ht = new Entry<K, V>*[M];
    std:memset(ht, 0, sizeof(Entry<K, V>*) * M);
    delete lazyRemoval;
    lazyRemoval = new Bitmap(M);
    for (int i = 0; i < old_capacity; i++)
    {
        if (old_ht[i])
        {
            put(old_ht[i]->key, old_ht[i]->value); // 将原散列表中的词条依次映射到新的表中
            delete old_ht[i];
        }
    }
    delete old_ht; // 释放原桶数组
}

}

#endif
