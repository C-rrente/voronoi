#include "/public/read.h"
#include "/public/colors.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
using namespace std;

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

void die();

struct Tile {
	//int32_t row, height;
	//s32 row, height;
	int owner = 0; //Index in the city vector of who owns me
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

void print_world(size_t rows, size_t cols) {
	clearscreen(); //Clears the screen
	resetcolor(); //Sets color to white font black background
	movecursor(0,0); //Moves the cursor to the top left of the screen
	bool bigcity = false; //This will tell if the char being output is the city
	bool noslope = false; //If we have a '-'
	bool pslope = false; //If we have a '+'
	bool infslope = false; //If we have a '|'
	for(size_t i = 0; i < rows; i++){
		for(size_t j = 0; j < cols; j++){
			for(City ci: city_list){
				if((int)i == ci.row and (int)j == ci.col){
					setcolor(0,0,0);
					setbgcolor(ci.r, ci.g, ci.b);
					cout << char(toupper(ci.name[0]));
					bigcity = true;
					break;
				}
			}
			int tile_owner = world.at(i).at(j).owner;
			if(i > 0 and i < cols-1 and j > 0 and j < rows-1){
				if(tile_owner == world.at(i-1).at(j).owner and tile_owner == world.at(i+1).at(j).owner and tile_owner == world.at(i).at(j+1).owner and tile_owner == world.at(i).at(j+1).owner and tile_owner == world.at(i+1).at(j+1).owner and tile_owner == world.at(i+1).at(j-1).owner and tile_owner == world.at(i-1).at(j+1).owner and tile_owner == world.at(i-1).at(j-1).owner){
					setcolor(0,0,0);
					setbgcolor(0,0,0);
				} else if(tile_owner != world.at(i+1).at(j).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					noslope = true;
				} else if(tile_owner != world.at(i-1).at(j).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					noslope = true;
				} else if(tile_owner != world.at(i).at(j+1).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					infslope = true;
				} else if(tile_owner != world.at(i).at(j-1).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					infslope = true;
				} else if(tile_owner != world.at(i+1).at(j+1).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					pslope = true;
				} else if(tile_owner != world.at(i+1).at(j-1).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					pslope = true;
				} else if(tile_owner != world.at(i-1).at(j+1).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					pslope = true;
				} else if(tile_owner != world.at(i-1).at(j-1).owner){
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
					pslope = true;
				} else {
					setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
				}
			} else if(i == 0){
				if(j == 0){
					//Check right and down
					if(tile_owner != world.at(i+1).at(j).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						noslope = true;
					} else if(tile_owner != world.at(i).at(j+1).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						infslope = true;
					}
				} else if(j == rows-1){
					//Check right and up
					if(tile_owner != world.at(i+1).at(j).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						noslope = true;
					} else if(tile_owner != world.at(i).at(j-1).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						infslope = true;
					}
				} 			
			} else if(i == cols-1){
				if(j == 0){
					//Check left and down
					if(tile_owner != world.at(i-1).at(j).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						noslope = true;
					} else if(tile_owner != world.at(i).at(j+1).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						infslope = true;
					}
				} else if(j == rows-1){
					//Check up and left
					if(tile_owner != world.at(i-1).at(j).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						noslope = true;
					} else if(tile_owner != world.at(i).at(j-1).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						infslope = true;
					}
				} else if(j > 0 and j < rows-1){
					if(tile_owner != world.at(i-1).at(j).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						noslope = true;
					} else if(tile_owner != world.at(i).at(j+1).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						infslope = true;
					} else if(tile_owner != world.at(i).at(j-1).owner){
						setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
						infslope = true;
					}
				} 			
			}
			//setcolor(0,0,0); //Foreground black text
			//setbgcolor(city_list.at(tile_owner).r, city_list.at(tile_owner).g, city_list.at(tile_owner).b);
			//if(i == 0 or i == rows-1 or j == 0 or j == cols-1){
			//	setbgcolor(0,0,0);
			//	cout << " ";
			//TODO Fix this trash lmao and learn how to detect edge
			//} else {
			if(noslope){
				cout << "-";
				noslope = false;
			} else if(infslope){
				cout << "|";
				infslope = false;
			} else if(pslope){
				cout << "+";
				pslope = false;
			} else if(bigcity){
				bigcity = false;
			} else {
				cout << " ";
			}
			//}
			//setbgcolor(0,0,0);
			//resetcolor();
		}
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
		city_name.resize(rand()%6+1);
		for(char &c: city_name){
			c = (rand() % 25) + 97; //This will select a random lowercase letter and append it to a string
		}
		city_name[0] = toupper(city_name[0]);
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
	print_world(rows, cols);
	vector<int> city_size (city_list.size());
	for(vector<Tile> v: world){
		for(Tile a: v){
			city_size.at(a.owner)++;
		}
	}
	int max = 0;
	City fcity;

	for(size_t i = 0; i < city_size.size(); i++){
		if(city_size.at(i) > max){
			max = city_size.at(i);
			fcity = city_list.at(i);
		}
	}
	resetcolor();
	cout << fcity.name << ": " << max << endl;
}
#endif
