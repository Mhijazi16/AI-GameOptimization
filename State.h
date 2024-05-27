#include "Point.h"
#include "Context.h"
#include <iostream>

using namespace std;

struct State{ 
  vector<vector<string>> Map;
  Point Player; 
  vector<Point> Meat;
  vector<Point> Boxes; 
  vector<Point> Bones; 
  string Action = "NULL";
  int Rows = 0; 
  int Cols = 0;

  State() = default;

  bool operator==(const State &other) const {
      return Map == other.Map &&
             Player == other.Player &&
             Meat == other.Meat &&
             Boxes == other.Boxes &&
             Action == other.Action &&
             Rows == other.Rows &&
             Cols == other.Cols;
  }

  string& map_at(Point point){
    return Map[point.x][point.y];
  }

  bool isWall(Point location){
    auto cell = map_at(location);
    for (auto& wall : WALL) {
      if(wall == cell)
        return true; 
    }
    return false;
  }

  bool isBox(Point location){
    for (auto& box : BOX) {
      if(box == map_at(location))
        return true; 
    }
    return false;
  }

  bool isPoint(Point location){
    for (auto& point : POINT) {
      if(point == map_at(location))
        return true; 
    }
    return false;
  }

  bool isSpace(Point& location){
    return map_at(location) == "  ";
  }

  bool isWallorBox(Point location){
    auto& str = map_at(location);
    if (isWall(location) || isBox(location))
      return true;
    return false;
  }

  bool player_on_point(){
    for (auto& point : Meat) 
      if (point == Player)  
        return true;
    return false;
  }

  Point& box_at(Point location){
    for (auto& box : Boxes) 
      if (box == location)
        return box;
    return Boxes[0]; 
  }

  void print_player_info(string icon){
    cout << icon <<  " Player : " << Player << '\n';
  }

  void print_boxes_info(string icon){
    cout << icon << " Boxes : " ; 
    for(auto& box : Boxes)
      cout << box << ",";
    cout << '\n';
  }
  
  void print_map(){
    cout << '\n';
    for (auto row : Map) 
    {
      for(auto cell : row)
        cout << cell; 
      cout << '\n';
    }

    auto icon = moon[m++%moon.size()]; 
    print_player_info(icon);
    print_boxes_info(icon);
  }

};
