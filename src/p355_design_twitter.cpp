/*Author: Md Shaifur Rahman
 * Date: March 26, 2026
 * Source: www.github.com/shaifurcodes
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <tuple>
#include <queue>

/**
 * Twitter design supporting:
 * - Posting tweets
 * - Following/unfollowing users
 * - Retrieving news feed (top 10 most recent tweets)
 *
 * Internally uses:
 * - Hash maps for user relationships
 * - Vector per user to store tweets in chronological order
 * - Max-heap (priority_queue) for merging tweet streams
 */
class Twitter {
 private:
  // Global timestamp to ensure unique ordering of tweets
  int time_;

  /**
   * Represents a single tweet.
   * Stores:
   * - time: global timestamp
   * - id: tweet identifier
   */
  struct Tweet {
    int time;
    int id;

    Tweet(int t, int i) : time(t), id(i) {}
  };

  // Maps userId -> list of their tweets (ordered by time)
  std::unordered_map<int, std::vector<Tweet>> tweets_;

  // Maps followerId -> set of followees
  std::unordered_map<int, std::unordered_set<int>> follow_map_;

 public:
  /**
   * Constructor initializes global timestamp.
   */
  Twitter() : time_(0) {}

  /**
   * Posts a new tweet for a given user.
   * Time complexity: O(1)
   */
  void postTweet(int userId, int tweetId) {
    tweets_[userId].emplace_back(++time_, tweetId);
  }

  /**
   * Retrieves the 10 most recent tweets in the user's news feed.
   *
   * Approach:
   * - Use a max heap to merge tweet streams from:
   *   - The user
   *   - All followees
   * - Each entry in heap stores:
   *   (tweet time, tweet id, user id, index in user's tweet list)
   *
   * Time complexity:
   * O(10 * log(F)) where F = number of followees + user
   */
  std::vector<int> getNewsFeed(int userId) {
    std::vector<int> result;
    result.reserve(10);

    auto it = tweets_.find(userId);

    // Heap node:
    // (tweetTime, tweetId, userId, indexInUserTweetVector)
    using Node = std::tuple<int, int, int, size_t>;

    // Comparator for max-heap based on tweet time
    auto cmp = [](const Node& a, const Node& b) {
      return std::get<0>(a) < std::get<0>(b);
    };

    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);

    /**
     * Push the most recent tweet of a user into the heap.
     */
    auto push_latest = [&](int uid) {
      auto it = tweets_.find(uid);
      if (it != tweets_.end() && !it->second.empty()) {
        size_t idx = it->second.size() - 1;
        const Tweet& t = it->second[idx];
        pq.emplace(t.time, t.id, uid, idx);
      }
    };

    // Push user's own latest tweet
    push_latest(userId);

    // Push latest tweets of all followees
    auto it_follow = follow_map_.find(userId);
    if (it_follow != follow_map_.end()) {
      for (int followee : it_follow->second) {
        push_latest(followee);
      }
    }

    /**
     * Extract up to 10 most recent tweets using k-way merge.
     */
    while (!pq.empty() && result.size() < 10) {
      auto [twt_time, twt_id, uid, idx] = pq.top();
      pq.pop();

      result.push_back(twt_id);

      // Push next older tweet from same user (if exists)
      if (idx > 0) {
        const auto& vec = tweets_.at(uid);
        const Tweet& next_tweet = vec[idx - 1];
        pq.emplace(next_tweet.time, next_tweet.id, uid, idx - 1);
      }
    }

    return result;
  }

  /**
   * Follows a user.
   * Ignores self-follow.
   */
  void follow(int followerId, int followeeId) {
    if (followerId == followeeId) return;

    follow_map_[followerId].insert(followeeId);
  }

  /**
   * Unfollows a user if relationship exists.
   */
  void unfollow(int followerId, int followeeId) {
    auto it = follow_map_.find(followerId);
    if (it != follow_map_.end()) {
      it->second.erase(followeeId);
    }
  }
};

int main(){
    return 0;
}
