# raspberry-pi-pico
Various Raspberry Pi Pico Projects

## SDK Setup

Run `git submodule update --init --recursive` in the repo root or clone this repo with `git clone --recursive <REPO_URL>`.

## Build

```
mkdir -p build
cd build
cmake ..
make -j
```