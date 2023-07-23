#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 255
#define MAX_FIELD_LENGTH 50
#define MAX_PLOT_LENGTH 2500
#define MAX_FIELDS 5
#define MAX_LIST_SIZE 100

typedef struct MovieCard Movie;
typedef struct FilmmakerCard Filmmaker;
typedef struct GenreCard Genre;
typedef struct LIST List;
typedef struct DATE Date;
typedef struct NODE Node;

struct DATE
{
	int day;
	int month;
	int year;
};

struct MovieCard
{
	int movieID;
	char movieTitle[MAX_FIELD_LENGTH];
	List *directors;
	List *genres;
	List *actors;
	Date yearOfRelease;
};

struct FilmmakerCard
{
	int id;
	char name[MAX_FIELD_LENGTH];
};

struct GenreCard
{
	int id;
	char tittle[MAX_FIELD_LENGTH];
};

struct NODE
{
	Genre *genreValue;
	Filmmaker *filmmakerValue;
	Movie *movieValue;
	struct NODE *next;
	struct NODE *prev;
};

struct LIST
{
	int typeOfList;
	Node *head;
	Node *cur;
	Node *tail;
	int count;

	char *(*getKey)(Node *);
	int (*compare)(Node *, Node *);
};

/* service function block */

/* Clear the input buffer*/
void clearInputBuffer();

/* input output block start*/

void safeInputString(char *input);

void safeInputInt(int *intInput);

/* date section*/

int isLeapYear(int year);

int isValidDate(Date date);

void getDate(Date *date);

int compareDates(const Date *date1, const Date *date2);

void printDate(Date date);

/* list logick block*/

/* getKey in List function's*/

char *getGenreTittle(Node *item);

char *getFilmmakerName(Node *item);

char *getMovieTittle(Node *item);

/* compare in List function's*/

int compareByTittle(Node *a, Node *b);

int compareByDate(Node *a, Node *b);

int compareById(Node *a, Node *b);

/* creating*/

List *createNewList(int type);

Node *createNode();

Node *createGenreNode(Genre *value, int id);

Node *createFilmmakerNode(Filmmaker *value, int id);

Node *createMovieNode(Movie *value, int id);

Node *copyNode(Node *node);

/* inserting*/

Node *findPlaceToInsert(List *list, const char *name);

Node *findPlaceInMovieList(List *list, Node *newNode);

void pushNodeInMovieList(List *list, Node *newNode);

void insertNodeInList(List *list, Node *newNode);

Node *findOrInsert(List *list, const char *name);

Node *insertInMovieList(List *mainList, List *movieList, const char *name);

/* deleting*/

void freeList(List *list);

void deleteNode(List *list, Node *node);

/* another*/

void printList(const char *beforetext, List *itemList);

List *addInfoInList(List *mainList, int type);

/* card's and movie's logic block*/

/* Filmmaker and Genre card's create*/

Genre *createNewGenre(const char *name);

Filmmaker *createNewFilmmaker(const char *name);

/* create movie*/

void addInfoInMovie(struct MovieCard *movie, List *mainGenreList, List *mainFilmmakerList);

Movie *createMovie(int id, List *mainGenreList, List *mainFilmmakerList);

/* filter movie's*/

List *filterByDate(List *list, Date date);

List *filterByTittle(List *list, const char *value);

/* sort movie's*/

void sortList(List *list);

void changeListSortParametr(List *list, int paramNum);

/* select movie*/

Node *getNodeById(List *list, int id);

Movie *getMovieById(List *list, int id);

/* edit movie

void editMovie(Movie *movie, List *mainGenreList, List *mainFilmmakerList);

/* another block*/

/* file output section*/

void fileWriteDate(FILE *file, Date date);

void fileWriteList(FILE *file, List *itemList);

void writeMovieInFile(FILE *file, Movie *movie);

int writeFile(const char *filename, List *list);

/* file input section*/

List *addInfoInListFromStr(List *mainList, char *names_str, int type);

int inputFromFile(FILE *file, Movie *movie, List *mainGenreList, List *mainFilmmakerList);

int inputFile(const char *filename, List *list, List *mainGenreList, List *mainFilmmakerList);

/* menu section*/

void printMovie(Movie *elem);

void printMovieDetail(Movie *movie);

void printData(List *list);

int fileMenu(List *list, int mode, List *mainGenreList, List *mainFilmmakerList);

int chooseCardMenu();

void changeSortMenu(List *list);

void searchParamMenu(List *list);

void printMenu();

int menu();

/* main and realisation*/

int main()
{

	menu();

	return 0;
}

/* service function block*/

/* Clear the input buffer*/
void clearInputBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

/* input output block start*/

void safeInputString(char *input)
{
	int c;
	int i = 0;

	while ((c = getchar()) != '\n' && c != EOF)
	{
		if (i < MAX_INPUT_LENGTH - 1)
		{
			input[i++] = (char)c;
		}
	}

	input[i] = '\0';
}

void safeInputInt(int *intInput)
{
	char input[MAX_INPUT_LENGTH];
	int noDigit;
	int flagBr;
	int i;

	noDigit = 1;

	while (noDigit == 1)
	{
		safeInputString(input);
		noDigit = 0;

		flagBr = 1;
		for (i = 0; flagBr && input[i] != '\0'; i++)
		{
			if (!isdigit(input[i]))
			{
				printf("Invalid input! Please enter a valid integer again.\n");
				noDigit = 1;
				flagBr = 0;
			}
		}
	}

	*intInput = atoi(input);
}

/* date section*/

int isLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int isValidDate(Date date)
{
	int flag;
	int daysInMonth[12];
	int i;

	flag = 1;
	if (date.year < 0 || date.month < 1 || date.month > 12 || date.day < 1)
		flag = 0;

	if (flag)
	{
		for (i = 0; i < 12; i++)
			daysInMonth[i] = 30 + (i + 1) % 2;
		daysInMonth[1] = 28 + isLeapYear(date.year);
	}

	return flag && date.day <= daysInMonth[date.month - 1];
}

void getDate(Date *date)
{
	do
	{
		printf("Enter the valid date (dd mm yyyy): ");
		scanf("%d %d %d", &date->day, &date->month, &date->year);
		clearInputBuffer();
	} while (!isValidDate(*date));
}

int compareDates(const Date *date1, const Date *date2)
{
	int res;
	res = 0;
	if (date1->year != date2->year)
		res = date1->year - date2->year;
	else if (date1->month != date2->month)
		res = date1->month - date2->month;
	else if (date1->day != date2->day)
		res = date1->day - date2->day;

	return res;
}

void printDate(Date date)
{
	printf("%02d.%02d.%04d", date.day, date.month, date.year);
}

/* list logick block*/

void freeList(List *list)
{
	if (list != NULL && list->head != NULL)
	{
		for (list->cur = list->head; list->cur->next != NULL; list->cur = list->cur->next)
			free(list->cur->prev);
		free(list->cur);
		free(list);
	}
}

char *getGenreTittle(Node *item)
{
	return item->genreValue->tittle;
}

char *getFilmmakerName(Node *item)
{
	return item->filmmakerValue->name;
}

char *getMovieTittle(Node *item)
{
	return item->movieValue->movieTitle;
}

int compareByTittle(Node *a, Node *b)
{
	return strcmp(a->movieValue->movieTitle, b->movieValue->movieTitle);
}

int compareByDate(Node *a, Node *b)
{
	return compareDates(&a->movieValue->yearOfRelease, &b->movieValue->yearOfRelease);
}

int compareById(Node *a, Node *b)
{
	return a->movieValue->movieID - b->movieValue->movieID;
}

List *createNewList(int type)
{

	List *newList = NULL;
	newList = (List *)malloc(sizeof(List));
	newList->head = NULL;
	newList->cur = NULL;
	newList->tail = NULL;
	newList->count = 0;

	newList->typeOfList = type;
	if (type == 0 || type == 2)
		newList->getKey = &getFilmmakerName;
	else if (type == 1)
		newList->getKey = &getGenreTittle;
	else if (type == 3)
	{
		newList->getKey = &getMovieTittle;
		newList->compare = &compareByTittle;
	}
	return newList;
}

Genre *createNewGenre(const char *name)
{
	Genre *value = NULL;
	value = (Genre *)malloc(sizeof(Genre));
	strncpy(value->tittle, name, 50);
	return value;
}

Filmmaker *createNewFilmmaker(const char *name)
{
	Filmmaker *value = NULL;
	value = (Filmmaker *)malloc(sizeof(Filmmaker));
	strncpy(value->name, name, 50);
	return value;
}

Node *createNode()
{
	Node *newNode = NULL;
	newNode = (Node *)malloc(sizeof(Node));
	newNode->genreValue = NULL;
	newNode->filmmakerValue = NULL;
	newNode->movieValue = NULL;
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

Node *createGenreNode(Genre *value, int id)
{
	Node *newNode = createNode();
	newNode->genreValue = value;

	return newNode;
}

Node *createFilmmakerNode(Filmmaker *value, int id)
{

	Node *newNode = createNode();
	newNode->filmmakerValue = value;

	return newNode;
}

Node *createMovieNode(Movie *value, int id)
{

	Node *newNode = createNode();
	newNode->movieValue = value;

	return newNode;
}

Node *copyNode(Node *node)
{
	Node *newNode = createNode();
	newNode->genreValue = node->genreValue;
	newNode->filmmakerValue = node->filmmakerValue;
	newNode->movieValue = node->movieValue;

	return newNode;
}

char *strtok_c90(char *str, const char *delimiters)
{
	static char *nextToken = NULL;
	char *startToken = NULL;
	char *endToken = NULL;

	int flag;
	int flagBr;

	flag = 0;
	flagBr = 1;

	if (str != NULL)
		nextToken = str;
	else if (nextToken == NULL)
		flag = 1;

	if (flag != 1)
	{
		startToken = nextToken;

		while (flagBr && *nextToken != '\0')
		{
			if (*nextToken != *delimiters)
			{
				startToken = nextToken;
				flagBr = 0;
			}
			if (flagBr)
				nextToken++;
		}

		if (*startToken == '\0')
		{
			nextToken = NULL;
			flag = 1;
		}

		if (flag != 1)
		{
			endToken = startToken;

			flagBr = 1;

			while (flagBr && *endToken != '\0')
			{
				if (*endToken == *delimiters)
				{
					*endToken = '\0';
					nextToken = endToken + 1;
					flagBr = 0;
				}
				if (flagBr)
					endToken++;
			}
		}
	}

	return startToken;
}

Node *findPlaceToInsert(List *list, const char *name)
{
	Node *res = NULL;
	int flag;
	flag = 1;
	for (list->cur = list->head; list->cur != NULL && flag; list->cur = list->cur->next)
		if (strcmp(list->getKey(list->cur), name) >= 0)
		{
			flag = 0;
			res = list->cur;
		}

	return res;
}

Node *findPlaceInMovieList(List *list, Node *newNode)
{
	Node *res = NULL;
	int flag;
	flag = 1;
	for (list->cur = list->head; list->cur != NULL && flag; list->cur = list->cur->next)
		if (list->compare(list->cur, newNode) >= 0)
		{
			flag = 0;
			res = list->cur;
		}

	return res;
}

void pushNodeInMovieList(List *list, Node *newNode)
{
	list->cur = findPlaceInMovieList(list, newNode);

	insertNodeInList(list, newNode);
}

void deleteNode(List *list, Node *node)
{
	if (node != NULL && list->head != NULL)
	{
		if (list->head->next == NULL)
		{
			list->count = 0;
			list->head = NULL;
		}
		else
		{
			if (node->next != NULL)
				node->next->prev = node->prev;
			else
				list->tail = node->prev;
			if (node->prev != NULL)
				node->prev->next = node->next;
			else
				list->head = node->next;
		}
		free(node);
	}
}

void insertNodeInList(List *list, Node *newNode)
{
	/* Add the new item to the list*/
	if (list->head == NULL) /* If head is NULL and list is empty*/
	{
		list->head = newNode;
		list->tail = newNode;
	}
	else /* If list not empty*/
	{
		if (list->cur == NULL) /* insert in tail*/
		{
			list->tail->next = newNode;
			newNode->prev = list->tail;
			list->tail = newNode;
		}
		else
		/* insert in middle or in head*/
		{
			newNode->prev = list->cur->prev;
			list->cur->prev = newNode;
			newNode->next = list->cur;
			if (newNode->prev == NULL)
				list->head = newNode;
			else
				newNode->prev->next = newNode;
		}
	}
	list->count++;
}

Node *findOrInsert(List *list, const char *name)
{
	Node *newNode = NULL;
	int flag;
	flag = 1;

	list->cur = findPlaceToInsert(list, name);

	if (list->cur != NULL && strcmp(list->getKey(list->cur), name) == 0)
	{
		flag = 0;
		newNode = list->cur;
	}

	if (flag)
	{
		if (list->typeOfList == 1)
			newNode = createGenreNode(createNewGenre(name), list->count + 1);
		else
			newNode = createFilmmakerNode(createNewFilmmaker(name), list->count + 1);

		insertNodeInList(list, newNode);
	}

	return newNode;
}

Node *insertInMovieList(List *mainList, List *movieList, const char *name)
{
	Node *newNode = NULL;
	int flag;

	Node *value = NULL;
	value = findOrInsert(mainList, name);

	flag = 1;

	movieList->cur = findPlaceToInsert(movieList, name);

	if (movieList->cur != NULL && strcmp(movieList->getKey(movieList->cur), name) == 0)
	{
		flag = 0;
		newNode = movieList->cur;
	}

	if (flag)
	{
		if (mainList->typeOfList == 1)
			newNode = createGenreNode(value->genreValue, movieList->count + 1);
		else
			newNode = createFilmmakerNode(value->filmmakerValue, movieList->count + 1);

		insertNodeInList(movieList, newNode);
	}

	return newNode;
	/* Return the index of the new added item*/
}

void printList(const char *beforetext, List *itemList)
{
	printf("%s ", beforetext);
	for (itemList->cur = itemList->head; itemList->cur != NULL; itemList->cur = itemList->cur->next)
	{
		if (itemList->cur->next == NULL)
			printf("%s;\n", itemList->getKey(itemList->cur));
		else
			printf("%s, ", itemList->getKey(itemList->cur));
	}
}

List *addInfoInList(List *mainList, int type)
{
	int choice = 1;
	char name[MAX_INPUT_LENGTH];
	List *newList = NULL;

	newList = createNewList(type % 2);

	while (choice)
	{
		printf("cur list: ");
		printList("", newList);
		if (type == 0)
			printf("Enter new Director Name: ");
		else if (type == 2)
			printf("Enter new Actor Name: ");
		else
			printf("Enter new Genre Title: ");
		safeInputString(name);
		insertInMovieList(mainList, newList, name);

		printf("Enter 0 if you want stop  or another number if you want continue: ");
		safeInputInt(&choice);
	}
	return newList;
}

/* movie card section*/

void addInfoInMovie(struct MovieCard *movie, List *mainGenreList, List *mainFilmmakerList)
{

	printf("Enter Movie Title: ");
	safeInputString(movie->movieTitle);

	printf("Enter Directors: ");
	movie->directors = addInfoInList(mainFilmmakerList, 0);

	printf("Enter Year of Release: ");
	getDate(&movie->yearOfRelease);

	printf("Enter Genres: ");
	movie->genres = addInfoInList(mainGenreList, 1);

	printf("Enter Actors: ");
	movie->actors = addInfoInList(mainFilmmakerList, 2);
}

Movie *createMovie(int id, List *mainGenreList, List *mainFilmmakerList)
{
	Movie *newMovie = NULL;
	newMovie = (Movie *)malloc(sizeof(Movie));
	newMovie->movieID = id;
	addInfoInMovie(newMovie, mainGenreList, mainFilmmakerList);

	return newMovie;
}

List *filterByDate(List *list, Date date)
{
	List *newList = NULL;
	Node *tempNode = NULL;
	newList = createNewList(list->typeOfList);
	for (list->cur = list->head; list->cur != NULL; list->cur = list->cur->next)
	{
		if (compareDates(&list->cur->movieValue->yearOfRelease, &date) == 0)
		{
			tempNode = copyNode(list->cur);
			pushNodeInMovieList(newList, tempNode);
		}
	}
	return newList;
}

List *filterByTittle(List *list, const char *value)
{
	List *newList = NULL;
	Node *tempNode = NULL;
	newList = createNewList(list->typeOfList);
	for (list->cur = list->head; list->cur != NULL; list->cur = list->cur->next)
	{
		if (strcmp(list->cur->movieValue->movieTitle, value) == 0)
		{
			tempNode = copyNode(list->cur);
			pushNodeInMovieList(newList, tempNode);
		}
	}
	return newList;
}

void sortList(List *list)
{

	int swapped;
	Node *tail = NULL;
	Movie *temp = NULL;

	do
	{
		list->cur = list->head;
		swapped = 0;

		while (list->cur != NULL && list->cur->next != tail)
		{
			if (list->cur->next != NULL && list->compare(list->cur, list->cur->next) > 0)
			{
				temp = list->cur->movieValue;
				list->cur->movieValue = list->cur->next->movieValue;
				list->cur->next->movieValue = temp;
				swapped = 1;
			}
			list->cur = list->cur->next;
		}

		tail = list->cur;
	} while (swapped);
}

void changeListSortParametr(List *list, int paramNum)
{
	switch (paramNum)
	{
	case 0:
		list->compare = &compareById;
		break;
	case 2:
		list->compare = &compareByDate;
		break;
	default:
		list->compare = &compareByTittle;
	}
	sortList(list);
}

Node *getNodeById(List *list, int id)
{
	Node *res = NULL;
	int flag;

	flag = 1;

	for (list->cur = list->head; list->cur != NULL && flag; list->cur = list->cur->next)
		if (list->cur->movieValue->movieID == id)
		{
			res = list->cur;
			flag = 0;
		}

	return res;
}

Movie *getMovieById(List *list, int id)
{
	Node *temp = NULL;
	Movie *res = NULL;
	temp = getNodeById(list, id);

	if (temp != NULL)
		res = temp->movieValue;

	return res;
}

void editMovie(Movie *movie, List *mainGenreList, List *mainFilmmakerList)
{
	int choice;
	do
	{
		system("clear");
		printMovieDetail(movie);
		printf("Chose field to edit: ");
		safeInputInt(&choice);
		printf("7) Exit");

		switch (choice)
		{
		case 2:
			printf("Input new tittle: ");
			safeInputString(movie->movieTitle);
			break;
		case 3:
			printf("Input new directors list: ");
			freeList(movie->directors);
			movie->directors = addInfoInList(mainFilmmakerList, 0);
			break;
		case 4:
			printf("Enter Year of Release: ");
			getDate(&movie->yearOfRelease);
			break;
		case 5:
			printf("Input new genres list: ");
			freeList(movie->genres);
			movie->genres = addInfoInList(mainGenreList, 1);
			break;
		case 6:
			printf("Input new actors list: ");
			freeList(movie->actors);
			movie->actors = addInfoInList(mainFilmmakerList, 2);
			break;
		case 7:
			printf("Exiting the eddit.\n");
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}

		printf("\n");
	} while (choice != 7);
}

/* file output start*/

void fileWriteDate(FILE *file, Date date)
{
	fprintf(file, "%02d.%02d.%04d;", date.day, date.month, date.year);
}

void fileWriteList(FILE *file, List *itemList)
{
	for (itemList->cur = itemList->head; itemList->cur != NULL; itemList->cur = itemList->cur->next)
		fprintf(file, "%s,", itemList->getKey(itemList->cur));
	fprintf(file, ";");
}

void writeMovieInFile(FILE *file, Movie *movie)
{
	fprintf(file, "%s;", movie->movieTitle);
	fileWriteList(file, movie->directors);
	fileWriteDate(file, movie->yearOfRelease);
	fileWriteList(file, movie->genres);
	fileWriteList(file, movie->actors);
	fprintf(file, "\n");
}

int writeFile(const char *filename, List *list)
{
	int flag;
	FILE *file = fopen(filename, "w");
	flag = 1;
	if (file == NULL)
	{
		printf("Error opening file %s\n", filename);
		flag = 0;
	}

	if (flag)
	{
		for (list->cur = list->head; list->cur != NULL; list->cur = list->cur->next)
			writeMovieInFile(file, list->cur->movieValue);

		fclose(file);
	}

	return (flag + 2) % 2;
}

/* file input start*/

List *addInfoInListFromStr(List *mainList, char *names_str, int type)
{
	List *newList = NULL;
	char temp[MAX_INPUT_LENGTH];

	char *str = strtok_c90(names_str, ",");

	newList = createNewList(type % 2);

	while (str != NULL)
	{
		strncpy(temp, str, MAX_INPUT_LENGTH);
		insertInMovieList(mainList, newList, temp);
		str = strtok_c90(NULL, ",");
	}

	return newList;
}

int inputFromFile(FILE *file, Movie *movie, List *mainGenreList, List *mainFilmmakerList)
{
	char line[MAX_PLOT_LENGTH];
	char *fields[MAX_FIELDS];
	int i;
	int flag;

	flag = 1;

	if (fgets(line, sizeof(line), file) != NULL)
	{
		char *str = strtok_c90(line, ";");
		for (i = 0; i < MAX_FIELDS; i++)
		{
			fields[i] = str;
			str = strtok_c90(NULL, ";");
		}

		strncpy(movie->movieTitle, fields[0], MAX_FIELD_LENGTH);
		movie->directors = addInfoInListFromStr(mainFilmmakerList, fields[1], 0);
		sscanf(fields[2], "%d.%d.%d", &movie->yearOfRelease.day, &movie->yearOfRelease.month, &movie->yearOfRelease.year);
		movie->genres = addInfoInListFromStr(mainGenreList, fields[3], 1);
		movie->actors = addInfoInListFromStr(mainFilmmakerList, fields[4], 2);

		flag = 0;
	}
	return flag;
}

int inputFile(const char *filename, List *list, List *mainGenreList, List *mainFilmmakerList)
{
	int flag;
	int returnFlag;
	Movie *newMovie = NULL;
	FILE *file = fopen(filename, "r");
	flag = 0;
	returnFlag = 0;
	if (file == NULL)
	{
		printf("Error opening file %s\n", filename);
		returnFlag = 1;
	}
	if (returnFlag == 0)
	{
		do
		{

			newMovie = (Movie *)malloc(sizeof(Movie));
			newMovie->movieID = list->count;

			flag = inputFromFile(file, newMovie, mainGenreList, mainFilmmakerList);
			if (!flag)
				pushNodeInMovieList(list, createMovieNode(newMovie, list->count));
		} while (!flag);

		fclose(file);
	}

	return returnFlag;
}

/* menu block start*/

void printMovie(Movie *elem)
{
	printf("|       %2d    |%25s |   ", elem->movieID, elem->movieTitle);
	printDate(elem->yearOfRelease);
	printf("      |");
	printf("\n+-------------+--------------------------+-------------------+\n");
}

void printMovieDetail(Movie *movie)
{
	char tmp[MAX_INPUT_LENGTH];
	printf("1)Movie ID: %d\n", movie->movieID);
	printf("2)Movie Title: %s\n", movie->movieTitle);
	printList("3)Movie Directors: ", movie->directors);
	printf("4)Year of Release: ");
	printDate(movie->yearOfRelease);
	printList("\n5)Movie Genres: ", movie->genres);
	printList("6)Movie Actors: ", movie->actors);
	printf("press Enter to continue");
	safeInputString(tmp);
}

void printData(List *list)
{
	printf("+-------------+--------------------------+-------------------+\n");
	printf("|      id     |            name          |   YearOfRealese   |\n");
	printf("+-------------+--------------------------+-------------------+\n");
	for (list->cur = list->head; list->cur != NULL; list->cur = list->cur->next)
	{
		printMovie(list->cur->movieValue);
	}
}

int fileMenu(List *list, int mode, List *mainGenreList, List *mainFilmmakerList)
{
	char name[MAX_INPUT_LENGTH];

	printf("Input file name:\n");
	safeInputString(name);

	if (mode == 0)
	{
		while (writeFile(name, list) != 0)
		{
			printf("wrong file name, try again:\n");
			safeInputString(name);
		}
	}
	else
	{
		while (inputFile(name, list, mainGenreList, mainFilmmakerList) != 0)
		{
			printf("wrong file name, try again:\n");
			safeInputString(name);
		}
	}

	return 0;
}

int chooseCardMenu()
{
	int choice;
	printf("Choose id: ");
	safeInputInt(&choice);
	return choice;
}

void changeSortMenu(List *list)
{
	int choice;
	printf("Sort parametrs:\n");
	printf("0: ID\n");
	printf("1: Tittle\n");
	printf("2: Date\n");
	safeInputInt(&choice);
	changeListSortParametr(list, choice);
}

void searchParamMenu(List *list)
{
	int choice;
	char name[MAX_INPUT_LENGTH];
	Date date;
	List *filteredList = NULL;

	printf("Search parametrs:\n");
	printf("1: Tittle\n");
	printf("2: Date\n");
	printf("another number: back\n");
	safeInputInt(&choice);
	printf("Input value to search:\n");

	if (choice == 1)
	{
		safeInputString(name);
		filteredList = filterByTittle(list, name);
	}
	else if (choice == 2)
	{
		getDate(&date);
		filteredList = filterByDate(list, date);
	}
	printData(filteredList);
}

void printMenu()
{
	printf("Menu:\n");
	printf("0: Add card from file\n");
	printf("1: Adding cards of objects of the subject area\n");
	printf("2: Editing cards\n");
	printf("3: Deleting cards\n");
	printf("4: Output card details\n");
	printf("5: Search for cards by parameter\n");
	printf("6: Sort cards by parameter\n");
	printf("7: File output\n");
	printf("8: Exit\n");
}

int menu()
{
	int choice;
	List *genreList = NULL;
	List *filmmakerList = NULL;
	List *movieList = NULL;

	genreList = createNewList(1);
	filmmakerList = createNewList(2);
	movieList = createNewList(3);

	do
	{
		system("clear");
		printData(movieList);
		printMenu();
		printf("Enter your choice: ");
		safeInputInt(&choice);

		switch (choice)
		{
		case 0:
			fileMenu(movieList, 1, genreList, filmmakerList);
			break;
		case 1:
			pushNodeInMovieList(movieList, createMovieNode(createMovie(movieList->count, genreList, filmmakerList), movieList->count));
			break;
		case 2:
			editMovie(getMovieById(movieList, chooseCardMenu()), genreList, filmmakerList);
			sortList(movieList);
			break;
		case 3:
			deleteNode(movieList, getNodeById(movieList, chooseCardMenu()));
			break;
		case 4:
			printMovieDetail(getMovieById(movieList, chooseCardMenu()));
			break;
		case 5:
			searchParamMenu(movieList);
			break;
		case 6:
			changeSortMenu(movieList);
			break;
		case 7:
			fileMenu(movieList, 0, NULL, NULL);
			break;
		case 8:
			printf("Exiting the program.\n");
			freeList(movieList);
			freeList(filmmakerList);
			freeList(genreList);
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}

		printf("\n");
	} while (choice != 8);

	return 0;
}
