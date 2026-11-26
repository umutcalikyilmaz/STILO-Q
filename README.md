[stilo]: https://github.com/umutcalikyilmaz/STILO
[stilo-problems]: https://github.com/umutcalikyilmaz/STILO#creating-problem-instances
[stilo-solvers]: https://github.com/umutcalikyilmaz/STILO#solving-problem-instances
[stilo-analyzers]: https://github.com/umutcalikyilmaz/STILO#analyzing-solver-configurations

# STILO-Q
STILO-Q is an extension of **[STILO][stilo]**, a metaheuristic framework for optimization under strict time limits. In addition to the metaheuristic methods provided by STILO, STILO-Q implements binary and multi-level quantum-inspired evolutionary algorithms (QEAs), including variants based on graph-based qudit representations and heuristic initialization.

## Relationship to STILO

STILO-Q retains the general functionalities of STILO, including problem modeling, synthetic instance generation, conventional metaheuristic solvers, and time-limited experimental analysis.

STILO-Q is distributed as a separate, self-contained library and uses the `STILOQ` namespace instead of `STILO`. Since it shares many classes and functionalities with its predecessor, this documentation often refers to the [STILO documentation][stilo] where appropriate, rather than repeating existing instructions. 

When following the original STILO documentation, references to the `STILO` namespace and the `STILO/` header directory should be replaced with `STILOQ` and `STILOQ/`, respectively.

## Installation
STILO is designed for Debian-based systems and is currently not compatible with Windows or macOS. STILO requires C++20. For standard installation, the following bash instructions should be executed in the project's root folder.

```bash
mkdir build
cd build
cmake ..
sudo make install
```

## Usage
### Importing the Library
After installation, STILO-Q can be imported to a project by adding the following lines in the `CMakeLists.txt` file.

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
STILOQ contains three solver classes:
- `ACOSolver` (Ant Colony Optimization Solver)
- `GASolver` (Genetic Algorithm Solver)
- `SASolver` (Simulated Annealing Solver)
- `BQEASolver` (Binary Quantum-Inspired Evolutionary Algorithm Solver)
- `MQEASolver` (Multi-Level Quantum-Inspired Evolutionary Algorithm Solver)


Each solver is derived from the base `Solver` class and provides a `solve()` member function for solving a given problem instance under a specified configuration. Solver inputs are standardized through the `SolverInput` class. The examples below demonstrate how to configure and use `BQEASolver` and `MQEASolver`. The purpose and behavior of the individual configuration parameters are described in detail in the referenced paper. The other solvers are explained in the STILO documentation.

`BQEASolver` and `MQEASolver` share the same algorithmic structure and the same configuration space. Therefore, the configurations for both solvers are controlled by the same class, `QEAConfiguration`. A `SolverInput` object has a member of this class. If a `SolverInput` instance is used as the input of a `BQEASolver` instance, it determines the configurations of the BQEA algorithm. Likewise, if it is used as an input for a `MQEASolver` instance, it determines the configurations of the BQEA algorithm. Both uses are shown below.

```
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

STILOQ::BQEASolver bqeaSolver;
bqeaSolver.solve(input);

STILOQ::MQEASolver mqeaSolver;
mqeaSolver.solve(input);
```

| Algorithm variant | Solver class | Graph structure | Initialization |
|---|---|---|---|
| BQEA | `BQEASolver` | Sequential | Standard |
| MQEA | `MQEASolver` | Sequential | Standard |
| MQEA-G | `MQEASolver` | Graph-based | Standard |
| MQEA-GH | `MQEASolver` | Graph-based | Heuristic |

It is important to note that the cited paper does not cover all configurations provided by STILO-Q. For example, in the paper, only BQEA configurations with a graph structure of `STILOQ::GraphStructure::Sequential`, and initialization type of `STILOQ::InitializationType::Standard` are considered. Similarly, MQEA configurations with `STILOQ::GraphStructure::MultiStage` are not considered. The framework allows the use of such configurations, although the paper does not provide any results on their effectiveness. Additionally, the cited paper does not cover the use of BQEA and MQEA configurations on any other problem than the traveling salesperson problem, even though STILO-Q allows them to be used for all implemented problem types.

### Analyzing Solver Configurations

STILO-Q contains the two analyzer classes provided by STILO: `SyntheticAnalyzer` and `InstanceAnalyzer`. For a detailed description of the usage and function of these classes, please refer to the STILO documentation. The most important difference of the analysis tools in STILO-Q is the directory that the result CSV files written to is `~/.stilo-q/instance/analysis`.
