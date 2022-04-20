# simpleScanner
A simple scanner written in C++ to detect documents 

## Usage

## Building
This project requires opencv. [Installation link](https://cv-tricks.com/installation/opencv-4-1-ubuntu18-04/)

```
g++-9 -std=c++17 main.cpp `pkg-config --libs --cflags opencv4` -o result -O3
```

## Running
```
./result [image_path] [result_path {optional}(default: "result.png")]
```
## Example

Test image
![Test image](https://github.com/aimbot6120/simpleScanner/blob/main/test.jpg)

Result image
![Result image](https://github.com/aimbot6120/simpleScanner/blob/main/result.png)

## What's next?
Better argparser\
Batch processing\
Crop preview\
Proper build instructions
