# CuPRL1.0
基于CUDA的栅格并行处理编程库
栅格处理具有数据量大和计算复杂的特点，许多高性能计算工具被用于加速栅格计算，并发展出可用于栅格并行处理的可编程库。现有并行库大部分基于进程间通信实现多进程并行，可扩展性较好，但需要大量的计算资源，一般在集群上才能发挥强大的加速性能，单机上加速比不高，还会占用其它任务处理资源。GPU是一种具有强大处理能力的协处理器，计算时不占用CPU计算资源，基于GPU加速的复杂栅格处理算法往往可以获得10倍以上的加速比。现有的可用于栅格处理的GPU并行库主要用于图像处理，设计时没有考虑栅格地理计算特征，栅格地理计算并行化的可用性不强。CUDA是英伟达GPU产品的通用并行编程平台，具有较好的开发生态环境，用来进行GPU并行编程库开发具有一定优势。cuPRL是通过研究CUDA的特性，给出了一种向用户提供参数的可编程开发模式，这种模式中封装了CUDA开发中的大部分语法和函数，让CUDA的数据拷贝、核函数调用，并行模型设置等并行细节对用户透明，并通过模板将自定义部分传入接口中执行。然后分类分析栅格局部计算、邻域计算、区域计算、全局计算特征，抽象每一种计算其所需参数。本文使用一种面向单个栅格单元计算的可编程接口，并通过参数总结了接口类型。最后通过C/C++实现了基于CUDA的栅格处理编程库，并分别使用了多层栅格叠置求平均值、坡度计算、欧氏分配、区域面积统计4个算法作为各个部分的测试算法。实验计算结果表明该库可用性，时间结果表明实现的并行栅格算法体现了GPU的加速特性。对于并行性强的栅格计算，计算复杂度低的算法一般有5-10倍的加速比，计算复杂度适中的算法一般有10-20倍的加速比，计算复杂度较高的算法一般有20倍以上加速比；对于并行性弱的栅格计算，对GPU加速有一定影响，但最高也能达到10倍左右的加速。