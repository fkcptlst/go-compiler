package main


func myprint(a int)


func main() {
	var v1, v2, v3, v4, v5, v6, v7, v8 int
	v1 = 1
	v2 = 2
	v3 = 3
	v4 = 4
	v5 = 5
	v6 = 6
	// v1 后面没使用，会覆盖 v1
	v7 = 7
	// v7 覆盖一个变量，被覆盖的变量会被 push
	v8 = 8

	myprint(v2)
	myprint(v3)
	myprint(v4)
	myprint(v5)
	myprint(v6)
	myprint(v7)
	myprint(v8)

	v1 = 11
	v2 = 22
	v3 = 33
	v4 = 44
	v5 = 55
	v6 = 66
	v7 = 77
	v8 = 88

	myprint(v2)
	myprint(v3)
	myprint(v4)
	myprint(v5)
	myprint(v6)
	myprint(v7)
	myprint(v8)
}
