## 水墨笔触转换

此部分基于论文 [Image-Based Rendering for Ink Painting](http://www.hcii-lab.net/download/papers/conference/Image-Based%20Rendering%20for%20Ink%20Painting.pdf)，项目主体用MATLAB实现。

要编译此项目，请按照以下步骤执行：

1. 安装 MATLAB Compiler SDK。

2. 在`lab/ink`文件夹下执行

   ```bash
   mcc -W cpplib:libinkdisp -T link:lib inkdisp.m lischinski.m -d libdisp
   ```

   即可在`libdisp`文件夹下找到生成的动态库。

要运行此组件，只需要将生成的`*.so`文件拷贝到项目相关目录中即可。
