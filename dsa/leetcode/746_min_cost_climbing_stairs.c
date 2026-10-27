// You are given an integer array cost where cost[i] is the cost of ith step on
// a staircase. Once you pay the cost, you can either climb one or two steps.
//
// You can either start from the step with index 0, or the step with index 1.
//
// Return the minimum cost to reach the top of the floor.

#include <stdlib.h>
int min(int a, int b) { return a < b ? a : b; }

int minCostClimbingStairs(int *cost, int costSize) {
  if (costSize < 3)
    return min(cost[0], cost[1]);

  int *dp = malloc(sizeof(int) * costSize), r;

  dp[0] = cost[0];
  dp[1] = cost[1];

  for (int i = 2; i < costSize; ++i) {
    dp[i] = min(dp[i - 1] + cost[i], dp[i - 2] + cost[i]);
  }

  r = min(dp[costSize - 1], dp[costSize - 2]);
  free(dp);

  return r;
}
