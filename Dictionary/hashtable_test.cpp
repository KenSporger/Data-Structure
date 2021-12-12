#include<iostream>
#include<assert.h>
#include<unordered_set>
#include<../Dictionary/Hashtable.h>

using namespace std;
using namespace mine;

int main()
{
    Hashtable<int, int> ht;
    unordered_set<int> st;

    for (int i =0; i < 10000; i++)
    {
        int n = rand() % 10000;
        if (st.find(n) != st.end())
        {
            assert(!ht.put(n,n));
        }else
        {
            st.insert(n);
            assert(ht.put(n,n));
        }   
        assert(ht.capacity() / ht.size() >= 2); // 测试装填因子
    }

    assert(ht.size() == st.size());

    auto iter = st.begin();
    while (iter != st.end())
    {
        assert(ht.get(*iter));
        ++iter;
    }

    iter = st.begin();
    while (iter != st.end())
    {
        assert(ht.remove(*iter));
        ++iter;
    }

    assert(ht.size() == 0);

    cout << "ALL TEST PASSED!" << endl;
}