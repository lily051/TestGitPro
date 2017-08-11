/*
 * ====================================================================
 * Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
 *
 *        Filename:  DemoSimpleHash.c
 * 
 *     Description:  针对SimpleHash的demo测试程序
 * 
 *          Author:  tyu, yutao NOSPAM hikvision DOT com
 *
 *            Date:  2009-11-28 14:54:38 中国标准时间
 *
 * Modification History:
 *            <version > <time> <author> <desc>
 * ====================================================================
 */
#if 0
char*   key[] = { "alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel",
                   "india", "juliet", "kilo", "lima", "mike", "november", "oscar", "papa",
                   "quebec", "romeo", "sierra", "tango", "uniform", "victor", "whisky", "x-ray",
                   "yankee", "zulu", "hikvision", "system", "linux", "disk", "family", "first", "second", "first", 
                   "third", "forth", "fiveth", "sixth"};

char*   imp[] = { "1th", "2th", "3th", "4th", "5th", "6th", "7th"};

#include "HPR_SimpleHash.h"
#include "DemoApp.h"

int Test_SimpleHash()
{
    HPR_HANDLE hHash = NULL;
    HPR_INT32 retVal = HPR_SimpleHashCreate(100, &hHash);
    if(retVal != HPR_OK)
    {
        fprintf(stderr, "Create Simple Hash failed\n");
        return -1;
    }

    int i = 0;
    for(; i < sizeof(key) / sizeof(key[0]); i++)
    {
        HPR_ITEM item;
        item.key = key[i];
        item.data= (HPR_VOIDPTR)i;

        retVal = HPR_SimpleHashEnter(&item, hHash);
        if(retVal != HPR_OK)
        {
            fprintf(stderr, "hash table has exist the same key(:%s)\n", key[i]);
        }
    }

    int index = 0;
    HPR_VOIDPTR data = NULL;
    HPR_INT32 findVal = HPR_OK;
    while(index * index < sizeof(key) / sizeof(key[0]))
    {
        findVal = HPR_SimpleHashFind(key[index * index], &data, hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%s) has value(:%d)\n", key[index * index], (HPR_INT32)data);
        }
        
        findVal = HPR_SimpleHashRemove(key[index * index], hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%s) has been removed\n", key[index * index]);
        }

        HPR_ITEM item;
        item.key = imp[index];
        item.data= (HPR_VOIDPTR)(index * index);
        retVal = HPR_SimpleHashEnter(&item, hHash);
        if(retVal != HPR_OK)
        {
            fprintf(stderr, "hash table has exist the same key(:%s)\n", item.key);
        }

        index++;
    }

    for(i = 0; i < sizeof(key) / sizeof(key[0]); i++)
    {
        findVal = HPR_SimpleHashFind(key[i], &data, hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%s) has value(:%d)\n", key[i], data);
        }
        else
        {
            fprintf(stderr, "no found the key(:%s)'s value\n", key[i]);
        }
    }

    for(i = 0; i < sizeof(imp) / sizeof(imp[0]); i++)
    {
        findVal = HPR_SimpleHashFind(imp[i], &data, hHash);
        if(findVal == HPR_OK)
        {
            fprintf(stderr, "the key(:%s) has value(:%d)\n", imp[i], data);
        }
        else
        {
            fprintf(stderr, "no found the key(:%s)'s value\n", imp[i]);
        }
    }

    HPR_SimpleHashDestroy(hHash);
    return 0;
}

#endif

#if 0

int main(int argc, char* argv[])
{
    return Test_SimpleHash();
}

#endif


