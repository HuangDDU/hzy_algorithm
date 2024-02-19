# Local Test Archive

This archive includes the local interactor and the sample tests to facilitate the development of problem solutions.

## Interactor

For your convenience we provide `local_runner.py` -- a script that runs and scores your solution on a specified test. It judges your solution in the same way the system interactor does, but it is **not** used to evaluate your solution in the system.

It should be run as:

`python3 local_runner.py %input file% %baseline results file% -- %command to execute solution%`

Example:

`python3 local_runner.py sample_tests/00.txt sample_tests/00_baseline.txt -- python3 my_solution.py`

You can also run the interactor on tests for which you do not have baseline results. In this case the interactor will not score your solution, but it will output raw cluster cost.

In this case it should be run as:

`python3 local_runner.py %input file% -- %command to execute solution%`

Example:

`python3 local_runner.py sample_tests/01.txt -- ./baseline`

## Tests

The `sample_tests` directory contains 4 sample tests and the corresponding baseline results. These tests are **not** present in the testing system.
