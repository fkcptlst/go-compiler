package main


func myprint(a int)


func add_1(a int, b int) (int, int) {
	return a + 1, b + 1
}


func main() {
	var a = 100;
	var b = 105;
	var c, d = add_1(a, b)
	myprint(c)				// 预计输出 101
	myprint(d)				// 预计输出 106
}
