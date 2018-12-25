#include"operation.h"

//返回当前路径最后一层文件夹的inode号
int operation::getInode(char *PATH) {
	if (PATH[0] != '/') {
		return -1;//路径非法
	}
	dir currentDir = disk->dirTree[disk->inodes[disk->dirTree[0].items[0].inodeNum].dirNumber];//根节点下目录
	int ans=0;
	char tpath[NAMESIZE*DIRNUM] = "";
	strcpy(tpath, PATH);
	char *fpath = strtok(tpath, "/");
	//得到根目录后的目录
	fpath = strtok(NULL, "/");
	//当前位于根目录
	if (fpath == NULL)
		return 0;
	int find = 0;
	//int match = 0;
	while (fpath !=NULL)
	{
		//match = 0;
		find = 0;
		for (int i = 0; i < DIRSIZE; i++) {
			if (!strncmp(fpath, currentDir.items[i].fileName, strlen(fpath))) {
				find = 1;
				if (disk->inodes[currentDir.items[i].inodeNum].fileType == TYPEDIR) {
					ans = currentDir.items[i].inodeNum;
					currentDir = disk->dirTree[disk->inodes[ans].dirNumber];

					//match = 1;
					break;
				}
				else {
					return -1;//PATH应该是当前路径，不应该是文件
				}
			}
		}
		if (find == 0)
			return -1;//无对应文件夹
		fpath = strtok(NULL, "/");
	}
	return ans;//为根节点,0
}


STATUS operation::newFS(char FSName[]) {
		memset(disk, '\0', sizeof(struct storage));
		strcpy(disk->diskName, FSName);
		disk->dirTree[0].itemNumber = 1;//注册目录项
		strcpy(disk->dirTree[0].items[0].fileName, "C");//根节点名
		disk->dirTree[0].items[0].inodeNum = 0;//根节点的inode号
		disk->spBlock.inodeArray[0] = 1;//分配inode
		disk->inodes[0].fileType = TYPEDIR;
		disk->inodes[0].fileSize = 0;
		disk->inodes[0].dirNumber = 1;
		strcpy(PATH, "/C");
		cout << "创建内存中的文件系统成功" << endl;
		return SUCCESS;
}

//退出文件系统并保安至磁盘
STATUS operation::exit() {
	FILE *fp;
	if ((fp = fopen(disk->diskName, "w")) == NULL) {
		cout << "在磁盘建立文件系统失败" << endl;
		return ERR_FILE_FAIL;
	}

	if (fwrite(disk, sizeof(struct storage), 1, fp) != 1) {
		cout << "将文件系统保存至磁盘失败" << endl;
		return ERR_FILE_EXIST;
	}
	cout << "将文件系统保存至磁盘成功" << endl;
	fclose(fp);
	return SUCCESS;
}

//打开文件系统
STATUS operation::sfs(char name[]) {
	FILE *fp;
	strcpy(PATH, "/C");
	if ((fp = fopen(name, "r")) == NULL) {
		cout << "在磁盘打开文件系统失败" << endl;
		return ERR_FILE_FAIL;
	}

	if (fread(disk, sizeof(struct storage), 1, fp)) {
		fclose(fp);
		cout << "打开文件系统成功" << endl;
		return SUCCESS;
	}
	cout << "读取文件系统失败" << endl;
	return ERR_FILE_FAIL;
}

STATUS operation::help() {
	cout << "**********************************************************************************************************" << endl;
	cout << "*                             文件系统操作手册                                                            *" << endl;
	cout << "*                                                                                                        *" << endl;
	cout << "*                       1、help   --查看指令格式                                                          *" << endl;
	cout << "*                       2、new <name>  --创建名为name的文件系统并保存至磁盘                                *" << endl;
	cout << "*                       3、sfs <name>  --打开名为name的文件系统                                           *" << endl;
	cout << "*                       4、exit   --退出当前文件系统并保存至磁盘                                           *" << endl;
	cout << "*                       5、mkdir <dir>  --在当前路径下创建名为dir的文件夹                                  *" << endl;
	cout << "*                       6、rmdir <dir>  --创建当前路径下名为name的文件夹                                   *" << endl;
	cout << "*                       7、ls  --显示当前文件夹下所有文件/文件夹                                           *" << endl;
	cout << "*                       8、cd <..>|<dir>  --返回上级目录或者移动到指定路径下                               *" << endl;
	cout << "*                       9、create <name>  --在当前路径下创建名为name的文件                                 *" << endl;
	cout << "*                      10、open <name>  --打开当前目录下名为name的文件                                     *" << endl;
	cout << "*                      11、close <name> --关闭已打开的文件                                                *" << endl;
	cout << "*                      12、read  <name>   --读取打开的文件内容                                            *" << endl;
	cout << "*                      13、write <name> --向文件写入内容                                                  *" << endl;
	cout << "*                      14、delete <name>  --删除文件或文件夹                                              *" << endl;
	cout << "*                      15、rename <name> <newName>  --重命名文件或文件夹                                  *" << endl;
	cout << "*                      16、move <name> <destPath> --将当前目录下名为name的文件移动至destPath路径下         *" << endl;
	cout << "*                      17、cls  --清空当前屏幕                                                            *" << endl;
	cout << "**********************************************************************************************************" << endl;
	return SUCCESS;
}

//创建文件夹
STATUS operation::mkdir(char *PATH,char *name) {
	int inodeNum = getInode(PATH);//父目录的inode号
	if (inodeNum == -1) {
		return ERR_PATH_FAIL;
	}
	int dirNum;//被分配到的目录下标
	int inodeNumGet;//被分配到的inode号
	//分配目录
	for (int i = 2; i < DIRNUM; i++) {
		if (disk->dirTree[i].itemNumber == 0) {
			disk->dirTree[i].itemNumber = 1;
			dirNum = i;
			break;
		}
	}
	//为新目录申请inode节点
	for (int i = 0; i < INODENUM; i++) {
		if (disk->spBlock.inodeArray[i] == 0) {
			inodeNumGet = i;
			disk->inodes[i].fileType = TYPEDIR;
			disk->inodes[i].dirNumber = dirNum;
			disk->inodes[i].blockHash[0] = NULL;
			disk->spBlock.inodeArray[i] = 1;
			break;
		}
	}
	//在上层目录建立指针
	for (int i = 0; i < DIRSIZE; i++) {
		if (strlen(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName) == 0) {
			disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber++;
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name);
			disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum = inodeNumGet;
			break;
		}
	}
	return SUCCESS;
}

//创建文件
STATUS operation::create(char *path, char *name) {
	int inodeNum = getInode(path);
	if(inodeNum == -1) {
		return ERR_PATH_FAIL;
	}
	int inodeNumGet;//被分配到的inode号
	//分配inode号
	for (int i = 0; i < INODENUM; i++) {
		if (disk->spBlock.inodeArray[i] == 0) {
			inodeNumGet = i;
			disk->inodes[i].fileType = TYPEFILE;
			disk->inodes[i].fileSize=0;
			disk->inodes[i].blockHash[0] = NULL;
			disk->spBlock.inodeArray[i] = 1;
			break;
		}
	}
	//在父节点建立指针
	for (int i = 0; i < DIRSIZE; i++) {
		if (strlen(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName) == 0) {
			disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber++;
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name);
			disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum = inodeNumGet;
			break;
		}
	}
	return SUCCESS;
}

//删除文件
STATUS operation::deleteFile(char *path,char *name) {
	int inodeNum = getInode(path);
	int flag = 0;
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			flag = 1;//找到名字对应文件
			if (!disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEFILE) {
				cout << "不能使用该指令删除文件夹" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else {
				strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
				disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//释放inode节点
				disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber--;
			}
		}
	}
	if (flag == 0) {
		cout << "文件不存在" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//要删除的文件夹的inodeNum
STATUS removeDir(int inodeNum) {
	for (int i = 0; i < DIRSIZE; i++) {
		if (disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum!=0 && disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEFILE) {
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
			disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//释放inode节点
			disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber--;
		}
		else if (disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum!=0 && disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
			disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//释放inode节点
			disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber--;
			removeDir(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum);
		}
	}
	return SUCCESS;
}

STATUS operation::rmdir(char *path, char *name) {
	int inodeNum = getInode(path);
	int flag = 0;
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			flag = 1;//找到名字对应文件夹
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEFILE) {
				cout << "不能使用该指令删除文件" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else{
				strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
				disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//释放inode节点
				disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber--;
				//递归删除文件夹
				removeDir(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum);
			}
		}
	}
	if (flag == 0) {
		cout << "文件夹不存在" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//罗列当前路径下所有文件
STATUS operation::ls(char *path) {
	int inodeNum = getInode(path);
	cout << setw(10) << "name" << setw(5) << "type" << setw(6)<<"size"<<endl;
	for (int i = 0; i < DIRSIZE; i++) {
		if (strlen(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName) != 0) {
			cout << setw(10) << disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType==TYPEDIR) {
				cout << setw(5) << "DIR" << setw(6) << "-";
			}
			else {
				cout << setw(5) << "FILE" << setw(6) << disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileSize;
			}
			cout << endl;
		}
	}
	return SUCCESS;
}

STATUS operation::cd(char *destPath) {
	char path[NAMESIZE*DIRNUM] = "";
	//回退至上级路径
	if (!strcmp(destPath, "..")) {
		if (strcmp(PATH, "/C") == 0) {
			return SUCCESS;
		}
		int len;
		for (int i = strlen(PATH)-1; i >= 0; i--) {
			if (PATH[i] == '/') {
				len = i;
				break;
			}
		}
		strncpy(path, PATH, len);
		strcpy(PATH, path);
	}
	//跳转路径,只能跳转至当前文件夹下的路径
	else {
		strcpy(path, PATH);
		strcat(path, "/");
		strcat(path, destPath);
		if (getInode(path) == -1 || getInode(path) == 0) {
			cout << "目录输入错误，进入失败" << endl;
		}
		else{
			strcpy(PATH, path);
		}
	}
	return SUCCESS;
}

STATUS operation::open(char *path,char *name) {
	int inodeNum = getInode(path);
	int find = 0;//文件是否存在
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 1) {
				cout << "文件已打开,请勿重复打开" << endl;
				return ERR_FILE_ALERADY_OPEN;
			}
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
				cout << "不能打开文件夹" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else {
				disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen = 1;
				char tmp[30] = "打开文件";
				cout << strcat(strcat(tmp, disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName),"成功") << endl;
				return SUCCESS;
			}
		}
	}
	if (find == 0) {
		cout << "文件不存在" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

STATUS operation::close(char *path, char *name) {
	int inodeNum = getInode(path);
	int find = 0;//文件是否存在
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
				cout << "不能关闭文件夹" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else {
				if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 0) {
					cout << "文件已关闭,请勿重复关闭" << endl;
					return ERR_FILE_NOT_OPEN;
				}
				disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen = 0;
				char tmp[30] = "关闭文件";
				cout << strcat(strcat(tmp, disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName), "成功") << endl;
				return SUCCESS;
			}
		}
	}
	if (find == 0) {
		cout << "文件不存在" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//文件重命名
STATUS operation::rename(char *path,char *oldName,char *newName) {
	int inodeNum = getInode(path);
	int findFile=0;//要改名的文件是否存在
	//int nameExist = 0;//新名字是否冲突
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, oldName) == 0) {
			findFile = 1;//找到对应的文件
			for (int j = 0; j < DIRSIZE; j++) {
				if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[j].fileName, newName) == 0){
					cout << strcat(strcat("已存在名为",newName),"的文件，改名失败") << endl;
					return ERR_FILE_EXIST;
				}
			}
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, newName);
			return SUCCESS;
		}
	}
	if (findFile == 0) {
		cout << "要改名的文件不存在" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//移动文件
STATUS operation::move(char *path, char *name, char *destPath) {
	int inodeNum = getInode(path);
	int findFile = 0;//要移动的文件是否存在
	int inodeNumOfFile;//要移动的文件的inode号
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			findFile = 1;//找到对应的文件
			
			int newInodeNum = getInode(destPath);
			int findNewFile = 0;
			for (int j = 0; j < DIRSIZE; j++) {
				if (strcmp(disk->dirTree[disk->inodes[newInodeNum].dirNumber].items[j].fileName, name) == 0)
					findNewFile = 1;
			}
			if (findNewFile == 1) {
				cout << "目标文件夹已存在同名文件" << endl;
				return ERR_FILE_EXIST;
			}
			//经检测，当前目录存在指定文件，且目标目录不存在同名文件,可进行移动
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
			inodeNumOfFile = disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum;//得到文件的inode号
			for(int j=0;j<DIRSIZE;j++){
				if (strlen(disk->dirTree[disk->inodes[newInodeNum].dirNumber].items[j].fileName) == 0) {
					disk->dirTree[disk->inodes[newInodeNum].dirNumber].items[j].inodeNum = inodeNumOfFile;
					strcpy(disk->dirTree[disk->inodes[newInodeNum].dirNumber].items[j].fileName, name);
					break;
				}
			}
			return SUCCESS;
		}
	}
	if (findFile == 0) {
		cout << "要移动的文件不存在" << endl;
		ERR_FILE_NOT_EXIST;
	}
}

//读文件
STATUS operation::read(char *path, char *name) {
	int inodeNum = getInode(path);
	int find = 0;//文件是否存在
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR){
				cout << "这是一个文件夹" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 0) {
				cout << "请先打开文件" << endl;
				return ERR_FILE_NOT_OPEN;
			}
			else {
				cout << "文件内容：";
				inode inode= disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum];
				for (int j = 0; j < ((inode.fileSize/ BLOCKSIZE)>0?inode.fileSize:1); j++) {
					for (int k = (BLOCKSIZE*inode.blockHash[j]);
						k < ((inode.blockHash[j] * BLOCKSIZE) +
						((inode.fileSize / BLOCKSIZE) ? BLOCKSIZE :
							(inode.fileSize % BLOCKSIZE))); k++) {
						cout << disk->free[k];
					}
				}
				cout << endl;
				cout << "读取文件完毕" << endl;
				return SUCCESS;
			}
			break;
		}
	}
	if (find == 0) {
		cout << "文件不存在" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

STATUS operation::write(char *path, char *name) {
	int inodeNum = getInode(path);
	int find = 0;//文件是否存在
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
				cout << "这是一个文件夹" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 0) {
				cout << "请先打开文件" << endl;
				return ERR_FILE_NOT_OPEN;
			}
			else {
				cout << "请输入要写入文件的内容：";
				char content[BLOCKNUM*BLOCKSIZE];
				cin >> content;
				disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileSize = strlen(content);
				for (int j = 0; j < (strlen(content) / BLOCKSIZE > 0 ? strlen(content) / BLOCKSIZE : 1); j++) {
					for (int k = 0; k < BLOCKNUM; k++) {
						//某物理块空闲
						if (disk->spBlock.blockArray[k] == 0) {
							int l, m;//l是物理块指针,m是逻辑快指针
							disk->spBlock.blockArray[k] = 1;
							disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].blockHash[j] = k;//从物理块映射到逻辑块
							//对于文件末尾不满一整块的情况，最后一块已经不能再被其他文件使用，因此最后部分写入垃圾数据无问题
							for (l = k*BLOCKSIZE, m = j*BLOCKSIZE; l < k*BLOCKSIZE + BLOCKSIZE; m++, l++) {
								disk->free[l] = content[m];
							}
							break;
						}
					}
				}
				cout << "写入文件完毕" << endl;
				return SUCCESS;
			}
			break;
		}
	}
	if (find == 0) {
		cout << "文件不存在" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}