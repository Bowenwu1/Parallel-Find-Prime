# Parallel-Find-Prime
利用MPI实现并行的线性筛数法

## 编译
* 串行

    ```shell
    g++ normal_find_prime.cpp -o normal_find_prime
    ```

* 并行

    ```shell
    mpic++ parallel_find_prime.cpp -o parallel_find_prime
    ```

## 运行
* 串行

    ```Shell
    ./normal_find_prime [n]
    ```

* 并行

    ```Shell
    mpirun -n [进程数] --oversubscribe parallel_find_prime [i]
    ```

    * 进程数 ：新建的进程数
    * $i$ : 筛选小于等于 $2^i*10^3$ 的素数

## 测试结果

### 计时方法

统计rank为0的进程所花的时间，使用MPI_WTime()获取时间，再做差



### Pipeline 通信方法测试结果

| $i$  | $n=2^i*10^3$ | 素数个数 | 进程数目($\lceil\frac{\sqrt{n}}{2}\rceil$) | Time       |
| ---- | ------------ | -------- | ------------------------------------------ | ---------- |
| 0    | 1000         | 168      | 16                                         | 0.00392103 |
| 1    | 2000         | 303      | 23                                         | 0.00455213 |
| 2    | 4000         | 550      | 32                                         | 0.00591898 |
| 3    | 8000         |          | 45                                         |            |
| 4    | 16000        |          | 64                                         |            |
| 5    | 32000        |          | 90                                         |            |
| 6    | 64000        |          | 127                                        |            |
| 7    | 128000       |          | 179                                        |            |
| 8    | 256000       |          | 253                                        |            |
| 9    | 512000       |          | 358                                        |            |



