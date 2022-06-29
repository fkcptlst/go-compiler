package main


func myprint(a int)


func main() {
	var v[3] int
	var i int
	v[0] = 0
	v[1] = 1
	v[2] = 2
	for i = 0; i < 3; i++ {
		var j = i
		myprint(v[j])
	}
}
