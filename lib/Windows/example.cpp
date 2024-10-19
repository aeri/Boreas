#include "LindaDriver.hpp"

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
