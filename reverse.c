 
/*
Aki Laaksonen
aki.laaksonen@student.lut.fi
Course CT30A3370
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* Function to allocate memory and add string to a list */
int add_to_list(char*** list, int* number_of_lines, char* new_line)
{
  if ((*number_of_lines) == 0 || (*list) == NULL)
    {
      (*number_of_lines) = 1;
      (*list) = (char**)malloc(sizeof(char*));
	  if ((*list) == NULL)
		{
		  fprintf(stderr, "malloc failed");
		  exit(1);
		}
    }
  else
    {
      (*number_of_lines)++;
      (*list) = (char**)realloc((*list), (*number_of_lines)*sizeof(char*));
	  if ((*list) == NULL)
		{
		  fprintf(stderr, "realloc failed");
		  exit(1);
		}
    }
  (*list)[(*number_of_lines)-1] = malloc(sizeof(char*));
  if ((*list)[(*number_of_lines)-1] == NULL)
		{
		  fprintf(stderr, "malloc failed");
		  exit(1);
		}
  
  strcpy((*list)[(*number_of_lines)-1], new_line);
  return 0;
}

/* Function to free memory allocated for a string in a list */
int free_list(char** list, int* number_of_lines)
{
  while ((*number_of_lines) > 0)
    {
      if (list[(*number_of_lines)-1] != NULL)
	{
	  free(list[(*number_of_lines)-1]);
	}
      (*number_of_lines)--;
    }
  return 0;
}

/* Function used to first remove a newline from the end of the string.
   Additionally the function splits the string at spaces and adds the substrings to a list. */
int strmod(char* input, char*** list, int* number_of_lines)
{
  char* input_string = (char*)malloc(strlen(input)-1 * sizeof(char));
  if (input_string == NULL)
		{
		  fprintf(stderr, "malloc failed");
		  exit(1);
		}
  char* new_line;
  strncpy(input_string, input, strlen(input)-1);
  new_line = strtok(input_string, " ");
  add_to_list(list, number_of_lines, new_line);
  while (new_line != NULL)
	{
	  new_line = strtok(NULL, " ");
	  if (new_line != NULL)
		add_to_list(list, number_of_lines, new_line);
	}
  return 0;
}

/* Function copies the input list into a new list that has the order reversed. */
int reverse_list(char*** list, int* number_of_lines, char*** reversed_list)
{
  int reversed_lines = 0;

  char** new_list = (char**)malloc(1 * sizeof(char*));
  if (new_list == NULL)
	{
	  fprintf(stderr, "malloc failed");
	  exit(1);
	}
  for (int current_line = *number_of_lines-1; current_line >= 0; current_line--)
	{
	  char* selected_line = (char*)malloc(strlen((*list)[current_line]) * sizeof(char));
	  if (selected_line == NULL)
		{
		  fprintf(stderr, "malloc failed");
		  exit(1);
		}
	  
	  strcpy(selected_line, (*list)[current_line]);
	 
	  add_to_list(&new_list, &reversed_lines, selected_line);
   
	}

  (*reversed_list) = new_list;
  
  return 0;
}

int main(int argc, char** argv)
{
  char** lines = (char**)malloc(1 * sizeof(char*));
  if (lines == NULL)
	{
	  fprintf(stderr, "malloc failed");
	  exit(1);
	}
  
  char** reversed_lines = (char**)malloc(1 * sizeof(char*));
  if (reversed_lines == NULL)
	{
	  fprintf(stderr, "malloc failed");
	  exit(1);
	}
  
  int number_of_lines = 0;
  char* line = NULL;
  size_t line_length = 0;

  /* Check if program has more than two arguments. */
  if(argc > 3)
	{
	  fprintf(stderr, "usage: reverse <input> <output>\n");
	  exit(1);
	}

  /* Check if program has more than two argument. */
  if(argc > 2)
	{
	  /* Checks if first and seconds argument are same and returns an error if they are. */
	  if (strcmp(argv[1],argv[2]) == 0)
		{
		  fprintf(stderr, "Input and output file must differ\n");
		  exit(1);
		}
	}

  /* Checks if program has one or more arguments. */
  if(argc > 1)
	{
	  char* input_filename = (char*)malloc(strlen(argv[1]) * sizeof(char));
	  if (input_filename == NULL)
		{
		  fprintf(stderr, "malloc failed");
		  exit(1);
		}

	  /* Copies the first argument into input_filename. */
	  strcpy(input_filename, argv[1]);

	  FILE* input_file = fopen(input_filename, "r");
	  if (input_file == NULL)
		{
		  fprintf(stderr, "error: cannot open file '%s'\n", input_filename);
		  exit(1);
		}
	  
	  free(input_filename);
	  input_filename = NULL;

	  /* Goes through input_file line by line and adds the lines into a list called lines. */
	  while( getline(&line, &line_length, input_file) > 0 )
		{
		  add_to_list(&lines, &number_of_lines, line);
		}

	  /* Reverses the order of lines in lines variable and copies them into reversed_lines. */
	  reverse_list(&lines, &number_of_lines, &reversed_lines);
	  
	  fclose(input_file);
	  input_file = NULL;

	  /* Checks if the program has two or more arguments. */
	  if (argc > 2)
		{
		  char* output_filename = (char*)malloc(strlen(argv[2]) * sizeof(char));
		  if (output_filename == NULL)
			{
			  fprintf(stderr, "malloc failed");
			  exit(1);
			}

		  /* Copies the second argument into output_filename. */
		  strcpy(output_filename, argv[2]);
		  
		  FILE* output_file = fopen(output_filename, "w");
		  if (output_file == NULL)
			{
			  fprintf(stderr, "error: cannot open file '%s'\n", output_filename);
			  exit(1);
			}
		  
		  free(output_filename);
		  output_filename = NULL;

		  /* Writes the lines in reversed_list into the output_file line by line. */
		  for (int current_line = 0; current_line < number_of_lines; current_line++)
			{
			  fprintf(output_file, "%s", reversed_lines[current_line]); 
			}

		  fclose(output_file);
		}
	  else
		{
		  /* Prints the lines in reversed_list into the command line. */
		  for (int current_line = 0; current_line < number_of_lines; current_line++)
			{
			  fprintf(stdout, "%s", reversed_lines[current_line]);
			}
		}
	}
  else
	{
	  /* Reads a line from the command line. */
	  getline(&line, &line_length, stdin);

	  /* Goes through the inputted line and splits it into lines at spaces. */
	  strmod(line, &lines, &number_of_lines);

	  /* Reverses the order of lines in lines variable and copies them into reversed_lines. */
	  reverse_list(&lines, &number_of_lines, &reversed_lines);

	  /* Prints the reversed_list into command line line by line. */
	  for (int current_line = 0; current_line < number_of_lines; current_line++)
		{
		  fprintf(stdout, "%s\n", reversed_lines[current_line]);
		}
	}

  int number_of_reversed_lines = number_of_lines;
  
  free_list(lines, &number_of_lines);
  free(lines);
  lines = NULL;
  free_list(reversed_lines, &number_of_reversed_lines);
  free(reversed_lines);
  reversed_lines = NULL;
  return 0;
}
