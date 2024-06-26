#include "Helper.h"
#include <unordered_map>


static State GAME;
static unordered_map<State, vector<double>, StateHash> q_table;

void select_level()
{
  cout << "Enter Level : " ; 
  cin >> lvl; 
  lvl--; 
}

void init()
{
  select_level();
  GAME.Map = levels[lvl];
  GAME.print_map();

  int x,y;
  do {
    cout << "Enter Player Position : " ; 
    cin >> x >> y;
  }while (GAME.map_at({x,y}) != "  ");

  GAME.map_at({x,y}) = PLAYER;
  GAME.refresh();
}

State apply_move(State state, int action){

  auto move = Actions[moves[action]]; 
  auto& player = state.Player;
  auto Target = player + move;
  auto Future = Target + move; 

  if(notAllowed(state, Target, Future))
    return state;
  state.Action = action;

  if (state.isPoint(Target)) {
    state.map_at(Target) = PLAYER;
    state.map_at(player) = "  ";
    player = Target;
    return state;
  }

  if (state.isBox(Target)) {
    if (state.isPoint(Future)){
      state.map_at(Future) = BONE; 
      state.Bones.push_back(Future);
    }
    else 
      state.map_at(Future) = BOX[lvl]; 
    state.map_at(Target) = "  ";
    state.box_at(Target) = Future;
  }

  if (state.isSpace(Target)) {
    if(state.player_on_point())
      state.map_at(player) = POINT[lvl];
    else 
      state.map_at(player) = "  ";
    state.map_at(Target) = PLAYER;
    player = Target;
  }

  return state;
}

vector<State> generate_children(State state){
  vector<State> children;
  for (int i = 0; i < 4; i++) {
    auto child = apply_move(state, i);
    if(child == state )
      continue;
    children.push_back(child);
  }
  return children;
}

double close_to_box(State& state){
  double sum = 0; 
  for (auto& box : state.Boxes){
    sum += 10000/box.distance(state.Player);
  }

  return sum;
}

double close_to_bone(State& state){
  double sum = 0;
  for (auto& bone : state.Bones) {
    sum += bone.distance(state.Player) * 5000;
  }
  return sum;
}

double Reward(State& state){

  if (isDeadlock(state))
    return 0;

  double sum = 0; 

  for (auto& box : state.Boxes){
    if (onGoal(state, box)) 
        sum += 900;
  }

  // sum += close_to_box(state);

  if (isGoal(state))
    sum += 1000000 * (lvl +1);
  // else 
  //   sum += close_to_bone(state);

  return sum;
}

void printQ(){
  int i = 0;
  for (auto& s : q_table) {
    cout << "S" << i++ << ": ";
    for (auto& val : s.second) {
      cout << val << " | ";
    }
    cout << endl << "-----------------------------------------" << endl;
  }
  cin.ignore();
}

double FindMax(State next){

  double maxi = 0;
  if (q_table.find(next) != q_table.end()) {
    for (int i = 0; i < 4; i++) {
      maxi = max(maxi, q_table[next][i]);
    }
  }
  return maxi;
}

void Qlearn(State state){

  while(!isGoal(state)){
    if (isDeadlock(state)) 
      return;

    if (q_table.find(state) == q_table.end())
      q_table[state] = {0,0,0,0};

    State  next_state;
    int index;     
    do
    {
      index = rand()%4;
      next_state = apply_move(state, index);
    }while(next_state == state);
      

    double reward = Reward(next_state) + 0.8432 * FindMax(next_state);
    q_table[state][index] = reward;

    state.print_map();
    state = next_state;
  };
  state.print_map();
}

void accumulate_rewards(string& reward, string move, int val ){
  reward += "| ";
  reward += move; 
  reward += " : ";
  reward += to_string(val);
}

void solve_game(State s){

  if (isGoal(s)) 
  {
    s.print_map();
    cout << " GOAL REACHED !!!" << endl;
    return;
  }

  int index = 0, i = 0,max = 0;
  string rewards = "";
  for (auto&val : q_table[s]){
    accumulate_rewards(rewards, moves[i], val);
    if(val > max)
    {
      max = val;
      index = i;
    }
    i++;
  }

  s.print_map(); 
  cout << rewards ;
  cin.ignore();
  solve_game(apply_move(s, index));
}

void Train(){
  int x; 
  cout << "Enter the number of Episodes to Train : "; 
  cin >> x; 

  auto states = generate_children(GAME);
  states.push_back(GAME);

  while (x-- > 0) 
    for (int i = 0; i < states.size(); i++)
      Qlearn(states[i]);
  cout << "Training Finished Now Keep clicking Enter to see Solving States" << endl;
  cin.ignore();
}

