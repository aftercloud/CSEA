功能实现情况请参看 gl_1 2018_9_10 0_31_51.mp4 和
pl_2 2018_9_10 0_28_48.mp4

如何使用：
（1）运行gl_1.exe,按下r键开启旋转，按下x，y，z键选择旋转轴，按下c键渐变颜色，按下方向键移动光源；
（2）运行gl_2.exe，按住鼠标左键并拖动将旋转视图，按住鼠标右键并水平拖动将缩放视图；

如何实现：
（1）颜色渐变：将当前颜色与目标颜色分成若干步，借助双缓冲的方式不断改变色彩即可；
（2）自动旋转：同样是借助双缓冲，使旋转角度每次自增一度即可；
（3）基于鼠标的相机控制：通过注册鼠标点击和移动时间，计算鼠标移动过程中和初始按下位置的差值，确定旋转和缩放的程度，只要在鼠标的拖动过程中不断调用绘图函数即可；

