package main


func main() {
	var a = 0;
	var b int;
	if a != 20 {
		a = 2;
		b = 4;
	} else {
		a = 3;
		if b != 4 {
			a = 1;
		} else {
			a = 4;
		}
	}
	myprint(a)
	myprint(b)
}
