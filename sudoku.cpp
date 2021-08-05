#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;


ifstream f("input.in"); // using file IO for processing the input, it's way easier when it comes to big inputs
ofstream g("output.out");
	
int mat[9][9]; // the sudoku grid

int correct[9][9]; // the correct solution, for the sake of testing from the dataset

int notes[9][9][10]; // list of notes

int line_borders_3x3[9][9];
int column_borders_3x3[9][9];

bool frq[1000002];

void add_notes()
{
	for(int line = 0; line <= 8; ++line)
		for(int column = 0; column <= 8; ++column)
			if(mat[line][column] == 0)
			{
				for(int digit = 1; digit <= 9; ++digit)
				{
					bool added = 1;
					for(int pos_line = 0; pos_line <= 8; ++pos_line)
						if(mat[pos_line][column] == digit)
							added = 0;
					for(int pos_column = 0; pos_column <= 8; ++pos_column)
						if(mat[line][pos_column] == digit)
							added = 0;
					for(int pos_line = line_borders_3x3[line][column]; pos_line <= line_borders_3x3[line][column] + 2; ++pos_line)
						for(int pos_column = column_borders_3x3[line][column]; pos_column <= column_borders_3x3[line][column] + 2; ++pos_column)
							if(mat[pos_line][pos_column] == digit)
								added = 0;
					notes[line][column][digit] = added;
				}
			}
}

int stuck = 0, solved_count = 0;

void remove_notes_rows(int line, int digit)
{
	for(int column = 0; column <= 8; ++column)
		notes[line][column][digit] = 0;
}

void remove_notes_columns(int column, int digit)
{
	for(int line = 0; line <= 8; ++line)
		notes[line][column][digit] = 0;
}

void remove_notes_3x3(int line, int column, int digit)
{
	for(int poz_line = line; poz_line <= line + 2; ++poz_line)
		for(int poz_column = column; poz_column <= column + 2; ++poz_column)
			notes[poz_line][poz_column][digit] = 0;
}

void remove_notes_spot(int line, int column)
{
	for(int digit = 1; digit <= 9; ++digit)
		notes[line][column][digit] = 0;
}

void activate_removals(int line, int column, int digit)
{
	++solved_count;
	stuck = 0;
	mat[line][column] = digit;
	remove_notes_spot(line, column);
	remove_notes_rows(line, digit);
	remove_notes_columns(column, digit);
	remove_notes_3x3(line_borders_3x3[line][column], column_borders_3x3[line][column], digit);
}

void unique_row()
{
	for(int line = 0; line <= 8; ++line)
		for(int digit = 1; digit <= 9; ++digit)
		{
			bool exists = 0;
			for(int column = 0; column <= 8; ++column)
				if(mat[line][column] == digit)
					exists = 1;
			if(exists == 0)
			{
				int note_column = -1;
				int count = 0;
				for(int column = 0; column <= 8; ++column)
					if(notes[line][column][digit] == 1)
					{
						++count;
						note_column = column;
					}
				if(count == 1)
					activate_removals(line, note_column, digit);
			}
		}
}

void unique_column()
{
	for(int column = 0; column <= 8; ++column)
		for(int digit = 1; digit <= 9; ++digit)
		{
			bool exists = 0;
			for(int line = 0; line <= 8; ++line)
				if(mat[line][column] == digit)
					exists = 1;
			if(exists == 0)
			{
				int note_line = -1;
				int count = 0;
				for(int line = 0; line <= 8; ++line)
					if(notes[line][column][digit] == 1)
					{
						++count;
						note_line = line;
					}
				if(count == 1)
					activate_removals(note_line, column, digit);
			}
		}
}

void unique_3x3()
{
	for(int start_line = 0; start_line <= 6; start_line += 3)
		for(int start_column = 0; start_column <= 6; start_column += 3)
			for(int digit = 1; digit <= 9; ++digit)
			{
				bool exists = 0;
				for(int line = start_line; line <= start_line + 2; ++line)
					for(int column = start_column; column <= start_column + 2; ++column)
						if(mat[line][column] == digit)
							exists = 1;
				if(exists == 0)
				{
					int note_line = -1;
					int note_column = -1;
					int count = 0;
					for(int line = start_line; line <= start_line + 2; ++line)
						for(int column = start_column; column <= start_column + 2; ++column)
							if(notes[line][column][digit] == 1)
							{
								++count;
								note_line = line;
								note_column = column;
							}
					if(count == 1)
						activate_removals(note_line, note_column, digit);
				}
			}
}

void one_note()
{
	for(int line = 0; line <= 8; ++line)
		for(int column = 0; column <= 8; ++column)
		{
			if(mat[line][column] == 0)
			{
				int cnt_notes = 0;
				int who = 0;
				for(int digit = 1; digit <= 9; ++digit)
					if(notes[line][column][digit] == 1)
					{
						cnt_notes++;
						who = digit;
					}
				if(cnt_notes == 1)
					activate_removals(line, column, who);
			}
		}
}

void reduction()
{
	for(int start_line = 0; start_line <= 6; start_line += 3)
		for(int start_column = 0; start_column <= 6; start_column += 3)
			for(int digit = 1; digit <= 9; ++digit)
			{
				bool exists = 0;
				for(int line = start_line; line <= start_line + 2; ++line)
					for(int column = start_column; column <= start_column + 2; ++column)
						if(mat[line][column] == digit)
							exists = 1;
				if(exists == 0)
				{
					int min_line = 10;
					int max_line = -1;
					int min_column = 10;
					int max_column = -1;
					for(int line = start_line; line <= start_line + 2; ++line)
						for(int column = start_column; column <= start_column + 2; ++column)
							if(notes[line][column][digit] == 1)
							{
								if(line < min_line)
									min_line = line;
								if(line > max_line)
									max_line = line;
								if(column < min_column)
									min_column = column;
								if(column > max_column)
									max_column = column;
							}
					if(min_line == max_line)
					{
						for(int column = 0; column <= 8; ++column)
							if(column < min_column || column > max_column)
							{
								if(notes[min_line][column][digit])
									stuck = 0;
								notes[min_line][column][digit] = 0;
							}
					}
					if(min_column == max_column)
					{	
						for(int line = 0; line <= 8; ++line)
							if(line < min_line || line > max_line)
							{
								if(notes[line][min_column][digit])
									stuck = 0;
								notes[line][min_column][digit] = 0;
							}
					}
				}
			}
}

void subset_reduction_3x3()
{
	for(int start_line = 0; start_line <= 6; start_line += 3)
		for(int start_column = 0; start_column <= 6; start_column += 3)
		{
			for(int msk = 1; msk < (1<<9); ++msk)
			{
				bool frq[10] = {0};
				int cnt_columns = 0;
				int cnt = 0;
				for(int line = start_line; line <= start_line + 2; ++line)
					for(int column = start_column; column <= start_column + 2; ++column)
						{
							if(msk & (1<<cnt))
							{
								if(mat[line][column] != 0)
									cnt_columns = 100;
								++cnt_columns;
								for(int digit = 1; digit <= 9; ++digit)
									if(notes[line][column][digit] == 1)
										frq[digit] = 1;
							}
							++cnt;
						}
				int cnt_digits = 0;
				for(int digit = 1; digit <= 9; ++digit)
					if(frq[digit] == 1)
						++cnt_digits;
				if(cnt_digits == cnt_columns)
				{
					cnt = 0;
					for(int line = start_line; line <= start_line + 2; ++line)
						for(int column = start_column; column <= start_column + 2; ++column)
							{
								if(!(msk & (1<<cnt)))
								{
									for(int digit = 1; digit <= 9; ++digit)
										if(frq[digit])
										{
											if(notes[line][column][digit] == 1)
											{
												notes[line][column][digit] = 0;
												stuck = 0;
											}
										}
								}
								++cnt;
							}
				}
			}
		}
}

void subset_reduction_1x9()
{
	for(int line = 0; line <= 8; ++line)
	{
		for(int msk = 1; msk < (1<<9); ++msk)
		{
			bool frq[10] = {0};
			int cnt_columns = 0;
			for(int column = 0; column <= 8; ++column)
			{
				if(msk & (1<<column))
				{
					if(mat[line][column] != 0)
						cnt_columns = 100;
					++cnt_columns;
					for(int digit = 1; digit <= 9; ++digit)
						if(notes[line][column][digit] == 1)
							frq[digit] = 1;
				}
			}
			int cnt_digits = 0;
			for(int digit = 1; digit <= 9; ++digit)
				if(frq[digit] == 1)
					++cnt_digits;
			if(cnt_digits == cnt_columns)
			{
				for(int column = 0; column <= 8; ++column)
				{
					if(!(msk & (1<<column)))
					{
						for(int digit = 1; digit <= 9; ++digit)
							if(frq[digit])
							{
								if(notes[line][column][digit] == 1)
								{
									notes[line][column][digit] = 0;
									stuck = 0;
								}
							}
					}
				}
			}
		}
	}
}

void subset_reduction_9x1()
{
	for(int column = 0; column <= 8; ++column)
	{
		for(int msk = 1; msk < (1<<9); ++msk)
		{
			bool frq[10] = {0};
			int cnt_lines = 0;
			for(int line = 0; line <= 8; ++line)
			{
				if(msk & (1<<line))
				{
					if(mat[line][column] != 0)
						cnt_lines = 100;
					++cnt_lines;
					for(int digit = 1; digit <= 9; ++digit)
						if(notes[line][column][digit] == 1)
							frq[digit] = 1;
				}
			}
			int cnt_digits = 0;
			for(int digit = 1; digit <= 9; ++digit)
				if(frq[digit] == 1)
					++cnt_digits;
			if(cnt_digits && cnt_digits == cnt_lines)
			{
				for(int line = 0; line <= 8; ++line)
				{
					if(!(msk & (1<<line)))
					{
						for(int digit = 1; digit <= 9; ++digit)
							if(frq[digit])
							{
								if(notes[line][column][digit] == 1)
								{
									notes[line][column][digit] = 0;
									stuck = 0;
								}
							}
					}
				}
			}
		}
	}
}

void check_notes()
{
	for(int line = 0; line <= 8; ++line)
		for(int column = 0; column <= 8; ++column)
		{
			g << line << " " << column << " ";
			for(int digit = 1; digit <= 9; ++digit)
				if(notes[line][column][digit] == 1)
					g << digit;
			g << '\n';
		}
}

void print_solution()
{
	g << "Done\n";
	for(int line = 0; line <= 8; ++line)
	{
		for(int column = 0; column <= 8; ++column)
			g << mat[line][column] << " ";
		g << '\n';
	}
}
bool org[9][9];

bool found;
void bkt(int L, int C)
{
	if(found)
		return;
	if(L == 9)
	{
		found = 1;
		// print_solution();
		return;
	}
	if(mat[L][C] != 0)
	{
		if(C == 8)
			bkt(L+1, 0);
		else
			bkt(L, C+1);
	}
	else
	{
		for(int digit = 1; digit <= 9 && !found; ++digit)
			if(notes[L][C][digit] == 1)
			{
				bool added = 1;
				for(int pos_line = 0; pos_line <= 8; ++pos_line)
					if(mat[pos_line][C] == digit)
						added = 0;
				for(int pos_column = 0; pos_column <= 8; ++pos_column)
					if(mat[L][pos_column] == digit)
						added = 0;
				for(int pos_line = line_borders_3x3[L][C]; pos_line <= line_borders_3x3[L][C] + 2; ++pos_line)
					for(int pos_column = column_borders_3x3[L][C]; pos_column <= column_borders_3x3[L][C] + 2; ++pos_column)
						if(mat[pos_line][pos_column] == digit)
							added = 0;
				if(added)
				{
					mat[L][C] = digit;
					if(C == 8)
						bkt(L+1, 0);
					else
						bkt(L, C+1);
					if(found)
						return;
					mat[L][C] = 0;
				}
			}
	}
}

void sudoku_solver()
{
	for(int line = 0; line <= 8; ++line)
		for(int column = 0; column <= 8; ++column)
		{
			line_borders_3x3[line][column] = (line)/3 * 3;
			column_borders_3x3[line][column] = (column/3) * 3;
			if(mat[line][column] > 0)
				++solved_count;
		}
	add_notes();
	
	while(solved_count < 81 && stuck == 0)
	{
		stuck = 1;
		unique_row();
		unique_column();
		unique_3x3();
		one_note();
		reduction();
		subset_reduction_3x3();
		subset_reduction_1x9();
		subset_reduction_9x1();	
	}
	
	/*
	g << "Logical Solver " << solved_count << '\n';
	g << '\n';
	
	print_solution();
	
	g << '\n';
	*/
	
	if(solved_count < 81)
	{
		for(int line = 0; line <= 8; ++line)
			for(int column = 0; column <= 8; ++column)
				if(mat[line][column] != 0)
					org[line][column] = 1;
		bkt(0, 0);
	}
}

int main()
{
	frq[100] = frq[500] = frq[1000] = frq[2000] = frq[4000] = frq[5000] = frq[8000] = 1;
	frq[10000] = frq[20000] = frq[40000] = frq[50000] = 1;
	frq[100000] = frq[200000] = frq[400000] = frq[500000] = frq[800000] = frq[1000000] = 1;

	
	double time_beginning = clock();
	int t;
	f >> t;
	for(int grid = 1; grid <= t; ++grid)
	{
		memset(mat, 0, sizeof(mat));
		memset(notes, 0, sizeof(notes));
		memset(org, 0, sizeof(org));
		stuck = 0;
		solved_count = 0;
		found = 0;
		string data;
		f >> data;
		int line = 0, column = 0;
		for(int pos = 0; pos <= 80; ++pos)
		{
			if(data[pos] != '.')
				mat[line][column] = data[pos] - '0';
			++column;
			if(column == 9)
				++line, column = 0;
		}
		sudoku_solver();
		bool incorrect = 0;
		for(int line = 0; line <= 8; ++line)	
			for(int column = 0; column <= 8; ++column)
				if(mat[line][column] == 0)
					incorrect = 1;
		if(incorrect)
		{
			g << grid << '\n';
			print_solution();
		}
		if(frq[grid] == 1) // intermediary points used for testing the speed of the algorithm 
		{
			double time_ending = clock();
			double times = time_ending - time_beginning;
			times /= CLOCKS_PER_SEC;
			g << "set 1: " << times << "s for " << grid << " grids " << '\n';
		}
	}
	
	
	return 0;
}
