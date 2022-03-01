#include "/public/read.h"
#include "/public/colors.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
using namespace std;


// Comment to test out commiting and pushing with git


//Mandate C++17 and above
#if __cplusplus < 201703L
#error This code requires C++17 and above, sucks to be you. Compile with --std=c++17
#else

//Set up alias type names
typedef int8_t      s8;
//using s8 = int8_t;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;


struct Tile {
	//int32_t row, height;
	//s32 row, height;
	u32 owner = 0; //Index in the city vector of who owns me
	char content = 0; //Whats in this tile
};
vector<vector<Tile>> world;

class City {
	public:
		static inline int count = 0;

		City() {
			// Increase every time object is created
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;
			name = "Fresno " + to_string(++count);
			row = 0;
			col = 0;
		}
		string name;
		u8 r,g,b;
		int row,col; // Hi! :) ¯\_(ツ)_/¯
};
vector<City> city_list;

void edge_detection(size_t rows, size_t cols) {
	cout << "EDGE" << endl;
	for (int row = 1; row < rows; row++) {
		for (int col = 1; col < cols; col++) {
			Tile tile_here = world.at(row).at(col);
			Tile prev_row = world.at(row).at(col - 1);
			Tile prev_col = world.at(row - 1).at(col);


			if (tile_here.owner != prev_row.owner) {
				if (tile_here.content == '-') world.at(row).at(col).content = '+'; 
				else world.at(row).at(col).content = '|';

				if (prev_row.content == '-') world.at(row).at(col - 1).content = '+';
				else world.at(row).at(col - 1).content = '|';
			}
			if (tile_here.owner != prev_col.owner) {
				if (tile_here.content == '|') world.at(row).at(col).content = '+';
				else world.at(row).at(col).content = '-';

				if (prev_col.content == '|') world.at(row - 1).at(col).content = '+';
				else world.at(row - 1).at(col).content = '-';
			}

			// If this tile is a city, mark it with the first letter of the name
			if (row == city_list.at(tile_here.owner).row && col == city_list.at(tile_here.owner).col) world.at(row).at(col).content = city_list.at(tile_here.owner).name.at(0);
		}
	}
}


//TODO: Make a print_world function that prints the world, colorized by the colors of the owning city state
void print_world(size_t rows, size_t cols) {
	// Original Print Function (by Kerney)
	int last_city = -1; //Reduce bandwidth requirement by caching last color
	clearscreen();
	resetcolor();
	movecursor(0,0);
	setcolor(0,0,0); //Foreground black text
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			//movecursor(i,j);
			auto owner = world.at(i).at(j).owner;
			auto content = world[i][j].content;
			if (!content) content = ' ';
			if (owner == -1) {
				if (last_city != -1) setbgcolor(0,0,0);
				last_city = owner;
				cout << content;
			}
			else {
				City c = city_list.at(owner);

				// Only draw with color if tile has content
				if (content != ' ') setbgcolor(c.r,c.g,c.b);
				else setbgcolor(0, 0, 0);

				last_city = owner;
				cout << content;
			}
		}
		cout << endl;
	}
	resetcolor();
}

void clear_tiles(){
	for(vector<Tile> &vec : world){
		for(Tile &t : vec){
			t.content = 0;
		}
	}
}


// Lloyd Function
void lloyd() {
	// For every city
	clear_tiles();
	for (size_t i = 0; i < city_list.size(); i++) {
		int row_sum = 0;
		int row_count = 0;
		int col_sum = 0;
		int col_count = 0;

		for(int row = 0; row < world.size(); row++){
			for(int col = 0; col < world.at(row).size(); col++){
				if (world.at(row).at(col).owner == i) {
					row_sum += row;
					row_count++;

					col_sum += col;
					col_count++;
				}
			}
		}
		city_list.at(i).row = row_sum / row_count;
		city_list.at(i).col = col_sum / col_count;
	}
}



void die() {
	cout << "BAD INPUT!\n";
	exit(1);
}

int main() {
	srand (time(NULL)); //Set the seed
	cout << RED << "Welcome to Voroni Kingdoms!\n" << RESET;
	cout << GREEN;
	unsigned int states = read("How many city states do you want: ");
	if (states < 2) die();
	cout << RESET;
	auto [rows,cols] = get_terminal_size(); //C++17 structured bindings
	for(size_t i = 0; i < states; i++){
		int nrow = rand() % rows;
		int ncol = rand() % cols;
		string city_name;
		City a;
		city_name.resize(5);
		for(char &c: city_name){
			c = (rand() % 25) + 97;
		}
		toupper(city_name[0]);
		a.name = city_name;
		a.row = nrow;
		a.col = ncol;
		city_list.push_back(a);
	}
	for (const City &c : city_list) cout << c.name << endl;
	world.resize(rows);
	for (size_t i = 0; i < (size_t)rows; i++) world[i].resize(cols);

	float dist = 0.0;
	float min_dist = 0.0;
	int mdi = 0; //Minimum Distance Index

	while (true) {
		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++){
				for(size_t k = 0; k < city_list.size(); k++){
					City ci = city_list.at(k);
					dist = hypot(abs(i-ci.row), abs(j-ci.col));
					if(k == 0){
						min_dist = dist;
						mdi = k;
					} else if(dist < min_dist){
						min_dist = dist;
						mdi = k;
					}
				}
				world.at(i).at(j).owner = mdi;
			}
		}

		edge_detection(rows, cols);
		print_world(rows, cols);

		resetcolor();
		string choice = read("Would you like to use the lloyd function applied?(y or n)> ");
		if (choice == "y") lloyd();
		else break;
	}

	vector<int> city_size (city_list.size());
	for(vector<Tile> v: world){
		for(Tile a: v){
			city_size.at(a.owner)++;
		}
	}
	int max = 0;
	City fcity;

	for(int i = 0; i < city_size.size(); i++){
		if(city_size.at(i) > max){
			max = city_size.at(i);
			fcity = city_list.at(i);
		}
	}
	resetcolor();
	cout << fcity.name << ": " << max << endl;
}
#endif
