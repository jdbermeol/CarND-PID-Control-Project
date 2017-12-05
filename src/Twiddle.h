#ifndef TWIDDLE_H
#define TWIDDLE_H

#include <vector>

class Twiddle {
  double tol;
  std::vector<double> params;
  std::vector<double> dp;
  int current_param;
  double best_error;
  bool initialize;
  bool first_chance;
public:

  Twiddle();

  /*
  * Destructor.
  */
  virtual ~Twiddle();

  /*
  * Initialize Twiddle.
  */
  void Init(double tol, const std::vector<double> params);

  std::vector<double> Update(double cte);

  bool Converged();
};

#endif /* TWIDDLE_H */
