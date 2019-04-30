
#ifndef __GTC_DICT_H_
#define __GTC_DICT_H_

#define DICT_OK 0
#define DICT_ERR -1

#ifdef __cplusplus
extern "C"
{
#endif
    /* 键值对结构体 */
    typedef struct dictEntry
    {
        //键
        void *key;

        //匿名联合体(值)
        union 
        {
            void *val;
            unsigned long long u64;
            long long s64;
            double d;
        }v;

        //下一元素的指针
        struct dictEntry *next;
    }dictEntry;

    /* 哈希表结构体 */
    typedef struct dictht
    {
        dictEntry **table;
        unsigned long size;
        unsigned long sizemask;
        unsigned long used;
    }dictht;

    /* 字典操作方法结构 */
    typedef struct dictType 
    {
        unsigned long long(*hashFunction)(const void *key);
        void *(*keyDup)(void *privdata, const void *key);
        void *(*valDup)(void *privdata, const void *obj);
        int(*keyCompare)(void *privdata, const void *key1, const void *key2);
        void(*keyDestructor)(void *privdata, void *key);
        void(*valDestructor)(void *privdata, void *obj);
    } dictType;

    /* 字典主操作结构 */
    typedef struct dict
    {
        dictType *type;
        void *privdata;
        dictht ht[2];
        long rehashidx; /* rehashing not in progress if rehashidx == -1 */
        unsigned long iterators; /* number of iterators currently running */
    }dict;

    /* 字典迭代结构 */
    typedef struct dictIterator 
    {
        //指向迭代器处理的字典
        dict *d;    

        //dictht中table数组的下标    
        long index;    

        //table是dict结构中dictht数组的下标，即标识ht[0]还是ht[1]
        //safe字段用于标识该迭代器是否为一个安全的迭代器
        int table, safe;    

        //entry和nextEntry分别指向当前的元素和下一个元素
        dictEntry *entry, *nextEntry;

        //字典的指纹
        long long fingerprint;    /* unsafe iterator fingerprint for misuse detection. */
    } dictIterator;

    /*
    安全迭代器说明
        如果是安全迭代器，则可以在迭代过程中使用dictAdd、dictDelete、dictFind等方法；
        如果不是，则只能使用dictNext遍历方法。
        如果使用安全迭代器将会设置safe为1，在 dictNext 函数中让字典iterators自增，
        这样dict字典库中的操作就不会触发渐进rehash（if (d->iterators == 0) dictRehash(d,1);），
        从而在一定程度上消除rehash影响(但是无法阻止用户删除元素)保证了字典结构的稳定。
        如果在不安全的迭代器中进行dictAdd、dictDelete、dictFind等方法，
        这些方法会判断当前dict是否在rehash（if (dictIsRehashing(d)) _dictRehashStep(d);）来调用一次_dictRehashStep，
        在不安全的迭代器中dict的iterators属性不会自增，因此为0，if (d->iterators == 0)判断成功，
        调用 dictRehash(d,1);，然后进行了rehash操作，会导致不可预期的问题，
        比如遍历重复元素或漏掉元素。也就是说在不安全的迭代器中调用dictAdd、dictDelete、dictFind这些方法会导致rehash，使遍历的元素不可预期。
    */

    typedef void (dictScanFunction)(void *privdata, const dictEntry *de);

    /* This is the initial size of every hash table */
    #define DICT_HT_INITIAL_SIZE     4

    /* ------------------------------- Macros ------------------------------------*/

    //获取键
    #define dictGetKey(de) ((de)->key)

    //获取值
    #define dictGetVal(de) ((de)->v.val)

    //获取字典大小
    #define dictSize(d) ((d)->ht[0].used+(d)->ht[1].used)

    //释放键内存
    #define dictFreeKey(d, entry) \
        if ((d)->type->keyDestructor) \
            (d)->type->keyDestructor((d)->privdata, (entry)->key)

    //释放值内存
    #define dictFreeVal(d, entry) \
        if ((d)->type->valDestructor) \
            (d)->type->valDestructor((d)->privdata, (entry)->v.val)

    //是否在进行 Rehash 操作
    #define dictIsRehashing(d) ((d)->rehashidx != -1)

    // 哈希操作
    #define dictHashKey(d, key) (d)->type->hashFunction(key)

    //键比较
    #define dictCompareKeys(d, key1, key2) \
        (((d)->type->keyCompare) ? \
             (d)->type->keyCompare((d)->privdata, key1, key2) : \
             (key1) == (key2))

    //键的赋值
    #define dictSetKey(d, entry, _key_) do { \
        if ((d)->type->keyDup) \
            (entry)->key = (d)->type->keyDup((d)->privdata, _key_); \
        else \
            (entry)->key = (_key_); \
     } while(0)

    //值得赋值
    #define dictSetVal(d, entry, _val_) do { \
        if ((d)->type->valDup) \
            (entry)->v.val = (d)->type->valDup((d)->privdata, _val_); \
        else \
            (entry)->v.val = (_val_); \
    } while(0)

    /* ------------------------------- API ------------------------------------*/

    //创建字典
    dict *dictCreate(dictType *type, void *privDataPtr);

    //添加键值对
    int dictAdd(dict *d, void *key, void *val);

    //添加或替换
    int dictAddOrReplace(dict *d, void *key, void *val);

    //删除键值对
    int dictDelete(dict *d, const void *key);

    //查找
    dictEntry *dictFind(dict *d, const void *key);

    //提取键的值
    void *dictFetchValue(dict *d, const void *key);

    //释放字典
    void dictRelease(dict *d);

    //重置size
    int dictResize(dict *d);

    //获取迭代器
    dictIterator *dictGetIterator(dict *d);

    //获取安全迭代器
    dictIterator *dictGetSafeIterator(dict *d);

    //迭代器下一个
    dictEntry *dictNext(dictIterator *iter);

    //获取键的离散值
    unsigned long long dictGetHash(dict *d, const void *key);

    //字典桶扫描
    unsigned long long dictScan(dict *d, unsigned long v, dictScanFunction *fn, void *privdata);

#ifdef __cplusplus
}
#endif

#endif /* __GTC_DICT_H_ */
