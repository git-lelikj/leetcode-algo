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
c
    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 111. Minimum Depth of Binary Tree
//      Given a binary tree, find its minimum depth.
//      The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
// -------------------------------------------------------------------------------------------------------
#if 0
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <cmath>
#include <stack>
#include <queue>
using namespace std;

/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

struct Node_control_rec
{
    Node_control_rec (TreeNode* node = NULL)
        : node_(node)
    {}
    TreeNode* node_ = NULL;
    bool checked_left_ = false;
    bool checked_right_ = false;
};

struct Flat_bin_tree
{
    vector<TreeNode*> nodes_;
    size_t max_depth_ = 0;
};

ostream& operator<<(ostream& os, const Flat_bin_tree &fbt)
{
    os << "depth: " << fbt.max_depth_ << ", nodes: ";
    for (auto node: fbt.nodes_) {
        if (node != NULL)
            os << node->val << " ";
        else
            os << "null ";
    }
    return os;
}

bool list_of_empty_nodes(list<TreeNode*> node_list)
{
    for (auto node: node_list)
        if (node != NULL) return false;
    return true;
}

Flat_bin_tree serialize(TreeNode* root)
{
    Flat_bin_tree flat_bin_tree;
    if (root == NULL)
        return flat_bin_tree;
    list<TreeNode*> this_level_nodes;
    this_level_nodes.push_back(root);
    flat_bin_tree.nodes_.push_back(root);
    flat_bin_tree.max_depth_ = 1;
    for (bool stop_condition = false; !stop_condition; ) {
        list<TreeNode*> next_level_nodes;
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
        }
        stop_condition = list_of_empty_nodes(next_level_nodes);
        if (!stop_condition) {
            // copy new level nodes to final flat binary tree
            copy(next_level_nodes.cbegin(), next_level_nodes.cend(), back_inserter(flat_bin_tree.nodes_));
            // swap this and next level nodes
            swap(this_level_nodes, next_level_nodes);
            ++flat_bin_tree.max_depth_;
        }
    }
    return flat_bin_tree;
}

TreeNode* deserialize(const Flat_bin_tree& flat_bin_tree)
{
    if (!flat_bin_tree.nodes_.size())
        return NULL;
    std::size_t expected_size = (std::pow(2, flat_bin_tree.max_depth_) - 1);
    if (flat_bin_tree.nodes_.size() != expected_size) {
        cout << "incorrect flat size: " << flat_bin_tree.nodes_.size() << ", expected: " << expected_size << endl;
        return NULL;
    }
    if (flat_bin_tree.nodes_[0] == NULL)
        return NULL;
    list<TreeNode*> this_level_nodes;
    // create root node, add it to this level nodes
    std::size_t curr_node_index = 0;
    TreeNode* root = new TreeNode(flat_bin_tree.nodes_[curr_node_index++]->val);
    this_level_nodes.push_back(root);
    for (size_t level = 2; level <= flat_bin_tree.max_depth_; ++level) {
//        cout << "deserialize: level: " << level << endl;
        list<TreeNode*> next_level_nodes;
        for (auto node: this_level_nodes) {
            if (node == NULL) {
                next_level_nodes.push_back(NULL);
                next_level_nodes.push_back(NULL);
                curr_node_index += 2;
            }
            else {
//                cout << "deserialize: processing:curr_index: " << node->val << ":" << curr_node_index << endl;
                if (flat_bin_tree.nodes_[curr_node_index]) {
                    TreeNode* left = new TreeNode(flat_bin_tree.nodes_[curr_node_index]->val);
                    node->left = left;
                    next_level_nodes.push_back(left);
//                    cout << "deserialize: add left: " << left->val << endl;
                }
                else {
                    next_level_nodes.push_back(NULL);
                }
                ++curr_node_index;
                if (flat_bin_tree.nodes_[curr_node_index]) {
                    TreeNode* right = new TreeNode(flat_bin_tree.nodes_[curr_node_index]->val);
                    node->right = right;
                    next_level_nodes.push_back(right);
//                    cout << "deserialize: add right: " << right->val << endl;
                }
                else {
                    next_level_nodes.push_back(NULL);
                }
                ++curr_node_index;
            }
        }
        swap(this_level_nodes, next_level_nodes);
    }
    return root;
}
#endif
// -------------------------------------------------------------------------------------------------------
// DFS (depth first traversal) solution with stack
// -------------------------------------------------------------------------------------------------------
#if 0
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (root == NULL)
            return 0;
        stack<Node_control_rec> traverse_stack;
        traverse_stack.push(Node_control_rec(root));
        bool stop = false;
        std::uint32_t min_depth = 0;
        while (!stop) {
            if (traverse_stack.empty()) {
                stop = true;
                continue;
            }
            // optimize: if stack size already exceeds min depth, pop current node and continue
            if (min_depth && (traverse_stack.size() > min_depth)) {
//                cout << "min depth exceeded, pop" << endl;
                traverse_stack.pop();
                continue;
            }
            Node_control_rec &top_node = traverse_stack.top();
            // if null node, pop and proceed -> shouldn happen
            if (top_node.node_ == NULL) {
                traverse_stack.pop();
                continue;
            }
//            cout << "top node: val:checked_left:checked_right: "
//                 << top_node.node_->val << ":"
//                 << top_node.checked_left_ << ":"
//                 << top_node.checked_right_ << ":"
//                 << endl;
            // if leaf, update min_depth
            if ((top_node.node_->left == NULL) and (top_node.node_->right == NULL)) {
                if ((min_depth == 0) || (traverse_stack.size() < min_depth)) {
                    min_depth = traverse_stack.size();
//                    cout << "update min depth: " << min_depth << endl;
                }
                // pop the node
                traverse_stack.pop();
                continue;
            }
            else {
                // if left and right processed, pop node from the stack
                if (top_node.checked_left_ && top_node.checked_right_) {
                    traverse_stack.pop();
//                    cout << "fully checked, pop" << endl;
                    continue;
                }
                // if left not processed, push it to stack
                if (!top_node.checked_left_) {
                    if (top_node.node_->left != NULL) {
                        top_node.checked_left_ = true;
                        traverse_stack.push(top_node.node_->left);
//                        cout << "left not checked, push" << endl;
                    }
                    else {
                        top_node.checked_left_ = true;
//                        cout << "left not checked and null, continue" << endl;
                    }
                    continue;
                }
                if (!top_node.checked_right_) {
                    if (top_node.node_->right != NULL) {
                        top_node.checked_right_ = true;
                        traverse_stack.push(top_node.node_->right);
//                        cout << "right not checked, push" << endl;
                    }
                    else {
                        top_node.checked_right_ = true;
//                        cout << "right not checked and null, continue" << endl;
                    }
                    continue;
                }
            }
        }
        return min_depth;
    }
};
#endif
// -------------------------------------------------------------------------------------------------------
// BFS (breadth first traversal) solution with queue
// -------------------------------------------------------------------------------------------------------
#if 0
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (root == NULL)
            return 0;
        std::size_t min_depth = 1;
        queue<TreeNode*> bfs_queue;
        bfs_queue.push(root);
        while (bfs_queue.size()) {
            std::size_t this_level_nodes = bfs_queue.size();
            for (std::size_t n = 0; n < this_level_nodes; ++n) {
                TreeNode* front_node = bfs_queue.front();
                if (front_node) {
                    // if it is leaf, stop and return min depth
                    if ((front_node->left == NULL) && (front_node->right == NULL)) {
                        return min_depth;
                    }
                    // push left (and right) to the queue
                    if (front_node->left != NULL)
                        bfs_queue.push(front_node->left);
                    if (front_node->right != NULL)
                        bfs_queue.push(front_node->right);
                }
                bfs_queue.pop();
            }
            // processed this level, update min_depth
            ++min_depth;
        }
        return min_depth;
    }
};

int main(int argc, char** argv)
{

    {
        Flat_bin_tree input_bin_tree;
        input_bin_tree.nodes_ = { new TreeNode(1),
                                  new TreeNode(2), new TreeNode(3),
                                  new TreeNode(4), NULL, new TreeNode(5), new TreeNode(6),
                                  new TreeNode(7), NULL, NULL,  new TreeNode(8), NULL, NULL, NULL, NULL
                                };
        input_bin_tree.max_depth_ = 4;
        TreeNode* root = deserialize(input_bin_tree);

        Solution s;
        int min_depth = s.minDepth(root);
        cout << "Binary tree min depth: " << min_depth << endl;

        Flat_bin_tree ouput_bin_tree = serialize(root);
        cout << ouput_bin_tree << endl;

    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 51. N-Queens
//Given an integer n, return all distinct solutions to the n-queens puzzle.

//Each solution contains a distinct board configuration of the n-queens' placement, where 'Q' and '.' both indicate a queen and an empty space respectively.

//For example,
//There exist two distinct solutions to the 4-queens puzzle:

//[
// [".Q..",  // Solution 1
//  "...Q",
//  "Q...",
//  "..Q."],

// ["..Q.",  // Solution 2
//  "Q...",
//  "...Q",
//  ".Q.."]
//]
// -------------------------------------------------------------------------------------------------------
#if 0
#include <iostream>
#include <vector>
#include <ostream>
#include <string>
#include <deque>
#include <stdexcept>

using namespace std;
using Result = vector<vector<string>>;

struct Cell
{
    static const Cell Out_of_boundary;

    int row_ = 0;
    int col_ = 0;

    Cell (int row = 0, int col = 0)
        : row_(row), col_(col)
    {}
};

const Cell Cell::Out_of_boundary = Cell{-1, -1};

bool operator==(const Cell &l, const Cell &r)
{
    return (l.row_ == r.row_) && (l.col_ == r.col_);
}

bool operator!=(const Cell &l, const Cell &r)
{
    return (!(l == r));
}

ostream& operator<<(ostream &os, const Cell &c)
{
    os << "(" << c.row_ << "," << c.col_ << ")";
    return os;
}

template <typename T>
class Chess_desk
{
public:
    const T Occupied_ = T{1};
    const T Free_ = T{0};

    Chess_desk(int dimension = 0)
        : dimension_(dimension)
    {
        desk_.resize(dimension_);
        for (size_t row = 0; row < dimension_; ++row)
            desk_[row].resize(dimension_, Free_);
        this->initialize();
    }

    void initialize()
    {
        for (size_t row = 0; row < dimension_; ++row)
            for (size_t col = 0; col < dimension_; ++col)
                desk_[row][col] = T{0};
    }

    size_t dimension() const
    {
        return dimension_;
    }

    vector<T>& operator[](size_t row)
    {
        if (row < dimension_)
            return desk_[row];
        else
            throw std::out_of_range("Row index out of range");
    }

    bool cell_out_of_boundary(const Cell &c)
    {
        return ((c.col_ < 0) || (c.col_ >= dimension_) ||
                (c.row_ < 0) || (c.row_ >= dimension_));
    }

    void mark_queen_cells(size_t row, size_t col, const T& mark_val)
    {
        desk_[row][col] = mark_val;
        for (int r = row; r < dimension_; ++r)
            desk_[r][col] = mark_val;
        for (int r = (row + 1), c = (col + 1); (r < dimension_) && (c < dimension_); ++r, ++c)
            desk_[r][c] = mark_val;
        for (int r = (row + 1), c = (col - 1); (r < dimension_) && (c >= 0); ++r, --c)
            desk_[r][c] = mark_val;
    }

    void set_queen(const Cell &c)
    {
        if ((c.row_ < dimension_) && (c.col_ < dimension_))
            this->mark_queen_cells(c.row_, c.col_, Occupied_);
    }

    void remove_queen(const Cell &c)
    {
        if ((c.row_ < dimension_) && (c.col_ < dimension_))
            this->mark_queen_cells(c.row_, c.col_, Free_);
    }

    template <typename Iter>
    void refresh(Iter begin, Iter end)
    {
        this->initialize();
        for (Iter it = begin; it != end; ++it)
            this->set_queen(*it);
    }

    Cell find_next_free_cell(const Cell &start_cell)
    {
        if ((start_cell.row_ < 0) || (start_cell.row_ >= dimension_) ||
            (start_cell.col_ < -1) || (start_cell.col_ >= dimension_))
            return Cell::Out_of_boundary;
        for (int col = start_cell.col_ + 1; col < dimension_; ++col) {
            if (desk_[start_cell.row_][col] == Free_)
                return Cell(start_cell.row_, col);
        }
        return Cell::Out_of_boundary;
    }

private:
    using Representation = vector<vector<T>>;

    int dimension_ = 0;
    Representation desk_;
};

template <typename T>
ostream& operator<<(ostream& os, Chess_desk<T>& desk)
{
    for (size_t row = 0; row < desk.dimension(); ++row) {
        for (size_t col = 0; col < desk.dimension(); ++col) {
            os << +desk[row][col] << " ";
        }
        os << endl;
    }
    return os;
}

template<typename T>
Result n_queens_backtrack(size_t n_queens)
{
    Result result;
    if (n_queens == 0) {
        vector<string> solution;
        result.push_back(solution);
        return result;
    }
    Chess_desk<T> desk(n_queens);
    deque<Cell> backtrack_queens;
    backtrack_queens.push_front(Cell(0, -1));
    size_t n_solutions = 0;
    while (backtrack_queens.size()) {
        Cell next_cell;
        Cell current_queen_cell = backtrack_queens.front();
        if ((next_cell = desk.find_next_free_cell(current_queen_cell)) != Cell::Out_of_boundary) {
            // update backtrack deque
            backtrack_queens.pop_front();
            backtrack_queens.push_front(next_cell);
            // refresh desk
            desk.refresh(backtrack_queens.begin(), backtrack_queens.end());
            // if it is last level (n_queens), fix solution
            if (backtrack_queens.size() == n_queens) {
                // format solution and add to result
                vector<string> solution;
                for (auto it = backtrack_queens.rbegin(); it != backtrack_queens.rend(); ++it) {
                    string s(n_queens, '.');
                    s[(*it).col_] = 'Q';
                    solution.push_back(s);
                }
                result.push_back(solution);
                ++n_solutions;
            }
            else {
                // push queen to the next level, out of boundary, for the next cycle search
                backtrack_queens.push_front(Cell(next_cell.row_ + 1, -1));
            }
        }
        else {
            // no free cell for this level, pop and backtrack
            backtrack_queens.pop_front();
            // reset previous queens to the desk
            if (!desk.cell_out_of_boundary(current_queen_cell)) {
                desk.refresh(backtrack_queens.begin(), backtrack_queens.end());
            }
        }
    }
    return result;
}

class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        return n_queens_backtrack<char>(n);
    }
};

int main(int argc, char** argv)
{
    int in = (argc > 1 ? atoi(argv[1]) : 0);

    Solution s;
    Result result = s.solveNQueens(in);
    cout << "N_queens " << in << " got " << result.size() << " solutions:\n";
    for (auto &solution: result) {
        for (auto &queen: solution)
            cout << queen << ",\n";
        cout << endl;
    }

    return 0;
}
#endif

// -------------------------------------------------------------------------------------------------------
// 218. The Skyline Problem
//    A city's skyline is the outer contour of the silhouette formed by all the buildings in that city when viewed from a distance.
//    Now suppose you are given the locations and height of all the buildings as shown on a cityscape photo (Figure A),
//    write a program to output the skyline formed by these buildings collectively (Figure B).
//    The geometric information of each building is represented by a triplet of integers [Li, Ri, Hi], where Li and Ri are the x coordinates
//    of the left and right edge of the ith building, respectively, and Hi is its height. It is guaranteed that
//    0 ? Li, Ri ? INT_MAX, 0 < Hi ? INT_MAX, and Ri - Li > 0. You may assume all buildings are perfect rectangles grounded on an absolutely flat surface at height 0.
//    For instance, the dimensions of all buildings in Figure A are recorded as: [ [2 9 10], [3 7 15], [5 12 12], [15 20 10], [19 24 8] ] .

//    The output is a list of "key points" (red dots in Figure B) in the format of [ [x1,y1], [x2, y2], [x3, y3], ... ] that uniquely defines a skyline.
//    A key point is the left endpoint of a horizontal line segment. Note that the last key point, where the rightmost building ends, is merely used to
//    mark the termination of the skyline, and always has zero height. Also, the ground in between any two adjacent buildings should be considered part of the skyline contour.

//    For instance, the skyline in Figure B should be represented as:[ [2 10], [3 15], [7 12], [12 0], [15 10], [20 8], [24, 0] ].

//    Notes:

//    The number of buildings in any input list is guaranteed to be in the range [0, 10000].
//    The input list is already sorted in ascending order by the left x position Li.
//    The output list must be sorted by the x position.
//    There must be no consecutive horizontal lines of equal height in the output skyline. For instance, [...[2 3], [4 5], [7 5], [11 5], [12 7]...] is not acceptable;
//    the three lines of height 5 should be merged into one in the final output as such: [...[2 3], [4 5], [12 7], ...]
// -------------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

class Solution {
public:
    vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
        using Buildings_type = vector<vector<int>>;
        using Skyline_type = vector<pair<int, int>>;

        Skyline_type skyline;
        if (buildings.size() == 0)
            return skyline;
        if (buildings.size() == 1) {
            if (buildings[0].size() != 3)
                return skyline;
            skyline.emplace_back(buildings[0][0], buildings[0][2]);
            skyline.emplace_back(buildings[0][1], 0);
        }
        // apply divide and conquier
        // 1. divide
        size_t median = buildings.size() / 2;
        // 2. solve parts
        Buildings_type buildings1(buildings.begin(), buildings.begin() + median);
        Skyline_type  skyline1 = getSkyline(buildings1);
    }
};

int main(int argc, char** argv)
{
    int in = (argc > 1 ? atoi(argv[1]) : 0);

    Solution s;
    vector<vector<int>> buildings = {
        {2, 9, 10}
    };

    vector<pair<int, int>> skyline = s.getSkyline(buildings);

    cout << "Result skyline size: " << skyline.size() << endl;
    for (auto &dot: skyline) {
        cout << "[" << dot.first << "," << dot.second << "] ";
    }
    cout << endl;

    return 0;
}
