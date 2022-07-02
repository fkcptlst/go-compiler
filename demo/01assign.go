package main


func myprint(a int)


func main() {
	var v1, v2, v3, v4, v5, v6, v7, v8 int
	v1 = 1
	v2 = v1 + 1
	v3 = v2 + 1
	v4 = v3 + 1
	v5 = v4 + 1
	v6 = v5 + 1
	v7 = v6 + 1
	v8 = v7 + 1

	myprint(v1)
	myprint(v2)
	myprint(v3)
	myprint(v4)
	myprint(v5)
	myprint(v6)
	myprint(v7)
	myprint(v8)

	v1 = 1
	v2 = v1 * 2
	v3 = v2 * 2
	v4 = v3 * 2
	v5 = v4 * 2
	v5 = v4 * 2
	v6 = v5 * 2
	v7 = v6 * 2
	v8 = v7 * 2

	myprint(v1)
	myprint(v2)
	myprint(v3)
	myprint(v4)
	myprint(v5)
	myprint(v6)
	myprint(v7)
	myprint(v8)
}
