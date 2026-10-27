#include <vector>
#include <cassert>

std::vector<int> bubbleSortSolution(std::vector<int>& input);
void swap(std::vector<int>& input, const int i, const int j);
int bubbleSort(std::vector<int>& input);

int main() {
    std::vector<int> input = { 5, 9, 1, 0, 5, 4, 3, 1, 2, 3, 0 };

    auto res = bubbleSortSolution(input);

    assert(res[0] == 6);
    assert(res[1] == 0);

    return 0;
}

std::vector<int> bubbleSortSolution(std::vector<int>& input) {
    std::vector<int> result;

    int size, i = 0, tmp;
    while (1) {
        size = input[i];
        if (size == 0) {
            break;
        }
        auto subInput = std::vector<int>(input.begin() + i + 1, input.begin() + i + size + 1);

        tmp = bubbleSort(subInput);
        result.push_back(tmp);
        i += size + 1;
    }

    return result;
}

int bubbleSort(std::vector<int>& input) {
    int res = 0;
    for (int i = 0; i < input.size();++i) {
        for (int j = 0; j < input.size() - i - 1; ++j) {
            if (input[j] > input[j + 1]) {
                swap(input, j, j + 1);
                ++res;
            }
        }
    }
    return res;
}

void swap(std::vector<int>& input, const int i, const int j) {
    int tmp = input[i];
    input[i] = input[j];
    input[j] = tmp;
}
