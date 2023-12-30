# Simple C HTTP Server
This is a basic implementation of a static HTTP server in C using the Winsock library.
## Inspired by
This project was developed with inspiration from the [cerveurus](https://github.com/Kiyoshika/cerveurus) project, which provided insights and inspiration for certain design decisions.

Please note that this project was created solely for learning purposes and is not intended to be used as real server.

## Usage
```bash
# Copy repo
git clone https://github.com/Virees/c-server
cd c-server

# Create the build folders
mkdir bin
mkdir build

# Compile
make
```
Inside the bin folder a ```HttpServer.exe``` file should be created. Copy the ```html``` and ```css``` folders inside the bin folder and run the .exe file.