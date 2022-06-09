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
   9 Operater:        ADD src1:          2      src2:         4      dst:         T2
  10 Operater:        SUB src1:         T2      src2:         4      dst:         T3
  11 Operater:        MUL src1:         T3      src2:         3      dst:         T4
  12 Operater:        DIV src1:         T4      src2:         2      dst:         T5
  13 Operater:     ASSIGN src1:         T5      src2:                dst:          b
  14 Operater:     ASSIGN src1:          0      src2:                dst:          a
  15 Operater:        RET src1:         T6      src2:                dst:           
  16 Operater:        RET src1:         T7      src2:                dst:           
  17 Operater:       PARA src1:          1      src2:                dst:           
  18 Operater:       PARA src1:          2      src2:                dst:           
  19 Operater:       CALL src1:       add1      src2:                dst:           
  20 Operater:     ASSIGN src1:         T6      src2:                dst:          a
  21 Operater:     ASSIGN src1:         T7      src2:                dst:          b
  22 Operater:        RET src1:         T8      src2:                dst:           
  23 Operater:        RET src1:         T9      src2:                dst:           
  24 Operater:       PARA src1:          a      src2:                dst:           
  25 Operater:       PARA src1:          b      src2:                dst:           
  26 Operater:       CALL src1:       add1      src2:                dst:           
  27 Operater:     ASSIGN src1:         T8      src2:                dst:          a
  28 Operater:     ASSIGN src1:         T9      src2:                dst:          b
----------------------
   0 Operater:     ASSIGN src1:          1      src2:                dst:          a
----------------------
   1 Operater:   FUN_PARA src1:          x      src2:                dst:           
   2 Operater:   FUN_PARA src1:          y      src2:                dst:           
   3 Operater:        ADD src1:          x      src2:         1      dst:         T0
   4 Operater:     ASSIGN src1:         T0      src2:                dst:          x
   5 Operater:        ADD src1:          y      src2:         1      dst:         T1
   6 Operater:     ASSIGN src1:         T1      src2:                dst:          y
   7 Operater:    FUN_RET src1:          x      src2:                dst:           
   8 Operater:    FUN_RET src1:          y      src2:                dst:           
----------------------
```

