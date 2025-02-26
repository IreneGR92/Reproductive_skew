# Reproductive competition

Simulation code, written in C++, to support the paper:
"Harsh environments promote the evolution of plural breeding".

## Getting Started

### Configuration

- Configs can be set in config.yml if you want to change the default values.
- you also can create a file in your $HOME directory called .reproductive_skew.yml to have your custom configuration
  values. This will overwrite the default values in config.yml.

### Pre conditions

- The software was built and tested on Ubuntu Linux -> this guide only covers Ubuntu Linux x86_64 if you are using macOS
  or Windows you may need to adapt some steps.
- Cmake, make and gcc installed
- The project now uses the `FetchContent` CMake module to download and build the `yaml-cpp` library directly from its
  GitHub repository. This means you no longer need to manually compile and reference `yaml-cpp` in the `CMakeLists.txt`
  file.

### Compile the binary

1. Make folder build and navigate into it `mkdir build` -> `cd build`
2. Generate make files with  `cmake .. -DCMAKE_BUILD_TYPE=Release`
3. Compile binary with `make`
4. The binary target App should be successfully built now.

### Run

Now you can normally run it by adding the relative path to the parameters yaml file as first run parameter:

`./App ../parameters/example.yml`

The output of the application is then stored in file `main_parameters_example.txt`
and `last_generation_parameters_example.txt`

You can modify the input parameters of the model by modifying the yml file. First lines allow you to choose between the
different models (with/without age-dependent plasticity and with/without relatedness building up from model dynamics).
