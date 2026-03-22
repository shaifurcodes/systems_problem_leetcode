/*Author: Md Shaifur Rahman 
 *Date: March 21, 2026
 * Source: www.github.com/shaifurcodes
 */
/*
 * LFU (Least Frequently Used) Cache
 *
 * The LFU cache evicts the key with the lowest access frequency.
 * If multiple keys have the same frequency, the least recently used (LRU)
 * key among them is evicted.
 *
 * Operations:
 * 1. get(key):
 *    - If key exists, return its value.
 *    - Increase its frequency count.
 *    - Move it to the corresponding higher frequency list.
 *
 * 2. put(key, value):
 *    - If key exists:
 *        - Update its value.
 *        - Increase its frequency.
 *    - If key does not exist:
 *        - If cache is full:
 *            - Evict the least frequently used key.
 *            - If tie, evict least recently used among them.
 *        - Insert new key with frequency = 1.
 *
 * Key Idea:
 * - Maintain frequency buckets.
 * - Track minimum frequency (min_freq) for O(1) eviction.
 *
 * Complexity:
 * - get(): O(1)
 * - put(): O(1)
 */
/*
 * Data Structures Used:
 *
 * 1. unordered_map<int, pair<int, list<KV>::iterator>> cache;
 *    - Maps key → (frequency, iterator to its position in frequency list)
 *    - Allows O(1) lookup and update.
 *
 * 2. unordered_map<int, list<pair<int, int>>> frequencies;
 *    - Maps frequency → doubly linked list of (key, value)
 *    - Each list maintains keys with same frequency in LRU order.
 *    - Front = most recently used
 *    - Back = least recently used (candidate for eviction)
 *
 * 3. list<pair<int, int>>
 *    - Stores (key, value) pairs.
 *    - Used because:
 *        - O(1) insertion (emplace_front)
 *        - O(1) deletion using iterator
 *
 * 4. min_freq:
 *    - Tracks the minimum frequency currently present in cache.
 *    - Used to quickly identify which list to evict from.
 *
 *
 * Overall Design:
 *
 *         key → (freq, iterator)
 *                │
 *                ▼
 *     freq → [ (k1,v1), (k2,v2), ... ]
 *
 * - cache gives O(1) access to key location
 * - frequencies groups keys by frequency
 * - list maintains LRU order within same frequency
 */

#include<iostream>
#include<unordered_map>
#include<list>

using namespace std;

class LFUCache{
    int min_freq;
    int capacity;
    using KV = pair<int, int>;
    unordered_map<int, list<KV>> frequencies;
    unordered_map<int, pair<int, list<KV>::iterator>> cache;
    void update_freq(int key){
        /*Remove the entry corresponding to the key from the current freq list to the freq+1 list, 
         * delete freq list if found empty and update min_freq if equals freq*/
        auto cache_it = cache.find(key);
        if(cache_it == cache.end())
            return;
        auto& [freq, freq_it] = cache_it->second;
        auto val = freq_it->second;
        auto& freq_list = frequencies[freq];
        freq_list.erase(freq_it);
        if(freq_list.empty()){
            frequencies.erase(freq);
            if(min_freq == freq)
                min_freq=freq+1;
        }
        auto& new_freq_list = frequencies[freq+1];
        new_freq_list.emplace_front(key, val);
        cache_it->second={freq+1,new_freq_list.begin()};
    }
    public:
    LFUCache(int capacity){
        this->capacity = capacity;
        min_freq = 0;
   }
   void put(int key, int val){
       if(capacity == 0)
           return;
       auto it = cache.find(key);
       //if key already exists, then update value and increment frequency, move the pair to the appropriate freq list, update min_freq
        if(it != cache.end()){
            it->second.second->second = val;
            update_freq(key);
        }
       //otherwise create new pair , add to freq and cache, if capacity exceeded, remove the tail element of min_freq keyed frequencies 
       else{
           if(cache.size() == capacity){
                auto& freq_list = frequencies[min_freq];
                auto [k, v] = freq_list.back();
                cache.erase(k);
                freq_list.pop_back();
                if(freq_list.empty())
                    frequencies.erase(min_freq);
           }
           frequencies[1].emplace_front(key, val);
           cache[key] = {1, frequencies[1].begin()};
           min_freq = 1;
       }
   }
   int get(int key){
       auto it = cache.find(key);
       if(it == cache.end())
           return -1;
       auto ret_val =  it->second.second->second; 
       update_freq(key);
       return ret_val;
  }
};

int main(){
    return 0;
}
