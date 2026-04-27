#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

#define DEBUG 1


const std::vector<int> find_min(std::vector<int>& input) {
    std::unordered_map<int, std::vector<int>> map;
    std::vector<int> result (input.size());

    // popolo la mappa
    for (int i = 0; i < input.size(); ++i) {
        map[input[i]].push_back(i);
    }
    // ordino il vettore con std::stable_sort O(nlog(n))
    std::stable_sort(input.begin(), input.end());

    std::vector<int> positions;
    // scorro il vettore ordinato e popolo il vettore dei risultati
    for (int i = 0; i < input.size() ; ++i) {
        positions = map[input[i]];
        for (const int position: positions) {
            result[position] = i + positions.size() -1;
        }
        i+=positions.size() -1 ;
    } 

    return result;
}

std::ostream& print_vector(const std::vector<int>& input, std::ostream& out) {
    for (const int elem: input) {
        out << elem << ' ';
    }
    return out;
}

void assert_vector(const std::vector<int> a, const std::vector<int> b) {
    assert(a.size() == b.size());

    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == b[i]);
    }
}

int main() {
    // table testing: 
    // input di test
    std::vector<std::vector<int>> inputs = {
        { 5, 6, 4, 5, 7, 9 }, 
        { 6, 5, 9, 11, 25, 19 }, 
        { 2, 4, 3, 6, 6, 9, 3 },
        { 1, 1, 1, 1, 1, 1, 1 },
    };
    // valori attesi
    std::vector<std::vector<int>> results = {
        { 2, 3, 0, 2, 4, 5 }, 
        { 1, 0, 2, 3, 5, 4 },
        { 0, 3, 2, 5, 5, 6, 2 },
        { 6, 6, 6, 6, 6, 6, 6 }
    };
    std::vector<int> result;

    for (int i = 0; i < inputs.size(); ++i) {
        if (DEBUG) {
            std::cout<<"INPUT : "; print_vector(inputs[i], std::cout) << std::endl;
        }

        result = find_min(inputs[i]);
        
        if (DEBUG) {
            std::cout<<"OUTPUT: "; print_vector(result, std::cout) << "\n\n";
        }

        assert_vector(result, results[i]);
    }

}