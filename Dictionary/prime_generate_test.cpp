#include<iostream>
#include<assert.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<../Util/Bitmap.h>


using namespace std;
using namespace mine;

int main()
{
    if (fork() == 0){
        execl("prime_generate", "prime_generate","100", nullptr);
    }else{
        wait(0);
        Bitmap bmap(100, "primes.txt");
        assert(!bmap.test(1) && bmap.test(7) && !bmap.test(35) && bmap.test(97));
    }
}