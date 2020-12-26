# Predefinition

assuming x,y integer

**CEILDIV(x,y)**: (x+y-1)/y

**ALIGN(x,y)**:CEILDIV(x,y) * y

**TRUNC(x,y)**: x/y*y
    
**ALIGN2POWER(x)**: minimum positive 2's power that is larger than x


# DDR memory


## Feature map memory
----
Assuming the data width is **FM_DATA_WIDTH** and FM_DATA_WIDTH \<= 8

Assuming the feature map is represented as **FM[D][H][W][B]**.

Assuming the Memory is stored in **8bit-addressed memory** **MEM**.

We assume all the dimension of each feature map are aligned to 8.

Following relationship reveal the mapping between the feature map and the memory location

    FM[d][h][w][b] = MEM[B*(h*ALIGN(W,8)*ALIGN(D,8)+TRUNC(d,8)*ALIGN(W,8)+w*8+d%8)+b]

**ONGOING**
CHANGING this result for input into 

    FM[d][h][w][b] = MEM[B*(h*ALIGN(W,8)*ALIGN(D,8)+d*ALIGN(W,8)+w)+b]



## Weight memory
----

### 3x3 Weight memory

Assuming the data width is **WT_DATA_WIDTH** and WT_DATA_WIDTH \<= 7

Assuming the 3x3 convolution weight is represented as **WT[ID][OD][3][3]**.




Following hardware parameters decides the storage pattern of the weight memory:

**INPUT_MINITILE_SIZE**: the number of input channels parallely processed in one winograd PE, should always 2's power
**WINO_HEIGHT**: the height of the wino systolic array
**WEIGHT_BUFFER_DEPTH**: the depth of the weight buffer in hardware, usually times of 1024

<!-- The **WINO_HEIGHT** decides the number of the weight port **WEIGHT_PORT_NUMBER**


| **WINO_HEIGHT** |  **WEIGHT_PORT_NUMBER** |
|:---:|:---:              |
| <=4 | = **WINO_HEIGHT** |
| >4  | 4                 | -->


Assuming the Memory is stored in 64bit-addressed memory **MEM**.


Fristly, we pack a single weight filter of **3\*3** 7bit data into a 64-bit word **WT_WORDS** by directly alignment:

    WT_WORDS[id][od].range( (i*3+j)*7+6,(i*3+j)*7+6 )= WT[id][od][i][j]
 
Overall the input depth and output depth are seperated into multi level tiling with each level tiling size to be the integer times of the next level tiling size

**ID**->**INDEPTH_MIDTILE**->**INDEPTH_MINITILE**

**OD**->**OUTDEPTH_MIDTILE**->**OUTDEPTH_MINITILE**->**PORT_MINITILE**


The **INDEPTH_MINITILE_SIZE** and **OUTDEPTH_MIDTILE_SIZE** are decided by hardware memory configurations.

The number and size of  **PORT_MINITILE** are decided by the following rule 

| **WINO_HEIGHT** | **PORT_MINITILE_NUMBER** | **PORT_MINITILE_SIZE[i]** 
|:---:|:---:|:---:|:---:|
| <=4 | **WINO_HEIGHT** |1 |
| >4  | 4               |CEILDIV( WINO_HEIGHT-i-1, 4)|

Following is an example of segment weight output depth 10 into port segments with **WINO_HEIHGT** to be 5


|**PORT_MINITILE** | included depth idx |**PORT_MINITILE_SIZE**|
|:---:|:---:|:---:|:---:|
|**PORT_MINITILE[0]**|0,5        | 2 |
| |1,6        |   |   |
|**PORT_MINITILE[1]**|2,7        |  1 |
|**PORT_MINITILE[2]**|3,8        |  1 |
|**PORT_MINITILE[3]**|4,9        |  1 |


Following is an example of segment weight output depth 8 into port segments with **WINO_HEIHGT** to be 4


|**PORT_SEGMENT** | included depth idx |**PORT_SEGMENT_SIZE**|**PORT_MINITILE_SIZE**|
|:---:|:---:|:---:|:---:|
|**PORT_SEGMENT[0]**|0,4        | 2 | 1 |
|**PORT_SEGMENT[1]**|1,5        | 2 | 1 |
|**PORT_SEGMENT[2]**|2,6        | 2 | 1 |
|**PORT_SEGMENT[3]**|3,7        | 2 | 1 |




**TODO: CURRENTLY ASSUME THAT ALL THE PORT SEGMENT ARE EQUAL SIZE**

The new index order goes following

**PORT_MINITILE_IDX**->**PORT_MINITILE_INNERIDX**->**OUTDEPTH_MIDTILE_IDX**->**INDEPTH_MIDTILE_IDX**->**INDEPTH_MINITILE_IDX**->**OUTDEPTH_MINITILE_IDX**->**IDEPTH_MINITILE_INNERIDX**

Following are the way to compute the upper bound of each index
|name| tile index | index upper bound|
|:---:|:---:|:---:|:---:|

|**OUTDEPTH_MIDTILE_NUM**|**OUTDEPTH_MIDTILE_IDX**|OD/OUTDEPTH_MIDTILE_SIZE |
|**OUTDEPTH_MINITILE_NUM**|**OUTDEPTH_MIDTILE_IDX**|OUTDEPTH_MIDTILE_SIZE、OUTDEPTH_MINITILE_SIZE|
|**PORT_MINITILE_NUM**|**PORT_MINITILE_IDX**|OUTDEPTH_MIDTILE_SIZE / PORT_MINITILE_SIZE |

|**INDEPTH_MIDTILE_NUM**|**INDEPTH_MIDTILE_IDX**|ID / INDEPTH_MIDTILE_SIZE |
|**INDEPTH_MINITILE_NUM**|**INDEPTH_MINITILE_IDX**|INDEPTH_MIDTILE_SIZE /INDEPTH_MINITILE_SIZE |
|**INDEPTH_MINITILE_SIZE**|**INDEPTH_MINITILE_INNERIDX**|INDEPTH_MINITILE_SIZE |



The mapping between weight and mem



    //id,od for WT_WORDS[id][od]
    //port_idx 
    

    outdepth_midtile_idx=od/OUTDEPTH_MIDTILE_SIZE
    outdepth_minitile_idx=od%OUTDEPTH_MIDTILE_SIZE/OUTDEPTH_MINI_TILE_SIZE
    port_minitile_idx=od%OUTDEPTH_MINI_TILE_SIZE/PORT_MINITILE_SIZE
    port_minitile_inneridx=od%PORT_MINITILE_SIZE/1

    indepth_midtile_idx=id/INDEPTH_MIDTILE_SIZE
    indepth_minitile_idx=id%INDEPTH_MIDTILE_SIZE/INDEPTH_MINITILE
    indepth_minitile_inneridx=id%INDEPTH_MINITILE_SIZE/1

    address=port_minitile_idx
    address=address*OUTDEPTH_MIDTILE_NUM+outdepth_midtile_idx
    address=address*INDEPTH_MIDTILE_NUM+indepth_midtile_idx
    address=address*INDEPTH_MINITILE_NUM+indepth_minitile_idx
    address=address*OUTDEPTH_MINITILE_NUM+outdepth_minitile_idx
    address=address*PORT_MINITILE_SIZE+port_minitile_inneridx
    address=address*INDEPTH_MINITILE_SIZE+indepth_minitile_inneridx

    MEM[address]=WT_WORDS[id][od]


# Memory Port

The memory ports are instantiated as multiple ports with specified bitwidth. The **PORT_NUMBER** and **PORT_WIDTH** are related to hardware configurations

## Read input memory port

|**WINO_WIDTH**|**PORT_WIDTH**|
|:---:  |:---:|
| 4     | 64  |
|>=8    | 128 | 

**PORT_NUMBER**: Always 4

**TODO**: Add load input function to make it less


# Onchip Memory Protocol


## Input Memory Buffer



Assuming the feature map is represented as **FM[D][H][W][B]**.

Assuming the input feature map is store in on-chp buffer memory **INBUFF[B][INBUFFER_HEIGHT][INBUFFER_WIDTH][INBUFFER_DEPTH]**

Here, parameters **INBUFFER_HEIGHT**,**INBUFFER_WIDTH** and **INBUFFER_DEPTH** are all 2's powers

We have

    //ROW_BIT
    //INDETPH_MINITILE_BIT
    indepth_minitile_bit=ap_uint<INDETPH_MINITILE_BIT> d%indepth_minitile_bit;
    indepth_minitile_bit=

    FM[d][h][w][b]




