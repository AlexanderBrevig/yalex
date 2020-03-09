/*
 * This is a sample that builds for ATmega328P
 * The serial can now be used to interact with your Arduino using yalex.s
 * 
 * You will need to copy files from https://github.com/AlexanderBrevig/yalex/tree/master/yalex/src to this directory, excluding demo.c
 * Then, paste the #defines below to the top of yalex_config.h or find a way to -Ddefine them.
 * 
 * STATS:
 *  Sketch uses 10188 bytes (33%) of program storage space. 
 *  Global variables use 1454 bytes of dynamic memory.
 */
 
#define YALEX_NUMERIC_TYPE int
#define YALEX_ADDRESS_TYPE unsigned char
#define YALEX_SIZE_STACK 10
#define YALEX_SIZE_TOKEN_STR 11
#define YALEX_SIZE_USER_TOKENS 5
#define YALEX_SIZE_LAMBDA_STACK_STR 16
#define YALEX_SIZE_LAMBDAS_STACK 10
#define YALEX_SIZE_REGISTERS 8

extern "C" { 
  #include "yalex.h"
  #include "yalex_system.h"
}

yalex_world world;
const char BUFFER_SIZE = 32;
char buffer[BUFFER_SIZE];

void replMessageCallback(const char* ptr) {
  Serial.println(ptr);
}

void tok_input_exec(yalex_world *world, stack_item **out) { pinMode(out[0]->data.number, INPUT); }
void tok_output_exec(yalex_world *world, stack_item **out) { pinMode(out[0]->data.number, OUTPUT); }
void tok_high_exec(yalex_world *world, stack_item **out) { digitalWrite(out[0]->data.number, HIGH); }
void tok_low_exec(yalex_world *world, stack_item **out) { digitalWrite(out[0]->data.number, LOW); }
  
void setup() {
  Serial.begin(115200);
  yalex_init(&world, replMessageCallback);
  char req_num[1] = { YALEX_TOKEN_NUM };
  yalex_system_token_register("input", req_num, 1, tok_input_exec);
  yalex_system_token_register("output", req_num, 1, tok_output_exec);
  yalex_system_token_register("high", req_num, 1, tok_high_exec);
  yalex_system_token_register("low", req_num, 1, tok_low_exec);
}

void loop() {
  if (Serial.available()) {
    memset(buffer, 0, BUFFER_SIZE);
    Serial.readBytesUntil('\0', buffer, BUFFER_SIZE);
    yalex_repl(&world, buffer);
  }
}
