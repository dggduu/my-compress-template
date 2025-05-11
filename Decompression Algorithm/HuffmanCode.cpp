#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>

using namespace std;

struct Node {
  char value;
  int freq;
  Node* left;
  Node* right;

  explicit Node(char value, int freq)
      : value(value), freq(freq), left(nullptr), right(nullptr) {}
};

struct CompareNode {
  bool operator()(Node* a, Node* b) const {
    return a->freq > b->freq;
  }
};

Node* BuildHuffmanTree(const string& str) {
  unordered_map<char, int> freq_map;
  for (char c : str) {
    ++freq_map[c];
  }

  priority_queue<Node*, vector<Node*>, CompareNode> pq;
  for (const auto& pair : freq_map) {
    pq.push(new Node(pair.first, pair.second));
  }

  while (pq.size() > 1) {
    Node* left = pq.top();
    pq.pop();
    Node* right = pq.top();
    pq.pop();

    Node* parent = new Node('\0', left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
  }

  return pq.empty() ? nullptr : pq.top();
}

void huffmanCodeGen(Node* root, const string& str,
                    unordered_map<char, string>& code_map) {
  if (!root) return;
  if (!root->left && !root->right) {
    code_map[root->value] = str;
  }
  huffmanCodeGen(root->left, str + '0', code_map);
  huffmanCodeGen(root->right, str + '1', code_map);
}

void listHuffmanCode(const unordered_map<char, string>& code_map) {
  if (code_map.empty()) return;

  cout << "Listing huffmanCodeMap:" << endl;
  for (const auto& pair : code_map) {
    cout << "Key: " << pair.first << " : Code: " << pair.second << endl;
  }
  cout << "Work Done!" << endl;
}

string HuffmanCodeEncode(const string& text,
                         const unordered_map<char, string>& huffman_code) {
  if (text.empty() || huffman_code.empty()) return "";

  string encoded_str;
  for (char c : text) {
    auto it = huffman_code.find(c);
    if (it != huffman_code.end()) {
      encoded_str += it->second;
    }
  }
  return encoded_str;
}

string HuffmanCodeDecode(const string& str, Node* root) {
  if (!root || str.empty()) return "";

  string decoded_text;
  Node* cur = root;

  for (char c : str) {
    if (c == '0' && cur->left != nullptr) {
      cur = cur->left;
    } else if (c == '1' && cur->right != nullptr) {
      cur = cur->right;
    }

    if (!cur->left && !cur->right) {
      decoded_text += cur->value;
      cur = root;
    }
  }

  return decoded_text;
}

void deleteHuffmanTree(Node* root) {
  if (!root) return;
  deleteHuffmanTree(root->left);
  deleteHuffmanTree(root->right);
  delete root;
}

int main() {
  string input_str;
  cout << "Input str: ";
  cin >> input_str;

  Node* root = BuildHuffmanTree(input_str);
  unordered_map<char, string> huffman_code_map;
  huffmanCodeGen(root, "", huffman_code_map);

  listHuffmanCode(huffman_code_map);

  string encoded_str = HuffmanCodeEncode(input_str, huffman_code_map);
  cout << "Encoded:\n" << encoded_str << endl;

  string decoded_str = HuffmanCodeDecode(encoded_str, root);
  cout << "Decoded:\n" << decoded_str << endl;

  deleteHuffmanTree(root);
  return 0;
}