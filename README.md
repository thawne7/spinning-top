# 摇摇乐（spinning-top）
## 2020年新工科联盟-Xilinx暑期学校（Summer School）项目
### 项目名称
摇摇乐
### 项目概要
基于SEA-S7开发平台实现，首先通过FPGA采集开发平台上自带的IMU传感感器的数据，随后对数据进行滤波，利用滤波后的数据计算得出一分钟内开发板被摇动的次数，类似手机记录每天行走的步数， 将次数数据发送给ESP32，最后由ESP32上传到服务器端进行显示。
### 预期结果
能够正确采集IMU数据并换算得出手摇开发板的次数，并将数据传输到AWS的云端服务器节点上，加上滤波算法，对IMU数据进行滤波.最后小组之间进行比较。
### 外设
* 开发平台SEA-S7
* FPGA板卡型号为xc7s15ftgb196-1
* JTAG仿真器
### 使用工具
* Arduino-IDE（用于ESP32开发和调试）
* vivado 2018.3（用于FPGA开发）。
### 小组成员
* 吴克楠 2018112764
* 吕卓 2018112766

### 仓库目录介绍：
Source文件夹下存放项目源代码，其中Gyro_qspi文件夹是包含FPGA源码的vivado工程文件，Gyro2AWSIoT文件夹是包含ESP32源码的arduino文件。
Images文件夹下存放作品照片及效果图。

### 作品照片：
![image](https://github.com/thawne7/spinning-top/blob/master/Images/IMG20200801160327.jpg)
### 效果图：
#### 在串口监视器中显示摇动次数：
![image](https://github.com/thawne7/spinning-top/blob/master/Images/QQ%E5%9B%BE%E7%89%8720200801105400.png)
#### 在AWS IoT平台上显示摇动次数：
![image](https://github.com/thawne7/spinning-top/blob/master/Images/QQ%E5%9B%BE%E7%89%8720200801105422.png)
![image](https://github.com/thawne7/spinning-top/blob/master/Images/QQ%E5%9B%BE%E7%89%8720200801105419.png)
