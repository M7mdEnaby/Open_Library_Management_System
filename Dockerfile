# Use an official C++ development environment as the base image
FROM ubuntu:20.04

# Install dependencies and basic tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libglfw3 \
    libssl-dev \
    wget \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/app

# Copy the project files into the container
COPY . .

# Build and install project dependencies
# 1. Build OpenSSL
RUN cd openssl_1 && ./config && make && make install && cd .. \
    && cd openssl_2 && ./config && make && make install && cd ..

# 2. Build ImGui
RUN mkdir -p build/vendor/ImGui \
    && cd build/vendor/ImGui \
    && cmake ../../../vendor/ImGui \
    && make

# Build the project
RUN mkdir -p build && cd build \
    && cmake .. \
    && make

# Set the command to run your application
CMD ["./build/YourExecutableName"]
