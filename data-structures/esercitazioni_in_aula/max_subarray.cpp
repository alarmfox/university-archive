#include <vector>
#include <cassert>

using namespace std;

//solutions
const int brute_force_solution(const vector<int>& input);
const int linear_solution(const vector<int>& input);
const int divide_et_impera_solution(const vector<int>& input);

//utils
const int sum(const vector<int>& input, const int start, const int end);
const int _cross_sum(const vector<int>& input, const int start, const int mid, const int end);
const int _divide_et_impera_solution(const vector<int>& input, const int start, const int end);

int main() {
    const vector<int>& input1 = { -1,-3,4,2 }, input2 = { -1,2,-5,7 };
    int res;

    res = brute_force_solution(input1);
    assert(res == 6);
    res = brute_force_solution(input2);
    assert(res == 7);

    res = linear_solution(input1);
    assert(res == 6);
    res = linear_solution(input2);
    assert(res == 7);

    res = divide_et_impera_solution(input1);
    assert(res == 6);
    res = divide_et_impera_solution(input2);
    assert(res == 7);


}

const int brute_force_solution(const vector<int>& input) {

    int max = 0, tmp;
    for (int i = 0; i < input.size(); ++i) {
        for (int j = i; j < input.size(); ++j) {
            tmp = sum(input, i, j);
            if (max < tmp) {
                max = tmp;
            }
        }
    }

    return max;
}

const int linear_solution(const vector<int>& input) {
    int current_sum = 0, max_sum = 0;
    for (int i = 0; i < input.size(); ++i) {
        current_sum = max(current_sum + input[i], input[i]);
        max_sum = max(current_sum, max_sum);
    }

    return max_sum;
}

const int divide_et_impera_solution(const vector<int>& input) {
    if (input.empty()) return 0;

    return _divide_et_impera_solution(input, 0, input.size() -1);
}

const int _divide_et_impera_solution(const vector<int>& input, const int start, const int end){

    if (start > end) {
        return -1;
    }

    if (start ==  end) {
        return input[start];
    }

    int mid = (start + end) / 2;

    return max(_cross_sum(input, start, mid, end), max(
        _divide_et_impera_solution(input, start, mid -1),
        _divide_et_impera_solution(input, mid+1, end)
    ));
}

const int _cross_sum(const vector<int>& input, const int start, const int mid, const int end){
    if (start == end) {
        return input[start];
    }

    int sum = 0;

    // da mid a sinistra
    int left_sum = 0;
    for (int i = mid; i > start; --i){
        sum+= input[i];
        if (sum > left_sum){
            left_sum = sum;
        }
    }

    sum = 0;
    int right_sum = 0;
    for (int i = mid; i <= end; ++i){
        sum+= input[i];
        if (sum > right_sum  ){
            left_sum = sum;
        }
    }

    return max(left_sum + right_sum - input[mid], max(left_sum, right_sum));

}

const int sum(const vector<int>& input, const int start, const int end) {
    int tmp = 0;
    for (int i = start; i <= end; ++i) {
        tmp += input[i];
    }

    return tmp;
}
