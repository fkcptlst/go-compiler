package main


func myprint(a int)


func main() {
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
