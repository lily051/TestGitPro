
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hak.h"

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    // 初始化库。
    Hak1((char*)103, "DATA", NULL);

    /*
    aaa 加密后变为
    4F04D08EE1DFE4F2606EA357FF8817DB

    123abc456 加密后变为
    AE781870FEE5FFB1C5D8ECBA55F2A966

    12345678 加密后变为
    33BE89A17803B5E12FD7E98A5E5E81F6
    */

    char* src = "12345678"; // 待加密明文。
    char aes[1024] = { '\0' };
    int aes_len = 1024;
    char dst[1024] = { '\0' };
    int dst_len = 1024;

    char* out = aes;
    char* in = src;
    char* outl = (char*)&aes_len;
    assert(!Hak5(out, in, outl)); // 加密。

    assert(!strcmp("33BE89A17803B5E12FD7E98A5E5E81F6", out)); // 确认加密后的内容是否正确。

    out = dst;
    in = aes;
    outl = (char*)&dst_len;
    assert(!Hak7(out, in, outl)); // 解密。

    assert(!strcmp(out, src)); // 确认解密后的内容是否正确。

    // 验证字串是否已加密。
    assert(Hak9("AE781870FEE5FFB1C5D8ECBA55F2A966", NULL, NULL));
    assert(!Hak9("Ae781870FEE5FFB1C5D8ECBA55F2A966", NULL, NULL));
    assert(!Hak9("AE781870FEE5FFB1C5D8ECBA55F2A96", NULL, NULL));
    assert(!Hak9("", NULL, NULL));

    // 反初始化库。
    Hak2(NULL, NULL, NULL);

    return 0;
}

