#include <sfml/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <queue>

struct Node
{
	//for actual algorithm
	//g cost
	int gcost=0;
	//hcost
	int hcost=0;
	//f cost
	int fcost=0;
	//points to parent node
	Node* parent = NULL;
	//determine if explored already
	bool ex=0;


	//sorta visual purpose but also algorithm
	bool wall=0;
	bool end=0;
	bool start=0;
	bool path=0;
	//half explored, visual
	bool hex = 0;

};

//determines if its done
bool done = 0;
//determines if there is a solution
bool nosol = 0;
//base node grid
std::vector<std::vector<Node>> base;
//determines which hcost calculation to use
int alt = 0;



//calculate hcost using manhattan distance
void chc(std::vector<std::vector<Node>>& n, int epx, int epy) {
	for (int i = 0; i < n.size();i++) {
		for (int j = 0; j < n[0].size(); j++) {
		    n[i][j].hcost = abs(i - epx) + abs(j - epy);
		
		}
	}
}

//first calculate surrounding area gcost by figuring out the movement cost and traversing the parents of each neighboring node
//if the previous fcost is lower than new fcost then it keeps the old fcost and the previous parent
void Astar(std::vector<std::vector<Node>>&n,int &spx,int &spy, bool fmg,int epx, int epy) {
	n[spx][spy].ex = 1;
	int gcostt=0;
	//int hcostt = 0;
	Node temp;
	for (int i = spx - 1 == -1 ? spx : spx - 1; i < spx + 2; i++) {
		for (int j = spy - 1 == -1 ? spy : spy - 1; j < spy + 2; j++) {
			if (i == n.size() || j == n[i].size() || n[i][j].wall || n[i][j].ex) {
				continue;
				//break;
			}
			n[i][j].hex = 1;
			temp = n[spx][spy];
			gcostt += temp.gcost;
			while (temp.parent != NULL) {
				temp = *temp.parent;
				gcostt += temp.gcost;
			}

			gcostt += abs(spx - i) == 1 && abs(spy - j) == 1 ? 14 : 10;


			if (n[i][j].gcost != 0) {
				if (n[i][j].gcost > gcostt) {
					n[i][j].gcost = gcostt;
					n[i][j].parent = &n[spx][spy];

				}
				
			}
			else {
				n[i][j].gcost = n[spx][spy].gcost + (abs(spx - i) == 1 && abs(spy - j) == 1 ? 14 : 10);
				if (alt != 1) {
					n[i][j].hcost = std::min(abs(epx - i), abs(epy - j)) * 14 + (std::max(abs(epx - i), abs(epy - j)) - std::min(abs(epx - i), abs(epy - j))) * 10;
					if (alt == 3) {
							n[i][j].hcost += abs(spx - i) == 1 && abs(spy - j) == 1 ? 18 : 10;
					}
				}
				n[i][j].parent = &n[spx][spy];
			}






		n[i][j].fcost = n[i][j].hcost + n[i][j].gcost;

			gcostt = 0;
		//	hcostt=0;
		}
	}


	unsigned int lowest = -1;
	int tx=-1,ty=-1;





	//could implement a priority queue here so im not wasting time checking every node and will already have the lowest node in the queue


	//finds lowest fcost
	for (int i = 0; i < n.size(); i++) {
		for (int j = 0; j < n[i].size(); j++) {
				if (n[i][j].end && (abs(i-spx) <=1 && abs(j - spy) <= 1)) {
					tx = i;
					ty = j;
					n[tx][ty].parent = &n[spx][spy];
					done = 1;
					spx = tx;
					spy = ty;
					return;
				}
			if (n[i][j].hex && !n[i][j].ex) {
				if (n[i][j].fcost < lowest) {
					lowest = n[i][j].fcost;
					tx = i;
					ty = j;
				}
			}
		}
	}

	if (tx == -1 && ty == -1) {
		//just determines whether to print no solution or not, wont if it was called from mazegen function.
		if (!fmg) {
			std::cout << "No solution\n";
			fmg = 1;
		}
		nosol = 1;
		return;
	}

	spx = tx;
	spy = ty;
}
/*
This function doesnt use any specific maze generation algorithm but what i did was basically place walls around the edge
the start and end node in the corners of the middle grid and then placed walls at every other node then randomly
placed walls above,below,left,and right of that node, it works well enough
*/
void mazegen(std::vector<std::vector<Node>>& n) {
	srand(time(0));
	int spx=0, spy=0;


	n = base;
	std::vector<std::vector<Node>>temp;
	

	while (!done) {

		if (alt == 0) {
			chc(n, n.size() - 2, n[0].size() - 2);
		}

	for (int i = 0; i < n.size(); i++) {
			for (int j = 0; j < n[i].size(); j++) {
				n[i][j].wall = 0;
				n[i][j].start = 0;
				n[i][j].end = 0;
				n[i][j].path = 0;
				if (i == n.size() - 1 || i == 0 || j == 0 || j == n[i].size() - 1) {
					n[i][j].wall = 1;
				}
			}
		}


		spx = 1;
		spy = 1;
		n[1][1].start = 1;
		n[n.size() - 2][n[0].size() - 2].end = 1;



		for (int i = 2; i < n.size() - 2; i++) {
			for (int j = 2; j < n[i].size() -2; j++) {

				if (!(j % 2) && !(i % 2)) {
					n[i][j].wall = 1;
					if (rand() % 4 == 1) {
						n[i + 1][j].wall = 1;
					}
					if (rand() % 4 == 1) {
						n[i - 1][j].wall = 1;
					}
					if (rand() % 4 == 1) {
						n[i][j + 1].wall = 1;
					}
					if (rand() % 4 == 1) {
						n[i][j - 1].wall = 1;
					}
				}



			}
		}


		if (!(n.size()%2)) {
			for (int j = 0; j < n[0].size()-2; j++) {
				if (!(j%2) && rand() % 2 == 1) {
					n[n.size()-2][j].wall = 1;
				}
			}
		}

		if (!(n[0].size() % 2)) {
			for (int i = 0; i< n.size() - 2; i++) {
				if (!(i % 2) && rand() % 2 == 1) {
					n[i][n[0].size()-2].wall = 1;
				}
			}
		}


		temp = n;

		while (!done && !nosol) {
			Astar(temp,spx,spy,1, n.size() - 2, n[0].size() - 2);

		}


		nosol = 0;

	}

	//once a solution has been found then i reset the exploration and make done 0 again, which is why you are able to skip the animation
	for (int i = 0; i < n.size(); i++) {
		for (int j  = 0; j < n[i].size(); j++) {
			//n[i][j].parent = NULL;
			n[i][j].ex = 0;
			n[i][j].hex = 0;

		}
	}
	done = 0;

	

}




int main() {

	int usert=0;
	int rows, columns;
	std::cout << "Enter size of grid: (example 9x9)\n";
	std::cin >> rows;
	std::cin.ignore();
	std::cin >> columns;
	std::cout << "Do you want to use \n(1)Manhattan Distance?(slower but produces smoother path)\nor\n(2)alternate method?(faster but takes more diagonals)\nor\n(3)bit of both(not as fast as #2 but produces a smooth path, this is experimental)\n";
	std::cin >> alt;
	std::cout << "Enter speed of algorithm in ms, recommended to use a larger value for smaller grid and smaller value for larger grid.\n";
	std::cin >> usert;

	system("CLS");

	std::cout << "===========Controls===========\n";
	std::cout << "Press 'r' to generate a new maze\n";
	std::cout << "Press 'c' to clear the map\n";
	std::cout << "Press 'w' to set the starting node\n";
	std::cout << "Press 'e' to set the end node\n";
	std::cout << "Left Click to place a wall\n";
	std::cout << "Right Click to remove a wall\n\n";
	std::cout << "Press 'Spacebar' lock everything in place and begin solving press again to pause\n";
	std::cout << "Left Click to skip animation and show solution\n";




	//bool to see if text should be shown
	bool displayt= rows* columns < 250;


	std::vector<std::vector<Node>> n(rows);

	Node* temp = NULL;
	//fill array
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			n[i].emplace_back();
		}
	}
	//set filled array with empty attributes to base
	base = n;
	//run maze gen
	mazegen(n);

	//keep track of end position and start position
	int epx = rows - 2, epy = columns - 2;
	int spx=1, spy=1;


	//bool to determine if algorithm has started
	bool h=0;


	//-------------------------------------------------------------//
	//-------------------------GRAPHICS----------------------------//
	//-------------------------------------------------------------//

	sf::Event ev;
	sf::RenderWindow window(sf::VideoMode(800, 800), "window");

	//rectangle
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f((double)window.getSize().x / columns, (double)window.getSize().y / rows));
	rect.setFillColor(sf::Color::White);
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Black);

	//fonts and text
	sf::Font f;
	f.loadFromFile("Fonts\\times.ttf");
	sf::Text t;
	t.setFont(f);
	t.setFillColor(sf::Color::Black);

	// 80x=12

	t.setCharacterSize(16);
	t.setCharacterSize(rect.getSize().x * 0.2);


	//mouse position
	int mx = 0, my = 0;

	
	//Program loop
	while (window.isOpen()) {
		//keep track of mouse position
		mx = sf::Mouse::getPosition(window).y / rect.getSize().y;
		my = sf::Mouse::getPosition(window).x / rect.getSize().x;
		

		if (done) {
			temp = &n[spx][spy];
			while ((*temp).parent != NULL) {
				temp = (*temp).parent;
				(*temp).path = 1;
			}
			done = 1;
		}



		//event loop
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}


			//mouse events		

		//	if (ev.type == sf::Event::MouseButtonReleased) {
			//	if (ev.key.code == 0) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (!h && mx > -1 && my > -1 && mx < rows && my < columns) {
						n[mx][my].wall = 1;
					}
					else {
						if (mx > -1 && my > -1 && mx < rows && my < columns && !n[spx][spy].end){
							while (!done && !nosol) {
							Astar(n, spx, spy,0,epx,epy);
							
							}
						}
						
					}

				}

		//	}
	

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				if (mx > -1 && my > -1 && mx < rows && my < columns) {
					if (n[mx][my].wall) {
						n[mx][my].wall = 0;
					}
					else if (n[mx][my].start) {
						n[mx][my].start = 0;
						spx = -1;
						spy = -1;
					}
					else if (n[mx][my].end) {
						n[mx][my].end = 0;
						epx = -1;
						epy = -1;
					}

				}

			}


			//keybaord events
			if (ev.type == sf::Event::KeyReleased) {
				//start algorithm, space bar
				if (ev.key.code == sf::Keyboard::Space) {
					//std::cout << "Spacebar\n";
					if (!h && spx != -1 && epx !=-1) {
						//std::cout << "run\n";
						h = 1;
						n[spx][spy].ex = 1;

						if (alt == 1) {
							chc(n,epx,epy);
						}

						//Astar(n, spx, spy, 0, epx, epy);
					}
					else {
						if (spx == -1 && epx == -1) {
							std::cout << "missing both start and end points.\n";
						}
						else if (spx == -1) {
							std::cout << "missing start point.\n";
						}
						else if (epx == -1) {
							std::cout << "missing end point.\n";

						}
						else {
							h=0;
						}
					}
				}

				//starting position
				if (ev.key.code == sf::Keyboard::W && !h) {
					if (mx > -1 && my > -1 && mx < rows && my < columns && (mx != epx || my != epy)) {
						//if its already down
						if (spx != -1 && spy != -1) {
							n[spx][spy].start = 0;

						}
						spx = mx;
						spy = my;
						n[spx][spy].start = 1;
					}

				}
				//ending position
				if (ev.key.code == sf::Keyboard::E && !h) {
					if (mx > -1 && my > -1 && mx < rows && my < columns && (mx!=spx || my!=spy)) {
						//if its already down
						if (epx != -1 && epy != -1) {
							n[epx][epy].end = 0;

						}
						epx = mx;
						epy = my;
						n[epx][epy].end = 1;
					}

				}
				//clear the grid
				if (ev.key.code == sf::Keyboard::C) {
					n = base;
					h = 0;
					spx = -1;
					spy = -1;
					epx = -1;
					epy = -1;
					done = 0;
					nosol = 0;
				}

				if ( ev.key.code == sf::Keyboard::R) {
					spx = 1;
					spy = 1;
					epx = rows - 2;
					epy = columns - 2;
					done = 0;
					h = 0;
					nosol = 0;
						mazegen(n);
				}
			}
		}


		//===========drawing===========
		window.clear(sf::Color::Black);
		rect.setPosition(0, 0);
		t.setPosition(0, 0);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {

				//node drawing
				if (n[i][j].wall) {
					rect.setFillColor(sf::Color(16, 16, 16));
				}
				else if (n[i][j].start) {
					rect.setFillColor(sf::Color(sf::Color::Green));
				}
				else if (n[i][j].end) {
					rect.setFillColor(sf::Color(sf::Color::Red));
				}
				else if (n[i][j].path) {
					rect.setFillColor(sf::Color(sf::Color::Yellow));
				}
				else if (n[i][j].ex) {
					rect.setFillColor(sf::Color(sf::Color::Magenta));

				}
				else if (n[i][j].hex) {
					rect.setFillColor(sf::Color(sf::Color(64, 64, 64)));

			}
				else {
					rect.setFillColor(sf::Color(sf::Color::White));

				}

				window.draw(rect);
				rect.move(rect.getSize().x, 0);
				//





				//textdrawing
				if (displayt && (n[i][j].hex || n[i][j].ex))
				{
					//hcost
					t.setString(std::to_string(n[i][j].hcost));
					t.setPosition(rect.getPosition().x - rect.getSize().x + 6, rect.getPosition().y);
					window.draw(t);
					//gcost
					t.setString(std::to_string(n[i][j].gcost));
					t.setPosition(rect.getPosition().x - 20, t.getPosition().y);
					window.draw(t);
					//fcost
					t.setString(std::to_string(n[i][j].fcost));
					t.setPosition(rect.getPosition().x - rect.getSize().x / 2 -8 , t.getPosition().y + rect.getSize().y / 2 -8);
					window.draw(t);
				}


			}
			rect.setPosition(0, rect.getPosition().y + rect.getSize().y);
			t.setPosition(0, rect.getPosition().y);
		}



		window.display();
		//=============================
	
		if (!done && h && !nosol) {
			Astar(n, spx, spy, 0, epx, epy);
		sf::sleep(sf::milliseconds(usert));
		}

	}
	return 0;
}