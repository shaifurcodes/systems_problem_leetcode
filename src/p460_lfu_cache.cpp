/*Author: Md Shaifur Rahman 
 *Date: March 21, 2026
 * Source: www.github.com/shaifurcodes
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
