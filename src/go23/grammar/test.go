package main

import "fmt"

var g int = ^20

func swap(x, y int) (int, int) {
   return y, x
}

func main() {
   // if语句





	var a,b int = 10,20
	if a < 20 {
	    fmt.Printf("a 小于 20\n" );
	} else {
	    fmt.Printf("a 不小于 20\n" );
	}

	// for语句
	var sum int
	for i:=0; i <= 10; i++ {
	    sum += i
	}
	for sum <= 15 { //10加到15
		sum += sum
	}
	for {}

	// 函数调用
	var s1,s2 int = "aaa","bbb"
	s3, s4 := swap(s1, s2)

	// 全局变量vs局部变量
	var g int = 10
	fmt.Printf ("结果: g = %d\n",  g)

	// 指针
	var tmp int= 20
	var p *int
	ip = &a
	fmt.Printf("*ip 变量的值: %d\n", *ip )
}