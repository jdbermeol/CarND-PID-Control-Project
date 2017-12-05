#include "Twiddle.h"

using namespace std;

Twiddle::Twiddle(){}


Twiddle::~Twiddle(){}

void Twiddle::Init(double tol, const vector<double> params){
  this->tol = tol;
  this->params = params;
  dp = params;
  for(auto &val: dp)
    val *= 0.1;
  current_param = 0;
  initialize = false;
  first_chance = true;
}

vector<double> Twiddle::Update(double cte){
  if(!initialize){
    initialize = true;
    best_error = cte;
    params[current_param] += dp[current_param];
    return params;
  }
  if(first_chance){
    if(cte < best_error){
      best_error = cte;
      dp[current_param] *= 1.1;
      current_param = (current_param + 1) % params.size();
      params[current_param] += dp[current_param];
      return params;
    } else {
      first_chance = false;
      params[current_param] -= 2 * dp[current_param];
      return params;
    }
  } else {
    if(cte < best_error){
      best_error = cte;
      dp[current_param] *= 1.1;
    } else {
      params[current_param] += dp[current_param];
      dp[current_param] *= 0.9;
    }
    first_chance = true;
    current_param = (current_param + 1) % params.size();
    params[current_param] += dp[current_param];
    return params;
  }
}

bool Twiddle::Converged(){
  double sum = 0.0;
  for(auto const &val: dp)
    sum += val;
  return sum < tol;
}