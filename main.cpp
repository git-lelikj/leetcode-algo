/*
 * main.cpp
 *
 *  Created on: May 17, 2017
 *      Author: jemkov
 */

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
