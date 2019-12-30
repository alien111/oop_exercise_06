#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <tuple>
#include <list>

#include "rectangle.h"
#include "Stack.h"
#include "Allocator.h"
#include "Vector.h"
#include "Allocator.h"
#include <map>

void menu() {
	std::cout << "0 : EXIT\n";
	std::cout << "1 : FILL THE VECTOR\n";
	std::cout << "2 : GET ITEM CENTER BY INDEX\n";
	std::cout << "3 : GET AMOUNT OF OBJECTS WITH SQUARE LESS THAN...\n";
	std::cout << "4 : GO THROUGH VECTOR WITH ITERATOR AND SHOW EVERY STEP\n";
	std::cout << "5 : CHANGE OBJECT BY INDEX\n";
	std::cout << "6 : RESIZE VECTOR\n";
	std::cout << "> ";
}

int main() {
    std::map<int,int,std::less<>, Allocator<int,100000>> m;
    
    for (int i = 0; i < 10; ++i) {
		m[i] = i * i;
	}

	m.erase(1);
	m.erase(2);

	int cmd;

	std::cout << "Enter size of your vector : ";
	size_t size;
	std::cin >> size;

	Containers::Vector< rectangle< int > > vec;
	vec.Resize(size);

	while(true) {

		menu();
		std::cin >> cmd;

		if (cmd == 0) return 0;
		else if (cmd == 1) {

			for (int i = 0; i < vec.Size(); i++) {
				
				std::cout << "Element number " << i << '\n';
				std::cout << "Enter vertices : \n";
				rectangle<int> rect(std::cin);
				vec[i] = rect;

			}

		} else if (cmd == 2) {

			std::cout << "Enter index : ";
			int index;
			std::cin >> index;
			std::cout << vec[index].center();

		} else if (cmd == 3) {

			int res = 0;
			std::cout << "Enter your square : ";
			double square;
			std::cin >> square;

			int cmdcmd;
			std::cout << "Do you want to use std::count_if? : 1 - yes; 0 - no; : ";
			std::cin >> cmdcmd;

			if (cmdcmd == 1) res = std::count_if(vec.begin(), vec.end(), [&square](rectangle<int>& i) {return i.area() < square;});
			else {
				auto it = vec.begin();
				auto end = vec.end();

				while (it != end) {
					if ((*it).area() < square) res++;
					++it;
				}
			}

			std::cout << "Amount is " << res << '\n';

		} else if (cmd == 4) {

			int cmdcmd;
			std::cout << "Do you want to use std::for_each? : 1 - yes; 0 - no; : ";
			std::cin >> cmdcmd;

			if (cmdcmd == 1) std::for_each(vec.begin(), vec.end(), [](rectangle<int>& i) -> void{i.print();});
			else {
				auto it = vec.begin();
				auto end = vec.end();

				int n = 0; 

				while (it != end) {
					std::cout << "___OBJECT_" << n << "__\n";
					std::cout << *it;
					++it;
					n++;
				}

			}

		} else if (cmd == 5) {

			int index;
			std::cout << "Enter index : ";
			std::cin >> index;

			if (index < 0 || index >= vec.Size()) {
				
				std::cout << "Out of range.\n";
			
			} else {

				std::cout << "Enter vertices : \n";
				rectangle<int> rect(std::cin);
				vec[index] = rect;

			}

		} else if (cmd == 6) {

			int size;
			std::cin >> size;

			if (size < 0) {

				std::cout << "Can't resize to non positive numbers.\n";

			} else {

				vec.Resize(size);

			}

		}
	
	}

}