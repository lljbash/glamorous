## Nerual Style Transfer

此部分基于论文 [Arbitrary Style Transfer in Real-time with Adaptive Instance Normalization](https://arxiv.org/abs/1703.06868) 作者自己发布的GitHub项目 [AdaIN-style](https://github.com/xunhuang1995/AdaIN-style)。本项目依赖 [Torch7](http://torch.ch) 及一些 Lua 包。要正确运行此组件，请按照以下步骤运行：

1. 下载并安装 torch7。

2. 尝试运行 AdaIN-style 样例 (in `lab/AdaIN-style`

   ```bash
   th test.lua -content path/to/content.jpg -style path/to/style.jpg
   ```

   并按照错误提示用`luarocks`安装需要的Lua包。

3. 此时即可调用工程中的相关C++类实现组件功能。


