#include"operation.h"

//���ص�ǰ·�����һ���ļ��е�inode��
int operation::getInode(char *PATH) {
	if (PATH[0] != '/') {
		return -1;//·���Ƿ�
	}
	dir currentDir = disk->dirTree[disk->inodes[disk->dirTree[0].items[0].inodeNum].dirNumber];//���ڵ���Ŀ¼
	int ans=0;
	char tpath[NAMESIZE*DIRNUM] = "";
	strcpy(tpath, PATH);
	char *fpath = strtok(tpath, "/");
	//�õ���Ŀ¼���Ŀ¼
	fpath = strtok(NULL, "/");
	//��ǰλ�ڸ�Ŀ¼
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
					return -1;//PATHӦ���ǵ�ǰ·������Ӧ�����ļ�
				}
			}
		}
		if (find == 0)
			return -1;//�޶�Ӧ�ļ���
		fpath = strtok(NULL, "/");
	}
	return ans;//Ϊ���ڵ�,0
}


STATUS operation::newFS(char FSName[]) {
		memset(disk, '\0', sizeof(struct storage));
		strcpy(disk->diskName, FSName);
		disk->dirTree[0].itemNumber = 1;//ע��Ŀ¼��
		strcpy(disk->dirTree[0].items[0].fileName, "C");//���ڵ���
		disk->dirTree[0].items[0].inodeNum = 0;//���ڵ��inode��
		disk->spBlock.inodeArray[0] = 1;//����inode
		disk->inodes[0].fileType = TYPEDIR;
		disk->inodes[0].fileSize = 0;
		disk->inodes[0].dirNumber = 1;
		strcpy(PATH, "/C");
		cout << "�����ڴ��е��ļ�ϵͳ�ɹ�" << endl;
		return SUCCESS;
}

//�˳��ļ�ϵͳ������������
STATUS operation::exit() {
	FILE *fp;
	if ((fp = fopen(disk->diskName, "w")) == NULL) {
		cout << "�ڴ��̽����ļ�ϵͳʧ��" << endl;
		return ERR_FILE_FAIL;
	}

	if (fwrite(disk, sizeof(struct storage), 1, fp) != 1) {
		cout << "���ļ�ϵͳ����������ʧ��" << endl;
		return ERR_FILE_EXIST;
	}
	cout << "���ļ�ϵͳ���������̳ɹ�" << endl;
	fclose(fp);
	return SUCCESS;
}

//���ļ�ϵͳ
STATUS operation::sfs(char name[]) {
	FILE *fp;
	strcpy(PATH, "/C");
	if ((fp = fopen(name, "r")) == NULL) {
		cout << "�ڴ��̴��ļ�ϵͳʧ��" << endl;
		return ERR_FILE_FAIL;
	}

	if (fread(disk, sizeof(struct storage), 1, fp)) {
		fclose(fp);
		cout << "���ļ�ϵͳ�ɹ�" << endl;
		return SUCCESS;
	}
	cout << "��ȡ�ļ�ϵͳʧ��" << endl;
	return ERR_FILE_FAIL;
}

STATUS operation::help() {
	cout << "**********************************************************************************************************" << endl;
	cout << "*                             �ļ�ϵͳ�����ֲ�                                                            *" << endl;
	cout << "*                                                                                                        *" << endl;
	cout << "*                       1��help   --�鿴ָ���ʽ                                                          *" << endl;
	cout << "*                       2��new <name>  --������Ϊname���ļ�ϵͳ������������                                *" << endl;
	cout << "*                       3��sfs <name>  --����Ϊname���ļ�ϵͳ                                           *" << endl;
	cout << "*                       4��exit   --�˳���ǰ�ļ�ϵͳ������������                                           *" << endl;
	cout << "*                       5��mkdir <dir>  --�ڵ�ǰ·���´�����Ϊdir���ļ���                                  *" << endl;
	cout << "*                       6��rmdir <dir>  --������ǰ·������Ϊname���ļ���                                   *" << endl;
	cout << "*                       7��ls  --��ʾ��ǰ�ļ����������ļ�/�ļ���                                           *" << endl;
	cout << "*                       8��cd <..>|<dir>  --�����ϼ�Ŀ¼�����ƶ���ָ��·����                               *" << endl;
	cout << "*                       9��create <name>  --�ڵ�ǰ·���´�����Ϊname���ļ�                                 *" << endl;
	cout << "*                      10��open <name>  --�򿪵�ǰĿ¼����Ϊname���ļ�                                     *" << endl;
	cout << "*                      11��close <name> --�ر��Ѵ򿪵��ļ�                                                *" << endl;
	cout << "*                      12��read  <name>   --��ȡ�򿪵��ļ�����                                            *" << endl;
	cout << "*                      13��write <name> --���ļ�д������                                                  *" << endl;
	cout << "*                      14��delete <name>  --ɾ���ļ����ļ���                                              *" << endl;
	cout << "*                      15��rename <name> <newName>  --�������ļ����ļ���                                  *" << endl;
	cout << "*                      16��move <name> <destPath> --����ǰĿ¼����Ϊname���ļ��ƶ���destPath·����         *" << endl;
	cout << "*                      17��cls  --��յ�ǰ��Ļ                                                            *" << endl;
	cout << "**********************************************************************************************************" << endl;
	return SUCCESS;
}

//�����ļ���
STATUS operation::mkdir(char *PATH,char *name) {
	int inodeNum = getInode(PATH);//��Ŀ¼��inode��
	if (inodeNum == -1) {
		return ERR_PATH_FAIL;
	}
	int dirNum;//�����䵽��Ŀ¼�±�
	int inodeNumGet;//�����䵽��inode��
	//����Ŀ¼
	for (int i = 2; i < DIRNUM; i++) {
		if (disk->dirTree[i].itemNumber == 0) {
			disk->dirTree[i].itemNumber = 1;
			dirNum = i;
			break;
		}
	}
	//Ϊ��Ŀ¼����inode�ڵ�
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
	//���ϲ�Ŀ¼����ָ��
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

//�����ļ�
STATUS operation::create(char *path, char *name) {
	int inodeNum = getInode(path);
	if(inodeNum == -1) {
		return ERR_PATH_FAIL;
	}
	int inodeNumGet;//�����䵽��inode��
	//����inode��
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
	//�ڸ��ڵ㽨��ָ��
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

//ɾ���ļ�
STATUS operation::deleteFile(char *path,char *name) {
	int inodeNum = getInode(path);
	int flag = 0;
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			flag = 1;//�ҵ����ֶ�Ӧ�ļ�
			if (!disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEFILE) {
				cout << "����ʹ�ø�ָ��ɾ���ļ���" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else {
				strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
				disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//�ͷ�inode�ڵ�
				disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber--;
			}
		}
	}
	if (flag == 0) {
		cout << "�ļ�������" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//Ҫɾ�����ļ��е�inodeNum
STATUS removeDir(int inodeNum) {
	for (int i = 0; i < DIRSIZE; i++) {
		if (disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum!=0 && disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEFILE) {
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
			disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//�ͷ�inode�ڵ�
			disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber--;
		}
		else if (disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum!=0 && disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
			disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//�ͷ�inode�ڵ�
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
			flag = 1;//�ҵ����ֶ�Ӧ�ļ���
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEFILE) {
				cout << "����ʹ�ø�ָ��ɾ���ļ�" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else{
				strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
				disk->spBlock.inodeArray[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum] = 0;//�ͷ�inode�ڵ�
				disk->dirTree[disk->inodes[inodeNum].dirNumber].itemNumber--;
				//�ݹ�ɾ���ļ���
				removeDir(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum);
			}
		}
	}
	if (flag == 0) {
		cout << "�ļ��в�����" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//���е�ǰ·���������ļ�
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
	//�������ϼ�·��
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
	//��ת·��,ֻ����ת����ǰ�ļ����µ�·��
	else {
		strcpy(path, PATH);
		strcat(path, "/");
		strcat(path, destPath);
		if (getInode(path) == -1 || getInode(path) == 0) {
			cout << "Ŀ¼������󣬽���ʧ��" << endl;
		}
		else{
			strcpy(PATH, path);
		}
	}
	return SUCCESS;
}

STATUS operation::open(char *path,char *name) {
	int inodeNum = getInode(path);
	int find = 0;//�ļ��Ƿ����
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 1) {
				cout << "�ļ��Ѵ�,�����ظ���" << endl;
				return ERR_FILE_ALERADY_OPEN;
			}
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
				cout << "���ܴ��ļ���" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else {
				disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen = 1;
				char tmp[30] = "���ļ�";
				cout << strcat(strcat(tmp, disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName),"�ɹ�") << endl;
				return SUCCESS;
			}
		}
	}
	if (find == 0) {
		cout << "�ļ�������" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

STATUS operation::close(char *path, char *name) {
	int inodeNum = getInode(path);
	int find = 0;//�ļ��Ƿ����
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
				cout << "���ܹر��ļ���" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			else {
				if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 0) {
					cout << "�ļ��ѹر�,�����ظ��ر�" << endl;
					return ERR_FILE_NOT_OPEN;
				}
				disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen = 0;
				char tmp[30] = "�ر��ļ�";
				cout << strcat(strcat(tmp, disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName), "�ɹ�") << endl;
				return SUCCESS;
			}
		}
	}
	if (find == 0) {
		cout << "�ļ�������" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//�ļ�������
STATUS operation::rename(char *path,char *oldName,char *newName) {
	int inodeNum = getInode(path);
	int findFile=0;//Ҫ�������ļ��Ƿ����
	//int nameExist = 0;//�������Ƿ��ͻ
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, oldName) == 0) {
			findFile = 1;//�ҵ���Ӧ���ļ�
			for (int j = 0; j < DIRSIZE; j++) {
				if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[j].fileName, newName) == 0){
					cout << strcat(strcat("�Ѵ�����Ϊ",newName),"���ļ�������ʧ��") << endl;
					return ERR_FILE_EXIST;
				}
			}
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, newName);
			return SUCCESS;
		}
	}
	if (findFile == 0) {
		cout << "Ҫ�������ļ�������" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

//�ƶ��ļ�
STATUS operation::move(char *path, char *name, char *destPath) {
	int inodeNum = getInode(path);
	int findFile = 0;//Ҫ�ƶ����ļ��Ƿ����
	int inodeNumOfFile;//Ҫ�ƶ����ļ���inode��
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			findFile = 1;//�ҵ���Ӧ���ļ�
			
			int newInodeNum = getInode(destPath);
			int findNewFile = 0;
			for (int j = 0; j < DIRSIZE; j++) {
				if (strcmp(disk->dirTree[disk->inodes[newInodeNum].dirNumber].items[j].fileName, name) == 0)
					findNewFile = 1;
			}
			if (findNewFile == 1) {
				cout << "Ŀ���ļ����Ѵ���ͬ���ļ�" << endl;
				return ERR_FILE_EXIST;
			}
			//����⣬��ǰĿ¼����ָ���ļ�����Ŀ��Ŀ¼������ͬ���ļ�,�ɽ����ƶ�
			strcpy(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, "");
			inodeNumOfFile = disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum;//�õ��ļ���inode��
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
		cout << "Ҫ�ƶ����ļ�������" << endl;
		ERR_FILE_NOT_EXIST;
	}
}

//���ļ�
STATUS operation::read(char *path, char *name) {
	int inodeNum = getInode(path);
	int find = 0;//�ļ��Ƿ����
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR){
				cout << "����һ���ļ���" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 0) {
				cout << "���ȴ��ļ�" << endl;
				return ERR_FILE_NOT_OPEN;
			}
			else {
				cout << "�ļ����ݣ�";
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
				cout << "��ȡ�ļ����" << endl;
				return SUCCESS;
			}
			break;
		}
	}
	if (find == 0) {
		cout << "�ļ�������" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}

STATUS operation::write(char *path, char *name) {
	int inodeNum = getInode(path);
	int find = 0;//�ļ��Ƿ����
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].fileName, name) == 0) {
			find = 1;
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileType == TYPEDIR) {
				cout << "����һ���ļ���" << endl;
				return ERR_TYPE_NOT_CORRECT;
			}
			if (disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].isOpen == 0) {
				cout << "���ȴ��ļ�" << endl;
				return ERR_FILE_NOT_OPEN;
			}
			else {
				cout << "������Ҫд���ļ������ݣ�";
				char content[BLOCKNUM*BLOCKSIZE];
				cin >> content;
				disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].fileSize = strlen(content);
				for (int j = 0; j < (strlen(content) / BLOCKSIZE > 0 ? strlen(content) / BLOCKSIZE : 1); j++) {
					for (int k = 0; k < BLOCKNUM; k++) {
						//ĳ��������
						if (disk->spBlock.blockArray[k] == 0) {
							int l, m;//l�������ָ��,m���߼���ָ��
							disk->spBlock.blockArray[k] = 1;
							disk->inodes[disk->dirTree[disk->inodes[inodeNum].dirNumber].items[i].inodeNum].blockHash[j] = k;//�������ӳ�䵽�߼���
							//�����ļ�ĩβ����һ�������������һ���Ѿ������ٱ������ļ�ʹ�ã������󲿷�д����������������
							for (l = k*BLOCKSIZE, m = j*BLOCKSIZE; l < k*BLOCKSIZE + BLOCKSIZE; m++, l++) {
								disk->free[l] = content[m];
							}
							break;
						}
					}
				}
				cout << "д���ļ����" << endl;
				return SUCCESS;
			}
			break;
		}
	}
	if (find == 0) {
		cout << "�ļ�������" << endl;
		return ERR_FILE_NOT_EXIST;
	}
}