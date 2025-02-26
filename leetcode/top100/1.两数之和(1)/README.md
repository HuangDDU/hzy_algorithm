1. me.cpp : 自己写的，用两层循环暴力求解，时间复杂度$O(n^2)$，空间复杂度$O(1)$,

2. ref.cpp : 官网参考，使用unordered_map进行打表查找，以空间换时间，时空复杂度均为$O(n^2)$。
3. ref_review_250226: 复习ref.cpp，复习for循环，unorder_map的使用

4. 这里区分unordered_map与map：<https://blog.csdn.net/qq_41979948/article/details/129693079。>

    - unordered_map对键无序，方便查找、插入、删除。底层哈希表。
    - map对键排序，方便遍历。底层红黑树。
