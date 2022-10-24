# Binary packages

## Install

You can download:

- Via .zip archive
- Via git clone git@github.com:stupakdm/binarypackages.git

## Execute

Execution:

- To compile programm you need to enter command from command line python3 main.py {args} where {args} - names of two branches(default: p10 and p9)
  - Example: python3 main.py p10 p9
- Programm prints a result in a JSON-format. Format of a result:
  - task1{branch_one}_{branch_two}_length 
  - task1{branch_one}_{branch_two}_packages 
  - task2{branch_one}_{branch_two}_length 
  - task2{branch_one}_{branch_two}_packages 
  - task3{branch_one}_{branch_two}_length 
  - task3{branch_one}_{branch_two}_packages