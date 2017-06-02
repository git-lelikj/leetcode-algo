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

template <typename F, typename Duration = nanoseconds, typename ...Args>
pair<string,typename Duration::rep> benchmark(F f, int iterations, Args&& ...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    string res;
    for (int c=0; c < iterations; ++c)
        res = f(std::forward<Args>(args)...);
    auto stop = std::chrono::high_resolution_clock::now();
    return pair<string,typename Duration::rep>(res, duration_cast<Duration>(stop-start).count()/iterations);
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
//        auto res = benchmark(mem_fn(&Solution::reverseWords), 1000000, s, "Ladenzon is full");
//        cout << "reverseWords: " << res.first << ", avg duration: " << res.second << " ns\n";
//    }

    return 0;
}
