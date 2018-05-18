#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;

#define MAX 1000

class contact {
public:
	char* fname, *lname;
	long long int phn_num;

};

contact ctcs[MAX];
int contact_count = 0;
class directory {
public:

	void create(string filePath) {
		ifstream myfile(filePath.c_str());
		char *contactline = new char[100];
		char *phn_num = new char[100];
		if (myfile.is_open()) {

			while (myfile.getline(contactline, ' ') && !myfile.eof()) {
				if (contactline != NULL && strcasecmp(contactline, "") != 0) {
					contact ctc;
					strtok(contactline, " ,");
					ctc.fname = strtok(NULL, " ,");
					ctc.lname = strtok(NULL, " ,");
					phn_num = strtok(NULL, " ,");
					removeAll(phn_num, ')');
					removeAll(phn_num, '(');
					removeAll(phn_num, '-');
					removeAll(phn_num, ' ');
					ctc.phn_num = atoll(phn_num);
					phn_num = new char[100];
					contactline = new char[100];
					ctcs[contact_count] = ctc;
					contact_count++;
				}
			}
			myfile.close();

		}

	}
// binary search with last name and first name
	int binarySearch(contact contacts[], int l, int r, char *lname,
			char *fname) {
		if (r >= l) {
			int mid = l + (r - l) / 2;

			if (strcasecmp(contacts[mid].lname, lname) == 0
					&& strcasecmp(contacts[mid].fname, fname) == 0)
				return mid;

			if ((strcasecmp(contacts[mid].lname, lname) > 0)
					|| ((strcasecmp(contacts[mid].lname, lname) == 0)
							&& (strcasecmp(contacts[mid].fname, fname) > 0)))
				return binarySearch(contacts, l, mid - 1, lname, fname);

			return binarySearch(contacts, mid + 1, r, lname, fname);
		}

		return -1;
	}

// binary search with last name, first name and phone number (for checking before inserting)
	int binarySearchithph(contact contacts[], int l, int r, contact contact) {
		if (r >= l) {
			int mid = l + (r - l) / 2;

			if (strcasecmp(contacts[mid].lname, contact.lname) == 0
					&& (strcasecmp(contacts[mid].fname, contact.fname) == 0)
					&& contacts[mid].phn_num == contact.phn_num)
				return mid;

			if ((strcasecmp(contacts[mid].lname, contact.lname) > 0)
					|| ((strcasecmp(contacts[mid].lname, contact.lname) == 0)
							&& (strcasecmp(contacts[mid].fname, contact.fname) > 0))
					|| ((strcasecmp(contacts[mid].lname, contact.lname) == 0)
							&& (strcasecmp(contacts[mid].fname, contact.fname) == 0)
							&& contacts[mid].phn_num > contact.phn_num))
				return binarySearchithph(contacts, l, mid - 1, contact);

			return binarySearchithph(contacts, mid + 1, r, contact);
		}

		return -1;
	}

	void searchDirectory(char* name) {
		char *fname = strtok(name, " ");
		char *lname = strtok(NULL, " ");
		int result = binarySearch(ctcs, 0, contact_count - 1, lname, fname);
		if (result == -1) {
			cout << "contact not found " << endl;
		} else {

			char *lname = (char*) "";
			char *fname = (char*) "";
			for (int i = result; i >= 0; i--) {
				if (i == result) {
					lname = ctcs[i].lname;
					fname = ctcs[i].fname;
				} else {
					if (strcasecmp(lname, ctcs[i].lname) != 0
							|| strcasecmp(fname, ctcs[i].fname) != 0) {
						break;
					} else {
						lname = ctcs[i].lname;
						fname = ctcs[i].fname;
						result = i;
					}
				}

			}
			for (int i = result; i < contact_count; i++) {
				if (i == result) {
					lname = ctcs[i].lname;
					fname = ctcs[i].fname;
					cout << ctcs[i].fname << " " << ctcs[i].lname << ", "
							<< ctcs[i].phn_num << endl;
				} else {

					if (strcasecmp(lname, ctcs[i].lname) != 0
							|| strcasecmp(fname, ctcs[i].fname) != 0) {
						break;
					} else {
						lname = ctcs[i].lname;
						fname = ctcs[i].fname;
						cout << ctcs[i].fname << " " << ctcs[i].lname << ", "
								<< ctcs[i].phn_num << endl;
					}
				}

			}
		}
	}

	// inserting elment in sorted array
	int insertInSortedArray(contact contacts[], contact contact, int n) {
		int i;
		for (i = n - 1; i >= 0; i--) {
			if ((strcasecmp(contacts[i].lname, contact.lname) > 0)
					|| ((strcasecmp(contacts[i].lname, contact.lname) == 0)
							&& (strcasecmp(contacts[i].fname, contact.fname) > 0))
					|| ((strcasecmp(contacts[i].lname, contact.lname) == 0)
							&& (strcasecmp(contacts[i].fname, contact.fname) == 0)
							&& (contacts[i].phn_num > contact.phn_num))) {
				contacts[i + 1] = contacts[i];
			} else {
				break;
			}
		}

		contacts[i + 1] = contact;

		return (n + 1);
	}
	void insertEntry(char* name, char* phone_number) {
		contact ctc;
		ctc.fname = strtok(name, " ");
		ctc.lname = strtok(NULL, " ");
		removeAll(phone_number, ')');
		removeAll(phone_number, '(');
		removeAll(phone_number, '-');
		removeAll(phone_number, ' ');
		ctc.phn_num = atoll(phone_number);
		int result = binarySearchithph(ctcs, 0, contact_count - 1, ctc);
		if (result == -1) {
			contact_count = insertInSortedArray(ctcs, ctc, contact_count);
			cout << "entry inserted successfully in the directory" << endl;
		} else {
			cout << "entry already exist in the directory" << endl;
		}
	}

// delete entry from directory
	void deleteEntry(char* name) {
		char *fname = strtok(name, " ");
		char *lname = strtok(NULL, " ");
		int result = binarySearch(ctcs, 0, contact_count - 1, lname, fname);

		if (result == -1) {
			cout << "unable to delete as entry not found " << endl;
		} else {

			char *lname = (char*) "";
			char *fname = (char*) "";
			for (int i = result; i < contact_count; i++) {
				if (i == result) {
					lname = ctcs[i].lname;
					fname = ctcs[i].fname;
				} else {
					if (strcasecmp(lname, ctcs[i].lname) != 0
							|| strcasecmp(fname, ctcs[i].fname) != 0) {
						break;
					} else {
						lname = ctcs[i].lname;
						fname = ctcs[i].fname;
						result = i;
					}
				}

			}

			for (int i = result; i < contact_count; i++)
				ctcs[i] = ctcs[i + 1];
			contact_count--;
			cout << "entry deleted successfully" << endl;
		}
	}

	void showDirectory() {
		for (int i = 0; i < contact_count; i++) {
			cout << (i + 1) << "\t" << ctcs[i].fname << "\t" << ctcs[i].lname
					<< ",\t" << ctcs[i].phn_num << endl;
		}
		return;
	}

	void swap(contact *con1, contact *con2) {
		contact temp;
		temp = *con1;
		*con1 = *con2;
		*con2 = temp;
	}
	int partition(contact contacts[], int left, int right) {
		int pivot, x, y;
		x = left - 1;
		pivot = right;
		for (y = left; y < right; y++) {
			if (contacts[y].phn_num <= contacts[pivot].phn_num) {
				x++;
				swap(&contacts[x], &contacts[y]);
			}
		}
		swap(&contacts[x + 1], &contacts[right]);

		return (x + 1);
	}
	int pivot_index(contact contacts[], int left, int right) {
		int pivot;
		int random;
		int temp;
		srand(time(NULL));
		random = rand();
		pivot = left + random % (right - left);
		swap(&contacts[pivot], &contacts[right]);

		return partition(contacts, left, right);
	}
	void quickSort(contact contacts[], int left, int right) {
		int piv_index;
		if (left < right) {
			piv_index = pivot_index(contacts, left, right);

			quickSort(contacts, left, piv_index - 1);
			quickSort(contacts, piv_index + 1, right);
		}
	}

	void merge(contact contacts[], int left, int mid, int right,
			bool isLastName) {
		int i = 0, j = 0, k = 0;
		int a = mid - left + 1;
		int b = right - mid;
		contact L[a];
		contact R[b];

		for (i = 0; i < a; i++) {
			L[i] = contacts[left + i];
		}
		for (j = 0; j < b; j++) {
			R[j] = contacts[mid + 1 + j];
		}

		i = 0;
		j = 0;
		k = left;

		while (i < a && j < b) {
			if (isLastName) {
				if (strcasecmp(L[i].lname, R[j].lname) <= 0) {
					contacts[k] = L[i];
					i++;
				} else {
					contacts[k] = R[j];
					j++;
				}
			} else {
				if (strcasecmp(L[i].fname, R[j].fname) <= 0) {
					contacts[k] = L[i];
					i++;
				} else {
					contacts[k] = R[j];
					j++;
				}
			}
			k++;
		}
		while (i < a) {
			contacts[k] = L[i];
			i++;
			k++;
		}
		while (j < b) {
			contacts[k] = R[j];
			j++;
			k++;
		}

		return;
	}
	void mergeSort(contact contacts[], int left, int right, bool islname) {
		if (left < right) {
			int mid = (left + right) / 2;
			mergeSort(contacts, left, mid, islname);
			mergeSort(contacts, mid + 1, right, islname);
			merge(contacts, left, mid, right, islname);
		}
		return;
	}

	void sortDirectory() {
		directory dir;
		mergeSort(ctcs, 0, contact_count - 1, true);

		int firstIndex = 0;
		int lastIndex = 0;
		char* lname = (char*) "";
		for (int i = 0; i < contact_count; i++) {

			if (i == 0) {
				lname = ctcs[i].lname;
			} else {

				if (strcasecmp(lname, ctcs[i].lname) != 0) {
					mergeSort(ctcs, firstIndex, lastIndex, false);
					firstIndex = i;

				}
			}
			lastIndex = i;
			lname = ctcs[i].lname;
			if (i == contact_count - 1) {
				mergeSort(ctcs, firstIndex, lastIndex, false);
			}
		}
		firstIndex = 0;
		lastIndex = 0;
		lname = (char*) "";
		char *fname = (char*) "";
		for (int i = 0; i < contact_count; i++) {
			if (i == 0) {
				lname = ctcs[i].lname;
				fname = ctcs[i].fname;
			} else {

				if (strcasecmp(lname, ctcs[i].lname) != 0
						|| strcasecmp(fname, ctcs[i].fname) != 0) {
					quickSort(ctcs, firstIndex, lastIndex);
					firstIndex = i;
				}
			}
			lastIndex = i;
			lname = ctcs[i].lname;
			fname = ctcs[i].fname;
			if (i == contact_count - 1) {
				quickSort(ctcs, firstIndex, lastIndex);
			}
		}
		return;
	}
	void removeAll(char s[], char x) {
		int i, j;
		for (i = 0; s[i] != 0; ++i) {
			while (s[i] == x) {
				j = i;
				while (s[j] != 0) {
					s[j] = s[j + 1];
					++j;
				}
			}
		}
	}

};
int main(int argc, char* argv[]) {
	directory dir;
	string filePath = argv[1];
	char *name;
	char fname[100];
	char lname[100];
	char phn_number[100];
	dir.create(filePath);
	dir.sortDirectory();
	int options = 0;
	while (true) {
		cin.clear(); //clear bad input flag
		//cin.ignore();
		cout << "Please provide options:" << endl;
		cout << "Insert                (press 1)" << endl;
		cout << "Search                (press 2)" << endl;
		cout << "Delete                (press 3)" << endl;
		cout << "Show Directory        (press 4)" << endl;
		cout << "Exit                (press 5)" << endl;
		while (!(cin >> options)) {
			cin.clear(); //clear bad input flag
			cin.ignore(); //discard input
			cout << "Invalid input; please re-enter.\n";
		}
		if (options == 1) {
			cout << "Please provide contact name and phone number:" << endl
					<< endl;
			while (!(cin >> fname >> lname >> phn_number)) {
				cin.clear(); //clear bad input flag
				cin.ignore(); //discard input
				cout << "Invalid input; please re-enter.\n";
			};
			dir.removeAll(lname, ',');
			name = strcat(fname, " ");
			name = strcat(name, lname);
			dir.insertEntry(name, phn_number);
		} else if (options == 2) {
			cout << "Please provide contact name to search:" << endl << endl;
			while (!(cin >> fname >> lname)) {
				cin.clear(); //clear bad input flag
				cin.ignore(); //discard input
				cout << "Invalid input; please re-enter.\n";
			};
			name = strcat(fname, " ");
			name = strcat(name, lname);
			dir.searchDirectory(name);
		} else if (options == 3) {
			cout << "Please provide contact name to delete:" << endl << endl;
			while (!(cin >> fname >> lname)) {
				cin.clear(); //clear bad input flag
				cin.ignore(); //discard input
				cout << "Invalid input; please re-enter.\n";
			};
			name = strcat(fname, " ");
			name = strcat(name, lname);
			dir.deleteEntry(name);
		} else if (options == 4) {
			cout << "phone Directory :" << endl;
			dir.showDirectory();
		} else if (options == 5) {
			break;
		} /*else {
			cout << "Please provide valid option:" << endl;
			cin >> options;
		}*/
	}
}
