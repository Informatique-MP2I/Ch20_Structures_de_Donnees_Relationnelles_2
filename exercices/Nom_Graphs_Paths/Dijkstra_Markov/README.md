# Markov Chains Algorithm Project

## Overview

This project implements a graph data structure and Dijkstra's algorithm to find
the shortest path in a Markov chain graph. The project is written in C and includes functionalities for creating, manipulating, and displaying graphs.

## Directory Structure

```
.
├── Doxyfile                   # Configuration file for Doxygen
├── Makefile                   # Makefile for building the project
├── README.md                  # This README file
├── include
│   ├── graph_list.h           # Header file with graph structure and function declarations
│   └── heap.h                 # Header file with heap structure and function declarations
└── src
    ├── graph_list.c           # Implementation of graph functions
    ├── heap.c                 # Implementation of heap functions
    └── main_dijkstra_markov.c # Main program file         
```

## Compilation

To compile the project, use the provided Makefile. The binary will be named
`dijkstra_markov` and will be placed at the `bin` directory.

```sh
make
```

This command will:

Compile the source files in `src/`.
Place the object files in `build/`.
Generate the binary `dijkstra_markov` in the `bin/` directory of the project.

## Running the Program

After compilation, run the program using the following command:

```sh
./bin/dijkstra_markov
```

The program will create a graph, run Dijkstra's algorithm for Markov chain, and display the
shortest path from the source to a specified target vertex.

## Generating Documentation

To generate the documentation using Doxygen, run:

```sh
make docs
```

The documentation will be generated in `docs/`.

You can open the generated documentation with something like :

```sh
firefox docs/html/index.html
```

from the root directory of the projet. 

## Cleaning Up

To clean up the build directory and remove the binary and documentation, use:

```sh
make clean
```

## Dijkstra's shortest paths Algorithm for Markov chains

The implementation includes a function for Markov chains via
Dijkstra's algorithm to find the
shortest paths from a source vertex to all other vertices in the graph.

## Example Usage

The `main_dijkstra_marov.c` file demonstrates how to create a graph
and print the shortest path to a target vertex. The function `print_sentence`
displays the path from the source to the target.

## License

This project is licensed under the GNU Lesser General Public License (LGPL). 

You can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This project is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this project. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.

## Authors

Asli Grimaud and Gilles Grimaud

## Acknowledgements

* Doxygen for documentation generation
* GNU Make for build automation

