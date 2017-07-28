# Glamorous

Maintained by llj_bash.

E-mail: lljbash@gmail.com

## Platform

Ubuntu 16.04 LTS

## Project

### Code

Writen mainly by C++ with C++11 standard.

Follow *Google C++ Style Guide*. (not exactly the same)

Well self-commented codes with necessary READMEs. Start reading from `project/app/app.h`.

### Prerequirements

- gcc 5.4.1

- CMake 2.8

- OpenCV 3.2

- Python 2.7 (libpython2.7.so)

- python-tk

- Pip2: networkx scipy numpy skimage matplotlib sklearn pdb

- Matlab R2017a (Installed in /usr/local/MATLAB/R2017a. Change it in CMakelists.txt)

- Matlab Compile SDK

- [Torch](http://torch.ch/docs/getting-started.html) (Installed in ~/torch/install/lib. Change it in CMakelists.txt)

- Some Lua libraries (unsup cutorch cunn cudnn). Call `luarocks` to install them if error occurs.

- Cuda 8.0

- Cudnn 5.1

### Compile

- Go to `project/pipeline/non-cpp/lua/style_transfer/models`, run `sh download_models.sh`.

- Go to `project/pipeline/non-cpp/lua/gan/icml2016/model`, download 6 models in `url`.

- Go to `pipeline/non-cpp/python/photo2sketch/imgsaliency`, run `python2 setup.py build && sudo python2 setup.py install`

- Download [image database](https://drive.google.com/file/d/0BwkXIzTQn8H2OEpDb0lnR24zZEk/view?usp=sharing).

- Change line 9 in `project/app/app.cc` to where you placed your database.

- Go to `project`

- `cp ../lab/ink/libdisp/libinkdisp.so pipeline/non-cpp/matlab/ink`

- `mkdir build && cd build`

- `cmake ..`

- `make -j$(nproc)`

### How to use

Go to `project/build/bin` (or somewhere you placed the executable file). Copy following 4 folders to the directory where you want to execute the program.

```
project/pipeline/non-cpp/lua/gan/icml2016
project/pipeline/non-cpp/lua/style_transfer/lualib
project/pipeline/non-cpp/lua/style_transfer/lua-style
project/pipeline/non-cpp/lua/style_transfer/models
```

#### testapp

Given text and style, generate image and video.

```
Ussage:
  ./testapp text style-number adj
```

#### server

A TCP-socket server for GlamorousApp. See `project/app/testserver.py` to find how to use it.

```
Ussage:
  ./server portno database_path
```

For more about style-number, see comments in `project/app/app.h`.

## Web

Use Python2 Django as web framework.

### Prerequirements

- Python 2.7
- Django
- PIL

### How to use

- Copy following 4 folders to `web/AIPainting/static`.

```
project/pipeline/non-cpp/lua/gan/icml2016
project/pipeline/non-cpp/lua/style_transfer/lualib
project/pipeline/non-cpp/lua/style_transfer/lua-style
project/pipeline/non-cpp/lua/style_transfer/models
```

- Copy `project/build/bin/server` to `web/AIPainting/static`.

- Change line 126 in `web/AIPainting/AIPainting/settings.py` to where your 'static' directory are.

- Change line 41 in `web/AIPainting/AIPainting/views.py` to where your image database are.

- **(IMPORTANT)** In `web/AIPainting/static`, run
```
python2 ../manage.py runserver 127.0.0.1:2333
```

Visit [AIPainting](127.0.0.1:2333/homepage) and enjoy painting!

