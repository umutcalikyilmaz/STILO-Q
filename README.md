[stilo]: https://github.com/umutcalikyilmaz/STILO
[stilo-problems]: https://github.com/umutcalikyilmaz/STILO#creating-problem-instances
[stilo-solvers]: https://github.com/umutcalikyilmaz/STILO#solving-problem-instances
[stilo-analyzers]: https://github.com/umutcalikyilmaz/STILO#analyzing-solver-configurations

# STILO-Q
STILO-Q is an extension of **[STILO][stilo]**, a metaheuristic framework for optimization under strict time limits. In addition to the metaheuristic methods provided by STILO, STILO-Q implements binary and multi-level quantum-inspired evolutionary algorithms (QEAs), including variants incorporating graph-based qudit representations and heuristic initialization.

## Relationship to STILO

STILO-Q retains the general functionalities of STILO, including problem modeling, synthetic instance generation, conventional metaheuristic solvers, and time-limited experimental analysis.

STILO-Q is distributed as a separate, self-contained library and uses the `STILOQ` namespace instead of `STILO`. Since it shares many classes and functionalities with its predecessor, this documentation often refers to the [STILO documentation][stilo] where appropriate, rather than repeating existing instructions. 

When following the original STILO documentation, references to the `STILO` namespace and the `STILO/` header directory should be replaced with `STILOQ` and `STILOQ/`, respectively.

## Installation
STILO-Q is designed for Debian-based systems and is currently not compatible with Windows or macOS. STILO-Q requires CMake 3.20 or later and a compiler supporting C++20. For standard installation, the following bash instructions should be executed in the project's root folder.

```bash
mkdir build
cd build
cmake ..
sudo make install
```

## Usage
### Importing the Library
After installation, STILO-Q can be imported into a project by adding the following lines to the `CMakeLists.txt` file.

```cmake
find_package(STILOQ REQUIRED)
target_link_libraries(my_project STILOQ::stiloq)
```

The framework headers can be included using:

```c++
#include <STILOQ/STILOQ.h>
```

### Creating Problem Instances

STILO-Q provides the same problem classes and instance generation mechanisms as STILO. Instances can be created either from complete problem data or by providing parameters for random instance generation.

For detailed descriptions of the supported problem types and instructions for creating problem instances, please refer to the [STILO documentation][stilo-problems].

### Solving Problem Instances
STILO-Q contains five solver classes:
- `ACOSolver` (Ant Colony Optimization Solver)
- `GASolver` (Genetic Algorithm Solver)
- `SASolver` (Simulated Annealing Solver)
- `BQEASolver` (Binary Quantum-Inspired Evolutionary Algorithm Solver)
- `MQEASolver` (Multi-Level Quantum-Inspired Evolutionary Algorithm Solver)


Each solver is derived from the base `Solver` class and provides a `solve()` member function for solving a given problem instance under a specified configuration. Solver inputs are standardized through the `SolverInput` class. The examples below demonstrate how to configure and use `BQEASolver` and `MQEASolver`. The purpose and behavior of the individual configuration parameters are described in detail in the referenced paper. The other solvers are described in the [STILO documentation][stilo-solvers].

`BQEASolver` and `MQEASolver` share the same algorithmic structure and use a common configuration class, `QEAConfiguration`, accessible through the `QEAConfig` member of a `SolverInput` instance. Depending on the solver to which the SolverInput object is passed, this configuration determines the behavior of either BQEA or MQEA. Both uses are demonstrated in the following example.

```cpp
STILOQ::SolverInput input;

input.problem = &problemInstance;   // Problem*
input.timeLimit = timeLimit;        // std::chrono::milliseconds

input.QEAConfig.populationSize = populationSize;                          // int
input.QEAConfig.measurementCount = measurementCount;                      // int
input.QEAConfig.graphStructure = graphStructure;                          // STILOQ::GraphStructure
input.QEAConfig.initializationType = initializationType;                  // STILOQ::InitializationType
input.QEAConfig.angleCalculationOperator = angleCalculation;              // STILOQ::AngleCalculationOperator
input.QEAConfig.limitAngle = limitAngle;                                  // double 
input.QEAConfig.globalBestRotationCount = globalBestRotationCount;        // int
input.QEAConfig.baseGlobalBestAngle = baseGlobalBestAngle;                // double
input.QEAConfig.iterationBestRotationCount = iterationBestRotationCount;  // int
input.QEAConfig.baseIterationBestAngle = baseIterationBestAngle;          // double
input.QEAConfig.modelBestRotationCount = modelBestRotationCount;          // int
input.QEAConfig.baseModelBestAngle = baseModelBestAngle;                  // double
input.QEAConfig.heuristicInfluence = heuristicInfluence;                  // double

// Solving the problem using BQEA
STILOQ::BQEASolver bqeaSolver;
auto bqeaOutput = bqeaSolver.solve(input);

// Solving the problem using MQEA
STILOQ::MQEASolver mqeaSolver;
auto mqeaOutput = mqeaSolver.solve(input);
```

The following table shows the solver classes and configuration settings corresponding to the four QEA variants evaluated in the referenced paper.

| Algorithm variant | Solver class | Graph structure | Initialization |
|---|---|---|---|
| BQEA | `BQEASolver` | `STILOQ::GraphStructure::Sequential` | `STILOQ::InitializationType::Standard` |
| MQEA | `MQEASolver` | `STILOQ::GraphStructure::Sequential` | `STILOQ::InitializationType::Standard` |
| MQEA-G | `MQEASolver` | `STILOQ::GraphStructure::SingleStage` | `STILOQ::InitializationType::Standard` |
| MQEA-GH | `MQEASolver` | `STILOQ::GraphStructure::SingleStage` | `STILOQ::InitializationType::Heuristic` |

#### Scope of the Experimental Evaluation

The experimental evaluation presented in the referenced paper does not cover all configurations supported by STILO-Q.  For example, only BQEA configurations using `STILOQ::GraphStructure::Sequential` and `STILOQ::InitializationType::Standard` are considered. Similarly, MQEA configurations using `STILOQ::GraphStructure::MultiStage` are not included in the evaluation. The framework supports the use of such configurations, but their performances are not evaluated in the referenced paper.

Furthermore, the experimental analysis is restricted to the application of these algorithms to the Traveling Salesperson Problem (TSP), although STILO-Q supports the application of BQEA and MQEA to all implemented problem types.

### Analyzing Solver Configurations

STILO-Q retains the two analyzer classes provided by STILO: `SyntheticAnalyzer` and `InstanceAnalyzer`. For detailed descriptions of these classes and their usage, please refer to the [STILO documentation][stilo-analyzers].

In STILO-Q, the CSV files generated by the analysis tools are stored in `~/.stilo-q/instance/analysis`.
