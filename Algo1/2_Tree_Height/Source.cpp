#include <iostream>

using namespace std;

int f_height(int root);

struct note
{
	int data;
	note* next;
};

class List
{
public:
	note* head;
	note* tail;

	List()
	{
		head = tail = NULL;
	}
	bool Is_empty()
	{
		return (head == NULL and tail == NULL);
	}
	void add_back(int child)
	{
		note* obj = new note();
		obj->data = child;
		obj->next = NULL;
		if (Is_empty())
			head = tail = obj;
		else
			tail->next = obj;
		tail = obj;
	}
	~List() {
		if (!Is_empty()) {
			note* tmp = head;
			while (tmp != NULL) {

			}
		}
	}
};

int* str_parent;
List** ptr_list;


int main()
{
	int num;
	int main_root = 0;

	cin >> num;
	str_parent = new int[num];

	for (int i = 0; i < num; i++)
	{
		cin >> str_parent[i];
	}

	ptr_list = new List * [num];

	for (int i = 0; i < num; i++)
		ptr_list[i] = new List();

	for (int i = 0; i < num; i++)
	{
		if (str_parent[i] != -1)
			ptr_list[str_parent[i]]->add_back(i);
		else
			main_root = i;
	}


	cout << f_height(main_root);


	return 0;
}

int f_height(int root)
{
	int height = 1;
	note* p = ptr_list[root]->head;
	while (p != NULL)
	{
		int tmp = f_height(p->data) + 1;
		if (height < tmp)
			height = tmp;
		p = p->next;
	}
	return height;
}




