
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include "jute/jute.h"
using namespace std;

class class_user {
public:
    int hp;
    int damage;
    int speed;
    int x;
    int y;
    void attack(int damage){

    }

    void move(int speed) {

    }
};
class class_enemy {
public:
    int hp;
    int damage;
    int speed;
    int x;
    int y;
    void attack(int damage) {

    }

    void move(int speed) {

    }
};
int main() {

    ifstream in("jute/data.json");
    string str = "";
    string tmp;
    while (getline(in, tmp)) str += tmp;
    jute::jValue v = jute::parser::parse(str);
    
    for (int stage = 0; stage < 3;stage++) {
        cout << v["game_data"]["stages"][stage]["stage_name"].as_string() << endl;
    }
   
    
    return 0;
}

