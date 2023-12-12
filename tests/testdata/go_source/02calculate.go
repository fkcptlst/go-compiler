package main


func myprint(a int)


func main() {
	var a int
	a = 4 - (2 + 4 - 4 * 2) + 3
	a++
	myprint(a)					// 预计输出 10

	a = 2 * 1895 / 10 * 5 - 1111 * 8 / 9
	myprint(a)					// 预计输出 908

	a = 1 + 2 * 99 / 20
	myprint(a)					// 预计输出10
}
