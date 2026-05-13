#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <queue>

using namespace std;

#define WIDTH 20
#define HEIGHT 20


struct Point {
    int x, y;
    Point(): x(0), y(0) {;}
    Point(int a, int b): x(a), y(b) {;}
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
};
const Point DIRECTIONS[] = {
    Point(-1,  0),
    Point( 0, -1),
    Point( 1,  0),
    Point( 0,  1),
};

enum State {
    OBSTACLE,
    FOOD,
    HEAD,
    BODY,
    WALL,
    BLANK,
};

State transfer(char c) {
    switch (c) {
        case 'O': return OBSTACLE;
        case 'F': return FOOD;
        case 'H': return HEAD;
        case 'B': return BODY;
        case '#': return WALL;
        case '.': return BLANK;
        default:
            throw invalid_argument("不合法的地图组成");
    }
}
 
class Snake {
private:
    queue<Point> body;
public:
    Snake(): body(queue<Point>()) {;}
    Snake(queue<Point> b): body(b) {;}

};

class Game {
private:
    vector<vector<State>> map;
    Snake snake;
    bool isWall(string line) {
        for(char c: line) if(c != '#') return false;
        return true;
    }

    vector<State> transLine(string l) {
        vector<State> row;
        for(char c: l)row.push_back(transfer(c));
        return row;
    }

    vector<vector<State>> readTerminal() {
        vector<vector<State>> map;
        int width;
        string line;
        cin >> line;
        width = line.length();
        if(!isWall(line)) throw invalid_argument("不合法的地图边界");
        map.push_back(transLine(line));
        
        do {
            cin >> line;
            if(line.length() != width) throw invalid_argument("地图非矩形");
            map.push_back(transLine(line));   
        } while(!isWall(line));
        return map;
    }

    Snake findSnake(Point head) {
        Point body, tail;
        for(Point dir: DIRECTIONS) {
            Point pos = head + dir;
            if(map[pos.x][pos.y] == BODY) {
                body = pos;
                break;
            }
        }
        for(Point dir: DIRECTIONS) {
            Point pos = body + dir;
            if(map[pos.x][pos.y] == BODY) {
                tail = pos;
                break;
            }
        }
        queue<Point> s;
        s.push(tail);
        s.push(body);
        s.push(head);
        return Snake(s);
    }

public:
    Game() {
        map = readTerminal();
        for(int i = 0; i < map.size(); i++) {
            for(int j = 0; j < map[i].size(); j++) {
                if(map[i][j] == HEAD) {
                    snake = findSnake(Point(i, j));
                    return;
                } 
            }
        }
    }
};




int main() {
    Game game = Game();

    
}