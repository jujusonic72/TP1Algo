#ifndef HashMap_h
#define HashMap_h

#include "Entry.h"
#include <list>
#include <vector>
#include <exception>

template <typename K, typename V, typename H>
class HashMap
{
public:
    typedef Entry<const K,V> Entry;
    class Iterator;
    
public:
    HashMap(int = 100);
    int size() const;
    bool empty() const;
    Iterator find(const K&);
    Iterator put(const K&, const V&);
    void erase(const K&);
    void erase(const Iterator&);
    Iterator begin();
    Iterator end();
    HashMap<K,V,H> operator=(const std::list<std::pair<K, V>> l);
    
protected:
    typedef std::list<Entry> Bucket;
    typedef std::vector<Bucket> BucketArray;
    typedef typename Bucket::iterator EntryItor;
    typedef typename BucketArray::iterator BucketItor;
    
    // for future HashDict
    Iterator finder(const K&);
    Iterator inserter(const Iterator&, const Entry&);
    void eraser(const Iterator&);
    
    static void nextEntry(Iterator& p) { ++(p.entryItor); }
    static bool isEndOfCurrentBucket(const Iterator& p) { return p.entryItor == p.bucketItor->end(); } //dont forget: p.bucketItor->end() == (*(p.bucketItor)).end() == Bucket.end()
    
private:
    int nbEntries;
    H hash;
    BucketArray bucketArray;
    
public:
  
    class Iterator
    {
    public:
        Iterator(const BucketArray&, const BucketItor&, const EntryItor& q);//TODO make private
        Entry& operator*() const;
        bool operator==(const Iterator&) const;
        Iterator& operator++();
        friend class HashMap;
    private:
        EntryItor entryItor;
        BucketItor bucketItor;
        const BucketArray* bucketArrayPtr;
    };
};

template <typename K, typename V, typename H>
HashMap<K,V,H>::HashMap(int nbBuckets):
nbEntries(0),
hash(),
bucketArray(nbBuckets)
{
}

template <typename K, typename V, typename H>
int HashMap<K,V,H>::size() const
{
    return nbEntries;
}

template <typename K, typename V, typename H>
bool HashMap<K,V,H>::empty() const
{
    return size() == 0;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::inserter(const Iterator& p, const Entry& e)
{
    // insert into bucket (list of Entry) before p
    EntryItor entryItor = p.bucketItor->insert(p.entryItor, e);//reminder: std::list.insert -> Inserts a copy of value before pos.
    nbEntries++;
    return Iterator(bucketArray, p.bucketItor, entryItor);
}
 
template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::put(const K& k, const V& v)
{
    Iterator p = finder(k);
    if (isEndOfCurrentBucket(p))
    {
        // k not found? insert at end of bucket
        return inserter(p, Entry(k, v));
    }
    else
    {
        // found it? replace value with v
        p.entryItor->setValue(v);
        return p;
    }
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::finder(const K& k)
{
    // get the bucket
    unsigned long hashIndex = hash(k) % bucketArray.size();
    BucketItor bucket = bucketArray.begin() + hashIndex;
    
    // search for k
    Iterator p(bucketArray, bucket, bucket->begin());
    while (!isEndOfCurrentBucket(p) && (*p).key() != k)
    {
        nextEntry(p);
    }
    return p;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::find(const K& k)
{
    Iterator p = finder(k);
    if (isEndOfCurrentBucket(p))
    {
        return end();
    }
    else
    {
        return p;
    }
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::eraser(const Iterator& p)
{
    // remove entry from bucket (erase from list of Entry)
    p.bucketItor->erase(p.entryItor);
    nbEntries--;
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::erase(const Iterator& p)
{
    eraser(p);
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::erase(const K& k)
{
    Iterator p = finder(k);
    if (isEndOfCurrentBucket(p))
    {
        throw std::runtime_error("Erase of nonexistent");
    }
    eraser(p);
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::end()
{
    return Iterator(bucketArray, bucketArray.end(), EntryItor());
}
  
template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::begin()
{
    if(empty())
    {
        return end();
    }
    
    // find nonempty bucket
    BucketItor bucketItor = bucketArray.begin();
    while (bucketItor->empty())
    {
        ++bucketItor;
    }
    
    return Iterator(bucketArray, bucketItor, bucketItor->begin());
}

///Iterator

template <typename K, typename V, typename H>
HashMap<K,V,H>::Iterator::Iterator(const BucketArray& a, const BucketItor& b, const EntryItor& q):
entryItor(q),
bucketItor(b),
bucketArrayPtr(&a)
{
}

template <typename K, typename V, typename H>
bool HashMap<K,V,H>::Iterator::operator==(const Iterator &other) const
{
    if (bucketArrayPtr != other.bucketArrayPtr || bucketItor != other.bucketItor)
    {
        return false;
    }
    else if (bucketItor == bucketArrayPtr->end())
    {
        // both at the end? (entryItor is invalid)
        return true;
    }
    else
    {
        // else use entry to decide
        return (entryItor == other.entryItor);
    }
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator& HashMap<K,V,H>::Iterator::operator++()
{
    // next entry in bucket
    ++entryItor;
    
    if (isEndOfCurrentBucket(*this))
    {
        // go to next bucket
        ++bucketItor;
        
        // find nonempty bucket
        while (bucketItor != bucketArrayPtr->end() && bucketItor->empty())
        {
            ++bucketItor;
        }
            
        if (bucketItor == bucketArrayPtr->end())
        {
            // we are at the end, return!
            return *this;
        }
            
        // set entryItor to first entry of new bucket
        entryItor = bucketItor->begin();
    }
    
    return *this;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Entry& HashMap<K,V,H>::Iterator::operator*() const
{
    return *entryItor;
}

template <typename K, typename V, typename H>
HashMap<K,V,H> HashMap<K,V,H>::operator=(const std::list<std::pair<K, V>> l){
    HashMap<K,V,H> hm = HashMap(l.size()*2);
    auto it = l.begin();
    for (int i = 0; i < l.size(); i++)
    {
        auto item = *it;
        put(item.first, item.second);
        ++it;
    }
    return hm;
}

#endif /* HashMap_h */