#include <iostream>
#include <vector>
using namespace std;

class Trie {
private:
  struct TrieNode {
    bool isEnd = false;                   // 是否是结尾
    vector<TrieNode *> next{26, nullptr}; // 对应26个字符
  };
  TrieNode *root;

public:
  Trie() { 
    root = new TrieNode(); // 初始根结点
}

  void insert(string word) {
    TrieNode *p = root;
    for (char ch : word) {
      int index = ch - 'a';
      if(p->next[index]){
        // 直接移动到已有结点
        p = p->next[index];
      }else{
        // 创建新结点
        p->next[index] = new TrieNode();
        p = p->next[index];
      }
      
    }
    p->isEnd = true;
  }

  TrieNode *search_p(string word) {
    // 寻找字符串对应的最终结点
    TrieNode *p = root;
    for (char ch : word) {
      int index = ch - 'a';
      if (p->next[index]) {
        p = p->next[index];
      } else {
        return nullptr;
      }
    }
    return p;
  }
  bool search(string word) {
    TrieNode *p = search_p(word);
    if (p && p->isEnd) {
        // 需要是终止结点
      return true;
    } else {
      return false;
    }
  }

  bool startsWith(string prefix) {
    TrieNode *p = search_p(prefix);
    if (p) {
      return true;
    } else {
      return false;
    }
  }
};

int main() {
  bool result;
  Trie trie = Trie();
  trie.insert("apple");
  result = trie.search("apple"); // 返回 True
  cout << result;
  result = trie.search("app"); // 返回 False
  cout << result;

  result = trie.startsWith("app"); // 返回 True
  cout << result;

  trie.insert("app");
  result = trie.search("app"); // 返回 True
  cout << result;
}