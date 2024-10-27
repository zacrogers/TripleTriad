package main

// #include <stdio.h>
// void hello() {
//    printf("Hello from C");
//}
// #include "tt.h"
import "C"
import "fmt"

func board_init() {

}

func board_state_json() {

}

func update_game() {

}

func main() {
	fmt.Println("Hello, Go!")
	C.hello()
	C.tt_board_init(0)
}
