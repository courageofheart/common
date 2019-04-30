
#include "common/dict.h"

#include <assert.h>
#include <limits.h>
#include <assert.h>

static unsigned int dict_force_resize_ratio = 5;   //字典扩容比率
static unsigned char dict_hash_function_seed[16] = "fate is all so";

#if defined(__X86_64__) || defined(__x86_64__) || defined (__i386__)
#define UNALIGNED_LE_CPU
#endif

#define ROTL(x, b) (unsigned long long)(((x) << (b)) | ((x) >> (64 - (b))))

#define U32TO8_LE(p, v)                                                        \
    (p)[0] = (unsigned char)((v));                                                   \
    (p)[1] = (unsigned char)((v) >> 8);                                              \
    (p)[2] = (unsigned char)((v) >> 16);                                             \
    (p)[3] = (unsigned char)((v) >> 24);

#define U64TO8_LE(p, v)                                                        \
    U32TO8_LE((p), (uint32_t)((v)));                                           \
    U32TO8_LE((p) + 4, (uint32_t)((v) >> 32));

#ifdef UNALIGNED_LE_CPU
#define U8TO64_LE(p) (*((unsigned long long*)(p)))
#else
#define U8TO64_LE(p)                                                           \
    (((unsigned long long)((p)[0])) | ((unsigned long long)((p)[1]) << 8) |                        \
     ((unsigned long long)((p)[2]) << 16) | ((unsigned long long)((p)[3]) << 24) |                 \
     ((unsigned long long)((p)[4]) << 32) | ((unsigned long long)((p)[5]) << 40) |                 \
     ((unsigned long long)((p)[6]) << 48) | ((unsigned long long)((p)[7]) << 56))
#endif

#define SIPROUND                                                               \
    do {                                                                       \
        v0 += v1;                                                              \
        v1 = ROTL(v1, 13);                                                     \
        v1 ^= v0;                                                              \
        v0 = ROTL(v0, 32);                                                     \
        v2 += v3;                                                              \
        v3 = ROTL(v3, 16);                                                     \
        v3 ^= v2;                                                              \
        v0 += v3;                                                              \
        v3 = ROTL(v3, 21);                                                     \
        v3 ^= v0;                                                              \
        v2 += v1;                                                              \
        v1 = ROTL(v1, 17);                                                     \
        v1 ^= v2;                                                              \
        v2 = ROTL(v2, 32);                                                     \
    } while (0)

/********************************************************
   Func Name: siphash
Date Created: 2019-4-29
 Description: 离散函数
	   Input: 
	  Output:
      Return: 离散值
     Caution: 
*********************************************************/
unsigned long long siphash(const unsigned char *in, const unsigned int inlen, const unsigned char *k)
{
#ifndef UNALIGNED_LE_CPU
    unsigned long long hash;
    unsigned char *out = (unsigned char*)&hash;
#endif
    unsigned long long v0 = 0x736f6d6570736575ULL;
    unsigned long long v1 = 0x646f72616e646f6dULL;
    unsigned long long v2 = 0x6c7967656e657261ULL;
    unsigned long long v3 = 0x7465646279746573ULL;
    unsigned long long k0 = U8TO64_LE(k);
    unsigned long long k1 = U8TO64_LE(k + 8);
    unsigned long long m;
    const unsigned char *end = in + inlen - (inlen % sizeof(unsigned long long));
    const int left = inlen & 7;
    unsigned long long b = ((unsigned long long)inlen) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    for (; in != end; in += 8) {
        m = U8TO64_LE(in);
        v3 ^= m;

        SIPROUND;

        v0 ^= m;
    }

    switch (left) {
    case 7: b |= ((unsigned long long)in[6]) << 48; /* fall-thru */
    case 6: b |= ((unsigned long long)in[5]) << 40; /* fall-thru */
    case 5: b |= ((unsigned long long)in[4]) << 32; /* fall-thru */
    case 4: b |= ((unsigned long long)in[3]) << 24; /* fall-thru */
    case 3: b |= ((unsigned long long)in[2]) << 16; /* fall-thru */
    case 2: b |= ((unsigned long long)in[1]) << 8; /* fall-thru */
    case 1: b |= ((unsigned long long)in[0]); break;
    case 0: break;
    }

    v3 ^= b;

    SIPROUND;

    v0 ^= b;
    v2 ^= 0xff;

    SIPROUND;
    SIPROUND;

    b = v0 ^ v1 ^ v2 ^ v3;
#ifndef UNALIGNED_LE_CPU
    U64TO8_LE(out, b);
    return hash;
#else
    return b;
#endif
}

/********************************************************
   Func Name: _dictNextPower
Date Created: 2019-4-29
 Description: 计算哈希表的容量
	   Input: 
	  Output:
      Return: 哈希表大小
     Caution: 哈希表容量是2的幂，方便取余计算
*********************************************************/
static unsigned long _dictNextPower(unsigned long size)
{
    unsigned long i = DICT_HT_INITIAL_SIZE;

    if (size >= LONG_MAX)
    {
        return LONG_MAX + 1LU;
    }

    while (1) 
    {
        if (i >= size)
        {
            return i;
        }

        i *= 2;

    }
}

/********************************************************
   Func Name: _dictReset
Date Created: 2019-4-29
 Description: 字典重置
	   Input: 
	  Output:
      Return: 
     Caution: 
*********************************************************/
static void _dictReset(dictht *ht)
{
    assert(ht);

    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}

/********************************************************
   Func Name: dictExpand
Date Created: 2019-4-29
 Description: 扩展字典
	   Input: 
	  Output:
      Return: error code
     Caution: 
*********************************************************/
int dictExpand(dict *d, unsigned long size)
{
    assert(d);
    assert(size);

    //新哈希表
    dictht n;
    //实际大小
    unsigned long realsize = _dictNextPower(size);

    //如果正在重新整理  或者   已使用大于size 不必扩充
    if (dictIsRehashing(d) || d->ht[0].used > size)
    {
        return DICT_ERR;
    }

    if (realsize == d->ht[0].size)
    {
        return DICT_ERR;
    }

    //分配新的哈希表
    n.size = realsize;
    n.sizemask = realsize - 1;
    n.table = calloc(1, realsize * sizeof(dictEntry *));
    assert(n.table);
    n.used = 0;

    //第一次初始化
    if (NULL == d->ht[0].table)
    {
        d->ht[0] = n;
        return DICT_OK;
    }

    d->ht[1] = n;
    //准备执行Rehash操作
    d->rehashidx = 0;

    return DICT_OK;

}

/********************************************************
   Func Name: _dictExpandIfNeeded
Date Created: 2019-4-29
 Description: 扩容
	   Input: 
	  Output:
      Return: error code
     Caution: 
*********************************************************/
static int _dictExpandIfNeeded(dict *d)
{
    assert(d);

    // d->rehashidx != -1 不扩容
    if (dictIsRehashing(d))
    {
        return DICT_OK;
    }

    if (!d->ht[0].size)
    {
        //字典第一次初始化
        return dictExpand(d, DICT_HT_INITIAL_SIZE);
    }

    if (d->ht[0].used >= d->ht[0].size && (d->ht[0].used/d->ht[0].size > dict_force_resize_ratio))
    {
        return dictExpand(d, d->ht[0].used * 2);
    }

    return DICT_OK;

}

/********************************************************
   Func Name: _dictInit
Date Created: 2019-4-29
 Description: 字典初始化
	   Input: 
	  Output:
      Return: 
     Caution: 
*********************************************************/
static void _dictInit(dict *d, dictType *type, void *privDataPtr)
{
    assert(d);

    _dictReset(&d->ht[0]);
    _dictReset(&d->ht[1]);
    d->type = type;
    d->privdata = privDataPtr;
    d->rehashidx = -1;
    d->iterators = 0;
}

/********************************************************
   Func Name: _dictKeyIndex
Date Created: 2019-4-29
 Description: 计算下标
	   Input: 
	  Output:
      Return: 下标
     Caution: 
*********************************************************/
static long _dictKeyIndex(dict *d, const void *key, unsigned long long hash, dictEntry **existing)
{
    assert(d);
    assert(key);

    unsigned long idx = 0, table = 0;
    dictEntry *de = NULL;

    if (existing)
    {
        *existing = NULL;
    }

    //扩展字典
    if (DICT_ERR == _dictExpandIfNeeded(d))
    {
        return -1;
    }

    for (table = 0; table <= 1; table++)
    {
        idx = hash & d->ht[table].sizemask;

        de = d->ht[table].table[idx];

        while (de)
        {
            if (dictCompareKeys(d,key,de->key))
            {
                if (existing)
                {
                    *existing = de;
                }
                return -1;
            }

            de = de->next;
        }

        //正在进行 Rehash 需要遍历ht[1]
        if (!dictIsRehashing(d))
        {
            break;
        }
    }

    return idx;
}

/********************************************************
   Func Name: _dictRehashStep
Date Created: 2019-4-29
 Description: 单步Rehash
	   Input: 
	  Output:
      Return: 
     Caution: 
*********************************************************/
static void _dictRehashStep(dict *d) 
{
    assert(d);

    if (0 == d->iterators)
    {
        dictRehash(d, 1);
    }
}

/********************************************************
   Func Name: dictSetHashFunctionSeed
Date Created: 2019-4-29
 Description: 设置哈希随机种子
	   Input: 
	  Output:
      Return: 
     Caution: 该随机数只允许设置一次
*********************************************************/
static void dictSetHashFunctionSeed(unsigned char *seed)
{
    //字典的随机种子不允许修改，会影响键值对提取
    //assert(seed);
    //memcpy(dict_hash_function_seed, seed, sizeof(dict_hash_function_seed));
}

/********************************************************
   Func Name: dictGetHashFunctionSeed
Date Created: 2019-4-29
 Description: 获取哈希随机种子
	   Input: 
	  Output:
      Return: 随机种子数组，长度是16
     Caution: 
*********************************************************/
static unsigned char *dictGetHashFunctionSeed(void) 
{
    return dict_hash_function_seed;
}

/********************************************************
   Func Name: dictGenericDelete
Date Created: 2019-4-29
 Description: 键值对释放
	   Input: 
	  Output:
      Return: if nofree return dictEntry else return NULL
     Caution: 
*********************************************************/
static int dictGenericDelete(dict *d, const void *key, int nofree)
{
    assert(d);
    assert(key);

    unsigned long long h = 0, idx = 0;
    dictEntry *de = NULL, *prevDe = NULL;
    unsigned int table = 0;

    //未初始化
    if (0 == d->ht[0].used && 0 == d->ht[1].used)
    {
        return DICT_ERR;
    }

    if (dictIsRehashing(d))
    {
        _dictRehashStep(d);
    }

    //离散值计算
    h = dictHashKey(d, key);

    for (table = 0; table <= 1; ++table)
    {
        idx = h & d->ht[table].sizemask;

        de = d->ht[table].table[idx];

        while (de)
        {
            if (dictCompareKeys(d, key, de->key))
            {
                if (prevDe)
                {
                    prevDe->next = de->next;
                }
                else
                {
                    d->ht[table].table[idx] = de->next;
                }
                if (!nofree)
                {
                    dictFreeKey(d, de);
                    dictFreeVal(d, de);
                    free(de);
                    de = NULL;
                }
                d->ht[table].used--;
                return DICT_OK;
            }

            prevDe = de;
            de = de->next;
        }

        if (!dictIsRehashing(d))
        {
            break;
        }
    }

    return DICT_ERR;

}

/********************************************************
   Func Name: dictRehash
Date Created: 2019-4-29
 Description: 重新整理字典
	   Input: 
	  Output:
      Return: 0 整理完毕 ， 1 未整理完毕
     Caution: 
*********************************************************/
int dictRehash(dict *d, unsigned int n)
{
    assert(d);

    // 最大访问空桶数量，进一步减少可能引起阻塞的时间
    unsigned int empty_visits = n * 10;

    //如果d->rehashidx == -1 ,不需要执行dictRehash
    if (!dictIsRehashing(d))
    {
        return 0;
    }

    // n 为字典每次迁移的步长，扩容时，每次只移动 n 个元素，防止 redis 阻塞
    while (n-- && d->ht[0].used)
    {
        dictEntry *de = NULL, *nextde = NULL;

        //校验是否越界(几乎不存在这种可能性)
        assert(d->ht[0].size > d->rehashidx);

        //找到第一个迁移的元素
        while (!d->ht[0].table[d->rehashidx])
        {
            d->rehashidx++;
            if (--empty_visits)
            {
                //ht[0]中大量的元素都是空的，为了减少阻塞 redis ，返回1，等待下次dictRehash
                return 1;
            }
        }

        de = d->ht[0].table[d->rehashidx];

        //将table[d->rehashidx]整条链表上的元素全部迁移
        while (de)
        {
            //ht[1]上的下标
            unsigned long long index = 0;

            // 链表上所有元素必须全部重新散列，而不是将整个链表迁移
            nextde = de->next;

            index = dictHashKey(d, de->key) & d->ht[1].sizemask;
            //推链法 de位于d->ht[1].table[index]原链表的开头
            /*
            设计方案
                因为 dictEntry 节点组成的链表没有指向链表表尾的指针， 
                所以为了速度考虑， 程序总是将新节点添加到链表的表头位置（复杂度为 O(1)）， 排在其他已有节点的前面
            */
            de->next = d->ht[1].table[index];
            d->ht[1].table[index] = de;

            d->ht[0].used--;
            d->ht[1].used++;

            //处理链表上下一个节点
            de = nextde;
        }

        d->ht[0].table[d->rehashidx] = NULL;
        d->rehashidx++;

    }

    if (!d->ht[0].used)
    {
        //d->ht[0]已经全部迁移完毕
        if (d->ht[0].table)
        {
            free(d->ht[0].table);
        }
        d->ht[0] = d->ht[1];
        _dictReset(&d->ht[1]);
        //停止dictRehash
        d->rehashidx = -1;
        return 0;
    }

    return 1;

}

/********************************************************
   Func Name: dictCreate
Date Created: 2019-4-29
 Description: 创建字典
	   Input: 
	  Output:
      Return: 字典对象
     Caution: 
*********************************************************/
dict *dictCreate(dictType *type, void *privDataPtr)
{
    assert(type);

    dict *d = calloc(1, sizeof(dict));
    assert(d);

    _dictInit(d, type, privDataPtr);

    return d;
}

/********************************************************
   Func Name: dictAddRaw
Date Created: 2019-4-29
 Description: 添加键
	   Input: 
	  Output:
      Return: 键值对实例
     Caution: 
*********************************************************/
dictEntry *dictAddRaw(dict *d, void *key, dictEntry **existing)
{
    long index = 0;
    dictEntry *entry = NULL;   //键值对实例
    dictht *ht = NULL;     //哈希表实例

    //正在Rehash
    if (dictIsRehashing(d))
    {
        _dictRehashStep(d);
    }

    //找到下标
    if ((index = _dictKeyIndex(d,key,dictHashKey(d,key),existing)) == -1)
    {
        return NULL;
    }

    //确认需要更新的哈希表
    ht = dictIsRehashing(d)? &d->ht[1] : &d->ht[1];

    //分配键值对内存
    entry = (dictEntry *)calloc(1, sizeof(dictEntry));
    assert(entry);
    entry->next = ht->table[index];
    ht->table[index] = entry;
    ht->used++;

    //设置键值
    dictSetKey(d, entry, key);

    return entry;
}

/********************************************************
   Func Name: dictAdd
Date Created: 2019-4-29
 Description: 添加键值对
	   Input: 
	  Output:
      Return: error code
     Caution: 
*********************************************************/
int dictAdd(dict *d, void *key, void *val)
{
    assert(d);
    assert(key);

    dictEntry *entry = dictAddRaw(d,key,NULL);

    if (!entry)
    {
        return DICT_ERR;
    }

    dictSetVal(d, entry, val);

    return DICT_OK;
}

/********************************************************
   Func Name: dictAddOrReplace
Date Created: 2019-4-29
 Description: 添加或替换
	   Input: 
	  Output:
      Return: 0 表示替换 1 表示新增
     Caution: 不存在key 执行添加操作 
*********************************************************/
int dictAddOrReplace(dict *d, void *key, void *val)
{
    assert(d);
    assert(key);

    dictEntry *entry = NULL, *existing = NULL, auxentry;

    entry = dictAddRaw(d, key, &existing);
    if (entry)
    {
        // add
        dictSetVal(d, entry, val);
        return 1;
    }

    //replace

    /*
    设计方案:
        浅拷贝的重要意义
            dictSetVal() 函数会修改 existing 中的成员属性 val 的值，但是原来的 val 内存还没有释放，所以需要 auxentry 来临时存储一下
        为什么 auxentry 不是指针类型
            如果 auxentry 是指针，那么 auxentry 和 existing 将会指向同一片内存， existing 修改 val 的操作将会影响 auxentry 的 val值
        但是这里希望是不影响 auxentry 的 val值，因为 val 的值是需要释放的
    */
    auxentry = *existing;
    dictSetVal(d, existing, val);
    dictFreeVal(d, &auxentry);

    return 0;

}

/********************************************************
   Func Name: dictDelete
Date Created: 2019-4-29
 Description: 删除键值对
	   Input: 
	  Output:
      Return: error code
     Caution: 
*********************************************************/
int dictDelete(dict *d, const void *key)
{
    assert(d);
    assert(key);

    return dictGenericDelete(d, key, 0);
}

/********************************************************
   Func Name: dictFind
Date Created: 2019-4-29
 Description: 查找键值对
	   Input: 
	  Output:
      Return: dictEntry
     Caution: 
*********************************************************/
dictEntry *dictFind(dict *d, const void *key)
{
    assert(d);
    assert(key);

    unsigned long long h = 0, idx = 0;
    dictEntry *de = NULL;
    unsigned int table = 0;

    //未初始化
    if (0 == d->ht[0].used && 0 == d->ht[1].used)
    {
        return NULL;
    }

    if (dictIsRehashing(d))
    {
        _dictRehashStep(d);
    }

    //离散值计算
    h = dictHashKey(d, key);

    for (table = 0; table <= 1; ++table)
    {
        idx = h & d->ht[table].sizemask;

        de = d->ht[table].table[idx];

        while (de)
        {
            if (dictCompareKeys(d, key, de->key))
            {
                return de;
            }

            de = de->next;
        }

        if (!dictIsRehashing(d))
        {
            break;
        }
    }

    return NULL;
}

/********************************************************
   Func Name: dictFind
Date Created: 2019-4-29
 Description: 查找键值对
	   Input: 
	  Output:
      Return: dictEntry
     Caution: 
*********************************************************/
void *dictFetchValue(dict *d, const void *key)
{
    assert(d);
    assert(key);

    dictEntry *de = NULL;

    de = dictFind(d, key);

    return de ? dictGetVal(de) : NULL;
}

/********************************************************
   Func Name: _dictClear
Date Created: 2019-4-29
 Description: 清理哈希表
	   Input: 
	  Output:
      Return: dictEntry
     Caution: 
*********************************************************/
static void _dictClear(dict *d, dictht *ht, void(callback)(void *))
{
    assert(d);
    assert(ht);  //哈希表是结构体中的结构体变量，ht的指针绝不会是NULL。只能是ht里面元素是空的

    dictEntry *de = NULL, *nextDe = NULL;
    unsigned long i = 0;

    if (callback)
    {
        callback(d->privdata);
    }

    /*
    设计方案
        ht->size && ht->used这个判断会加快哈希表的遍历，针对于哈希表后面全是空的情况
    */
    for (i = 0; i < ht->size && ht->used; ++i)
    {
        de = ht->table[i];

        while (de)
        {
            nextDe = de->next;

            dictFreeKey(d, de);
            dictFreeVal(d, de);
            free(de);
            de = NULL;

            ht->used--;
            de = nextDe;
        }
    }

    //释放哈希表结构
    free(ht->table);

    _dictReset(ht);
}

/********************************************************
   Func Name: dictRelease
Date Created: 2019-4-29
 Description: 释放字典
	   Input: 
	  Output:
      Return: 
     Caution: 
*********************************************************/
void dictRelease(dict *d)
{
    assert(d);

    _dictClear(d, &d->ht[0], NULL);
    _dictClear(d, &d->ht[1], NULL);

    free(d);
    d = NULL;
}

/********************************************************
   Func Name: dictFingerprint
Date Created: 2019-4-29
 Description: 迭代器指纹
	   Input: 
	  Output:
      Return: 
     Caution: 迭代器指纹的计算与字典紧密相关，字典大小等变化都会引起迭代器指纹的变化
*********************************************************/
long long dictFingerprint(dict *d) 
{
    long long integers[6] = {0}, hash = 0;
    int j = 0;

    integers[0] = (long) d->ht[0].table;
    integers[1] = d->ht[0].size;
    integers[2] = d->ht[0].used;
    integers[3] = (long) d->ht[1].table;
    integers[4] = d->ht[1].size;
    integers[5] = d->ht[1].used;

    for (j = 0; j < 6; j++) 
    {
        hash += integers[j];
        hash = (~hash) + (hash << 21);
        hash = hash ^ (hash >> 24);
        hash = (hash + (hash << 3)) + (hash << 8);
        hash = hash ^ (hash >> 14);
        hash = (hash + (hash << 2)) + (hash << 4);
        hash = hash ^ (hash >> 28);
        hash = hash + (hash << 31);
    }

    return hash;
}

/********************************************************
   Func Name: dictRelease
Date Created: 2019-4-29
 Description: 释放字典
	   Input: 
	  Output:
      Return: 
     Caution: 
*********************************************************/
int dictResize(dict *d)
{
    assert(d);

    unsigned long minimal = 0;

    if (dictIsRehashing(d))
    {
        return DICT_ERR;
    }

    minimal = d->ht[0].used;

    if (minimal < DICT_HT_INITIAL_SIZE)
    {
        minimal = DICT_HT_INITIAL_SIZE;
    }

    return dictExpand(d, minimal);

}

/********************************************************
   Func Name: dictGetIterator
Date Created: 2019-4-29
 Description: 获取普通迭代器
	   Input: 
	  Output:
      Return: 普通迭代器
     Caution: 
*********************************************************/
dictIterator *dictGetIterator(dict *d)
{
    assert(d);

    dictIterator *iter = calloc(1, sizeof(dictIterator));

    assert(iter);

    iter->d = d;
    iter->index = -1; 
    iter->table = 0;
    iter->safe = 0;
    iter->entry = NULL;
    iter->nextEntry = NULL;
    iter->fingerprint = 0;

    return iter;
}

/********************************************************
   Func Name: dictGetSafeIterator
Date Created: 2019-4-29
 Description: 获取安全迭代器
	   Input: 
	  Output:
      Return: 安全迭代器
     Caution: 
*********************************************************/
dictIterator *dictGetSafeIterator(dict *d)
{
    assert(d);

    dictIterator *i = dictGetIterator(d);

    i->safe = 1;

    return i;
}

/********************************************************
   Func Name: dictNext
Date Created: 2019-4-29
 Description: 迭代器下一个
	   Input: 
	  Output:
      Return: dictEntry
     Caution: 
*********************************************************/
dictEntry *dictNext(dictIterator *iter)
{
    assert(iter);

    dictht *ht = NULL;

    while (1)
    {
        if (NULL == iter->entry)
        {
            ht = &iter->d->ht[iter->table];
            if (-1 == iter->index && 0 == iter->table)
            {
                //获取第一个dictEntry
                if (iter->safe)
                {
                    iter->d->iterators++;
                }
                else
                {
                    iter->fingerprint = dictFingerprint(iter->d);
                }
            }

            iter->index++;

            if (iter->index >= (long)ht->size)
            {
                if (dictIsRehashing(iter->d) && 0 == iter->table)
                {
                    //字典已经处于rehash的中间状态，所以还要遍历ht[1]中的元素
                    iter->table++;
                    iter->index = 0;
                    ht = &iter->d->ht[iter->table];
                }
                else
                {
                    break;
                }
            }

            iter->entry = ht->table[iter->index];
        }
        else
        {
            iter->entry = iter->nextEntry;
        }

        if (iter->entry)
        {
             /* We need to save the 'next' here, the iterator user may delete the entry we are returning. */
            iter->nextEntry = iter->entry->next;
            return iter->entry;
        }
    }

    return NULL;
}

/********************************************************
   Func Name: dictGetHash
Date Created: 2019-4-29
 Description: 获取键的离散值
	   Input: 
	  Output:
      Return: 离散值
     Caution: 
*********************************************************/
unsigned long long dictGetHash(dict *d, const void *key)
{
    assert(d);
    assert(key);

    return dictHashKey(d, key);
}

/********************************************************
   Func Name: rev
Date Created: 2019-4-29
 Description: 整数二进制反转
	   Input: 
	  Output:
      Return: 反转后的整数
     Caution: 
*********************************************************/
static unsigned long rev(unsigned long v) 
{
    unsigned long s = 8 * sizeof(v); // bit size; must be power of 2
    unsigned long mask = ~0;

    while ((s >>= 1) > 0) 
    {
        mask ^= (mask << s);
        v = ((v >> s) & mask) | ((v << s) & ~mask);
    }

    return v;
}


/********************************************************
   Func Name: dictScan
Date Created: 2019-4-29
 Description: 字典桶扫描
	   Input: 
	  Output:
      Return: 下一个离散值
     Caution: 在字典dictResize时，可能存在重复遍历
*********************************************************/
unsigned long long dictScan(dict *d, unsigned long v, dictScanFunction *fn, void *privdata)
{
    assert(d);

    dictht *t0 = NULL, *t1 = NULL;
    const dictEntry *de = NULL, *next = NULL;
    unsigned long m0 = 0, m1 = 0;

    if (0 == dictSize(d))
    {
        return 0;
    }

    if (!dictIsRehashing(d))
    {
        //字典没有进行rehash操作

        t0 = &d->ht[0];
        m0 = d->ht[0].sizemask;

        //遍历当前桶
        de = t0->table[v & m0];
        while (de)
        {
            next = de->next;
            //回调函数中可能修改de，所以需要next来缓存下一个节点
            fn(privdata, de);
            de = next;
        }

        //向v的最高位加1，并向低位方向进位

        //将v中所有无效位置1，有效位保持不变
        v |= ~m0;

        //二进制反转v
        v = rev(v);
        //v++ 因为无效位都是1，并且无效位现在都处于低位，加1会导致向高位进1，无效位全部变成0，有效位加1
        v++;
        //反转v 无效位都是0，被省略，有效位反转，实现最高位加1，并向低位方向进位
        v = rev(v);
    }
}









