# millet porridge III --stage3

```
package main

var a int=1;

func add1(x,y int) (int, int) {
	x = x + 1
	y = y + 1
	return x,y
}

func main() {
	var a int;
	var b int = (2+4-4)*3/2;
	a=0;
	a,b=add1(1,2)
	a,b=add1(a,b)
	
}

```



```
/*对应add1函数*/
/**/
1 ADD x 1 T0 
2 ASSIGN T0  x 
3 ADD y 1 T1
4 ASSIGN T1  y
5 FUN_RET x  
6 FUN_RET y  
----------------------
0 ASSIGN 1  a
----------------------
7 ADD 2 4 T2
8 SUB T2 4 T3
9 MUL T3 3 T4
10 DIV T4 2 T5
11 ASSIGN T5  b
12 ASSIGN 0  a
13 PARA 1  
14 PARA 2  
15 CALL add1  
16 RET T6  
17 RET T7  
18 ENDCALL add1  
19 ASSIGN T6  a
20 ASSIGN T7  b
21 PARA a  
22 PARA b  
23 CALL add1  
24 RET T8  
25 RET T9  
26 ENDCALL add1  
27 ASSIGN T8  a
28 ASSIGN T9  b
----------------------
```

```
   1 Operater:        ADD src1:          x      src2:         1      dst:         T0
   2 Operater:     ASSIGN src1:         T0      src2:                dst:          x
   3 Operater:        ADD src1:          y      src2:         1      dst:         T1
   4 Operater:     ASSIGN src1:         T1      src2:                dst:          y
   5 Operater:    FUN_RET src1:          x      src2:                dst:           
   6 Operater:    FUN_RET src1:          y      src2:                dst:           
----------------------
   0 Operater:     ASSIGN src1:          1      src2:                dst:          a
----------------------
   7 Operater:        ADD src1:          2      src2:         4      dst:         T2
   8 Operater:        SUB src1:         T2      src2:         4      dst:         T3
   9 Operater:        MUL src1:         T3      src2:         3      dst:         T4
  10 Operater:        DIV src1:         T4      src2:         2      dst:         T5
  11 Operater:     ASSIGN src1:         T5      src2:                dst:          b
  12 Operater:     ASSIGN src1:          0      src2:                dst:          a
  13 Operater:       PARA src1:          1      src2:                dst:           
  14 Operater:       PARA src1:          2      src2:                dst:           
  15 Operater:       CALL src1:       add1      src2:                dst:           
  16 Operater:        RET src1:         T6      src2:                dst:           
  17 Operater:        RET src1:         T7      src2:                dst:           
  18 Operater:    ENDCALL src1:       add1      src2:                dst:           
  19 Operater:     ASSIGN src1:         T6      src2:                dst:          a
  20 Operater:     ASSIGN src1:         T7      src2:                dst:          b
  21 Operater:       PARA src1:          a      src2:                dst:           
  22 Operater:       PARA src1:          b      src2:                dst:           
  23 Operater:       CALL src1:       add1      src2:                dst:           
  24 Operater:        RET src1:         T8      src2:                dst:           
  25 Operater:        RET src1:         T9      src2:                dst:           
  26 Operater:    ENDCALL src1:       add1      src2:                dst:           
  27 Operater:     ASSIGN src1:         T8      src2:                dst:          a
  28 Operater:     ASSIGN src1:         T9      src2:                dst:          b
----------------------
```

