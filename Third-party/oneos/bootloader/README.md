

# CMIOT Bootloader OTA移植指南


- [简介](#section11660541593)

- [使用说明](#section18867101215181)

  （1）[资源](#section18867101211200)

  （2）[数据结构](#section18867101211236)

  （3）[接口描述](#section18867101248536)

  （4）[升级结果返回码](#section18456801275696)

  （5）[适配流程](#section18456801211236)

- [断电异常保护机制](#section18867101215488)

- [注意事项](#section68521336131912)

- [FAQ](#section68521336132233)

  

## 简介<a name="section11660541593"></a>

​		远程升级（Firmware Over The Air）可通过无线网络实现固件升级。OneOS的OTA为差分升级，采用此方式可极大降低升级包的大小。OneOS OTA组件分两部分：BootLoader和App，BootLoader实现固件还原功能、升级功能，App部分完成更新检测、固件下载、版本校验，升级结果上报。

​		以下为bootloader ota适配相关说明。。



## 使用说明<a name="section18867101215181"></a>

### （1） 资源<a name="section18867101211200"></a>

​        	在bootloader阶段，需要依赖的资源有：

​      	 ①内存，用于存放还原过程中的数据，通常内存空间越大越好；

​      	 ②堆栈，程序必须运行在堆栈已经完成正确初始化的C环境下；

​      	 ③flash操作接口，包括下载及升级相关的存储介质的读/写/擦除接口。

### （2） 数据结构<a name="section18867101211236"></a>

- FLASH和分区相关结构体。

  ```c
  typedef enum
  {
      FAL_UPGRADE_PART   = 0x00,
      FAL_DOWNLOAD_PART  = 0x01,
      FAL_BACKUP_PART    = 0x02,
  } fal_part_type_e;
  
  typedef struct ota_fal_flash_ops
  {
      ota_int32(*init)(void);
      ota_int32(*read)(ota_uint32 addr, ota_uint8 *buf, ota_size_t size);
      ota_int32(*write)(ota_uint32 addr, const ota_uint8 *buf, ota_size_t size);
      ota_int32(*erase)(ota_uint32 addr, ota_size_t size);
  } ota_fal_flash_ops_t;
  
  typedef struct ota_fal_flash
  {
      ota_int8   name[FAL_NAME_MAX];        /* flash 名称 */
      ota_size_t size;                      /* flash 大小 */
      ota_size_t erase_size;                /* flash 最小擦除单位 */
      ota_fal_flash_ops_t ops;              /* flash 读写擦操作函数 */
  } ota_fal_flash_t;
  
  typedef struct ota_fal_part_info
  {
      ota_int8   name[FAL_NAME_MAX];        /* 分区 名称 */
      ota_int8   flash_name[FAL_NAME_MAX];  /* 分区 所在flash名称 */
      ota_uint32 address;                   /* 分区 地址 */
      ota_uint32 size;                      /* 分区 大小 */
      ota_uint32 part_type;                 /* 分区 类型 */
  } ota_fal_part_info_t;
  ```

### （3） 接口描述<a name="section18867101248536"></a>


- 堆空间初始化函数`ota_heap_init`

  ```c
  ota_int32 ota_heap_init(ota_uint8 *buf, ota_size_t buf_size, ota_size_t block_size);
  ```

  | 参数           | 说明                                         |
  | :------------- | :------------------------------------------- |
  | **buf**        | 升级可使用内存空间                           |
  | **buf_size**   | 升级可使用内存大小                           |
  | **block_size** | 一次分配最小单元（按block_size的整数倍分配） |
  | **返回值**     | 成功0， 失败-1                               |

  ​        注：当bootloader中堆不可用时，可以使用该接口进行内置堆管理。提供给还原升级的内存应尽可能大，除了系统栈所需内存，和预留静态占用的内存外，剩余内存可以都用于还原升级。该内存可以通过定义一个静态数组的方式进行预分配。 为了提高内存的利用率，block_size不宜过大，内置对管理模块会对buf_size 和block_size进行合理性检查，当初始化返回失败时，根据提示log进行调整即可。

  

- flash分区表初始化函数`ota_fal_init`

  ```c
  ota_int32 ota_int32 ota_fal_init(ota_fal_flash_t *flash_tab, ota_size_t flash_tab_size, ota_fal_part_info_t *part_tab, ota_size_t part_tab_size);
  ```

  | 参数           | 说明            |
  | :------------- | :-------------- |
  | **flash_tab**  | flash类型表     |
  | **flash_size** | flash类型表大小 |
  | **part_tab**   | 分区划分表      |
  | **part_size**  | 分区划分表大小  |
  | **返回值**     | 成功0， 失败-1  |

  

- 升级进度条显示函数`update_proc_bar`

  ```c
  void ota_update_proc_bar(ota_int32 percentage);
  ```

  | 参数           | 说明                |
  | -------------- | ------------------- |
  | **percentage** | 当前升级进度：0~100 |

  ​        注：该接口需用户实现，在升级过程中被调用，不需要应用代码主动调用。通常是将进度条通过屏幕显示，因此需要在该函数中实现屏幕显示进度条的功能。

  

- LOG格式化输出函数`ota_printf`

  ```c
  void ota_printf(const ota_int8 *fmt, ...);
  ```

  ​        注：升级过程中的LOG信息，该接口需用户实现，无需打印日志时置为空函数即可。

  

- 升级启动函数`ota_start_up`

  ```c
  ota_int32 ota_start_up(void);
  ```

  | 参数       | 说明                             |
  | :--------- | -------------------------------- |
  | **void**   | 无参数                           |
  | **返回值** | 升级结果返回码（见返回码说明表） |

​               注：升级启动前，应完成flash分区表初始化。



### （4） 升级结果返回码<a name="section18456801275696"></a>

| **状态码** |       **说明**        |
| :--------: | :-------------------: |
|   **0**    |       升级成功        |
|   **1**    |      不需要升级       |
|  ***2***   |       内存不足        |
|  ***3***   |      无效升级包       |
|  ***4***   |   前置版本校验错误    |
|  ***5***   |    升级包类型错误     |
|  ***6***   |     升级包不存在      |
|  ***7***   |      未找到分区       |
|  ***8***   |     分区读取错误      |
|  ***9***   |     分区写入错误      |
|  ***10***  |     分区擦除错误      |
|  ***11***  |   分区数据备份错误    |
|  ***12***  |   分区数据还原错误    |
|  ***13***  |      CRC校验错误      |
|  ***14***  |   流信息初始化错误    |
|  ***15***  |   头信息初始化错误    |
|  ***16***  |      解压头错误       |
|  ***17***  |    解压数据体错误     |
|  ***18***  |  覆盖线差值计算错误   |
|  ***19***  |    构建新数据错误     |
|  ***20***  |     FALSH擦除失败     |
|  ***21***  |     数值解析错误      |
|  ***22***  |    还原块长度错误     |
|   **23**   |  差分包数据校验错误   |
|   **24**   | 新APP分区数据校验错误 |

​        （注：返回码0和1都表示升级成功，其余表示升级失败。）

### （5） 适配流程<a name="section18456801211236"></a>

1. **适配各分区flash读写、擦除接口**

   **<1>** Flash操作接口

   ```
     ota_int32(*init)(void);
   ```

   ​       flash的初始化一般在bootloader硬件初始化阶段已完成，init接口为NULL；若需要在fal初始化中完成，则可以在init接口中完成flash的硬件初始化。成功返回0，失败返回-1。

   ```c
     ota_int32(*read)(ota_uint32 addr, ota_uint8 *buf, ota_size_t size);
   ```

   ​       数据读取接口，成功返回读取长度，失败返回-1。

   ```c
     ota_int32(*write)(ota_uint32 addr, const ota_uint8 *buf, ota_size_t size);
   ```

   ​       数据写入接口，成功返回写入长度，失败返回-1。

   ```c
     ota_int32(*erase)(ota_uint32 addr, ota_size_t size);
   ```

   ​       数据擦除接口，成功返回0，失败返回-1。

   

    **<2>** 将flash操作操作函数，flash名称，容量，块大小（擦除大小）填写至`ota_fal_flash_t`类型的表中。如果有多个不同的flash设备，根据实际情况，实现相应的操作函数，并将硬件信息添加到flash表中即可。

   

2. **填写分区表**

   ​        `ota_fal_part_info_t`类型的分区表中包含了需要升级的“APP”分区(将要被升级的系统分区)以及“download”分区(存放升级时使用的差分包)，除了download分区名称是固定的，其他“APP”的名称可以任意命名。每个分区，包含了分区名称，所在flash名称，起始地址，分区大小，分区类型（当使用LuckPand算法时，可以存在多个升级分区FAL_UPGRADE_PART，但有且仅有一个下载分区FAL_DOWNLOAD_PART），分区名称不可重复。

   

3. **初始化分区信息**

    将flash表和分区表作为参数，调用`ota_fal_init`接口完成分区信息初始化。

   

4. **启动升级**

   **<1>** 调用`ota_start_up`接口，启动升级。对比返回码，可以查看升级结果以及错误码。

   **<2>** 检查升级状态

   ​          在启动升级时，将读取download分区最后一个块的起始地址第**3**和**4**字节`update_result`，判断是否需要进入还原升级，当`update_result`的值等于**100**时，表示需要进入还原升级,  同时起始地址第**38**个字节`code`等于5表示整包升级，不等于5表示差分升级；升级后会将升级结果写入`update_result`， **1**表示升级成功， **99**表示升级失败。（注：APP读取升级结果后，应该将升级结果字段`update_result`数值清除）

   ```c
       struct cmiot_update_state
       {
         unsigned char  inited;
         unsigned short update_result;
         ...
         unsigned char  code;
         ...
       };
   ```

   

5. **参考示例**

​      移植可参考demo目录中`ota_main.c`或直接使用`ota_main.c`（需要修改相关分区信息和接口），调用`cmiot_ota_main()`实现升级还原。

  |      目录      | 说明                                |
  | :------------: | :---------------------------------- |
  |    luckpand    | luckpand算法适配示例                |
  |    luckspar    | luckspar算法适配示例                |
  |    vf_linux    | 基于文件系统的虚拟flash测试使用示例 |



## 断电异常保护机制<a name="section18867101215488"></a>

1. 根据块还原顺序从download分区和app分区读取对应的差分数据和原始数据，简单校验app分区中该块是否已正确还原，如果已正确还原，则继续执行步骤1读取下一块数据，否则执行步骤2。
2. 读取数据备份区域，校验该区域中数据是否正确还原，如果已正确还原，将该区域数据拷贝至app分区对应区域，否则执行步骤3。
3. 基于步骤1读取数据，使用差分还原算法还原出新数据后写入数据备份区域，继续执行步骤2。



## 注意事项<a name="section68521336131912"></a>

1. ​    差分还原算法使用内存和实际还原块大小(-b block_size)密切相关。通常，在恢复过程中，MCU使用大约2到3倍实际还原块大小内存+解压算法所需的内存。该工具默认使用LZMA解压，需要额外的6KB内存，否则可能会导致还原升级时内存不足而升级失败。
2. download分区除差分包存放区域外，还另外需要两个区域用于备份和存放信息，所以download分区大小至少需要三个扇区。
3. flash操作接口适配时，当读写成功时返回读写数据的长度，读写失败时，返回-1；擦除失败时返回-1，擦除成功返回0。
4. 在没有完善堆可使用ota_heap_init初始化升级使用内存，有完善堆空间时可不使用。



## FAQ<a name="section68521336132233"></a>

详细见[FAQ](./FAQ.md)。















