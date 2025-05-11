1. me.cpp：搜索限界条件：到达边界、访问过、字符不相同，找到解后即回溯。先污染然后治理，使用visited数组判断字符是否被访问过。
2. me2.cpp：相较于me.cpp限制了最后一步的搜索。
3. ref.cpp：使用空字符''代表访问过。
3. ref2.cpp：尽可能往参考解上靠，只是变量不一样，不知道为什么过不了。
> 最终也只是参考：https://leetcode.cn/problems/word-search/solutions/2361646/79-dan-ci-sou-suo-hui-su-qing-xi-tu-jie-5yui2/?envType=study-plan-v2&envId=top-100-liked