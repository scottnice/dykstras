#include <iostream>
#include <queue>
#include <bitset>
#include <unordered_map>

const int ROWS = 10;
const int COLS = 20;
std::bitset<ROWS*COLS> checked;
    
class Node {
    public:
    int x;
    int y;
    int g;
    int f;
    Node(int x, int y, int g, int f) : x(x), y(y), g(g), f(f){}
    Node(const Node& n) : x(n.x), y(n.y), g(n.g), f(n.f){}
    Node() : x(0), y(0), g(0), f(0){}
    bool operator==(const Node& n1) const {
        return this->x == n1.x && this->y == n1.y;
    }
};

template <>
struct std::hash<Node>
{
  std::size_t operator()(const Node& k) const
  {
    using std::size_t;
    using std::hash;
    using std::string;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return ((hash<int>()(k.x)
             ^ (hash<int>()(k.y) << 1)));
  }
};

int calculateMDistance(Node c, Node d){
    return c.x - d.x + c.y - d.y;
}

bool isValid(int x, int y, char nodes[ROWS][COLS]){
    return x >= 0 && x < ROWS && y >= 0 && y < COLS
    && nodes[x][y] != '#';
}

bool isChecked(Node n){
    return checked.test(n.x * COLS + n.y);
}

void setChecked(Node n){
    checked.set(n.x * COLS + n.y, true);
}

std::vector<Node> neighbors(Node current, char nodes[ROWS][COLS]){
    int y = current.y;
    int x = current.x;
    int left = current.x -1;
    int right = current.x + 1;
    int bottom = current.y + 1;
    int top = current.y -1;
    
    std::vector<Node> vNodes;
    
    if(isValid(left, y, nodes)){
        vNodes.push_back(Node(left, y, 0, current.f + 1));
    }
    if(isValid(right, y, nodes)){
        vNodes.push_back(Node(right, y, 0, current.f + 1));
    }
    if(isValid(x, top, nodes)){
        vNodes.push_back(Node(x, top, 0, current.f + 1));
    }
    if(isValid(x, bottom, nodes)){
        vNodes.push_back(Node(x, bottom, 0, current.f + 1));
    }
    return vNodes;
}

std::vector<Node> dijkstra(Node start, Node end, char nodes[ROWS][COLS]){
    std::unordered_map<Node, int> dist;
    std::unordered_map<Node, Node> prev;
    auto cmp = [](Node left, Node right) { return (left.f) < (right.f); };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> q(cmp);
    q.push(start);
    dist[start] = 0;
    while(!q.empty()){
        Node top = q.top();
        if(top == end){
            break;
        }
        q.pop();
        
        if(isChecked(top))
            continue;
            
        setChecked(top);
        auto n = neighbors(top, nodes);
        
        for(auto&& v : n){
            q.push(v);
            auto searchD = dist.find(v);
             if(searchD == dist.end() || v.f < searchD->second) {
                dist[v] = v.f;
                prev[v] = top;
            }
        }
    }
    
    std::vector<Node> result;
    auto search = prev.find(end); 
    if(search != prev.end()){
        result.push_back(end);
    }
    while(search != prev.end()){
        // path exists
        result.push_back(search->second);
        search = prev.find(search->second);
    }
    
    return result;
}

void outputNodes(char n[ROWS][COLS]){
    for(int i = 0; i < ROWS; ++ i){
        for(int j = 0; j < COLS; ++j){
            std::cout << n[i][j];
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

int main()
{
    char nodes[ROWS][COLS] = {
        {
            '.','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'
        },
        {
            '.','#','.','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#','.'
        },
        {
            '.','#','.','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#','.'
        },
        {
            '.','#','.','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#','.'
        },
        {
            '.','.','.','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#','.'
        },
        {
            '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'
        },
        {
            '.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'
        },
        {
            '.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'
        },
        {
            '.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'
        },
        {
            '.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'
        }
    };
    std::cout << "Djikstra" << "\n" << std::flush;
    auto v = dijkstra(Node(0, 0, 0, 0), Node(ROWS-1,COLS/2,0,0), nodes);
    outputNodes(nodes);
    std::cout << "====== PATH FOUND ========\n";
    for(auto&& n : v){
        nodes[n.x][n.y] = 'x';
    }
    outputNodes(nodes);
    return 0;
}
