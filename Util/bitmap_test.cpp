#include<iostream>
#include<../Util/Bitmap.h>

using namespace std;
using namespace mine;

int main()
{
    char s[] = "00110101000101000101000100000101000001";
    int len = strlen(s);
    Bitmap bmap(len);

    for (int i = 0; i < len; i++)
    {
        if (s[i] == '1') bmap.set(i);
    }
    bmap.fprint("bitmap.txt");
    Bitmap bmap2(len, "bitmap.txt");
   
    for (int i = 0; i < len; i++)
    {
        if (s[i] == '1') assert(bmap2.test(i));
        else assert(!bmap2.test(i));
    }

    for (int i = 0; i < len; i++)
    {
        if (s[i] == '1') bmap2.clear(i);
    }

    for (int i = 0; i < len; i++)
    {
         assert(!bmap2.test(i));
    }
    
}