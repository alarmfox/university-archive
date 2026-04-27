#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

struct Graph {

  struct Node {
    int value;
    vector<Node *> adj;
    Node(const int value) : value(value){};
  };

  unordered_map<int, unique_ptr<Node>> nodes;

  vector<int> V() const {
    vector<int> res;
    for (const auto &node : nodes) {
      res.push_back(node.first);
    }
    return res;
  }

  inline int size() const { return nodes.size(); }

  vector<Node *> adj(const int u) {
    Node *v = nodes[u].get();

    if (v == nullptr) {
      return vector<Node *>();
    }
    return v->adj;
  }

  void insertNode(int n) {
    if (nodes[n] == nullptr) {
      nodes[n] = unique_ptr<Node>(new Node(n));
    }
  }

  void insertEdge(int u, int v, const int w = 0) {
    insertNode(u);
    insertNode(v);
    Node *n = nodes[u].get();
    n->adj.push_back(nodes[v].get());
  }
};

void bfs(Graph &g, int r) {
  bool visited[g.size()];
  for (int i = 0; i < g.size(); i++) {
    visited[i] = false;
  }

  queue<int> q;
  q.push(r);
  visited[r] = true;

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    cout << u << ' ';
    for (const auto &v : g.adj(u)) {
      if (!visited[v->value]) {
        q.push(v->value);
        visited[v->value] = true;
      }
    }
  }
}

void dfs(Graph &g, int r) {
  bool visited[g.size()];
  for (int i = 0; i < g.size(); i++) {
    visited[i] = false;
  }
  stack<int> s;
  s.push(r);
  visited[r] = true;
  while (!s.empty()) {
    int u = s.top();
    s.pop();
    visited[u] = true;
    cout << u << "-";
    for (const auto &v : g.adj(u)) {
      if (!visited[v->value]) {
        s.push(v->value);
      }
      cout << v->value << "-";
    }
    cout << endl;
  }
}

int main() {
  Graph g = Graph();

  g.insertEdge(3, 1);
  g.insertEdge(6, 1);
  g.insertEdge(1, 6);
  g.insertEdge(1, 2);
  g.insertEdge(1, 3);
  g.insertEdge(3, 6);

  dfs(g, 1);

  return 0;
}
