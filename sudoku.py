mat = [[0 for x in range(9)] for y in range(9)]

correct = [[0 for x in range(9)] for y in range(9)]

notes = [[[0 for z in range(10)] for x in range(9)] for y in range(9)]

line_borders_3x3 = [[0 for x in range(9)] for y in range(9)]

column_borders_3x3 = [[0 for x in range(9)] for y in range(9)]


def add_notes():
	for line in range(0, 9):
		for column in range(0, 9):
			if (mat[line][column] == 0):
				for digit in range(1, 10):
					added = 1;
					for pos_line in range(0, 9):
						if (mat[pos_line][column] == digit):
							added = 0;
					for pos_column in range(0, 9):
						if (mat[line][pos_column] == digit):
							added = 0;
					for pos_line in range(line_borders_3x3[line][column], line_borders_3x3[line][column] + 3):
						for pos_column in range(column_borders_3x3[line][column], column_borders_3x3[line][column] + 3):
							if (mat[pos_line][pos_column] == digit):
								added = 0;
					notes[line][column][digit] = added


stuck = 0

solved_count = 0


def remove_notes_rows(line, digit):
	for column in range(0, 9):
		notes[line][column][digit] = 0


def remove_notes_columns(column, digit):
	for line in range(0, 9):
		notes[line][column][digit] = 0


def remove_notes_3x3(line, column, digit):
	for poz_line in range(line, line + 3):
		for poz_column in range(column, column + 3):
			notes[poz_line][poz_column][digit] = 0


def remove_notes_spot(line, column):
	for digit in range(1, 10):
		notes[line][column][digit] = 0


def unique_row():
	global solved_count
	global stuck
	for line in range(0, 9):
		for digit in range(1, 10):
			exists = 0
			for column in range(0, 9):
				if (mat[line][column] == digit):
					exists = 1
			if (exists == 0):
				note_column = -1
				count = 0
				for column in range(0, 9):
					if (notes[line][column][digit] == 1):
						count = count + 1
						note_column = column
				if (count == 1):
					solved_count = solved_count + 1
					stuck = 0
					mat[line][note_column] = digit
					remove_notes_spot(line, note_column)
					remove_notes_rows(line, digit)
					remove_notes_columns(note_column, digit)
					remove_notes_3x3(line_borders_3x3[line][note_column], column_borders_3x3[line][note_column], digit)


def unique_column():
	global solved_count
	global stuck
	for column in range(0, 9):
		for digit in range(1, 10):
			exists = 0
			for line in range(0, 9):
				if (mat[line][column] == digit):
					exists = 1
			if (exists == 0):
				note_line = -1
				count = 0
				for line in range(0, 9):
					if (notes[line][column][digit] == 1):
						++count;
						note_line = line;
				if (count == 1):
					solved_count = solved_count + 1
					stuck = 0
					mat[note_line][column] = digit
					remove_notes_spot(note_line, column)
					remove_notes_rows(note_line, digit)
					remove_notes_columns(column, digit)
					remove_notes_3x3(line_borders_3x3[note_line][column], column_borders_3x3[note_line][column], digit)


def unique_3x3():
	global solved_count
	global stuck
	for start_line in range(0, 9, 3):
		for start_column in range(0, 9, 3):
			for digit in range(1, 10):
				exists = 0
				for line in range(start_line, start_line + 3):
					for column in range(start_column, start_column + 3):
						if (mat[line][column] == digit):
							exists = 1

				if (exists == 0):
					note_line = -1
					note_column = -1
					count = 0
					for line in range(start_line, start_line + 3):
						for column in range(start_column, start_column + 3):
							if (notes[line][column][digit] == 1):
								count = count + 1
								note_line = line
								note_column = column

					if (count == 1):
						solved_count = solved_count + 1
						++solved_count
						stuck = 0
						mat[note_line][note_column] = digit
						remove_notes_spot(note_line, note_column)
						remove_notes_rows(note_line, digit)
						remove_notes_columns(note_column, digit)
						remove_notes_3x3(line_borders_3x3[note_line][note_column],
										 column_borders_3x3[note_line][note_column], digit)


def one_note():
	global solved_count
	global stuck
	for line in range(0, 9):
		for column in range(0, 9):
			if (mat[line][column] == 0):
				cnt_notes = 0
				who = 0
				for digit in range(1, 10):
					if (notes[line][column][digit] == 1):
						cnt_notes = cnt_notes + 1
						who = digit
				if (cnt_notes == 1):
					solved_count = solved_count + 1
					stuck = 0
					mat[line][column] = who
					remove_notes_spot(line, column)
					remove_notes_rows(line, who)
					remove_notes_columns(column, who)
					remove_notes_3x3(line_borders_3x3[line][column], column_borders_3x3[line][column], who)


def reduction():
	global solved_count
	global stuck
	for start_line in range(0, 9, 3):
		for start_column in range(0, 9, 3):
			for digit in range(1, 10):
				exists = 0
				for line in range(start_line, start_line + 3):
					for column in range(start_column, start_column + 3):
						if (mat[line][column] == digit):
							exists = 1
				if (exists == 0):
					min_line = 10
					max_line = -1
					min_column = 10
					max_column = -1
					for line in range(start_line, start_line + 3):
						for column in range(start_column, start_column + 3):
							if (notes[line][column][digit] == 1):
								if (line < min_line):
									min_line = line
								if (line > max_line):
									max_line = line
								if (column < min_column):
									min_column = column
								if (column > max_column):
									max_column = column
					if (min_line == max_line):
						for column in range(0, 9):
							if (column < min_column or column > max_column):
								if (notes[min_line][column][digit]):
									stuck = 0
								notes[min_line][column][digit] = 0
					if (min_column == max_column):
						for line in range(0, 9):
							if (line < min_line or line > max_line):
								if (notes[line][min_column][digit]):
									stuck = 0
								notes[line][min_column][digit] = 0


def subset_reduction_3x3():
	global solved_count
	global stuck
	for start_line in range(0, 9, 3):
		for start_column in range(0, 9, 3):
			for msk in range(1, 512):
				frq = []
				for i in range(0, 10):
					frq.append(0)
				cnt_columns = 0
				cnt = 0;
				for line in range(start_line, start_line + 3):
					for column in range(start_column, start_column + 3):
						if (msk & (2 ** cnt) != 0):
							if (mat[line][column] != 0):
								cnt_columns = 100
							cnt_columns = cnt_columns + 1
							for digit in range(1, 10):
								if (notes[line][column][digit] == 1):
									frq[digit] = 1
						cnt = cnt + 1
				cnt_digits = 0
				for digit in range(1, 10):
					if (frq[digit] == 1):
						cnt_digits = cnt_digits + 1
				if (cnt_digits == cnt_columns):
					cnt = 0
					for line in range(start_line, start_line + 3):
						for column in range(start_column, start_column + 3):
							if ((msk & (2 ** cnt)) == 0):
								for digit in range(1, 10):
									if (frq[digit] == 1):
										if (notes[line][column][digit] == 1):
											notes[line][column][digit] = 0
											stuck = 0
							cnt = cnt + 1


def subset_reduction_1x9():
	global solved_count
	global stuck
	for line in range(0, 9):
		for msk in range(1, 512):
			frq = []
			for i in range(0, 10):
				frq.append(0)
			cnt_columns = 0
			for column in range(0, 9):
				if ((msk & (2 ** column)) != 0):
					if (mat[line][column] != 0):
						cnt_columns = 100
					++cnt_columns
					for digit in range(1, 10):
						if (notes[line][column][digit] == 1):
							frq[digit] = 1
			cnt_digits = 0
			for digit in range(1, 10):
				if (frq[digit] == 1):
					cnt_digits = cnt_digits + 1
			if (cnt_digits == cnt_columns):
				for column in range(0, 9):
					if ((msk & (2 ** column)) == 0):
						for digit in range(1, 10):
							if (frq[digit] == 1):
								if (notes[line][column][digit] == 1):
									notes[line][column][digit] = 0
									stuck = 0


def subset_reduction_9x1():
	global solved_count
	global stuck
	for column in range(0, 8):
		for msk in range(1, 512):
			frq = []
			for i in range(0, 10):
				frq.append(0)
			cnt_lines = 0
			for line in range(0, 9):
				if ((msk & (2 ** line) != 0)):
					if (mat[line][column] != 0):
						cnt_lines = 100
					cnt_lines = cnt_lines + 1
					for digit in range(1, 10):
						if (notes[line][column][digit] == 1):
							frq[digit] = 1
			cnt_digits = 0
			for digit in range(1, 10):
				if (frq[digit] == 1):
					cnt_digits = cnt_digits + 1
			if (cnt_digits == cnt_lines):
				for line in range(0, 9):
					if ((msk & (2 ** line)) == 0):
						for digit in range(1, 10):
							if (frq[digit] == 1):
								if (notes[line][column][digit] == 1):
									notes[line][column][digit] = 0
									stuck = 0


org = [[0 for x in range(9)] for y in range(9)]

found = 0


def bkt(L, C):
	global found
	if (found == 0):
		if (L == 9):
			found = 1
			return
		if (L != 9):
			if (mat[L][C] != 0):
				if (C == 8):
					bkt(L + 1, 0)
				else:
					bkt(L, C + 1)
			else:
				for digit in range(1, 10):
					if (notes[L][C][digit] == 1):
						added = 1
						for pos_line in range(0, 9):
							if (mat[pos_line][C] == digit):
								added = 0
						for pos_column in range(0, 9):
							if (mat[L][pos_column] == digit):
								added = 0
						for pos_line in range(line_borders_3x3[L][C], line_borders_3x3[L][C] + 3):
							for pos_column in range(column_borders_3x3[L][C], column_borders_3x3[L][C] + 3):
								if (mat[pos_line][pos_column] == digit):
									added = 0
						if (added == 1):
							mat[L][C] = digit
							if (C == 8):
								bkt(L + 1, 0)
							else:
								bkt(L, C + 1)
							if (found):
								return
							mat[L][C] = 0
					if (found == 1):
						return;


def sudoku_solver():
	global solved_count
	global stuck
	for line in range(0, 9):
		for column in range(0, 9):
			line_borders_3x3[line][column] = (line) // 3 * 3
			column_borders_3x3[line][column] = (column // 3) * 3
			if (mat[line][column] > 0):
				solved_count = solved_count + 1

	add_notes()

	while (solved_count < 81 and stuck == 0):
		stuck = 1
		unique_row()
		unique_column()
		unique_3x3()
		one_note()
		reduction()
		subset_reduction_3x3()
		subset_reduction_1x9()
		subset_reduction_9x1()

	if (solved_count < 81):
		for line in range(0, 9):
			for column in range(0, 9):
				if (mat[line][column] != 0):
					org[line][column] = 1
		bkt(0, 0)

f = open("input.in", "r")
t = int(f.readline())
for grid in range(0, t):
	for line in range(0, 9):
		for column in range(0, 9):
			mat[line][column] = 0
			org[line][column] = 0
			for digit in range(0, 10):
				notes[line][column][digit] = 0
	data = f.readline()
	stuck = 0
	solved_count = 0
	found = 0
	line = 0
	column = 0
	for pos in range(0, 81):
		if (data[pos] != '.'):
			mat[line][column] = int(data[pos])
		else:
			mat[line][column] = 0
		column = column + 1
		if (column == 9):
			line = line + 1
			column = 0

	sudoku_solver()
