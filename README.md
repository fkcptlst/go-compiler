# 所以你★star★了吗！！！
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
	var b int = 4-(2+4-4)+3;
	a=0;
	if a < 20 {
	a = 2;
	} else {
	if a < 10 {
	a = 1;
	} else {
	a = 3;
	}
	}
    for i := 0; i <= 10; i++ {
                a = i;
    }
	a,b=add1(a,b)
	a,b=add1(1,2)
	// var
}
```

```
   9 Operater:        ADD src1:    IMM:      2      src2:   IMM:      4      dst:    VAR:     T8
  10 Operater:        SUB src1:    VAR:     T8      src2:   IMM:      4      dst:    VAR:    T10
  11 Operater:        SUB src1:    IMM:      4      src2:   VAR:    T10      dst:    VAR:    T11
  12 Operater:        ADD src1:    VAR:    T11      src2:   IMM:      3      dst:    VAR:    T13
  13 Operater:     ASSIGN src1:    VAR:    T13      src2: NULL_:             dst:    VAR:      b
  14 Operater:     ASSIGN src1:    IMM:      0      src2: NULL_:             dst:    VAR:      a
  15 Operater:       IFGE src1:    VAR:      a      src2:   IMM:     20      dst:    VAR:  ELSE0
  16 Operater:     ASSIGN src1:    IMM:      2      src2: NULL_:             dst:    VAR:      a
  17 Operater:       GOTO src1:  NULL_: ENDIF0      src2: NULL_:             dst:  NULL_:       
  18 Operater:      LABEL src1:  NULL_:  ELSE0      src2: NULL_:             dst:  NULL_:       
  19 Operater:       IFGE src1:    VAR:      a      src2:   IMM:     10      dst:    VAR:  ELSE1
  20 Operater:     ASSIGN src1:    IMM:      1      src2: NULL_:             dst:    VAR:      a
  21 Operater:       GOTO src1:  NULL_: ENDIF1      src2: NULL_:             dst:  NULL_:       
  22 Operater:      LABEL src1:  NULL_:  ELSE1      src2: NULL_:             dst:  NULL_:       
  23 Operater:     ASSIGN src1:    IMM:      3      src2: NULL_:             dst:    VAR:      a
  24 Operater:      LABEL src1:  NULL_: ENDIF1      src2: NULL_:             dst:  NULL_:       
  25 Operater:      LABEL src1:  NULL_: ENDIF0      src2: NULL_:             dst:  NULL_:       
  26 Operater:     ASSIGN src1:    IMM:      0      src2: NULL_:             dst:    VAR:      i
  27 Operater:       IFGT src1:    VAR:      i      src2:   IMM:     10      dst:    VAR:ENDFOR0
  28 Operater:      LABEL src1:  NULL_:FORLOOP0      src2: NULL_:             dst:  NULL_:       
  29 Operater:     ASSIGN src1:    VAR:      i      src2: NULL_:             dst:    VAR:      a
  30 Operater:        ADD src1:    VAR:      i      src2:   IMM:      1      dst:    VAR:      i
  31 Operater:       IFLE src1:    VAR:      i      src2:   IMM:     10      dst:    VAR:FORLOOP0
  32 Operater:      LABEL src1:  NULL_:ENDFOR0      src2: NULL_:             dst:  NULL_:       
  33 Operater:        RET src1:    VAR:    T22      src2: NULL_:             dst:  NULL_:       
  34 Operater:        RET src1:    VAR:    T23      src2: NULL_:             dst:  NULL_:       
  35 Operater:       PARA src1:    VAR:      a      src2: NULL_:             dst:  NULL_:       
  36 Operater:       PARA src1:    VAR:      b      src2: NULL_:             dst:  NULL_:       
  37 Operater:       CALL src1:    VAR:   add1      src2: NULL_:             dst:  NULL_:       
  38 Operater:     ASSIGN src1:    VAR:    T22      src2: NULL_:             dst:    VAR:      a
  39 Operater:     ASSIGN src1:    VAR:    T23      src2: NULL_:             dst:    VAR:      b
  40 Operater:        RET src1:    VAR:    T26      src2: NULL_:             dst:  NULL_:       
  41 Operater:        RET src1:    VAR:    T27      src2: NULL_:             dst:  NULL_:       
  42 Operater:       PARA src1:    IMM:      1      src2: NULL_:             dst:  NULL_:       
  43 Operater:       PARA src1:    IMM:      2      src2: NULL_:             dst:  NULL_:       
  44 Operater:       CALL src1:    VAR:   add1      src2: NULL_:             dst:  NULL_:       
  45 Operater:     ASSIGN src1:    VAR:    T26      src2: NULL_:             dst:    VAR:      a
  46 Operater:     ASSIGN src1:    VAR:    T27      src2: NULL_:             dst:    VAR:      b
----------------------
   1 Operater:   FUN_PARA src1:    VAR:      x      src2: NULL_:             dst:  NULL_:       
   2 Operater:   FUN_PARA src1:    VAR:      y      src2: NULL_:             dst:  NULL_:       
   3 Operater:        ADD src1:    VAR:      x      src2:   IMM:      1      dst:    VAR:     T2
   4 Operater:     ASSIGN src1:    VAR:     T2      src2: NULL_:             dst:    VAR:      x
   5 Operater:        ADD src1:    VAR:      y      src2:   IMM:      1      dst:    VAR:     T4
   6 Operater:     ASSIGN src1:    VAR:     T4      src2: NULL_:             dst:    VAR:      y
   7 Operater:    FUN_RET src1:    VAR:      x      src2: NULL_:             dst:  NULL_:       
   8 Operater:    FUN_RET src1:    VAR:      y      src2: NULL_:             dst:  NULL_:       
----------------------
   0 Operater:     ASSIGN src1:    IMM:      1      src2: NULL_:             dst:    VAR:      a
----------------------
```

生成汇编：

```shell
./tjugo ../example/<A>.go
```

运行汇编：

```shell
. ../scripts/run_asm.sh <A>
```

