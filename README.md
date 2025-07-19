# 3D Point Cloud Filter

Removes false obstacle points from depth camera data using plane approximation.  

## Build Instructions

### Point filter build

In the source directory execute:  

```commandline
mkdir -p build && cd build
cmake .. && make
```

### Python visualziation packages setup 

To create Python virtual environment with packages execute:  

```commandline
python3 -m venv myenv
source myenv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
```

## Usage

### Filter run

From the build directory run:  

```commandline
./filter points.txt planes.txt
```

Outputs will be:  
<input_points>_good.txt (filtered points)  
<input_points>_bad.txt (discarded points)  

### Visualizetion

With activated environment:  

```commandline
python visualize.py <input_points>_good.txt
```

## Quick Start with Scripts
Instead of running all steps manually, you can use the provided automation scripts:  

### On Linux/macOS
Use the shell script:  

```bash
./build_and_run.sh points.txt planes.txt
```

This script:    
- Builds the C++ project
- Sets up a Python virtual environment
- Runs the filter
- Visualizes the filtered points

### On Windows

Use the batch script in Command Prompt:  
```commandline
build_and_run.bat points.txt planes.txt
```