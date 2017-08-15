#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define N 9

void main()
{
	int i, j, value;
	int sudoku[N][N];
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			sudoku[i][j] = 0;
	printf("Please Enter The Initial Numbers: \n");
	scanf("%d %d %d", &i, &j, &value);
	while (1)
	{
	if (i == -1)
	break;
	sudoku[i][j] = value;
	scanf("%d %d %d", &i, &j, &value);
	}

	printf("\n\n");

	if(fork() == 0)//child 1.
	{
		double diff;
		struct timeval t1,t2;
		printf("Im Son1, this is my pid: %d \n\n", getpid());
		gettimeofday(&t1,NULL);
		if(sudokuSon1(sudoku)!=0)
		{	
		gettimeofday(&t2,NULL);
		diff=(t2.tv_sec -t1.tv_sec)*1000.0;
		diff=diff+(t2.tv_usec -t1.tv_usec)/1000.0;
		printf("\nMy Time to solve the sudoku: %lf millisecond\n", diff);
		}
		else
		{	
		gettimeofday(&t2,NULL);
		diff=(t2.tv_sec -t1.tv_sec)*1000.0;
		diff=diff+(t2.tv_usec -t1.tv_usec)/1000.0;
		printf("\nMy Time to know that there is No sulotion: %lf millisecond\n", diff);
		}
		printf("\n\n");
		exit(1);
	}
	wait();

	if(fork() == 0)//child 2.
	{
		struct timeval t1,t2;
		double diff;
		printf("Im Son 2, this is my pid: %d\n\n", getpid());
		gettimeofday(&t1,NULL);
		if(sudokuSon2(sudoku)!=0)
		{
		gettimeofday(&t2,NULL);
		diff=(t2.tv_sec -t1.tv_sec)*1000.0;
		diff=diff+(t2.tv_usec -t1.tv_usec)/1000.0;
		printf("\nMy Time to solve the sudoku: %lf millisecond\n", diff);
		}
		else
		{
		gettimeofday(&t2,NULL);
		diff=(t2.tv_sec -t1.tv_sec)*1000.0;
		diff=diff+(t2.tv_usec -t1.tv_usec)/1000.0;
		printf("\nMy Time to know that there is No sulotion: %lf millisecond\n", diff);
		}
		exit(1);
	}
	printf("\n");
	wait();
}

int sudokuSon1(int sudoku[][N])//first sudoko algoritim.
{
	int k,i,j;
	for (k = 0; k < N; k++)
		if(Sud_Solve1(sudoku, k, 0)==0)
		{
			printf("There is no sulotion to the sudoku\n");
			return 0;
		}
		
	printf("This is my sulotion:\n\n");
	for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
				printf("%d  ", sudoku[i][j]);
			printf("\n");
		}
}

int sudokuSon2(int sudoku[][N])//second sudoko algoritim.
{
	int i, j;
	int temp[N][N];
	int temp2[N][N];

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			temp[i][j] = 0;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			if (sudoku[i][j] == 0)
				temp[i][j] = count(sudoku, i, j);//temp is a prriority table.
		
		
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			temp2[i][j] = temp[i][j];//temp2=temp

	if(Sud_Solve2(sudoku, temp,temp2)==0)
		{
		printf("There is no sulotion to the sudoku\n");
		return 0;
		}
	
	printf("This is my sulotion:\n\n");
	for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
				printf("%d  ", sudoku[i][j]);
				printf("\n");
		}
}

int Sud_Solve1(int puzzle[][9], int row, int col)// recorsive algoritim that solve sudoko row by row.
{
	int i;
	if (row < 9 && col < 9)
	{
		if (puzzle[row][col] != 0)
		{
			if ((col + 1) < 9) return Sud_Solve1(puzzle, row, col + 1);
			else if ((row + 1) < 9) return Sud_Solve1(puzzle, row + 1, 0);
			else return 1;
		}
		else
		{
			for (i = 0; i < 9; ++i)
			{
				if (isAvailable(puzzle, row, col, i + 1))
				{
					puzzle[row][col] = i + 1;
					if ((col + 1) < 9)
					{
						if (Sud_Solve1(puzzle, row, col + 1)) return 1;
						else puzzle[row][col] = 0;
					}
					else if ((row + 1) < 9)
					{
						if (Sud_Solve1(puzzle, row + 1, 0)) return 1;
						else puzzle[row][col] = 0;
					}
					else return 1;
				}
			}
		}
		return 0;
	}
	else return 1;
}

int Sud_Solve2(int puzzle[][N], int temp[][N],int temp2[][N])// recorsive algoritim that solve sudoko with a priiority table.
{
	int i, j, cnt = 0, row = 0, col = 0,count=0;
	
	
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		if (temp[i][j]>cnt)//looking for the biggest number in the prriority table.
		{
			cnt = temp[i][j];
			row = i;
			col = j;
		}
	}
	if (cnt == 0)
		return Sud_Solve1(puzzle, row, col);//only if prriority table is empty, keep solving like child 1(row by row).

	for (i = 0; i < 9; ++i)
	{
		if (isAvailable(puzzle, row, col, i + 1))
		{
			puzzle[row][col] = i + 1;
			temp[row][col] = 0;

			if (Sud_Solve2(puzzle, temp,temp2)) return 1;
			else
			{
				puzzle[row][col] = 0;
				temp[row][col] = temp2[row][col];
			}
		}
	}
	return 0;
}


int isAvailable(int puzzle[][9], int row, int col, int num)// the function return if a number is avilable.
{
	int rowStart = (row / 3) * 3;
	int colStart = (col / 3) * 3;
	int i, j;

	for (i = 0; i < 9; ++i)
	{
		if (puzzle[row][i] == num) return 0;
		if (puzzle[i][col] == num) return 0;
		if (puzzle[rowStart + (i % 3)][colStart + (i / 3)] == num) return 0;
	}
	return 1;
}


int count(int puzzle[][N], int row, int col)//function that count to the priiority table.
{
	int i, j, count = 0;
	for (i = row, j = 0; j < N; j++)
	{
		if (puzzle[i][j] != 0)
			count++;
	}
	for (i = col, j = 0; j < N; j++)
	{
		if (puzzle[j][i] != 0)
			count++;
	}

	int rowStart = (row / 3) * 3;
	int colStart = (col / 3) * 3;
	for (i = rowStart; i < rowStart + 3; i++)
	{
		for (j = colStart; j < colStart + 3; j++)
		if (puzzle[i][j] != 0)
			count++;
	}
	return count;
}
