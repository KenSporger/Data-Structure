#ifndef __MINE_BITMAP_H
#define __MINE_BITMAP_H

#include<fstream>
#include<assert.h>
#include<memory.h>

namespace mine
{
struct Bitmap
{
    Bitmap(int c = 8, int v = 0); // 容量（bit）、初值（0 or 1）
    Bitmap(int c, const char* file); // 从文件中读取位图，最多c位
    bool test(int x);   // 测试第x比特是否为1
    void set(int x);    // 设置第x比特为1
    void clear(int x);  // 清零第x比特
    void fprint(const char* file);  // 存储位图数据到文件
    void print(); // 按位打印
private:
    char *bmap; 
    int M;          // 字节数
    int capacity;   // 比特数
};

Bitmap::Bitmap(int c, int v)
{
    assert(v == 0 || v == 1);
    capacity = c;
    M = (c+7) / 8;
    bmap = new char[M];
    memset(bmap, v * 255, M);
}

Bitmap::Bitmap(int c, const char* file)
{
    std::ifstream fs(file);
    assert(fs.is_open());

    capacity = c;
    M = (c+7) / 8;
    bmap = new char[M]; // 计算需要读取的字节数
    memset(bmap, 0, M);

    int i = 0;
    char temp = 0;
    while (i < capacity && fs >> temp)// EOF或者读取到所需字节数
    {
        if (temp == '1') set(i);
        i++;
    } 
    fs.close();
}

bool Bitmap::test(int x)
{
    assert(x >= 0 && x < capacity);
    return bmap[x/8] & (0x01 << (x % 8));
}

void Bitmap::set(int x)
{
    assert(x >= 0 && x < capacity);
    bmap[x/8] |= (0x01 << (x % 8));
}

void Bitmap::clear(int x)
{
    assert(x >= 0 && x < capacity);
    bmap[x/8] &= (~(0x01 << (x % 8)));
}

void Bitmap::fprint(const char* file)
{
    std::ofstream out(file);
    assert(out.is_open());

    for (int i = 0; i < capacity; i++)
        out << test(i);
    out.close();
}

void Bitmap::print()
{
    for (int i = 0; i < capacity; i++)
        std::cout << test(i);
}

}

#endif
