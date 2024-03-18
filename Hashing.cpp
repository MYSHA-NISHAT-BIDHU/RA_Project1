#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <chrono>
#include <algorithm>
#include <random>
#include <fstream>

class PerfectHashing {
private:
    std::vector<int> keys;
    std::vector<std::vector<int>> hashTable;

    unsigned int hash(int key) {
        const unsigned int multiplier = 2654435761; // A prime number
        unsigned int hashValue = key * multiplier;
        hashValue = hashValue >> 16; // Shift right by 16 bits
        return hashValue % keys.size(); // Ensure index is within range
    }

public:
    PerfectHashing(std::vector<int>& inputKeys) : keys(inputKeys) {
        // Initialize and build your Perfect Hashing data structure
        hashTable.resize(keys.size());

        for (int key : keys) {
            int index = hash(key);
            hashTable[index].push_back(key);
        }
    }

    bool search(int key) {
        int index = hash(key);
        for (int k : hashTable[index]) {
            if (k == key)
                return true;
        }
        return false;
    }
};

class HashingWithChaining {
private:
    int size;
    std::vector<std::list<int>> table;

    unsigned int hash(int key) {
        const unsigned int multiplier = 2654435761; // A prime number
        unsigned int hashValue = key * multiplier;
        hashValue = hashValue >> 16; // Shift right by 16 bits
        return hashValue % size; // Ensure index is within range
    }

public:
    HashingWithChaining(int n) : size(n), table(n) {}

    void insert(int key) {
        int index = hash(key);
        table[index].push_back(key);
    }

    bool contains(int key) {
        int index = hash(key);
        for (int k : table[index]) {
            if (k == key) return true;
        }
        return false;
    }
};

class RedBlackTree {
private:
    std::set<int> rbTree;

public:
    void insert(int key) {
        rbTree.insert(key);
    }

    bool contains(int key) {
        return rbTree.find(key) != rbTree.end();
    }
};

// Function to generate random numbers
std::vector<int> generateRandomNumbers(int n) {
    std::vector<int> randomNumbers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, INT_MAX); // Generate random integers in the range [0, INT_MAX]

    for (int i = 0; i < n; ++i) {
        randomNumbers.push_back(dis(gen));
    }

    return randomNumbers;
}

int main() {
    std::ofstream outputFile("timing_results.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    outputFile << "n,PerfectHashingInsertTime,PerfectHashingQueryTime,HashingWithChainingInsertTime,HashingWithChainingQueryTime,RedBlackTreeInsertTime,RedBlackTreeQueryTime\n";

    for (int n = 25; n <= (1 << 24); n *= 2) {
        // Generate n random numbers
        std::vector<int> randomNumbers = generateRandomNumbers(n);

        // Shuffle the keys
        std::shuffle(randomNumbers.begin(), randomNumbers.end(), std::mt19937(std::random_device()()));

        // Measure the time taken for insertion and querying for each data structure
        auto start = std::chrono::steady_clock::now();
        PerfectHashing perfectHashing(randomNumbers);
        auto end = std::chrono::steady_clock::now();
        auto perfectHashingInsertTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        start = std::chrono::steady_clock::now();
        for (int key : randomNumbers) {
            //perfectHashing.contains(key);
        }
        end = std::chrono::steady_clock::now();
        auto perfectHashingQueryTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        start = std::chrono::steady_clock::now();
        HashingWithChaining hashingWithChaining(n);
        for (int key : randomNumbers) {
            hashingWithChaining.insert(key);
        }
        end = std::chrono::steady_clock::now();
        auto hashingWithChainingInsertTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        start = std::chrono::steady_clock::now();
        for (int key : randomNumbers) {
            hashingWithChaining.contains(key);
        }
        end = std::chrono::steady_clock::now();
        auto hashingWithChainingQueryTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        start = std::chrono::steady_clock::now();
        RedBlackTree redBlackTree;
        for (int key : randomNumbers) {
            redBlackTree.insert(key);
        }
        end = std::chrono::steady_clock::now();
        auto redBlackTreeInsertTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        start = std::chrono::steady_clock::now();
        for (int key : randomNumbers) {
            redBlackTree.contains(key);
        }
        end = std::chrono::steady_clock::now();
        auto redBlackTreeQueryTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Write the timing results to the CSV file
        outputFile << n << "," << perfectHashingInsertTime << "," << perfectHashingQueryTime << "," << hashingWithChainingInsertTime << "," << hashingWithChainingQueryTime << "," << redBlackTreeInsertTime << "," << redBlackTreeQueryTime << "\n";
    }

    outputFile.close();

    std::cout << "Timing results saved to 'timing_results.csv'." << std::endl;

    return 0;
}
