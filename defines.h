#pragma once

#define INODENUM 256//inode�ڵ���
#define BLOCKNUM 1000//���̿���
#define BLOCKSIZE 512//���̿��С
#define SUPERBLOCKSIZE 5024//�������С
#define FILESPACESIZE 1024*1024*1024//�ļ��洢�ռ��С,1G
#define NAMESIZE 16//�����ļ������ļ�������
#define DIRSIZE 16//�ļ��а�����Ŀ¼����
#define DIRNUM 32//�ļ�����
#define FSSIZE FILESPACESIZE+SUPERBLOCKSIZE+30+256*216+32*324//���������С
#define TYPEDIR 0//�ļ�������
#define TYPEFILE 1//�ļ�����

//����������״̬	
enum STATUS
{
	SUCCESS,ERR_PATH_FAIL,ERR_FILE_EXIST,ERR_FILE_NOT_EXIST,ERR_FILE_FAIL,ERR_TYPE_NOT_CORRECT,ERR_FILE_NOT_OPEN,ERR_FILE_ALERADY_OPEN
};

//������
typedef struct  superBlock
{
	//int freeInodeNUM;//����inode��
	//int nextInode;//��һ��inode�ڵ���
	int inodeArray[INODENUM];//inode��־����,0Ϊ���У�1Ϊ��ռ��
	//int freeBlockNum;//���ô��̿���
	//int nextBlock;//��һ���̿���
	int blockArray[BLOCKNUM];//���̿��־����
}superBlock;

//�ļ�Ŀ¼��ṹ
typedef struct dirItem {
	char fileName[NAMESIZE];//�ļ���
	int inodeNum;//�ļ�inode�ڵ��
}dirItem;

//inode�ڵ�ṹ
typedef struct inode {
	int fileType;//�ļ�����
	int isOpen;//�ļ��Ƿ񱻴�
	int fileSize;//�ļ���С
	int dirNumber;//�ļ��ж�Ӧ��dirTree�����±�
	int blockHash[50];//�ļ���ӳ�������һ������
}inode;

//�ļ���
typedef struct dir {
	dirItem items[DIRSIZE];
	int itemNumber;//�ļ�������Ŀ¼����Ŀ
}dir;

//���ڵ�
 struct storage {
	char diskName[30];//�ļ�ϵͳ��
	superBlock spBlock;
	inode inodes[INODENUM];//inode����
	dir dirTree[DIRNUM];//dir��
	char free[FILESPACESIZE];//�洢�ļ�����
};

extern struct storage *disk;//���̱���
extern char fileName[30];//��ǰ�򿪵��ļ���
extern char PATH[NAMESIZE*DIRNUM];//��ǰ·��