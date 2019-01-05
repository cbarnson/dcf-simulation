#include <iostream>
#include <vector>
#include "Simulator.h"
#include "config.h"

int main() {
  std::vector<int> n;
  n.push_back(10);
  n.push_back(20);
  n.push_back(40);
  n.push_back(80);
  std::vector<double> phi;
  phi.push_back(0.25);
  phi.push_back(0.5);
  phi.push_back(0.9);

  int case_num = 1;

  for (unsigned int i = 0; i < n.size(); i++) {
    for (unsigned int j = 0; j < phi.size(); j++) {
      PHI = phi[j];
      std::cout << "Case #" << case_num++ << ", n = " << n[i]
                << ", phi = " << phi[j] << std::endl;
      std::cout << "==========================" << std::endl;
      Simulator sim(n[i]);
      sim.run();
      std::cout << std::endl;
    }
  }

  return 0;
}
