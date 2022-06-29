package main


// 声明 myprint 函数
func myprint(x int)


func test_assign() {
	var v1, v2, v3, v4, v5, v6, v7, v8 int
	v1 = 1
	v2 = v1
	v3 = v2
	v4 = v3
	v5 = v4
	v6 = v5
	v7 = v6
	v8 = v7

	myprint(v2)
	myprint(v3)
	myprint(v4)
	myprint(v5)
	myprint(v6)
	myprint(v7)
	myprint(v8)

	v1 = 2
	v2 = v1
	v3 = v2
	v4 = v3
	v5 = v4
	v5 = v4
	v6 = v5
	v7 = v6
	v8 = v7

	myprint(v2)
	myprint(v3)
	myprint(v4)
	myprint(v5)
	myprint(v6)
	myprint(v7)
	myprint(v8)
}


func test_calculate() {
	var a int = 4 - (2 + 4 - 4 * 2) + 3
	a++
	myprint(a)					// 预计输出 10
}


func test_control() {
	// 分支语句
	var a int = 5
	if a < 10 {
		myprint(1)				// 预计输出 1
		if a < 5 {
			myprint(11)
		} else {
			myprint(12)			// 预计输出 12
		}
	} else {
		myprint(2)
	}

	// 循环语句
	var sum = 0
	var i int
	for i = 0; i < 10; i++ {
		var j = i
		sum += j
	}
	myprint(sum)				// 预计输出 45
}


func add_1(a int, b int) (int, int) {
	return a + 1, b + 1
}


func test_func() {
	var a = 100;
	var b = 105;
	var c, d = add_1(a, b)
	myprint(c)				// 预计输出 101
	myprint(d)				// 预计输出 106
}


func test_array() {
	var v[5] int
	v[0] = 1
	v[1] = 2
	v[2] = 3
	v[3] = 4
	v[4] = 5

	var i1 int
	for i1 = 0; i1 < 5; i1++ {
		// var j = i1
		// myprint(v[i1])
		// var k = i1
		// v[j] = k + 1
	}

	// var i2 int
	// for i2 = 0; i2 < 5; i2++ {
	// 	var j = i2
	// 	myprint(v[j])				// 依次输出 1, 2, 3, 4, 5
	// }
}


func main() {
	test_assign()

	test_calculate()

	// test_control()

	test_func()

	test_array()
}
