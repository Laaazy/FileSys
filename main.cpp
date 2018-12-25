#include<iostream>
#include"defines.h"
#include"operation.h"
#include<map>

using namespace std;

struct storage *disk = new storage;//磁盘变量
char PATH[NAMESIZE*DIRNUM] = "";//当前路径
char fileName[30];//当前打开的文件名
int opened = 0;//文件是否已经打开

int main() {
	map<string, int> command = {
		{"help",1},
		{"new",2},
		{ "sfs",3 },
		{ "exit",4 },
		{ "mkdir",5 },
		{ "rmdir",6 },
		{ "ls",7 },
		{ "cd",8 },
		{ "create",9 },
		{ "open",10 },
		{ "close",11 },
		{ "read",12 },
		{ "write",13 },
		{ "delete",14 },
		{ "rename",15 },
		{ "move",16 },
		{"cls",17},
	};

	operation operate;
	operate.help();

	string s;
	char arg1[NAMESIZE] = "";
	char arg2[NAMESIZE] = "";
	char content[BLOCKNUM*BLOCKSIZE]="";
	bool flag = true;
	int commandNum;
	while (flag)
	{
		cout << PATH << ">";
		cin >> s;
		if (command.count(s) > 0) {
			commandNum = command.find(s)->second;
		}
		else{
			commandNum = 0;
		}
		switch (commandNum)
		{
			case 1:
				operate.help();
				break;
			case 2:cin >> arg1;
				operate.newFS(arg1);
				break;
			case 3:cin >> arg1;
				operate.sfs(arg1);
				break;
			case 4:operate.exit();
				break;
			case 5:cin >> arg1;
				operate.mkdir(PATH,arg1);
				break;
			case 6:cin >> arg1;
				operate.rmdir(PATH, arg1);
				break;
			case 7:
				operate.ls(PATH);
				break;
			case 8:cin >> arg1;
				operate.cd(arg1);
				break;
			case 9:cin >> arg1;
				operate.create(PATH, arg1);
				break;
			case 10:cin >> arg1;
				operate.open(PATH, arg1);
				break;
			case 11:cin >> arg1;
				operate.close(PATH, arg1);
				break;
			case 12:cin >> arg1;
				operate.read(PATH, arg1);
				break;
			case 13:cin >> arg1;
				operate.write(PATH, arg1);
				break;
			case 14:cin >> arg1;
				operate.deleteFile(PATH, arg1);
				break;
			case 15:cin >> arg1;
				cin >> arg2;
				operate.rename(PATH, arg1, arg2);
				break;
			case 16:cin >> arg1;
				cin >> arg2;
				operate.move(PATH, arg1, arg2);
				break;
			case 17:system("cls");
				operate.help();
				break;
		default:
			cout << "命令有误，请重新输入" << endl;
			break;
		}
	}
	return 0;
}

//#include<stdio.h>
//#include<iostream>
//#include"defines.h"
//
//using namespace std;
//
//int main() {
//	printf("%d\n",	FSSIZE);
//	return 0;
//}
