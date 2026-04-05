#include "NonComparisonSorts.h"
#include <iostream>
#include <algorithm>

// ---------------------------------------------------------------------------
// Counting Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #1 -- Steps 1a, 1b, 1c (Count Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #2 -- Step 2 (Placement Phase)
//
void counting_sort(std::vector<int>& data) {
    if (data.size() <= 1) return; // Already sorted
    // 1a. Find the min and max values
    int min_val = *std::min_element(data.begin(), data.end());
    int max_val = *std::max_element(data.begin(), data.end());
    
    // 1b. Create a count array of size (max - min + 1) initialized to 0
    int k = max_val - min_val + 1; // Range of vallues
    std::vector<int> count(k, 0); // Count array to store the count of each unique object

    // 1c. Count the occurrences of each value in data
    for (int num : data) {
        count[num - min_val]++; // Increament the count for this number
    }
     
    // 2. Build the output array
    int inx = 0; // Index for the original data array
    for (int i = 0; i < k; ++i) {
        while (count[i] > 0) {
            data[inx++] = i + min_val; // Place the value back into data
            count[i]--; // Decrease the count for this number
        }
    }
}

// ---------------------------------------------------------------------------
// Bucket Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #3 -- Steps 1a, 1b (Distribution Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #4 -- Steps 2-3 (Sort + Concatenate)
//
void bucket_sort(std::vector<int>& data, int num_buckets) {
    if (data.size() <= 1) return; // Already sorted

    // 1a. Find the min and max values to determine bucket ranges
    int min_val = *std::min_element(data.begin(), data.end());
    int max_val = *std::max_element(data.begin(), data.end());

    // Escape case: if all values are the same, juest return
    if (min_val == max_val) return;

    int range = max_val - min_val + 1; // Total range of values
    int range_per_bucket = static_cast<int>(
        std::ceil(static_cast<double>(range) / num_buckets)); // Range of values per buckets

    // 1b. Create num_buckets empty buckets (vectors)
    std::vector<std::vector<int>> buckets(num_buckets); // Create buckets
    for (int num : data) {
        int bucket_index = (num - min_val) / range_per_bucket; // Determine bucket ranges
        if (bucket_index >= num_buckets) bucket_index = num_buckets - 1;
        buckets[bucket_index].push_back(num); // Place number in the appropriate buckets
    }

    // 2. Sort each bucket and concatenate back into data
    int inx = 0; // Index for the original data array
    for (auto& bucket : buckets) {
        std::sort(bucket.begin(), bucket.end()); 
        // 3. Concatenate sorted buckets back into data
        for (int num : bucket) {
            data[inx++] = num; // Place sorted numbers back into data
        }
    }
}

// ---------------------------------------------------------------------------
// Radix Sort (LSD)
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #5 -- Step 0 + Pass 1 Detail
// ? SEE DIAGRAM: cpp_diagrams.md #6 -- Passes 2-3 (Stability in Action)
//
static void counting_sort_by_digits(std::vector<int>& data, int exp) {
    int n = static_cast<int>(data.size()); // Number of elements in data

    std::vector<int> output(n); // Output array to hold sorted numbers
    int count[10] = { 0 }; // k = 10 for digits 0 - 9

    // Count occurences of each digit in the current position stability
    for (int i = 0; i < n; i++) {
        int digit = (data[i] / exp) % 10; // Get the digit at the current position
        count[digit]++;
    }

    // Convert count to cumalative count to determine positions in putput
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }

    // Build the output array by placing numbers in the correct position
    for (int i = n - 1; i >= 0; --i) {
        int digit = (data[i] / exp) % 10;
        output[count[digit] - 1] = data[i]; // Place number in output
        count[digit]--; // Decrease count for this digit
    }

    // Copy the output array back to data, so that it now contains sorted numbers
    data = output;
}

void radix_sort(std::vector<int>& data) {
    if (data.size() <= 1) return; // Already sorted

    int max_val = *std::max_element(data.begin(), data.end()); // bigO (n) 802

    for (int exp = 1; max_val / exp > 0; exp *= 10) { // 1s, 10s. 100s...
        counting_sort_by_digits(data, exp); // Sort data by the current digit
    }
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------
void print_vector(const std::vector<int>& data, const std::string& label) {
    if (!label.empty()) std::cout << label << ": ";
    std::cout << "[";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if (i + 1 < data.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
