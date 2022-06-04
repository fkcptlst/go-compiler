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
	// var
}
