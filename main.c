#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSTR_SIZE 64
#define STACK_SIZE 32

int hash(const char *str) {
  if (!str)
    return 0;

  // FNV-1a constants
  int hash = 2166136261u;      // FNV offset basis
  const int prime = 16777619u; // FNV prime

  // Process string
  while (*str) {
    hash ^= (int)*str++;
    hash *= prime;
  }

  // Additional mixing (avalanche) to improve distribution
  // This helps especially with short strings
  hash ^= hash >> 16;
  hash *= 0x85ebca6b;
  hash ^= hash >> 13;
  hash *= 0xc2b2ae35;
  hash ^= hash >> 16;

  return hash;
}

struct Instr {
  int cmd;
  int argStr;
  char arg[56];
};

int parseInstr(char instr[INSTR_SIZE], struct Instr *parseDest) {
  instr[strlen(instr) - 1] = 0;
  int i = 0;
  int iCmd = 0;
  int iArg = -1;
  char cmd[8] = {};
  char arg[56] = {};
  int escape = 0;
  int argStr = 0;
  while (i < 64) {
    char cchar = instr[i];

    if (!cchar) {
      break;
    }

    // comment
    if (cchar == ';') {
      if (!cmd[0]) {
        parseDest->argStr = 2;
      }
      break;
    }

    // space
    if (cchar == ' ') {
      if (escape) {
        arg[iArg] = cchar;
        iArg++;
      } else {
        // leading space
        if (iCmd == 0) {
          printf("Syntax error: invalid command leading space\n");
          return 1;
        }
        // handle cmd-arg seperation space
        else {
          iArg = 0;
        }
      }
    }
    // string
    else if (cchar == '"') {
      if (escape) {
        escape = 0;
      } else {
        argStr = 1;
        escape = 1;
      }
    }
    // char
    else {
      // cmd
      if (iArg == -1) {
        cmd[iCmd] = cchar;
        iCmd++;
      }
      // arg
      else {
        arg[iArg] = cchar;
        iArg++;
      }
    }
    i++;
  }

  parseDest->cmd = hash(cmd);
  strcpy(parseDest->arg, arg);
  parseDest->argStr = argStr;

  return 0;
}

int main(int argc, char **argv) {

  // read program file
  char *ssPath = argv[1];

  if (!ssPath) {
    printf("Error: No file path specified\n");
    return 1;
  }

  FILE *fptr;

  fptr = fopen(ssPath, "r");

  if (fptr == NULL) {
    printf("Error: file not found\n");
    return 1;
  }

  char fcontent[64];

  // generate program instructions line-by-line
  int instrCount = 16;
  int instrNum = 0;
  struct Instr instructions[instrCount] = {};

  int defList[16] = {};
  int defSlot = 0;

  while (fgets(fcontent, 64, fptr)) {
    struct Instr newInstr = {};
    int error = parseInstr(fcontent, &newInstr);
    if (error) {
      // handle error
      return 1;
    }
    // save new instruction when not comment
    if (newInstr.argStr != 2) {

      // extend instr buffer
      if(instrNum == instrCount) {
        printf("warn: buffer overflow\n");
      }

      instructions[instrNum] = newInstr;
      instrNum++;
      // save define sections for further lookup
      if (newInstr.cmd == 30079725) {
        int defIdent = strtol(newInstr.arg, NULL, 10);

        if (defIdent < 0 || defIdent > 15) {
          printf("Range Error: section definition out of range 0-15\n");
          return 1;
        }

        defList[defIdent] = instrNum;
      }
    }
  }

  // execute program
  float stack[STACK_SIZE] = {};
  int newSlot = 0;

  int exit = 0;

  for (int i = 0; i < instrCount; i++) {
    if (exit)
      break;

    struct Instr *cInstr = &instructions[i];
    switch (cInstr->cmd) {
    case 1787214203: // comment
      break;

    case 559112230: // push
      stack[newSlot] = strtof(cInstr->arg, NULL);
      newSlot++;
      break;

    case 1587477686: // pop
      newSlot--;
      break;

    case 519818912: // swap
      float temp = stack[newSlot - 2];
      stack[newSlot - 2] = stack[newSlot - 1];
      stack[newSlot - 1] = temp;
      break;

    case 1498617564: // dup
      stack[newSlot] = stack[newSlot - 1];
      newSlot++;
      break;

    case 219464352: // add
      stack[newSlot - 2] = stack[newSlot - 2] + stack[newSlot - 1];
      newSlot--;
      break;

    case 193096630: // subtract
      stack[newSlot - 2] = stack[newSlot - 1] - stack[newSlot - 2];
      newSlot--;
      break;

    case 1994384907: // multiply
      stack[newSlot - 2] = stack[newSlot - 1] * stack[newSlot - 2];
      newSlot--;
      break;

    case 674428405: // divide
      stack[newSlot - 2] = stack[newSlot - 1] / stack[newSlot - 2];
      newSlot--;
      break;

    case 2029104015: // mod
      stack[newSlot - 2] = (int)stack[newSlot - 1] % (int)stack[newSlot - 2];
      newSlot--;
      break;

    case 11587803: // print
      if (cInstr->argStr) {
        printf("%s\n", cInstr->arg);
      } else {
        printf("%f\n", stack[newSlot - 1]);
      }
      break;

    case 1377218127: // read
      float input;
      int result;

      while (1) {
        if (cInstr->argStr) {
          printf("%s", cInstr->arg);
        }

        result = scanf("%f", &input);

        if (result > 0) {
          break;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF)
          ;

        if (c == EOF) {
          printf("\nError: End of input reached\n");
          return 1;
        }
      }

      stack[newSlot] = input;
      newSlot++;
      break;

    case 1519729184: // if
      int boolValue = 0;
      if (strcmp(cInstr->arg, "same") == 0) {
        boolValue = stack[newSlot - 1] == stack[newSlot - 2];
      } else if (strcmp(cInstr->arg, "less") == 0) {
        boolValue = stack[newSlot - 1] < stack[newSlot - 2];
      } else if (strcmp(cInstr->arg, "more") == 0) {
        boolValue = stack[newSlot - 1] > stack[newSlot - 2];
      } else {
        printf("Type Error: Unknown boolean operand \"%s\"\n", cInstr->arg);
        return 1;
      }
      if (!boolValue) {
        i++;
      }
      break;

    case 30079725: // def
      break;

    case 371664971: // goto
      int gotoIdent = strtol(cInstr->arg, NULL, 10);

      if (gotoIdent < 0 || gotoIdent > 15) {
        printf("Range Error: section definition call out of range 0-15\n");
        return 1;
      }
      i = defList[gotoIdent] - 1;
      break;

    case 2049911839: // debug
      puts("-- Stack --");
      for (int i = newSlot - 1; i >= 0; i--) {
        printf("[%d] %f\n", i, stack[i]);
      }
      putchar('\n');
      break;

    case 0:
      exit = 1;
      break;

    default:
      printf("Type Error: Unknown command %d @ instr. %d\n", cInstr->cmd, i);
      return 1;
    }
  }

  fclose(fptr);
  return 0;
}
