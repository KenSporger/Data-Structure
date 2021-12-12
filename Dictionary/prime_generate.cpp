// 素数生成器：输出一个记录n以内的素数的位图
#include<iostream>
#include<../Util/Bitmap.h>

using namespace std;
using namespace mine;

int main(int argc, char *argv[])
{
    assert(argc == 2);
    int n = atoi(argv[1]);
    Bitmap* bmap = new Bitmap(n, 1);
    
    bmap->clear(0);
    bmap->clear(1);
    
    for (int i = 2; i < n; i++)
    {
        if(bmap->test(i)){
            for (int j = 2*i; j < n; j+=i){
                bmap->clear(j);
            }   
        }
    }

    bmap->fprint("primes.txt");
}
