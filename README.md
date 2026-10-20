lacam
---
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.23475645.svg)](https://doi.org/10.5281/zenodo.23475645)

> **Hexagonal LaCAM.** This repository is a modified version of [Kei18/lacam](https://github.com/Kei18/lacam) by Keisuke Okumura, the code of the paper "LaCAM: Search-Based Algorithm for Quick Multi-Agent Pathfinding" (AAAI-23). It adds hexagonal grids (odd-row, even-row, odd-column and even-column layouts) to the original square grids. Version 1.0.0 is archived on Zenodo: [10.5281/zenodo.23475645](https://doi.org/10.5281/zenodo.23475645). If you use it, please cite both the original paper and this archive (see [CITATION.cff](CITATION.cff)).

The code repository of the paper ["LaCAM: Search-Based Algorithm for Quick Multi-Agent Pathfinding"](https://kei18.github.io/lacam) (AAAI-23).
Now supports both square and hexagonal grids!

## Building

All you need is [CMake](https://cmake.org/) (≥v3.16). The code is written in C++(17).

First, clone this repo with submodules.

```sh
git clone --recursive https://github.com/syedjameel/hexagonal-lacam.git
cd hexagonal-lacam
```
Then, build the project.

```sh
cmake -B build && make -C build
```

### Docker

You can also use the [docker](https://www.docker.com/) environment (based on Ubuntu18.04) instead of the native one.

```sh
# ~10 min, mostly for CMake build
docker compose up -d
docker compose exec dev bash
> cmake -B build && make -C build
```

## Usage

### Square Grid (Default)
```sh
build/main -i assets/random-32-32-10-random-1.scen -m assets/random-32-32-10.map -N 50 -v 1
```

### Hexagonal Grid
```sh
# Using hexagonal grid with odd-row configuration (default)
build/main -m assets/hex-4-4.map -i assets/hex-4-4.scen -N 2 -g hex

# Using hexagonal grid with specific configuration
build/main -m assets/hex-4-4.map -i assets/hex-4-4.scen -N 2 -g hex -hc odd-row
build/main -m assets/hex-4-4.map -i assets/hex-4-4.scen -N 2 -g hex -hc even-row
build/main -m assets/hex-4-4.map -i assets/hex-4-4.scen -N 2 -g hex -hc odd-column
build/main -m assets/hex-4-4.map -i assets/hex-4-4.scen -N 2 -g hex -hc even-column
```

The result will be saved in `build/result.txt`.

<details><summary>Output File</summary>

This is an example output of `random-32-32-10-random-1.scen`.
`(x, y)` denotes location.
`(0, 0)` is the left-top point.
`(x, 0)` is the location at `x`-th column and 1st row.

```
agents=50
map_file=random-32-32-10.map
solver=planner
solved=1
soc=1316
soc_lb=1113
makespan=55
makespan_lb=53
sum_of_loss=1191
sum_of_loss_lb=1113
comp_time=1
seed=0
starts=(11,6),(29,9),[...]
goals=(7,18),(1,16),[...]
solution=
0:(11,6),(29,9),[...]
1:(10,6),(29,10),[...]
[...]
```

</details>

You can find details of all parameters with:
```sh
build/main --help
```

## Visualizer

This repository is compatible with [kei18@mapf-visualizer](https://github.com/kei18/mapf-visualizer).
For example,

```sh
mapf-visualizer assets/random-32-32-10.map build/result.txt
```

## Grid Types

### Square Grid
The traditional square grid with 4-connected movement (up, down, left, right).

### Hexagonal Grid
Support for hexagonal grids with 6-connected movement. Four different coordinate systems are supported:
- **odd-row**: Odd rows are offset to the right
- **even-row**: Even rows are offset to the right  
- **odd-column**: Odd columns are offset down
- **even-column**: Even columns are offset down

The hexagonal grid implementation follows the offset coordinate system described in [Red Blob Games' Hexagonal Grids](https://www.redblobgames.com/grids/hexagons/#neighbors-offset).

## Experiments

The experimental script is written in Julia ≥1.7.
Setup may require around 10 minutes.

```sh
sh scripts/setup.sh
```

Edit the config file as you like.
Examples are in `scripts/config` .
The evaluation starts by following commands.

```
julia --project=scripts/ --threads=auto
> include("scripts/eval.jl"); main("scripts/config/mapf-bench.yaml")
```

## Notes

- The grid maps and scenarios in `assets/` are from [MAPF benchmarks](https://movingai.com/benchmarks/mapf.html).
- The empirical data of the manuscript was obtained with [[exp/AAAI2023]](https://github.com/Kei18/lacam/releases/tag/exp%2FAAAI2023).
- LaCAM with different design choices: see [[pilot/greedy]](https://github.com/Kei18/lacam/releases/tag/pilot%2Fgreedy) and [[pilot/dbs]](https://github.com/Kei18/lacam/releases/tag/pilot%2Fdbs)
- `tests/` is not comprehensive. It was used in early developments.
- Auto formatting (clang-format) when committing:

```sh
git config core.hooksPath .githooks && chmod a+x .githooks/pre-commit
```

## Licence

This software is released under the MIT License, see [LICENSE.txt](LICENCE.txt).

## Author

[Keisuke Okumura](https://kei18.github.io) is a Ph.D. student at Tokyo Institute of Technology, interested in controlling multiple moving agents.
