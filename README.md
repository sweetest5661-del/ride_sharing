# Ride Sharing

## SmartRide sources

The `SmartRide/` folder contains the C and C++ source files from the Desktop SmartRide folder. The existing root-level `algorithms.c` and `main.cpp` are kept separately.

- `SmartRide/algorithms.c` demonstrates searching, sorting, stacks, queues, linked lists, trees, and hash tables.
- `SmartRide/main.cpp` is a console ride-sharing application with rider and driver management, ride booking, shared rides, and route calculations. It includes the adjacent `SmartRide/algorithms.c`.

## Build

From the repository root:

```sh
gcc SmartRide/algorithms.c -o smartride-c
g++ SmartRide/main.cpp -o smartride-cpp
```
