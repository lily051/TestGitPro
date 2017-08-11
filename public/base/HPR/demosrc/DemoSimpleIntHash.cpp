/*
 * ====================================================================
 * Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
 *
 *        Filename:  DemoSimpleIntHash.c
 * 
 *     Description:  针对SimpleIntHash的demo测试程序
 * 
 *          Author:  tyu, yutao NOSPAM hikvision DOT com
 *
 *            Date:  2009-11-28 14:54:38 中国标准时间
 *
 * Modification History:
 *            <version > <time> <author> <desc>
 * ====================================================================
 */
#include "HPR_SimpleIntHash.h"
#include "DemoApp.h"

#if 0

HPR_UINT32 key[] = {0, 102, 10101, 12345, 54321, 123456, 654321, 1234567, 7654321, 12345678, 87654321,
                    98765, 56789, 20091129, 54321, 123456};

HPR_UINT32 imp[] = {2345, 5432, 23456, 100, 10000, 65432, 765432, 654327, 657432};

int Test_SimpleIntHash()
{
    HPR_HANDLE hHash = NULL;
    HPR_INT32 retVal = HPR_SimpleIntHashCreate(100, &hHash);
    if(retVal != HPR_OK)
    {
        fprintf(stderr, "Create Simple Hash failed\n");
        return -1;
    }

    int i = 0;
    for(; i < sizeof(key) / sizeof(key[0]); i++)
    {
        HPR_INTITEM item;
        item.key = key[i];
        item.data= (HPR_VOIDPTR)i;

        retVal = HPR_SimpleIntHashEnter(&item, hHash);
        if(retVal != HPR_OK)
        {
            fprintf(stderr, "hash table has exist the same key(:%d)\n", key[i]);
        }
    }

    int index = 0;
    HPR_VOIDPTR data = NULL;
    HPR_INT32 findVal = HPR_OK;    
    while(index * index < sizeof(key) / sizeof(key[0]))
    {
        findVal = HPR_SimpleIntHashFind(key[index * index], &data, hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%d) has value(:%d)\n", key[index * index], (HPR_INT32)data);
        }
        
        findVal = HPR_SimpleIntHashRemove(key[index * index], hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%d) has been removed\n", key[index * index]);
        }

        HPR_INTITEM item;
        item.key = imp[index];
        item.data= (HPR_VOIDPTR)(index * index);
        retVal = HPR_SimpleIntHashEnter(&item, hHash);
        if(retVal != HPR_OK)
        {
            fprintf(stderr, "hash table has exist the same key(:%d)\n", item.key);
        }

        index++;
    }

    fprintf(stderr, "\n\n");

    for(i = 0; i < sizeof(key) / sizeof(key[0]); i++)
    {
        findVal = HPR_SimpleIntHashFind(key[i], &data, hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%d) has value(:%d)\n", key[i], data);
        }
        else
        {
            fprintf(stderr, "no found the key(:%d)'s value\n", key[i]);
        }
    }

    fprintf(stderr, "\n\n");

    for(i = 0; i < sizeof(imp) / sizeof(imp[0]); i++)
    {
        findVal = HPR_SimpleIntHashFind(imp[i], &data, hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%d) has value(:%d)\n", imp[i], data);
        }
        else
        {
            fprintf(stderr, "no found the key(:%d)'s value\n", imp[i]);
        }
    }

    HPR_SimpleIntHashDestroy(hHash);
    return 0;
}

#if 1

int main(int argc, char* argv[])
{
    return Test_SimpleIntHash();
}

#endif

#endif
