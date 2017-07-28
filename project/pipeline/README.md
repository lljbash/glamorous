提供了一套封装良好的自动作图、风格变换、视频生成的API。

采用流水线模式，组件采用工厂模式，使用样例见project/app/app(.h/.cc)。
可以自行扩展组件，见project/pipeline/src/component/README

include目录中的头文件自恰，足够配合libpipeline.a使用。

