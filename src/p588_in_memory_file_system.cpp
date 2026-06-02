#include<map>
#include<vector>
#include<string>
using namespace std;

class FileSystem{
	struct Node{
		string content;
		bool is_file = false;
		map<string, Node*> children;
		~Node(){
			for(auto& [ key, n] : children)
				delete n ;
		}
	};
	Node* root;
	Node* traverse(const string& path){
		Node* cur_node = root;
		int i = 1;
		int n = path.size();
		while(i<n){
			int j = path.find('/', i);
		 	if(j==(int)string::npos) j = n;
			string cur_name = path.substr(i, j - i);
			if(!cur_node->children.count(cur_name))
				cur_node->children[cur_name] = new Node();	
			cur_node = cur_node->children[cur_name];
			i = j+1;
		}
		return cur_node;
	}
	public:
		FileSystem(){
			root = new Node();
		} 
		~FileSystem(){
			delete root;
		}
		vector<string> ls(const string& path){
			vector<string> file_list; 	
			Node* cur_node = traverse(path);
			if(cur_node->is_file) 
				return { path.substr( path.rfind('/')+1 ) };
			for(auto& [name, n] : cur_node->children)
				file_list.push_back(name);	
			return file_list;
		}
		void mkdir(const string& path){
			traverse(path);
		}
		void addContentToFile(const string& path, const string & content){
			Node* cur_node = traverse(path);
			cur_node->content += content;
			cur_node->is_file = true; 
		}
		string readContentFromFile(const string& path){
			Node* cur_node = traverse(path);
			return cur_node->content;
		}

};	


int main(){
	return 0;
}
