## GAN_text_to_image 

参考文献：[Generative Adversarial Text to Image Synthesis](https://arxiv.org/abs/1605.05396)

github地址：https://github.com/reedscot/icml2016

目录结构：

- icml2016/

  - txt2img_demo.lua: 调用网络生成图片的程序
  - model/: 训练好的模型，命名统一为`xxx_net_gen`，`xxx_net_txt`，xxx为数据集名称
  - result/: 每个数据集结果对应一个文件夹

- Txt2Img_GAN.hpp

  - 封装好的类的定义

- Txt2Img_GAN.cpp

  - 封装好的类的实现

- main.cpp

  - 使用示例，返回生成好的图片

    ```c++
    Txt2Img_GAN gan("input text here", "coco(dataset name)");
    cv::Mat dst = gan.generate();
    ```



**Note**: Txt2Img_GAN.hpp 中 Txt2Img_GAN 的 root_dir 需要指向 icml2016 所在的路径