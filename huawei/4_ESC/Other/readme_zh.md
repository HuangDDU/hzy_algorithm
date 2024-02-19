# Local Test Archive
# 本地测试发布

This archive includes the local interactor and the sample tests to facilitate the development of problem solutions.
这个发布包括了本地的交互器和一些开发过程中的测试用用例。

## Interactor
## 本地的交互器

For your convenience we provide `local_runner.py` -- a script that runs and scores your solution on a specified test. It judges your solution in the same way the system interactor does, but it is **not** used to evaluate your solution in the system.
为了方便我们提供了`local_runner.py`，该脚本运行并评价得分在特定的测试用例上，他与评估系统的计算方式相同，但是并**不**直接评估。

It should be run as:
按照如下方式运行

`python3 local_runner.py %input file% %baseline results file% -- %command to execute solution%`

Example:
示例：

`python3 local_runner.py sample_tests/00.txt sample_tests/00_baseline.txt -- python3 my_solution.py`

You can also run the interactor on tests for which you do not have baseline results. In this case the interactor will not score your solution, but it will output raw cluster cost.
即使没有baseline得分，你也可以在测试用例上运行交互器。这种情况下交互不会评估解决方案，而是输出原始集群得分。

In this case it should be run as:
这种情况下，按照如下方式运行（我的本地系统，使用python命令，而不是python3命令）

`python3 local_runner.py %input file% -- %command to execute solution%`
`python local_runner.py %input file% -- %command to execute solution%`

Example:
示例：

`python3 local_runner.py sample_tests/01.txt -- ./baseline`
`python local_runner.py sample_tests/01.txt -- ./baseline`

## Tests
## 测试

The `sample_tests` directory contains 4 sample tests and the corresponding baseline results. These tests are **not** present in the testing system.
这里`sample_tests`文件夹包含4个测试用例以及对应的baseline结果，这里的测试用例里**不会**在测试系统中出现