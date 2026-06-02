#include <map>
#include <vector>
#include <string>
using namespace std;

/**
 * In-memory file system supporting hierarchical directory and file operations.
 *
 * Design:
 *   - Each node in the trie represents either a directory or a file.
 *   - Directory nodes have children; file nodes carry content.
 *   - std::map is used for children to maintain lexicographic ordering,
 *     which allows ls() to return sorted results without an explicit sort.
 *   - Memory is managed via raw pointers with a recursive destructor —
 *     deleting a node cascades down and frees the entire subtree.
 *
 * Time complexity:
 *   - All operations: O(d) traversal where d = number of path components.
 *   - ls() on a directory: O(k) where k = number of direct children.
 *
 * Space complexity: O(N) where N = total number of nodes in the trie.
 */
class FileSystem {

    /**
     * Trie node representing a single path component (directory or file).
     * Owns its children — destructor recursively frees the entire subtree.
     */
    struct Node {
        string content;               // non-empty only for file nodes
        bool is_file = false;         // distinguishes files from directories
        map<string, Node*> children;  // sorted map for lexicographic ls()

        // Recursively deletes all children, cascading down the subtree.
        ~Node() {
            for (auto& [key, n] : children)
                delete n;
        }
    };

    Node* root; // root of the trie, represents "/"

    /**
     * Walks the trie along the given absolute path, creating missing nodes
     * along the way (mkdir -p semantics).
     *
     * @param path  Absolute path starting with '/'.
     * @return      Pointer to the node at the end of the path.
     */
    Node* traverse(const string& path) {
        Node* cur_node = root;
        int i = 1;                   // skip leading '/'
        int n = path.size();
        while (i < n) {
            int j = path.find('/', i);
            if (j == (int)string::npos) j = n;  // no more '/' — last component
            string cur_name = path.substr(i, j - i);
            if (!cur_node->children.count(cur_name))
                cur_node->children[cur_name] = new Node();
            cur_node = cur_node->children[cur_name];
            i = j + 1;
        }
        return cur_node;
    }

public:

    // Initializes the file system with an empty root directory.
    FileSystem() : root(new Node()) {}

    // Frees the entire trie via cascading Node destructors.
    ~FileSystem() { delete root; }

    /**
     * Lists the contents of a directory, or returns the filename if path
     * points to a file.
     *
     * @param path  Absolute path to a file or directory.
     * @return      Sorted list of names at the given path.
     */
    vector<string> ls(const string& path) {
        Node* cur_node = traverse(path);
        if (cur_node->is_file)
            return { path.substr(path.rfind('/') + 1) };  // return just the filename
        vector<string> file_list;
        for (auto& [name, n] : cur_node->children)
            file_list.push_back(name);  // map guarantees sorted order
        return file_list;
    }

    /**
     * Creates a directory and all intermediate directories along the path
     * (equivalent to mkdir -p).
     *
     * @param path  Absolute path of the directory to create.
     */
    void mkdir(const string& path) {
        traverse(path);  // traverse creates missing nodes as a side effect
    }

    /**
     * Appends content to a file, creating it and any intermediate
     * directories if they do not already exist.
     *
     * @param path     Absolute path to the file.
     * @param content  Content to append.
     */
    void addContentToFile(const string& path, const string& content) {
        Node* cur_node = traverse(path);
        cur_node->content += content;
        cur_node->is_file = true;
    }

    /**
     * Reads and returns the full content of a file.
     *
     * @param path  Absolute path to the file.
     * @return      The file's content as a string.
     */
    string readContentFromFile(const string& path) {
        return traverse(path)->content;
    }
};

int main() {
    return 0;
}
