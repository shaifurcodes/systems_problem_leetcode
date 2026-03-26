/* 
 * Author: Md Shaifur Rahman
 * Date: March 25, 2026
 * Source: www.github.com/shaifurcodes
 *
 * Description:
 *   AllOne Data Structure supporting O(1):
 *     - inc(key)
 *     - dec(key)
 *     - getMaxKey()
 *     - getMinKey()
 *
 * Design:
 *   - Doubly-linked list of frequency buckets (BucketList).
 *   - Each bucket stores keys with the same frequency.
 *   - Hash map maps key -> (bucket iterator, key iterator).
 *
 * Invariants:
 *   - Buckets are ordered by frequency (ascending).
 *   - Each key exists in exactly one bucket.
 *   - No empty buckets exist.
 */

#include <string>
#include <list>
#include <unordered_map>
#include <iterator>

class AllOne {
 private:
  // Forward declarations.
  struct FreqBucket;
  struct KeyInfo;

  // Map from key -> metadata (bucket + position in bucket).
  using MapType = std::unordered_map<std::string, KeyInfo>;
  using MapIt = MapType::iterator;

  // Doubly-linked list of frequency buckets.
  using BucketList = std::list<FreqBucket>;
  using BucketIt = BucketList::iterator;

  // Each bucket stores iterators to keys in the map.
  using KeyList = std::list<MapIt>;
  using KeyIt = KeyList::iterator;

  // Represents a bucket of keys with the same frequency.
  struct FreqBucket {
    int freq;          // Frequency count.
    KeyList key_itrs;  // Keys belonging to this frequency.

    explicit FreqBucket(int f) : freq(f) {}
  };

  // Stores metadata for each key.
  struct KeyInfo {
    BucketIt bucket_it;  // Pointer to its frequency bucket.
    KeyIt key_it;        // Position inside bucket's key list.
  };

  BucketList buckets_;  // Sorted list of frequency buckets.
  MapType key_map_;     // Key -> KeyInfo mapping.

 public:
  // Increments the frequency of a key.
  // If key does not exist, insert with frequency = 1.
  void inc(const std::string& key) {
    MapIt map_it = key_map_.find(key);

    // Case 1: New key.
    if (map_it == key_map_.end()) {
      // Ensure frequency-1 bucket exists at front.
      if (buckets_.empty() || buckets_.front().freq != 1) {
        buckets_.emplace_front(1);
      }

      // Insert into map.
      MapIt new_map_it =
          key_map_.emplace(key, KeyInfo{buckets_.begin(), {}}).first;

      // Add key to bucket.
      buckets_.front().key_itrs.emplace_front(new_map_it);
      new_map_it->second.key_it = buckets_.front().key_itrs.begin();
    } 
    // Case 2: Existing key → move to next frequency bucket.
    else {
      KeyInfo& cur_key_info = map_it->second;
      BucketIt cur_bucket_it = cur_key_info.bucket_it;
      int cur_freq = cur_bucket_it->freq;

      // Get next bucket (freq + 1).
      BucketIt next_bucket_it = std::next(cur_bucket_it);

      // Create next bucket if needed.
      if (next_bucket_it == buckets_.end() ||
          next_bucket_it->freq != cur_freq + 1) {
        next_bucket_it =
            buckets_.emplace(next_bucket_it, cur_freq + 1);
      }

      // Move key to next bucket.
      next_bucket_it->key_itrs.splice(
          next_bucket_it->key_itrs.begin(),
          cur_bucket_it->key_itrs,
          cur_key_info.key_it);

      // Update key metadata.
      cur_key_info.bucket_it = next_bucket_it;
      cur_key_info.key_it = next_bucket_it->key_itrs.begin();

      // Remove old bucket if empty.
      if (cur_bucket_it->key_itrs.empty()) {
        buckets_.erase(cur_bucket_it);
      }
    }
  }

  // Decrements the frequency of a key.
  // Removes the key if its frequency becomes 0.
  void dec(const std::string& key) {
    MapIt map_it = key_map_.find(key);
    if (map_it == key_map_.end()) return;

    KeyInfo& cur_info = map_it->second;
    BucketIt cur_bucket_it = cur_info.bucket_it;
    int cur_freq = cur_bucket_it->freq;

    // Case 1: Frequency becomes 0 → remove key completely.
    if (cur_freq == 1) {
      cur_bucket_it->key_itrs.erase(cur_info.key_it);

      if (cur_bucket_it->key_itrs.empty()) {
        buckets_.erase(cur_bucket_it);
      }

      key_map_.erase(map_it);
    } 
    // Case 2: Move key to previous frequency bucket.
    else {
      BucketIt prev_bucket_it;

      // Find previous bucket.
      if (cur_bucket_it != buckets_.begin()) {
        prev_bucket_it = std::prev(cur_bucket_it);
      } else {
        prev_bucket_it = buckets_.end();
      }

      // Create bucket if missing.
      if (prev_bucket_it == buckets_.end() ||
          prev_bucket_it->freq != cur_freq - 1) {
        prev_bucket_it =
            buckets_.emplace(cur_bucket_it, cur_freq - 1);
      }

      // Move key to previous bucket.
      prev_bucket_it->key_itrs.splice(
          prev_bucket_it->key_itrs.begin(),
          cur_bucket_it->key_itrs,
          cur_info.key_it);

      // Update metadata.
      map_it->second.bucket_it = prev_bucket_it;
      map_it->second.key_it = prev_bucket_it->key_itrs.begin();

      // Remove old bucket if empty.
      if (cur_bucket_it->key_itrs.empty()) {
        buckets_.erase(cur_bucket_it);
      }
    }
  }

  // Returns any key with maximum frequency.
  std::string getMaxKey() {
    if (!buckets_.empty()) {
      return buckets_.back().key_itrs.front()->first;
    }
    return "";
  }

  // Returns any key with minimum frequency.
  std::string getMinKey() {
    if (!buckets_.empty()) {
      return buckets_.front().key_itrs.front()->first;
    }
    return "";
  }
};

int main() {
  return 0;
}
