#include <vector>
#include <unordered_map>
#include <random>

/**
 * A set-like container supporting O(1) average-time insert, remove, and
 * uniformly random element retrieval.
 *
 * Invariant: nums[i] == v  iff  num_to_indx[v] == i
 *
 * Design:
 *   - nums          : contiguous array for O(1) random access via index
 *   - num_to_indx   : hash map from value to its index in nums
 *   - rng           : Mersenne Twister seeded once from OS entropy
 *
 * All three operations are O(1) average. The swap-to-last delete strategy
 * avoids shifting elements — only the removed element and the last element
 * are touched.
 */
class RandomizedSet {
public:
    /**
     * Seeds the Mersenne Twister engine from std::random_device.
     * random_device reads OS entropy (/dev/urandom on macOS/Linux) and is
     * called exactly once — rng handles all subsequent random generation.
     */
    RandomizedSet() : rng(std::random_device{}()) {}

    /**
     * Inserts val into the set if not already present.
     *
     * @param val  value to insert
     * @return     true if val was absent and inserted, false if already present
     *
     * Complexity: O(1) average
     */
    bool insert(int val) {
        if (num_to_indx.count(val) != 0)
            return false;

        // record index before push_back — nums.size() is the next index
        num_to_indx[val] = static_cast<int>(nums.size());
        nums.push_back(val);
        return true;
    }

    /**
     * Removes val from the set if present.
     *
     * Strategy: swap val with the last element, update the last element's
     * index in the map, then pop the last position. This avoids shifting
     * and keeps both structures consistent in O(1).
     *
     *   before remove(2):  nums = [1, 2, 3]  map = {1:0, 2:1, 3:2}
     *   swap 2 with last:  nums = [1, 3, 3]  map = {1:0, 2:1, 3:1}
     *   erase val + pop:   nums = [1, 3]     map = {1:0, 3:1}
     *
     * Edge case: if val is already the last element, the swap is a no-op
     * and the erase + pop correctly remove it without special casing.
     *
     * @param val  value to remove
     * @return     true if val was present and removed, false if absent
     *
     * Complexity: O(1) average
     */
    bool remove(int val) {
        auto it = num_to_indx.find(val);
        if (it == num_to_indx.end())
            return false;

        int last_value  = nums.back();
        int remove_indx = it->second;  // it not used after this line — no rehash risk

        // point last_value to the vacated slot, then overwrite that slot
        num_to_indx[last_value] = remove_indx;
        nums[remove_indx]       = last_value;

        // remove val's entry and shrink the array
        num_to_indx.erase(val);
        nums.pop_back();
        return true;
    }

    /**
     * Returns a uniformly random element from the set.
     *
     * Uniform distribution is guaranteed because each value occupies exactly
     * one slot in nums — random index selection gives equal probability to
     * every element.
     *
     * Precondition: set must be non-empty (undefined behaviour if empty)
     *
     * @return  a random element from the set
     *
     * Complexity: O(1)
     */
    int getRandom() {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(nums.size()) - 1);
        return nums[dist(rng)];
    }

private:
    std::vector<int>             nums;         // values in insertion/swap order
    std::unordered_map<int, int> num_to_indx;  // value → index in nums
    mutable std::mt19937         rng;          // fast PRNG, mutable for const correctness
};
int main(){
	return 0;
}
