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

//TODO: Make a print_world function that prints the world, colorized by the colors of the owning city state
void print_world(size_t rows, size_t cols) {
	clearscreen(); //Clears the screen
	resetcolor(); //Sets color to white font black background
	movecursor(0,0); //Moves the cursor to the top left of the screen
	/*
	   setcolor(0,0,0); //Foreground black text
	   setbgcolor(100,200,100); //Set the background color to limeish green
	   cout << " "; //Prints a block of color (100,200,100)
	   */
	bool bigcity = false;
	for(size_t i = 0; i < rows; i++){
		for(size_t j = 0; j < cols; j++){
			for(City ci: city_list){
				if(i == ci.row and j == ci.col){
					setcolor(255,255,255);
					cout << char(toupper(ci.name[0]));
					bigcity = true;
					break;
				}
			}
			int tile_owner = world.at(i).at(j).owner;
			setcolor(0,0,0); //Foreground black text
			setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
			//setbgcolor(100,200,100); //Set the background color to limeish green
			if(!bigcity) cout << " "; //Prints a block of color (100,200,100)
			else bigcity = false;
		}
	}
	//YOU: For every tile in the world, set the background color (using setbgcolor(r,g,b)) to be the color of the owner's city then print a space.
	//It will print out one block of color of some sort of lime green
	//So you are going to do a doubly-nested for loop iterating over the whole world, printing the colors of the owner of each tile one per space or something
}


// Lloyd Function
void lloyd() {
	// For every city
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
	//city_list.resize(states);
	//for (const City &c : city_list) cout << c.name << endl;
	cout << RESET;
	//This is just a dummy for loop that will add Fresno and hopefully random cities to the world
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
	//TODO: Read in cities here from a file or allow the user to add/delete cities
	//DONE Kinda

	//They can either be randomly generated or manually typed in
	//Every city will have a color and a name
	//And saved to a file

	//Size the 2D vector to be the size of the terminal
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
		//TODO: Run the Voronoi calculation on the world map and set each tile's owner to be the index of the city that is closest to it
		//Simplest algorithm: for every tile in the world, find the nearest city, and set the owner of that tile to be the index of the city in the vector of cities
		//Hypot - a function that does the pythagorean theorem
		//Something like that: float distance = hypot(my_row-city_row,my_col-city_col); 
		//Example - this sets the owner of tile(row 10, column 30) to be city #12:

		//print_world();
		print_world(rows, cols);

		resetcolor();
		string choice = read("Would you like to use the lloyd function applied?(y or n)> ");
		if (choice == "y") lloyd();
		else break;
	}

	//TODO: Calculate which city state is the biggest, and then print its name and size
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
