package main


func myprint(a int)


func param(a int) {
	a = 5
}


func ret_1() (int, int) {
	var a = 1
	var b = 5
	return a, b
}


func main() {
	var a = 1
	param(a)
	myprint(a)
	var b, c = ret_1()
	myprint(b)
	myprint(c)
}
