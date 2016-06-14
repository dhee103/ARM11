#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BIT_0 0
#define BIT_1 1
#define BIT_2 2
#define BIT_3 3
#define CARRY_BIT 4
#define BLK_BUT 5
#define RED_BUT 6

#define PIN_0 0
#define PIN_1 2
#define PIN_2 3
#define PIN_3 7

#define MASK 1
#define NUM_BITS 4
#define MAX_NUM 15
#define WAIT 1000

int bitToPin(int bit);

// blinks leds to represent n 
void blink(uint32_t n) {

  // set normal LED pin's as outputs
  pinMode (PIN_0, OUTPUT) ;
  pinMode (PIN_1, OUTPUT) ;
  pinMode (PIN_2, OUTPUT) ;
  pinMode (PIN_3, OUTPUT) ;

  // clear normal LED pin's 
  digitalWrite (PIN_0, HIGH);
  digitalWrite (PIN_1, HIGH); 
  digitalWrite (PIN_2, HIGH);
  digitalWrite (PIN_3, HIGH);
  
  for (int i = 0; i < NUM_BITS; i++) {
	if ((n & (MASK << i)) != 0) {
	    digitalWrite (bitToPin(i), LOW);  
	}  
  }
  delay(WAIT);
}	

// returns pin number of a given bit (LED)
int bitToPin(int bit) {
  int pin;
    switch(bit) {
		case BIT_0: pin = PIN_0; break;
		case BIT_1: pin = PIN_1; break;
		case BIT_2: pin = PIN_2; break;
		case BIT_3: pin = PIN_3; break;
	}
  return pin;	
}

// returns the number represented by the LED's
int getNum() {
  int num = 0;
  while (digitalRead(BLK_BUT) != HIGH) {
	  if (digitalRead(RED_BUT) == HIGH) {
		  blink(num);
		  num++; 
    }
  }
  if (num == 0) {
	  return num;
  }
  return (num-1);
}

int main (void)
{
  // sets up GPIO pins & sets buttons as inputs and carry bit as output	
  wiringPiSetup () ;	
  pinMode(RED_BUT, INPUT);
  pinMode(BLK_BUT, INPUT);
  pinMode(CARRY_BIT, OUTPUT);
  int repeat = 0;
  // repeats calculator until user enters 0 on appropriate prompt
  do {
	 digitalWrite(CARRY_BIT, HIGH);
	  int first, second, res;
	  blink(0);
	  // allows user to choose operation. Repeats until valid one chosen
	  do {
		  printf("Choose operation:\n 1)Mul\n 2)Div\n 3)Add\n 4)Sub\n");
		  delay(WAIT);
		  res = getNum();
		  if (res < 1 || res > 4) {
			  printf("Invalid operation\n");
		  }
	  } while (res < 1 || res > 4);
	  // clears LED's
	  blink(0); 
	  // Prompts user to enter numbers using the counter
	  printf("Enter first number: \n");
	  first = getNum();
	  blink(0);
	  printf("Enter second number: \n");
	  second = getNum();
	  
	  // Performs the appropriate operation
	  switch (res) {
		  case 1: res = first * second; break;
		  case 2: 
		  if (second == 0) { res = -1; break; }	  
		  res = first / second; break;
		  case 3: res = first + second; break;
		  case 4: res = first - second; break;
	  }

	  // Checks for overflow and performs appropriate action
	  if (res > MAX_NUM) {
		  digitalWrite(CARRY_BIT, LOW);
		  blink(MAX_NUM);
		  
	  }
	  // Checks for underflow and performs appropriate action
	  else if(res < 0) {
		  digitalWrite(CARRY_BIT, LOW);
		  blink(0);	
	  }  
	  else {
		  blink(res);
	  }
	  delay(WAIT);
	  //do while
	  printf("Would you like another calculation?\n 0) No\n 1) Yes\n\n");
	  delay(WAIT);
	  repeat = getNum(); 
  }while(repeat);
  
  return 0 ;
}
