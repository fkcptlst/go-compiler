package main


func myprint(a int)


func main() {
	var a int = 5
	if a < 10 {
		myprint(1)				// 预计输出 1
		if a < 5 {
			myprint(11)
		} else {
			myprint(12)			// 预计输出 12
		}
	} else {
		myprint(2)
	}
}
