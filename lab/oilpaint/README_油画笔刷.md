# 油画笔刷模拟算法

### 依赖

- C++
- OpenCV3

### 使用

编译方式为，在ubuntu环境下输入**g++ main.cpp -o test \`pkg-config --cflags --libs opencv\` **。

输入文件名和笔刷风格（0或1），展示出原图和生成的图片。

### 其他说明

- 笔刷大小可以通过改变参数***R[]***实现。
- 中间绘画过程是否加入即放大倍数可以通过改变常量***IF_ENLARGE***和***ENLARGE_TO***参数实现。

### License

```
    Copyright 2016 Taewoo You

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
```
