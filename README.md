# x86 汇编语言 - 从实模式到保护模式

> 第1部分：预备知识

---

## 第1章 十六进制计数法

* 进制转换

## 第2章 处理器、内存和指令

* 处理器
  * 寄存器：双向器件，可以在一端接受输入并加以锁存，同时，它也会在另一端产生一模一样的输出
  * 算术逻辑单元/算术逻辑部件(Arithmetic Logic Unit, ALU)：专门负责运算的电路，可以计算加法、减法或者乘法，也可以做逻辑运算
  * 控制器
  * 处理器内部总线
* 字节byte、字word、双字double word
* 尽管内存的最小组成单位是字节，但是，经过精心的设计和安排，它能够按字节、字、双字和四字进行访问（单次访问，而非一个个字节访问后的组合）
* 大端序和小端序
  * 常Little Endian：高字节位于高地址部分
* 指令和指令集
  * 一般来说，指令由操作码和操作数构成，但也有小部分指令仅有操作码，而不含操作数
  * 对于Intel x86处理器来说：指令长度不定，短的1字节，长的可能达到15字节
  * 立即数：操作数直接在指令中给出，不需要再次访问内存
  * 指令和数据要分开存放——代码区和数据区
  * 一个处理器能够识别的指令的集合成为该处理器的**指令集**
* 古老的Intel 8086处理器
  * 是Intel公司第一款16位处理器
  * 处理器内部有8个16位的*通用*（大部分可以根据需要用于多种目的）寄存器：AX、BX、CX、DX、SI、DI、BP、SP
    * 这8个寄存器的前4个又各自可以拆分成两个8位的寄存器来使用：AH、AL...
  * 在指令中使用了绝对内存地址（物理地址），这样的程序是无法定位的。要使用相对地址或逻辑地址，而不能使用真实的物理地址。当程序加载时，这些相对地址还要根据程序实际被加载到的位置重新计算。（重定位）
  * 重定位：任何时候，程序的重定位都是非常棘手的，当然也有好集中解决的办法
    * 8086处理器上，这个问题很好解决，因为处理器访问内存时使用了分段机制
  * 内存分段机制
    * 一个内存单元的地址实际上就可以用“段：偏移”表示，这就是通常所说的**逻辑地址**
    * 为了在硬件一级提供对“段地址：偏移地址”内存访问模式的支持，处理器至少要提供两个段寄存器，分别是代码段寄存器CS和数据段寄存器DS
      * 对CS内容的改变将导致处理器从新的代码段开始执行
      * 当处理器访问内存时，它把指令中指定的内存地址看成是段内的**偏移地址**，而不是物理地址。
  * 8086中的内存分段机制
    * 由于8086自身的局限性，它的做法还要复杂一些
    * 8086内部有8个16位通用寄存器，还有4个段寄存器：CS，DS，ES附加段寄存器（可用来指向额外的数据段，默认使用的是DS），SS栈段寄存器
    * IP（Instruction Pointer）是指令指针寄存器，只和CS一起使用（CS指向代码段起始地址，IP指向段内偏移），只有处理器才能改变它的内容。CS：IP共同形成逻辑地址，并由总线接口部件变换成物理地址来取得指令，然后处理器根据当前指令长度改变IP值，使其指向下一指令
    * 段寄存器和IP寄存器都是16位的，按原先方式直接相加只能得到16位的物理地址。但8086却提供了20根地址线（换句话说，它提供的是20位的物理地址）
      * 使用20位地址线是因为16位物理地址只能访问64KB内存，及时那个年代也有点小了。而20位地址线可访问多达1MB的内存
      * 都是16位的寄存器，如何形成20位物理地址？将段寄存器的内容左移4位再和偏移地址相加
    * 最多可以将1MB内存分成65536个段，每个段16字节
      * 8086处理器的逻辑分段，起始地址都是16的倍数，这称为是按照16字节对齐的。
    * 如果不允许段之间重叠，每个段最大长度是64KB，1MB的内存最多可以划分成16个段

## 第3章 汇编语言和汇编软件

## 第4章 虚拟机的安装和使用



> 第2部分：实模式

---

## 第5章 编写主引导扇区代码

## 第6章 相同的功能，不同的代码

* 检测点 6.1

  MOVSB指令每次传送一个**字节**，MOVSW指令每次传送一个**字**。原始数据在段内的偏移地址在寄存器**SI**中，要传送的目标位置的偏移地址在寄存器**DI**中，如果要连续传送多个字或字节，则需要**rep**前缀，在寄存器**CX**中设置传送的次数，并设置传送的方向。其中**cld**指令指示正向传送，**std**指示反向传送。反向传送时，每传送一次，SI和DI的内容将**减小**

* 检测点 6.2

  下面哪些指令是错误的，为什么？

  A. add ax, [bx] 正确

  B. mov ax, [si] 正确

  C. mov ax, [cx] 错误，在8086处理器上，如果要用寄存器来提供偏移地址，只能使用BX、SI、DI、BP

  D. mov dx, [di] 正确

  E. mov dx, [ax] 错误，同C

  F. inc byte [di] 正确

  G. div word [bx] 正确

* 计算机中的负数

  * 如果要扩展有符号数：8位到16位，将其最高位（也就是辨别符号的那一位）扩展到高8位即可
  * 几乎所有的处理器指令既能操作无符号数，又能操作有符号数，有几条除外，如：
    * 除法指令和乘法指令：
      * div工作于无符号数
      * idiv工作于有符号数

* 检测点 6.3

  假如以下声明的是有符号数，那么，其中的负数是：(判断最高位是否为1)

  data0 db **0xf0**, 0x05, 0x66, **0xff**, **0x81**

  data1 dw 0xfff, **0xffff**, 0x8b, **0x8a08**

* 检测点 6.4

  1. 当**ZF=1**时，表明计算结果为0；jz指令的意思是当**ZF=1**，即计算结果为**0**时转移；je指令的意思是当**ZF=1**，即计算结果为**0**时转移；jnz指令的意思是当**ZF=0**，即计算结果不为**0**时转移；jne指令的意思是当**ZF=0**，即计算结果不为**0**时转移。

  2. 写一段小程序，先比较寄存器AX和BX中的数值，然后，当AX的内容大于BX的内容时，转移到标号lbb处执行；等于时，转移到lbz处；AX小于BX时，转移到lbl处：

     ```asm
     cmp BX, AX
     ja lbb
     je lbz
     jb lbl
     ```

* 检测点 6.5

  反汇编命令定位到`jmp near $`，然后在此设置断电，并用"c"连续执行到该断点位置（无限循环），反汇编此语句以及后面的语句，发现Bochs会把后面的一大堆0也反汇编成指令。

* 本章习题

  1. 在某程序中声明和初始化了以下的有符号数。请问，正数和负数各有多少？

     ```assembly
     data1 db 0x05, 0xff, 0x80, 0xf0, 0x97, 0x30
     data2 dw 0x90, 0xfff0, 0xa0, 0x1235, 0x2f, 0xc0, 0xc5bc
     ```

  2. 如果可能的话，尝试编写一个主引导扇区程序来做上面的工作。

     ```assembly
     
     ```

  3. 请问下面的循环将执行多少次：

     ```assembly
     	   mov cx, 0
     delay: loop delay
     times 510-($-$$) db 0
     				 db 0x55 0xaa
     ```

## 第7章 比高斯更快的计算

* 8086处理器的寻址方式
  * 寄存器寻址：最简单的寻址方式，指令执行时，操作的数位于寄存器中，可以从寄存器里取得。
  * 立即寻址：又叫立即数寻址，也就是说，指令的操作数是一个立即数
  * 内存寻址：
    * 直接寻址(段超越)
    * 寄存器间接寻址(段超越)
    * 寄存器相对寻址 变址寄存器( SI DI )基址寄存器(BP BX)
    * 基址加变址寻址
    * 相对基址加变址寻址

## 第8章 硬盘和显卡的访问与控制

* 用户程序的结构
  
  * 分段、段的会变地址和段内汇编地址
  
    * Intel处理器要求段在内存中的起始物理地址是16字节对齐的，相应的，汇编语言源程序中定义的各个段，也有对齐方面的要求：具体做法是使用`align=`子句
    * 取得某个段的汇编地址`section.段名称.start`
    * 段定义语句可以包含`vstart=0`子句，表示在引用标号时，地址从它所在段的开头计算，而且从0开始计算
  
  * 用户程序头部
  
    * 加载器必须了解一些必要的信息，至少足以知道如何加载用户程序
  
    * 头部需要在源程序以一个段的形式出现
  
    * 起码要包含以下信息
  
      1. 用户程序的尺寸：加载器需要根据此信息决定读多少个逻辑扇区
      2. 应用程序的入口点：包括短地址和偏移地址，必须在头部给出第一条指令的段地址和偏移地址——即所谓的程序入口点（**Entry Point**）
      3. 段重定位表：
         * 程序加载到内存后，每个段的地址必须重新确定一下
  
      * ```asm
        SECTION header vstart=0                     ;定义用户程序头部段 
            program_length  dd program_end          ;程序总长度[0x00]
            
            ;用户程序入口点
            code_entry      dw start                ;偏移地址[0x04]
                            dd section.code_1.start ;段地址[0x06] 
            
            realloc_tbl_len dw (header_end-code_1_segment)/4
                                                    ;段重定位表项个数[0x0a]
            
            ;段重定位表           
            code_1_segment  dd section.code_1.start ;[0x0c]
            code_2_segment  dd section.code_2.start ;[0x10]
            data_1_segment  dd section.data_1.start ;[0x14]
            data_2_segment  dd section.data_2.start ;[0x18]
            stack_segment   dd section.stack.start  ;[0x1c]
            
            header_end:    
        ```
  
* 加载器的工作流程

  * 初始化和决定加载位置

  * 准备加载用户程序

  * 外围设备及其接口

    * 不可能将所有I/O接口直接和处理器相连，设备那么多——总线技术
    * 每个设备的I/O接口都抢着和处理器说话，冲突？——输入输出控制设备集中器（I/O Controller Hub, ICH）芯片（南桥）

  * I/O端口和端口访问

    * 端口是处理器和外围设备通过I/O接口交流的窗口，每个I/O接口都可能拥有好几个端口，分别用于不同目的
    * 连接硬盘的PATA/SATA接口
      * 命令端口，向该端口写入0x20时，表明是从硬盘读数据；写入0x30时，表明是向硬盘写数据
      * 状态端口，处理器根据这个端口的数据来判断硬盘工作是否正常，操作是否成功，发生了什么错误
      * 参数端口，通过这些端口告诉硬盘读写的扇区数量、其实逻辑扇区号
      * 数据端口，16位，通过这个端口连续地取得要读出的数据，或者通过这个端口连续发送要写入硬盘的数据
    * 端口在不同计算机系统中有不同的实现方式
      * 端口号映射到内存地址空间
      * 端口独立编址，不和内存发生关系，处理器的地址线既连接内存，也连接每一个I/O接口
    * `in`指令：不影响标志位，一字节/两字节，端口号DX或立即数，端口号是源
    * `out`指令：不影响标志位，一字节/两字节，端口号DX或立即数，端口号是目的

  * 通过硬盘控制器端口读扇区数据

    * 硬盘读写基本单位是扇区（典型的块设备）
    * 最经典的方式是向磁盘控制器分别发送磁头号、柱面号和扇区号——CHS模式
    * 实际上我们并不关心扇区的物理位置，我们希望扇区统一编址——逻辑扇区
    * 最早的逻辑扇区编址方法是LBA28，用28个比特来表示逻辑扇区号，每个扇区512字节，可以管理128GB的硬盘
    * 随硬盘技术发展，后推出LBA48，可以管理128PB的硬盘

  * 采用LBA28来访问硬盘，主硬盘控制器被分配了8位端口，端口号从0x1f0到0x1f7，现要从硬盘上读逻辑扇区，过程如下

    1. 设置读取扇区数量，要写入0x1f2端口，8位端口，所以每次最多256个扇区（0表示256）

       * ```assembly
         mov dx, 0x1f2
         mov al, 0x01	;1个扇区
         out dx, al
         ```

       * 每读一个扇区，这个值减1，因此，如果读写过程中发生错误，该端口包含着尚未读取的扇区数

    2. 设置起始LBA扇区号，将28位的扇区号分为4段，0x1f3~0x1f7，假定要读写的起始扇区号是0x02

       * ```assembly
         mov dx, 0x1f3
         mov al, 0x02
         out dx, al	;LBA地址7~0
         inc dx		;0x1f4
         mov al, 0x02
         out dx, al	;LBA地址15~8
         inc dx		;0x1f5
         out dx, al	;LBA地址23~16
         inc dx		;0x1f6
         mov al, 0xe0;LBA模式，主硬盘，以及LBA
         地址27~242s
         ```

    3. 向端口0x1f7写入0x20，请求硬盘读，这也是个8位端口

       * ```assembly
         mov dx, 0x1f7
         mov al, 0x20	;读命令
         out dx, al
         ```

    4. 操作完成后，端口0x1f7既是命令端口，又是状态端口

       ```assembly
       	mov dx, 0x1f7
       .waits:
       	int al, dx
       	and al, 0x88
       	cmp al, 0x08
       	jnz .waits		;不忙，且硬盘已准备好数据传输
       ```

    5. 连续读出数据，0x1f0是硬盘接口的数据端口，而且还是一个16位的端口

       * ```assembly
         	mov cx, 256		;总共要读取的字数
         	mov dx, 0x1f0
         .readw:
         	in ax, dx
         	mov [bx], ax
         	add bx, 2
         	loop .readw
         ```

    * 0x1f1端口是错误寄存器，包含硬盘驱动器最后一次执行命令后的状态（错误原因）

  * 过程调用

    * 处理器支持一种叫**过程调用**的指令执行机制。

    * 过程（Procedure）又叫例程，或者子程序、子过程、子例程（Sub-routine），实质都是一段普通的代码，处理器可以用过程调用指令转移到这段代码执行，在遇到过程返回指令时返回到调用处的下一条指令接着执行

    * 调用过程的指令是`call`，8086处理器支持四种调用方式

      1. 16位相对近调用：

         * 被调用的目标过程位于当前代码段内，而非不同的代码段，所以只需要得到偏移地址即可。
         * `call near proc_1`，等效于`call proc_1`
         * `near`不是必须的，如果call指令中没有提供任何关键字，则编译器认为该指令是近调用
         * 执行阶段，处理器看到操作码0xE8，就知道它应当调用一个过程，于是，它用IP当前内容加上指令中的操作数再加上3，得到一个新的**偏移地址**，接着将**IP的原有内容压栈**。最后用刚才计算出的偏移地址**取代IP原有的内容**
         * 操作数是有符号数，所以调用范围是`-2^15~2^15-1`

      2. 16位间接绝对近调用

         * 指令中的操作数不是偏移量，而是真实的**偏移地址**，故称绝对地址

         * 直接取代指令指针寄存器IP原有的内容

         * 这个偏移地址不是直接出现在指令中，而是由16位的通用寄存器或16位的内存单元间接给出

           * ```assembly
             call cx				;目标在CX中，省略了关键字near，下同
             call [0x3000]		;要先访问内存才能取得目标偏移地址
             call [bx]			;要先访问内存才能取得目标偏移地址
             call [bx+si+0x02]	;要先访问内存才能取得目标偏移地址
             ```

         * 操作数是16位的绝对地址，因此，它可以调用当前代码段任何位置处的过程

      3. 16位直接绝对远调用

         * 段间调用，即调用另一个代码段内的过程，所以称为远调用（far call）
         * `call 0x2000:0x0030`编译后机器码位`9A 30 00 00 20`，按规定，偏移地址在前，段地址在后（低位在前/低--小端）
         * 会把CS和IP都压栈

      4. 16位间接绝对远调用

         * 必须使用关键字`far`

         * 被调用过程所在的段地址和偏移地址是间接给出的

         * ```assembly
           call far [0x2000]
           call far [proc_1]
           call far [bx]
           call far [bx+si]
           ```

    * 返回指令`ret`和`retf`

      * 分别对应`call`和`call far`
      * `ret`弹出原`IP`值，`retf`弹出原`CS:IP`值
      * 尽管`call`指令通常需要`ret/retf`和它配对，但`ret/retf`指令却不依赖于`call`指令

  * 加载用户程序

  * 用户程序重定位

  * 将控制权交给用户程序

  * 8086处理器的无条件跳转指令

* 用户程序的工作流程

* 编译和运行程序并观察结果

## 第9章 中断和动态时钟显示



> 第3部分 32位保护模式

---

## 第10章 32位x86处理器编程架构

## 第11章 进入保护模式

## 第12章 存储器的保护

## 第13章 程序的动态加载和执行

## 第14章 任务和特权级保护

## 第15章 任务切换

## 第16章 分页机制和动态页面分配

## 第17章 中断和异常的处理与抢占式多任务

## bochsrc file setup

* basic set up: [me.bxrc](./me.bxrc)
* [The configuration file bochsrc](http://bochs.sourceforge.net/doc/docbook/user/bochsrc.html)

## 常见的汇编指令

> [汇编快速入门](https://www.kanxue.com/book-section_list-31.htm)

### 数据传送指令

* 包括
  * 通用传送指令（都不影响标志位）
  * 累加器专用传送指令
  * 地址传送指令
  * 标志传送指令
* 通用传送指令
  1. 基本传送指令MOV
     * 存储器操作数之间不能直接传送
     * 立即数不能直接送段寄存器
     * 段寄存器之间不能直接传送
     * CS只可以作为源操作数
     * 源操作数和目的操作数宽度必须相同
  2. 堆栈指令PUSH、POP
     * LIFO
     * SS SP 指向栈顶
     * push -> SP-2
     * 堆栈操作都按字操作
     * 操作数可以是CPU内部寄存器或存储单元
     * PUSH CS合法，POP CS非法
     * 执行push，低字节放在低地址， 高字节放在高地址
  3. 交换指令XCHG
     * 寄存器之间，寄存器和存储器之间
     * 存储器之间不能直接交换
     * 段寄存器不能作为操作数(仅一个也不行？)
     * 允许字或字节操作
* 累加器专用传送指令
  * 输入指令IN
    * 用于CPU从外设端口接收数据
    * 具体形式有四种
      * IN AL, data8; 从8位端口地址输入一个字节
      * IN AX, data8; 从8位端口地址输入一个字
      * IN AL, DX; 从16位端口地址输入一个字节
      * IN AX, DX; 从16位端口地址输入一个字
  * 输出指令OUT
    * 用于CPU向外设端口发送数据
    * 四种形式
      * OUT data8, AL; 向8位地址端口输出一个字节
      * OUT data8, AX; 向8位地址端口输出一个字
      * OUT DX, AL; 向16位地址端口输出一个字节
      * OUT DX, AX; 向16位地址端口输出一个字
* 目的地址传送指令
  * 8086提供三条
    * LEA 
    * LDS
    * LES
  * LEA (Load Effective Address)
    * LEA reg16, mem
    * reg16 -- 16位通用寄存器；mem -- 存储单元
    * 源操作数必须以寄存器间接寻址、变址寻址、基址加变址寻址等方式表示的存储器操作数
  * LDS (Load pointer using DS)
    * LDS reg16, mem;
    * 把源操作数指定的4个相继字节的数据分别送到指令指定的寄存器以及DS寄存器中
    * (reg16) <- (mem)
    * (DS) <- (mem+2)
* 标志传送指令
  * LAHF (Load AH from Flags)
    * 标志寄存器低八位 -> (AH)
    * (AH) <- (PSW的低字节)
  * SAHF (Store AH into Flags)
    * (AH) -> 标志寄存器低八位
    * (PSW的低字节) <- (AH)
  * PUSHF (Push Flags)
    * 标志进栈
    * ((SP+1), (SP)) <- (PSW)
    * SP = SP - 2
  * POPF (Pop Flags)
    * 标志出栈
    * (PSW) <- ((SP+1), (SP))
    * SP = SP + 2

### 算术运算指令

* 加法指令

  * 5条

    * ADD (Addition) (无进位)加法指令
    * ADC (Add with Carry) 带进位加法指令
    * INC (Increment) 加1指令
    * AAA (ASCII adjust for addition) 加法ASCII调整指令
    * DAA (Decimal adjust for addition) 加法十进制调整指令

  * ADD

    * 源操作数和目的操作数不能同时为存储器， 不能为段寄存器
    * 指令影响标志位
    * 溢出标志位O
    * 进位标志位C

  * ADC

    * 主要用于多字节运算 (加上上次运算的进位标志C)

    * 如：求两个32位数12345678H与ABCDEFABH之和

      ```assembly
      MOV AX, 1234H
      MOV BX, 5678H
      ADD BX, EFABH
      ADC AX, ABCDH
      ```

  * 加1指令INC
    * INC dest;
    * dest: 通用寄存器，存储器
    * 用于在循环程序中修改地址指针和循环次数
    * 影响S, Z, A, P, O; 不影响C

* 减法指令

  * 7条
    * SUB (Subtraction) 减法指令
    * SBB (Subtraction with Borrow) 进位减法指令
    * DEC (Decrement by 1) 减1指令
    * NEG (Negate) 求补指令
    * CMP (Compare) 比较指令
    * AAS (ASCII Adjust for Subtraction) 减法ASCII调整指令
    * DAS (Decimal Adjust for Subtraction) 减法十进制调整指令
  * SUB
  * SBB
    * 减去上次运算的进位标志位C
  * 减1指令DEC
    * 用于在循环程序中修改地址指针和循环次数
    * 不影响C标志位
  * 求补指令NEG
    * NEG dest;
    * 功能：(dest) <- 0 - (dest)
    * dest：通用寄存器，存储器
    * 把操作数按位取反后加1
    * 标志位
      * 进位C：操作数为0时求补，一般使C=1
      * 溢出O：对-128或-32768求补，O=1，否则O=0
  * 比较指令CMP
    * CMP dest, src; (dest) - (src)
    * 结果不保留，只是用来影响标志位
    * 比较两个数之间大小关系
      1. 根据Z标志，判断两者是否相等
      2. 根据C标志，判断两个无符号数的大小
      3. 用S、O标志，判断两个带符号数的大小

* 乘法指令

  * 无符号乘法MUL
    * MUL SRC
    * 操作
      1. 字节操作数：AX <- (AL)*(SRC)
      2. 字操作数：DX:AX <- (AX)*(SRC)
  * 带符号乘法IMUL
    * IMUL SRC
    * 操作：同MUL，但操作数和乘积均带符号
    * 按有符号数的规则相乘

* 除法指令

  * 无符号除法DIV
    * DIV SRC
    * 操作
      1. 字节除数：AL <- (AX)/(SRC)之商；AH <- (AX)/(SRC)之余
      2. 字除数：AX <- (DX:AX)/(SRC)之商；DX <- (DX:AX)/(SRC)之余
  * 带符号除法IDIV
    * IDIV SRC
    * 操作：同DIV
  * 字节扩展指令
    * CBW; Convert Byte to Word
    * 将AL字节扩展成字，符号位及高字节在AH中
  * 字扩展指令
    * CWD; Convert Word to Double Word
    * AX扩展成双字，符号位及高字在DX中

* 十进制调整指令（略）

  * 6条
    * AAA非压缩BCD码的加法十进制调整
    * DAA压缩BCD码的加法十进制调整
    * AAS非压缩BCD码的减法十进制调整
    * DAS压缩BCD码的减法十进制调整
    * AAM乘法的十进制调整
    * AAD除法的十进制调整

### 逻辑运算和移位指令（位操作类指令）

* 逻辑运算指令
  * 5条
    * AND
    * TEST 测试指令
    * OR
    * XOR
    * NOT
  * AND
    * AND dest, src;
    * 有0则0
    * 两操作数不能同为存储器操作数
  * TEST
    * TEST dest, src;
    * 规则同AND，但结果不保存，用来改变标志位
  * OR
    * 有1则1
  * XOR
    * 不同为1 相同为0
* 移位指令
  * 8条
    * SAL (Shift Arithmetic Left)算术左移
    * SAR (Shiftarithmeticright)算术右移
    * SHL (Shift logical left)逻辑左移
    * SHR (Shiftlogicalright)逻辑右移
    * ROL (Rotateleft)循环左移
    * ROR (Rotateright)循环右移
    * RCL (Rotateleftwith carry)带进位循环左移
    * RCR (Rotateright withcarry)带进位循环右移
  * 格式：操作码 reg/mem, Count
    * Count=1或CL，指定移位次数
    * 移1位或移位次数在CL中
  * 逻辑左移/算术左移指令SHL/SAL
    * 实现相同的操作，相当于无符号数X2
    * 右边的位补0
  * 逻辑右移
    * 相当于无符号数除以2
    * 高位补0
    * 最低位移入CF
  * 算术右移
    * 高位补符号位0/1（最高位不变）
    * 最低位移入CF
  * 不含进位标志循环左移指令ROL
    * 左边移出的位补到右边
    * 并把CF设为该位
  * 不含C的循环右移指令ROR
    * 同上，只是方向向右，CF同样设为循环移动的位
  * 含C循环左移RCL
    * 进位值（原CF）到低位，高位进CF
  * 含C循环右移RCR
    * 进位值（原CF）到高位，低位进CF

### 串操作指令 转移指令 处理器控制指令

#### 串操作指令

* “串”是内存中一段地址相连的字节或字

* 串操作也叫数据块操作

* 可实现存储器数据间的直接传送

* 8086中有5种基本串操作

  * MOVS（Move string）串传送指令
  * CMPS（Compare string）串比较指令
  * SCAS（Scan string）串扫描指令
  * LODS（Load from string）取串指令
  * STOS （Store in to string）存串指令

* 串传送指令MOVS

  * 串传送有2种格式

    * MOVSB

      * 字节传送，把数据段中SI寻址的数据传送到附加段中DI寻址的存储区域中，然后修改SI、DI

        ```assembly
        (ES:DI) <- (DS:SI)
        (SI) <- (SI) +/- 1
        (DI) <- (DI) +/- 1
        ```

      * 当方向标志D=0时用+，D=1时用-

    * MOVSW

      * 字传送，同上，只是一次传送1个字

* 从串中取数指令LODS

  * 2种格式

    * LODSB (字节)

      * ```assembly
        (AL) <- (DS:SI)
        (SI) <- (SI) +/- 1
        ```

    * LODSW (字)

      * ```assembly
        (AX) <- (DS:SI)
        (SI) <- (SI) +/- 2
        ```

* 存入串指令STOS

  * 2种格式

    * STOSB

      * ```assembly
        (ES:DI) <- (AL)
        (DI) <- (DI) +/- 1
        ```

    * STOSW

      * ```assembly
        (ES:DI) <- (AX)
        (DI) <- (DI) +/- 2
        ```

  * 与REP联用时，可用来建立一串相同的值，串长度由CX内容决定

* 串比较指令CMPS

  * 2种格式

    * CMPSB
    * CMPSW

  * 执行操作

    * ```assembly
      (ES:DI) - (DS:SI)
      (SI) <- (SI) +/- 1(2)
      (DI) <- (DI) +/- 1(2)
      ```

    * 结果不存，置标志

* 串扫描指令SCAS

  * 2种格式

    * SCASB

      * ```assembly
        (AL) - (ES:DI)
        (DI) <- (DI) +/- 1
        ```

    * SCASW

      * ```assembly
        (AL) - (ES:DI)
        (DI) <- (DI) +/- 1
        ```

  * 两数相减，只影响标志，不影响操作数

* CMPS和SCAS可与前缀REPE/REPZ和REPNE/REPNZ联合工作

  1. 当相等/为0时重复串操作
  2. 当不相等/不为0时重复串操作

#### 转移指令

* 转移指令控制程序从一处转换到另一处执行

* 在CPU内部，转移是通过将目标地址传送给IP来实现的

* 2种：

  * 无条件转移指令
  * 条件转移指令

* 无条件转移指令JMP(Jump)

  * 格式：JMP 语句标号

* 条件转移指令

  1. 根据单个条件标志转移
     * Z标志：JZ/JNZ
     * C标志：JC/JNC
     * P标志：JP(JPE)/JNP(JPO)
     * S标志：JS/JNS
     * O标志：JO/JNO
  2. 根据两个无符号数大小关系转移
     * JB、JNAE；JNB、JAE
     * JBE、JNA；JNBE、JA
  3. 根据两个带符号数比较结果转移
     * JL(JNGE)/JNL(JGE); 小于跳转/不小于跳转
     * JLE(JNG)/JNLE(JG)

  * 所有条件转移指令都是段内（-128～+127）？范围内转移

* 过程（子程序）调用指令

  * 子程序 -- 程序中具有独立功能的部分编写成独立程序模块
  * 子程序调用
    * CALL 子过程名
  * 返回指令 RET
    * 在子程序的结尾，用来返回主程序

* 循环控制指令

  * 无条件循环
    * LOOP 语句标号
    * 操作
      * (CX) <- (CX) - 1
      * 若CX != 0, 转向目标地址执行，否则执行LOOP指令之后的指令
  * 条件循环
    * LOOPZ/LOOPE 语句标号
    * 操作
      * (CX) <- (CX) - 1
      * 若CX != 0**且Z=1**, 转向目标地址执行，否则执行LOOP指令之后的指令
    * LOOPNZ/LOOPNE

* 中断指令（interrupt）

  * 中断调用
    * INT n
    * n -- 中断号， 0~255
  * 中断返回
    * IRET

#### 处理器控制类指令

* 标志处理指令
  * CLC （Clearcarryflag)清C标志
  * STC（Setcarryflag )置C标志
  * CMC（Complementcarryflag）对C求反
  * CLD（Cleardirectionflag)清D标志
  * STD（Setdirectionflag)置D标志
  * CLI（Clearinterruptflag)清I标志
  * STI （Setinterruptenableflag)置I标志
* 其它处理器控制指令
  * NOP（Nooperation)空操作
  * HLT（Halt) CPU暂停状态
  * WAITCPU等待状态
  * ESC交权
  * LOCK（Lockbus)总线锁定

### OPCode - 简介

略