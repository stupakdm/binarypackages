# Binary packages

## Install

You can download:

- Via .zip archive
- Via git clone git@github.com:stupakdm/binarypackages.git

## Execute

Execution:

- To compile programm you need to
  - Change rules for file execute.sh with a command: chmod +x ./execute.sh
  - Enter command: ./execute.sh {argv1} {argv2}
- It will compile main.cpp and uses argv1 and argv2 as branches. 
- Programm will download binary packages from this branches.
- Example: ./execute p10 p9
  
  Programm prints a result in a files 
  - packages11_result.txt - first task. 
  - packages12_result.txt - second task.
  - packages2_result.txt - third task. 
