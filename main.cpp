/*
 * main.cpp
 *
 *  Created on: May 17, 2017
 *      Author: jemkov
 */

#include <chrono>
#include <functional>
using namespace std;
using namespace std::chrono;

template <typename Result, typename F, typename Duration = nanoseconds, typename ...Args>
pair<Result,typename Duration::rep> benchmark(F f, int iterations, Args&& ...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    Result res;
    for (int c=0; c < iterations; ++c)
        res = f(std::forward<Args>(args)...);
    auto stop = std::chrono::high_resolution_clock::now();
    return pair<Result,typename Duration::rep>(res, duration_cast<Duration>(stop-start).count()/iterations);
}

template <typename F, typename Duration = nanoseconds, typename ...Args>
typename Duration::rep benchmark_void(F f, int iterations, Args&& ...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int c=0; c < iterations; ++c)
        f(std::forward<Args>(args)...);
    auto stop = std::chrono::high_resolution_clock::now();
    return duration_cast<Duration>(stop-start).count()/iterations;
}

// ---------------------------------------------------------
// 561. Array Partition I
// ---------------------------------------------------------
#if 0
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
        if (nums.size() % 2) {
        	return 0;
        }
        else {
        	sort(nums.begin(), nums.end());
        	int idx = 0;
        	int sum = accumulate(
        			nums.begin(), nums.end(), 0,
        			[&idx](const int& a, const int& b)
					{
        				return ((idx++ % 2) == 0 ? a + b : a );
					}
        	);
        	return sum;
        }
    }
};

int main(int, char**)
{
	vector<int> v = {1, 4, 3, 2, 10, 8};
	Solution s;

	auto sum = s.arrayPairSum(v);
	if (sum == -1) {
		cout << "Array Partition 1: error: odd number of elements\n";
	}
	else {
		cout << "Array Partition 1: sum: " << sum << endl;
	}

	return 0;
}
#endif

// ---------------------------------------------------------
// 217. Contains Duplicate
// ---------------------------------------------------------
#if 0
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


// std sort/unique solution: O(n*log(n))
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        if (!nums.size())
        	return false;
        sort(nums.begin(), nums.end());
        return (unique(nums.begin(), nums.end()) != nums.end());
    }
};
#endif

#if 0
#include <unordered_set>

// std unordered set solution: O(n)?
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        if (!nums.size())
            return false;
        unordered_set<int> u_set;
        for (auto num: nums) {
            auto res = u_set.insert(num);
            if (res.second == false)
                break;
        }
        return (u_set.size() == nums.size() ? false : true);
    }
};

int main(int, char**)
{
    vector<int> v = {1, 1, 1, 25, 8, 4, 3, 2, 10, 11};
    Solution s;

    cout << "contains duplicate: " << ( s.containsDuplicate(v)==true ? "yes" : "no" ) << endl;

    return 0;
}

#endif

// ---------------------------------------------------------
// 219. Contains Duplicate II
//      Given an array of integers and an integer k, find out whether there are two distinct indices i and j in the array such that
//      nums[i] = nums[j] and the absolute difference between i and j is at most k.
// ---------------------------------------------------------
#if 0
using namespace std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        if (nums.size() == 0)
            return false;
        unordered_map<int, int> u_map;
        for (int i = 0; i < nums.size(); ++i) {
            auto res = u_map.insert({nums[i], i});
            if (res.second == false) {
                if ((i - res.first->second) <= k)
                    return true;
                else
                    res.first->second = i;
            }
        }
        return false;
    }
};

int main(int, char**)
{
    vector<int> v = {1,2,3,2,2};
	Solution s;

    cout << "contains duplicate: " << ( s.containsNearbyDuplicate(v, 1)==true ? "yes" : "no" ) << endl;

	return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 220. Contains Duplicate III
//      Given an array of integers, find out whether there are two distinct indices i and j in the array
//      such that the absolute difference between nums[i] and nums[j] is at most t and the absolute difference
//      between i and j is at most k.
// -------------------------------------------------------------------------------------------------------
#if 0
using namespace std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

std::uint32_t abs_diff(int a, int b)
{
    return static_cast<std::uint32_t>(std::abs(static_cast<double>(a) - static_cast<double>(b)));
}

class Solution {
public:
    bool brute_force(vector<int>& nums, int k, int t) {
        vector<pair<int, int>> v;
        for (int i=0; i < nums.size(); ++i) {
            v.push_back({nums[i], i});
        }
        sort(v.begin(), v.end(), [](const pair<int,int>& a, const pair<int,int>& b) { return a.first < b.first; } );
        for (int i=0; i < v.size()-1; ++i) {
            for (int j=i+1; j < v.size(); ++j) {
                if (abs_diff(v[j].first, v[i].first) <= t) {
                    if (abs_diff(v[j].second, v[i].second) <= k)
                        return true;
                }
                else
                    break;
            }
        }
        return false;
    }

    bool optimized(vector<int>& nums, int k, int t) {
        if ((nums.size() <= 1) || (t < 0))
            return false;
        unordered_map<int, int> u_map;
        u_map.insert({nums[0], 0});
        for (int i = 1; i < nums.size(); ++i) {
            auto it_duplicate = u_map.find(nums[i]);
            if (it_duplicate != u_map.end() && (i - it_duplicate->second) <= k)
                return true;
            for (int j = (nums[i] - t); j <= (nums[i] + t); ++j) {
                if (j == nums[i])
                    continue;
                auto it_almost_duplicate = u_map.find(j);
                if (it_almost_duplicate != u_map.end() && (i - it_almost_duplicate->second) <= k)
                    return true;
            }
            if (it_duplicate != u_map.end()) {
                it_duplicate->second = i;
            }
            else {
                u_map.insert({nums[i], i});
            }
        }
        return false;
    }

    bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
        if ((nums.size() <= 1) || (t < 0))
            return false;
//        if (t < nums.size())
//            return this->optimized(nums, k, t);
//        else
            return this->brute_force(nums, k, t);
    }
};

int main(int, char**)
{
    vector<int> v = {-1,2147483648};
    int k = 1;
    int t = 2147483648;
    Solution s;

    cout << "contains almost duplicate: " << ( s.containsNearbyAlmostDuplicate(v, k, t)==true ? "yes" : "no" ) << endl;

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// benchmark template tries
// -------------------------------------------------------------------------------------------------------
#if 0
//#include <chrono>
//#include <functional>
//using namespace std;
//using namespace std::chrono;

//template <typename F, typename T, typename Duration = nanoseconds, typename ...Args>
//pair<int,typename Duration::rep> benchmark(F f, T t, int iterations, Args&& ...args)
//{
//    auto start = std::chrono::high_resolution_clock::now();
//    int res = 0;
//    for (int c=0; c < iterations; ++c)
//        res = (t.*f)(std::forward<Args>(args)...);
//    auto stop = std::chrono::high_resolution_clock::now();
//    return pair<int,typename Duration::rep>(res, duration_cast<Duration>(stop-start).count()/iterations);
//}

template <typename F, typename ...Args>
int wrapper(F f, Args&& ...args)
{
    return f(std::forward<Args>(args)...);
}

int foo(int a, int b)
{
    return a + b;
}

int iterations = 100000000;
{
    int res = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int c=0; c < iterations; ++c)
        res = s.findComplement(in);
    auto stop = std::chrono::high_resolution_clock::now();

    cout << "complement: " << res << ", avg duration: " << duration_cast<nanoseconds>(stop-start).count()/iterations << " ns\n";
}
{
    int res = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int c=0; c < iterations; ++c)
        res = s.findComplement2(in);
    auto stop = std::chrono::high_resolution_clock::now();

    cout << "complement2: " << res << ", avg duration: " << duration_cast<nanoseconds>(stop-start).count()/iterations << " ns\n";
}
{
    cout << "builtin clz: " << __builtin_clz(in) << endl;
    int res = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int c=0; c < iterations; ++c)
        res = s.findComplement3(in);
    auto stop = std::chrono::high_resolution_clock::now();

    cout << "complement3: " << res << ", avg duration: " << duration_cast<nanoseconds>(stop-start).count()/iterations << " ns\n";
}

//    {
//        auto res = benchmark(&Solution::findComplement, s, 100000000, in);
//        cout << "bench complement 1: " << res.first << ", avg duration: " << res.second << " ns\n";
//    }
//    {
//        auto res = benchmark(&Solution::findComplement2, s, 100000000, in);
//        cout << "bench complement 2: " << res.first << ", avg duration: " << res.second << " ns\n";
//    }
//    {
//        auto res = benchmark(&Solution::findComplement3, s, 100000000, in);
//        cout << "bench complement 3: " << res.first << ", avg duration: " << res.second << " ns\n";
//    }
//    {
//        auto res = wrapper(foo, 55, 66);
//        cout << "wrapper global function: " << res << endl;
//    }
//    {
//        auto res = wrapper(mem_fn(&Solution::findComplement), s, 5);
//        cout << "wrapper mem_fn: " << res << endl;
//    }
//    {
//        Solution s;
//        auto f = bind(&Solution::findComplement, s, _1);
//        auto res = f(5);
//        cout << "wrapper bind: " << bind(&Solution::findComplement, s, _1)(5) << endl;
//    }
//    {
//        Solution s;
//        auto f = bind(&Solution::findComplement, s);
//        auto res = wrapper(f, 5);
////        cout << "wrapper bind: " << res << endl;
//    }

{
    auto res = benchmark(foo, 1000000, 55, 66);
    cout << "bench foo: " << res.first << ", avg duration: " << res.second << " ns\n";
}

#endif

// -------------------------------------------------------------------------------------------------------
// 476. Number Complement
//      Given a positive integer, output its complement number. The complement strategy is to flip the bits
//      of its binary representation.
//      Note:
//          The given integer is guaranteed to fit within the range of a 32-bit signed integer.
//          You could assume no leading zero bit in the integer’s binary representation.
//
// https://graphics.stanford.edu/~seander/bithacks.html
// -------------------------------------------------------------------------------------------------------

#if 0
using namespace std;
#include <iostream>
#include <limits>

class Solution {
public:
    Solution()
    {}

    int findComplement(int num) {
        if (num==0)
            return 1;
        if ((num==1) || (num < 0) || (num > std::numeric_limits<int>::max()))
            return 0;
        int out = 0;
        for (int pos = 0; num; num >>=1 ,++pos) {
            out |= !(num & 1) << pos;
        }
        return (out);
    }
    int findComplement2(int num) {
        if (num==0)
            return 1;
        if ((num==1) || (num < 0) || (num > std::numeric_limits<int>::max()))
            return 0;
        int out = num;
        int mask = ~0;
        int leading_zeros = 0;
        for (; mask&num; mask >>= 1, num >>= 1, ++leading_zeros)
            ;
        return (~out&~(mask <<= leading_zeros));
    }
    int findComplement3(int num) {
        if (num==0)
            return 1;
        if ((num==1) || (num < 0) || (num > std::numeric_limits<int>::max()))
            return 0;
        int out = num;
        int len = (32 - __builtin_clz(num));
        int mask = ~0 >> len;
        return (~out&~(mask <<= len));
    }
};

int main(int, char**)
{
    int in = 5;
    Solution s;

    {
        auto res = benchmark(mem_fn(&Solution::findComplement), 100000000, s, in);
        cout << "bench complement 1: " << res.first << ", avg duration: " << res.second << " ns\n";
    }
    {
        auto res = benchmark(mem_fn(&Solution::findComplement2), 100000000, s, in);
        cout << "bench complement 2: " << res.first << ", avg duration: " << res.second << " ns\n";
    }
    {
        auto res = benchmark(mem_fn(&Solution::findComplement3), 100000000, s, in);
        cout << "bench complement 3: " << res.first << ", avg duration: " << res.second << " ns\n";
    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 557. Reverse Words in a String III
//      Given a string, you need to reverse the order of characters in each word within a sentence while
//      still preserving whitespace and initial word order.
// -------------------------------------------------------------------------------------------------------
#if 0
using namespace std;
#include <iostream>
#include <string>

class Solution {
public:
    string reverseWords(string s) {
        auto word_start = s.find_first_not_of(" ", 0);
        while (word_start != string::npos) {
            auto word_end = s.find_first_of(" ", word_start);
            string::size_type rvrs_end = ( word_end != string::npos ? word_end - 1  : s.size() - 1 );
            while (word_start < rvrs_end) {
                swap(s[rvrs_end--], s[word_start++]);
            }
            word_start = s.find_first_not_of(" \0", word_end, 2);
        }
        return s;
    }
};

int main(int, char**)
{
    Solution s;

    {
        auto res = benchmark(mem_fn(&Solution::reverseWords), 1000000, s, "Ladenzon is full");
        cout << "reverseWords: " << res.first << ", avg duration: " << res.second << " ns\n";
    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 151. Reverse Words in a String
//      Given an input string, reverse the string word by word.
//      For example,
//      Given s = "the sky is blue",
//      return "blue is sky the".
//      Update (2015-02-12):
//          For C programmers: Try to solve it in-place in O(1) space.
// -------------------------------------------------------------------------------------------------------
#if 0
using namespace std;
#include <iostream>
#include <string>

class Solution {
public:
    void reverseWords(string& s) {
        string result;
        if (s.size() <= 0)
            return;
        int word_start = 0, word_end = 0;
        word_end = s.size() - 1;
//        cout << "entry: " << word_start << ":" << word_end << endl;
        while (word_end >= 0) {
            for (; word_end >= 0 && s[word_end]==' '; --word_end)
                ;
//            cout << "word search: " << word_start << ":" << word_end << endl;
            if (word_end < 0)
                break;
            for (word_start = word_end; word_start >= 0 && s[word_start] != ' '; --word_start)
                ;
//            cout << "append: " << word_start + 1 << ":" << word_end << endl;
            result.append(s, word_start + 1, (word_end - word_start));
            result.append(" ");
            word_end = word_start;
        }
        if (result[result.size()-1] == ' ') {
//        	cout << "remove trailing space from result\n";
        	result.erase(result.size()-1, 1);
        }
//        cout << "result: " << result << endl;
        swap(result, s);
    }
};

int main(int argc, char** argv)
{
    Solution s;
#if 0
abc def
#endif

    string str = (argc > 1 ? argv[1] : "abc");
    s.reverseWords(str);
    cout << "Reversed words: " << str << endl;

//    {
//        auto f = mem_fn(&Solution::reverseWords);
//        auto res = benchmark<int>(f, 10000000, s, str);
//        cout << "Reversed words: " << str << ", avg duration: " << res.second << " ns\n";
//    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 204. Count Primes
//      Count the number of prime numbers less than a non-negative number, n
// -------------------------------------------------------------------------------------------------------
#if 0
using namespace std;
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

class Solution {
public:
    inline bool isPrime_brute(int n) {
        if (n <= 1)
            return false;
        if (n < 4)
            return true;
        if (n % 2 == 0)
            return false;
        for (int div = 2; div < n; ++div) {
            if ((n % div) == 0)
                return false;
        }
        return true;
    }

    inline bool isPrime_opt1(int n) {
        if (n <= 1)
            return false;
        if (n < 4)
            return true;
        if (n % 2 == 0)
            return false;
        for (int div = 2; div*div <= n; ++div) {
            if ((n % div) == 0)
                return false;
        }
        return true;
    }

    inline int countPrimes_brute(int n) {
        int res = 0;
        for (int c = 1; c < n; ++c) {
            if (isPrime_opt1(c))
                ++res;
        }
        return res;
    }

    inline int countPrimes_sieve(int n) {
        --n;
//        cout << "test: " << n << endl;
        if (n <= 1)
            return 0;
        if (n == 2)
            return 1;
        // number of odds - 1 (starting from 3)
        int n_elements = std::ceil((double)n / 2.0) - 1;
//        cout << "n elems: " << n_elements << endl;
        // seive only odds, starting from 3
        vector<bool> sieve(n_elements, true);
//        for (auto elem: sieve) cout << elem << " "; cout << endl;
        // index of currently processed value
        int idx = 0;
        // index of current value square
        int square_idx = 3;
        // step to sieve
        int step = 3;
        while (square_idx < n_elements) {
            // if current is prime, sieve starting from its square index with step
            if (sieve[idx] == true) {
                for (int mark = square_idx; mark < n_elements; mark += step)
                    sieve[mark] = false;
            }
//            cout << "idx: " << idx << ", square idx: " << square_idx << endl;
//            for (auto elem: sieve) cout << elem << " "; cout << endl;
            ++idx;
            // this is optimization of:
            // square_idx = 2*sqr(i) + 6*i + 3; delta_square_idx = square_idx(i+1) - square_idx(i) = step(i) + step(i+1)
            square_idx += step;
            // step = value = i*2 + 3; delta_step = step(i+1) - step(i) = 2
            step += 2;
            square_idx += step;
        }
        // return +1 for "2"
        return count(sieve.begin(), sieve.end(), true) + 1;
    }

    int countPrimes(int n) {
        return countPrimes_sieve(n);
    }
};

int main(int argc, char** argv)
{
    Solution s;
    int in = (argc > 1 ? atoi(argv[1]) : 0);

//    {
//        auto f = mem_fn(&Solution::isPrime_brute);
//        auto res = benchmark<bool>(f, 1000, s, in);
//        cout << "Is prime: " << res.first << ", avg duration: " << res.second << " ns\n";
//    }
//    {
//        auto f = mem_fn(&Solution::countPrimes);
//        auto res = benchmark<int>(f, 100, s, in);
//        cout << "Count primes: " << res.first << ", avg duration: " << res.second << " ns\n";
//        // brute force: in: 1500000: Count primes: 114155, avg duration: 375296794 ns
//    }
    {
        auto f = mem_fn(&Solution::countPrimes);
        auto res = benchmark<int>(f, 1, s, in);
        cout << "Count primes: " << res.first << ", avg duration: " << res.second << " ns\n";
        // sieve: in: 1000001:  Count primes: 78498, avg duration:   34798167 ns
        // sieve: in: 1500000:  Count primes: 114155, avg duration:  52675663 ns
        // sieve: in: 10000000: Count primes: 664579, avg duration: 364715165 ns
    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 172. Factorial Trailing Zeroes
//      Given an integer n, return the number of trailing zeroes in n!.
//      Note: Your solution should be in logarithmic time complexity.
// -------------------------------------------------------------------------------------------------------
#if 0
using namespace std;
#include <iostream>

class Solution {
public:
    int trailingZeroes(int n) {
    	int zero_count = 0;
    	for (int div = 5; static_cast<int>(n / div) > 0; div *= 5) {
    		zero_count += static_cast<int>(n / div);
    	}
    	return zero_count;
    }
    int trailingZeroes_opt(int n) {
    	int zero_count = 0;
    	for (long long div = 5; static_cast<int>(n / div) > 0; div += (div << 2)) {
    		zero_count += static_cast<int>(n / div);
    	}
    	return zero_count;
    }
};

int main(int argc, char** argv)
{
    Solution s;
    int in = (argc > 1 ? atoi(argv[1]) : 0);
    {
        auto f = mem_fn(&Solution::trailingZeroes);
        auto res = benchmark<int>(f, 1000000, s, in);
        cout << "Trailing zeroes: " << res.first << ", avg duration: " << res.second << " ns\n";
        // input: 101 		Trailing zeroes: 24, avg duration: 			40 ns
        // input: 1000 		Trailing zeroes: 249, avg duration: 		55 ns
        // input: 1000000	Trailing zeroes: 249998, avg duration: 		110 ns
        // input: 10^9		Trailing zeroes: 249999998, avg duration: 	144 ns
    }
    {
        auto f = mem_fn(&Solution::trailingZeroes_opt);
        auto res = benchmark<int>(f, 1000000, s, in);
        cout << "Trailing zeroes opt: " << res.first << ", avg duration: " << res.second << " ns\n";
        // input: 101 		Trailing zeroes: 24, avg duration: 			36 ns
        // input: 1000 		Trailing zeroes: 2499, avg duration: 		48 ns
        // input: 1000000	Trailing zeroes: 249998, avg duration:  	84 ns
        // input: 10^9		Trailing zeroes: 249999998, avg duration: 	126 ns
    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 9. Palindrome Number
//    Determine whether an integer is a palindrome. Do this without extra space.
// -------------------------------------------------------------------------------------------------------
#if 0
using namespace std;
#include <iostream>
#include <cmath>

class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0)
            return false;
        int n_digits = 0;
        for (int div = x; div; div /= 10, ++n_digits)
            ;
        for (int digit = 1; digit <= n_digits / 2; ++digit) {
            if ( (x / (int)std::pow(10,(digit - 1))) % 10 !=
                 (x / (int)std::pow(10,(n_digits - digit))) % 10 )
                return false;
        }
        return true;
    }
};

int main(int argc, char** argv)
{
    Solution s;
    int in = (argc > 1 ? atoi(argv[1]) : 0);
    {
        auto f = mem_fn(&Solution::isPalindrome);
        auto res = benchmark<bool>(f, 1, s, in);
        cout << "Palindrome: " << res.first << ", avg duration: " << res.second << " ns\n";
    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 111. Minimum Depth of Binary Tree
//      Given a binary tree, find its minimum depth.
//      The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
// -------------------------------------------------------------------------------------------------------
using namespace std;
#include <iostream>
#include <vector>
#include <list>

/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

struct Flat_bin_tree
{
    vector<TreeNode> nodes_;
    size_t max_depth_;
};

bool list_of_empty_nodes(list<TreeNode*> node_list)
{
    for (auto node: node_list)
        if (node != NULL) return false;
    return true;
}

Flat_bin_tree serialize(TreeNode* root)
{
    Flat_bin_tree flat_bin_tree;
    list<TreeNode*> this_level_nodes, next_level_nodes;
    this_level_nodes.push_back(root);
    size_t n_levels = 0;
    for (bool stop_condition = false; !stop_condition; ) {
        // populate next level nodes list
        for (auto node: this_level_nodes) {
            if (node == NULL) {
                next_level_nodes.push_back(NULL);
                next_level_nodes.push_back(NULL);
            }
            else {
                next_level_nodes.push_back(node->left);
                next_level_nodes.push_back(node->right);
            }
            stop_condition = list_of_empty_nodes(next_level_nodes);
        }
    }
    return flat_bin_tree;
}

TreeNode* deserialize(const Flat_bin_tree& flat_bin_tree)
{
    return nullptr;
}

class Solution {
public:
    int minDepth(TreeNode* root) {

    }
};

int main(int argc, char** argv)
{
    Solution s;
    int in = (argc > 1 ? atoi(argv[1]) : 0);
    {
//        auto f = mem_fn(&Solution::isPalindrome);
//        auto res = benchmark<bool>(f, 1, s, in);
//        cout << "Palindrome: " << res.first << ", avg duration: " << res.second << " ns\n";
    }

    return 0;
}
