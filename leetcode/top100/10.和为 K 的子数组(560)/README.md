1. 我的
    - me.cpp : 打表记录所有区间内的累加和，然后扫描. 时间复杂度$O(N^2)$,空间复杂度$O(N^2)$. 爆了超内存。
    - me2.cpp : 不打表了，直接扫描每行累加和. 时间复杂度$O(N^2)$,空间复杂度$O(1)$. 爆了超时。

2. 参考：
    - ref1.cpp : 打哈希表记录前缀和出现的次数，确定前缀区间. 时间复杂度$O(N)$,空间复杂度$O(N)$.