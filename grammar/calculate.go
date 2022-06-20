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
	a++;
	if a < 20 {
		a = 2;
	} else {
		a = 3;
	}

    for i := 0; i <= 10; i++ {
        a = i;
    }
	a,b=add1(a,b)
	a,b=add1(1,2)

	var array [5]int

	for i := 0; i <= 4; i++ {
        array[i] = i;
        a= i;
    }
}
