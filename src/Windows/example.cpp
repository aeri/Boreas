/*
 * Copyright (c) 2020 Naval Alcalá
 * Copyright (c) 2020 Rubén Rodríguez
 *
 * This file is part of Boreas.
 * Boreas is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Boreas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Boreas.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "LindaDriver.hpp"

#include <iostream>

using namespace std;


int main (){
	
	LD windriver = LD("localhost", "11777");
	
	Tuple t("A","B","C","1","2","3");
	Tuple t2("A","?X","C","?Y","2","3");
	
	bool found;
	
	windriver.readNoteX(t, found );

	windriver.postNote(t);
	windriver.readNote(t2);
	
	windriver.removeNote(t);
		
	windriver.stop();
	
	return 0;
}
