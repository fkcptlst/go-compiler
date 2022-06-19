package main

func myprint(x int);

func fun1(x int) {
	myprint(x);
}

func fun2()(int) {
	var ret int = 9999;
	return ret;
}

func main() {
	myprint(5678);
	fun1(1234)
	var a int = fun2();
	myprint(a);
	// var
}
