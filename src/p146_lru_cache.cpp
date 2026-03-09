/**
 * Problem: LRU Cache (LeetCode #146)
 * Concept: Doubly linked list + hash map for O(1) access and eviction
 * Description:
 *  Design a cache that supports get and put operations in O(1) time.
 *
 * Algorithm:
 *  1. Use a doubly linked list to track usage order (head = most recent, tail = least recent).
 *  2. Use an unordered_map to store key -> node mapping for O(1) access.
 *  3. get(key): if key exists, move node to front, return value.
 *  4. put(key, value): if key exists, update and move to front; 
 *     else, if cache full, remove tail (LRU) and insert new node at front.
 *
 * Complexity:
 *  Time: O(1) per operation
 *  Space: O(capacity)
 */

#include<iostream>
#include<unordered_map>
using namespace std;

class LRUCache
{
    struct Node{
        Node* prev;
        Node* next;
        int key;
        int value;
        Node(int k, int v):key(k), value(v), prev(nullptr), next(nullptr){}
    };
    int capacity;
    Node *head;
    Node *tail;
    unordered_map<int, Node*> cache;  
    void add_front(Node *node){
        node->next = head->next;
        node->prev = head;
        head->next = node;
        node->next->prev = node;
    }
    void remove(Node *node){
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    void move_to_front(Node *node){
        remove(node);
        add_front(node);
    }
    void remove_lru(){
        Node *lru_node = tail->prev;
        remove(lru_node);
        cache.erase(lru_node->key);
        delete lru_node;
    }

    public:
    LRUCache(int capacity){
        this->capacity = capacity;
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }
    ~LRUCache(){
       Node *node = head; 
       while(head != nullptr){
            node = head;
            head = head->next;
            delete node;
        }
    }
    void put(int k, int v){
        auto it =  cache.find(k);
        if(it != cache.end()){
            it->second->value = v;
            move_to_front(it->second);
        }else{
            if(cache.size() == capacity)
                remove_lru();
            Node *node = new Node( k , v);
            add_front(node);
            cache[k] = node;
        }
    }
    int  get(int k){
        auto it = cache.find(k);
        if(it == cache.end())
            return -1;
        move_to_front(it->second);
        return it->second->value;
    }
};

int main(){
    LRUCache lRUCache(2);
    lRUCache.put(1, 1); // cache is {1=1}
    lRUCache.put(2, 2); // cache is {1=1, 2=2}
    cout<<lRUCache.get(1)<<endl;    // return 1
    lRUCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
    cout<<lRUCache.get(2)<<endl;    // returns -1 (not found)
    lRUCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
    cout<<lRUCache.get(1)<<endl;    // return -1 (not found)
    cout<<lRUCache.get(3)<<endl;    // return 3
    cout<<lRUCache.get(4)<<endl;    // return 4
    return 0;
}
