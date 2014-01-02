#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

class wordobj
{
public:
	string word, type;
	vector <string> * def;

	wordobj(string w = "", string t = "", int size = 100)
	{
		word = w;
		type = t;
		def = new vector<string>(100);
	}
	~wordobj()
	{
		delete def;
		def = NULL;
	}
	void Printdef()
	{
		for(int i = 0; i < def->size(); i++)
		{
			if(def->at(i) != "")
				cout << def->at(i) << " ";
		}
		cout << "\n";
	}
};

template <typename t>
class Link
{
public:
	t * data;
	Link * next;

	Link(t * d, Link * n = NULL)
	{
		data = d;
		next = n;
	}
	~Link()
	{}
};

template <typename kittens>
class Hashtable
{
protected:
	Link<kittens> * * table;
	int num_entries, size;

	kittens * Get(string, int);
	void Insert(kittens*, int);
	void Delete(kittens*, int);           

public:
	Hashtable(int);
	~Hashtable();
	void Insert(kittens*);
	int Hashvalue(string);
	kittens* Get(string);
	void Delete(kittens*);
};

template <typename kitten>
Hashtable::Hashtable(int s = 50000)
{
	size = s;
	num_entries = 0;
	table = new Link<wordobj> * [size];
	for(int i = 0; i < size; i++)
		table[i] = NULL;
}

template <typename kitten>
Hashtable::~Hashtable()
{
	delete [] table;
	table = NULL;
}

template <typename kitten>
void Hashtable::Delete(wordobj * input, int hvalue)
{
	if(table[hvalue]->data->word == input->word)
		table[hvalue] = NULL;
	else
	{
		Link<wordobj> * temp = table[hvalue];
		while(temp != NULL)
		{
			if(temp->data->word == input->word)
				table[hvalue] = NULL;
			else
				temp = temp->next;
		}
	}
}

template <typename kitten>
void Hashtable::Delete(wordobj * input)
{Delete(input, Hashvalue(input->word));}

template <typename kitten>
int Hashtable::Hashvalue(string input)
{
	const int init = 21512713, mult = 96169, emergency = 876127;
	int value = init;
	for(int i = 0; i < input.length(); i++)
		value = value * mult + input[i];
	if(value < 0)
		value = abs(value);
	if(value < 0)
		value = emergency;
	return value % size;
}

template <typename kitten>
void Hashtable::Insert(wordobj * input, int hvalue)
{
	if(table[hvalue] == NULL)
	{
		table[hvalue] = new Link<wordobj>(input);
		num_entries++;
	}
	else
		table[hvalue] = new Link<wordobj>(input, table[hvalue]);
}

template <typename kitten>
void Hashtable::Insert(wordobj * input)
{Insert(input, Hashvalue(input->word));}

template <typename kitten>
wordobj * Hashtable::Get(string input, int hvalue)
{
	if(table[hvalue] == NULL)
	{
		cout << "Word does not exist in dictionary\n";
		return NULL;
	}
	if(table[hvalue]->data->word == input)
		return table[hvalue]->data;
	else
	{
		Link<wordobj> * temp = table[hvalue];
		while(temp != NULL)
		{
			if(temp->data->word == input)
				return temp->data;
			else
				temp = temp->next;
		}
		cout << "Word not found\n";
		return NULL;
	}
}

template <typename kitten>
wordobj* Hashtable::Get(string input)
{return Get(input, Hashvalue(input));}

class Player
{
protected:
	string name, gender;
	int health, max_carryweight, curr_carryweight, invlen;
	Link<wordobj> * inventory;
	Room * curr;

public:
	Player()
	{
		name = "";
		gender = "";
		health = 100; //max health is 100
		max_carryweight = 200; //max carryweight is 200
		curr_carryweight = 0;
		invlen = 0; //length of inventory list
		inventory = NULL;
		Room * curr = NULL;
	}
	~Player()
	{
		delete inventory;
		inventory = NULL;
	}

	void setname(string n)
	{name = n;}
	string getname()
	{return name;}
	void setroom(Room * c)
	{curr = c;}
	Room * getroom()
	{return curr;}
	void setgender(string g)
	{gender = g;}
	string getgender()
	{return gender;}
	void setcurrweght(int w)
	{curr_carryweight = w;}
	void setmaxweight(int w)
	{max_carryweight = w;}
	void add(wordobj * item) //add item to inventory
	{
		inventory = new Link<wordobj>(item, inventory);
		invlen++;
	}
	Link<wordobj> * getinv() //get first link of inventory Linked list
	{return inventory;}
	void remove(string item)
	{
		Link<wordobj> * temp = inventory;
		while(temp != NULL)
		{
			if(temp->next->data->word == item)
			{
				Link<wordobj> * hold = temp->next;
				temp->next = hold->next;
				delete hold;
				invlen--;
			}
		}
	}
	void Printinv()
	{
		Link<wordobj> * temp = inventory;
		while(temp != NULL)
		{
			cout << temp->data->word << "\n";
			temp = temp->next;
		}
	}
	void Printplayer()
	{
		cout << name << "\n";
		cout << gender << "\n";
		cout << "Health " << health << "\n";
		cout << "Burden " << curr_carryweight << "\n";
		Printinv();
	}
};

class Room
{
protected:
	string name;
	vector <string> * directions;
	vector <string> * description;
	Link <wordobj> * items;
	Link <Player> * occupants;

public:
	Room()
	{
		name = "";
		directions = new vector<string>(8);
		description = new vector<string>(20);
		items = NULL;
		occupants = NULL;
	}
	~Room()
	{
		delete directions;
		directions = NULL;
		delete description;
		description = NULL;
		delete items;
		items = NULL;
		delete occupants;
		occupants = NULL;
	}
	void setname(string n)
	{name = n;}
	string getname()
	{return name;}
	void setdir(string inf)
	{
		int choose = NULL;
		while(inf != "")
		{
		string temp = split(inf);
		if(temp == "N")
		{choose = 0;}
		else if(temp == "NE")
		{choose = 1;}
		else if(temp == "E")
		{choose = 2;}
		else if(temp == "SE")
		{choose = 3;}
		else if(temp == "S")
		{choose = 4;}
		else if(temp == "SW")
		{choose = 5;}
		else if(temp == "W")
		{choose = 6;}
		else if(temp == "NW")
		{choose = 7;}
		directions->at(choose) = split(inf);
		}
	}
	void setdesc(string wrd)
	{description->push_back(wrd);}
	void additems(wordobj * itm)
	{items = new Link<wordobj>(itm, items);}
	Link<wordobj> * getitems()
	{return items;}
	void rmitem(string itm)  //remove item
	{
		Link<wordobj> * temp = items;
		while(temp != NULL)
		{
			if(temp->next->data->word == itm)
			{
				Link<wordobj> * hold = temp->next;
				temp->next = hold->next;
				delete hold;
				hold = NULL;
			}
		}
	}
	void setocc(Player * occ)
	{occupants = new Link<Player>(occ, occupants);}
	void rmocc(string occ)//remove occupant
	{
		Link<wordobj> * temp = items;
		while(temp != NULL)
		{
			if(temp->next->data->word == occ)
			{
				Link<wordobj> * hold = temp->next;
				temp->next = hold->next;
				delete hold;
				hold = NULL;
			}
		}
	}
	void Rmprint()
	{
		cout << name << "\n";
		for(int i = 0; i < description->size(); i++)
		{
			if(description->at(i) != "")
				cout << description->at(i) << " ";
		}
	}
};

void readin_words(Hashtable<wordobj> * index)
{
	wordobj * hold;
	string tempword, temptype;
	int hvalue;
	ifstream dictionary("dictionary.txt");
	while(!dictionary.eof())
	{
		if(dictionary.fail())
		{
			cout << "Error reading file\n";
			exit(1);
		}
		dictionary >> tempword >> temptype;
		hold = new wordobj(tempword, temptype);
		hvalue = index->Hashvalue(hold->word);
		index->Insert(hold);
	}
}

void readin_def(Hashtable<wordobj> * index)  
{
	fstream defs("definitions.txt");
	string identifier, nextword;
	while(true)
	{
		defs >> identifier;
		if(identifier == "|")  // | is end of file character 
			break;
		wordobj * temp = index->Get(identifier);
		while(defs.peek() != '\n')
		{
			defs >> nextword;
			temp->def->push_back(nextword);
		}
	}
}

string case_insensitive(string &input)
{
	for(int i = 0; i < input.size(); i++)
	{
		if((i == 0 || input[i-1] == 32) && input[i] > 96)  //32 is the ascii value for a space, 97 is for a.
			input[i] = input[i]-32;    //it also happens to be the 'distance' between capital and lowercase letters.
	}
	return input;
}

string split(string &input)
{
	string temp;
	for(int i = 0; i < input.size(); i++)
	{
		if(input[i] == 32) //32 is ascii code for space
		{
			temp = input.substr(0,i);
			input = input.substr(i+1, input.size());
			return temp;
		}
	}
	temp = input;
	input = "";
	return temp;
}

void Makeplayer(Player * A, string info, Hashtable<wordobj> * dictionary)
{
	string n = split(info);
	string g = split(info);
	A->setname(n);
	A->setgender(g);
	string temp;
	while(info != "")
	{
		temp = split(info);
		A->add(dictionary->Get(temp));
	}
	A->setcurrweght(atoi(split(info).c_str()));
}


void readin_room(Hashtable<Room> * map, Hashtable<wordobj> * dictionary)
{
	Room * curr = new Room();
	ifstream rooms("rooms.txt");
	string temp;
	int count = 0;
	rooms >>temp;
	curr->setname(temp);
	while(true)
	{
		if(count == 0) //0 is description for room
		{
			rooms >> temp;
			if(temp == "|" || temp == "")  // | is end of file character 
				break;
			while(rooms.peek() != '\n')
			{
				curr->setdesc(temp);
				rooms >> temp;
			}
			curr->setdesc(temp);
		}
		else if(count == 1) //items in room
		{
			rooms >> temp;
			if(temp == "|")
				break;
			while(rooms.peek() != '\n')
			{
				wordobj * A = dictionary->Get(temp);
				curr->additems(A);
				rooms >> temp;
			}
		}
		else if(count == 2) //occupants -> split off into makePlayer function
		{
			rooms.ignore();
			getline(rooms, temp);
			if(temp == "|")
				break;
			Player * NPC = new Player();
			Makeplayer(NPC, temp, dictionary);
			curr->setocc(NPC);
		}
		else if(count == 3)
		{
			getline(rooms, temp);  //indices relate directly to compass directions
			curr->setdir(temp);    //  0 = N, 1 = NE, 2 = E...7 = NW
		}
		else
		{
			map->Insert(curr);   //store new room in hash table
		}
		count++;
	}
}

void verbcase(string verb, wordobj * noun, Player * PC)
{
	if(verb == "Look")
	{
		if(noun->word == "")
			PC->getroom()->Rmprint();
		else
		{
			noun->Printdef();
		}
	}
	else if(verb == "Get") //if there is a valid noun, move related object to player inventory.  Else, print "What do you want to get?"
	{
		if(noun->word == "")
		{
			cout << "What do you want to get?\n";
			return;
		}
		else
		{
			Link<wordobj> * temp = PC->getroom()->getitems();
			while(temp != NULL)
			{
				if(temp->data->word == noun->word)
				{
					PC->add(noun);
					PC->getroom()->rmitem(noun->word);
					return;
				}
				temp = temp->next;
			}
			cout << "You can't get that\n";
		}
	}	

	else if(verb == "I" || verb == "Inv" || verb == "Inventory")
	{PC->Printinv();}
	else if(verb == "North" || verb == "N")
		cout << "North";
		//move north.  If there is no North, print "You can't go there"
	else if(verb == "South" || verb == "S")
		cout << "South";
		//move south.  If there is no south, print "You can't go there"
	else if(verb == "East" || verb == "E")
		cout << "East";
		//move east
	else if(verb == "West" || verb == "W")
		cout << "East";
		//move west
}

void parse(string input, Hashtable<wordobj> * dictionary)  //userinput-related words(words, etc) should all begin with capital letters, system-related words (type, etc) 
{						  //begin with lowercase letters.						
	case_insensitive(input);
	string tempword = split(input);
	wordobj * inf = dictionary->Get(tempword);
	
	if(inf->type == "verb")
	{
		wordobj * noun = dictionary->Get(input);	
		verbcase(inf->word, noun);
	}
	else if(inf->type == "noun")
		inf->Printdef();  //prints the description of the object that is stored in the word object.
}

int main()
{
	Hashtable<wordobj> * index = new Hashtable<wordobj>(5000);
	Hashtable<Room> * map = new Hashtable<Room>(100);
	readin_words(index);
	readin_def(index);
	readin_room(map, index);
	string userin;
	/*cout << "Enter a word\n";
	getline(cin, userin);
	parse(userin, index);*/
	system("Pause");
}